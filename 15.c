#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//toutes les consatntes utiles au code

#define TAILLE_NOM_EQUIPE 32
#define NOMBRE_TIC_MAXIMUM 5000
#define TAILLE_NOM_PERSO 50
#define TAILLE_NOM_COMPETENCE 32
#define TAILLE_NOM_EFFET 32
#define TAILLE_DESCRIPTION 512
#define TAILLE_CATEGORIE_PERSO 32
#define NOMBRE_PERSO 8
#define TAILLE_AFFICHAGE 107 //multiple de 3 (99)+8
#define NOMBRE_EFFET 6 //nombre de tous les effets possibles du jeu
#define TAILLE_EQUIPE 3
#define TAILLE_TYPE 20
#define TAILLE_CIBLE 20
#define NOMBRE_COMPETENCE 4
#define SEUIL_ACTION 100

//toutes les structures utiles au code

typedef struct{
  char nom[TAILLE_NOM_COMPETENCE];
  int valeur;
  char description[TAILLE_DESCRIPTION];
  char type[TAILLE_CIBLE]; // soin dégat ..?
  char cible[TAILLE_CIBLE];
  int tour_actif;
  int tour_recharge;
  int tour_recharge_restant;
} Competence;//acceleration protection regeneration provocation et brulure

typedef struct{
  char nom[TAILLE_NOM_EFFET];
  int tour_restant;
  int valeur;
} Effet;

typedef struct{
  char nom[TAILLE_NOM_PERSO];
  int pv;
  int pvmax;
  int attaque;
  int defense;
  int agilite;
  int vitesse;
  int barre_action;// les personnages chargent une barre d'action pour attaquer grâce a leur vitesse comme dans rclaeraid shadow legends
  char categorie[TAILLE_CATEGORIE_PERSO];
  Effet effet_special[NOMBRE_EFFET];
  int nbr_effet_actif;
  Competence* competence[NOMBRE_COMPETENCE];
  int equipe;
}  Combattant;

int meilleur_scan(){
    int retour = 0;
    int valeur;
    retour = scanf("%d", &valeur);
    while (retour != 1)
    {   
    printf("Erreur de saisie.");
    while(getchar()!='\n'){} // Ligne facultative de sécurisation
        retour = scanf("%d", &valeur);
    }
    return valeur;
}

// les procédures suivantes permettent de vérifier qu'il n y a pas d'erreur lors de l'ouverture du fichier ou qu'un pointeur passé en argument soit nul
void verifier_erreur_fichier(FILE* fichier){
  if (fichier==NULL){
    printf("Erreur lors de l'ouverture du fichier.\n");
    exit(EXIT_FAILURE);}
}

void verifier_erreur_int(int* a){
    if (a==NULL){
        printf("Erreur : pointeur passé en argument est NULL.\n");
        exit(EXIT_FAILURE);
    }
}

void verifier_erreur_char(char* a){
    if (a==NULL){
        printf("Erreur : pointeur passé en argument est NULL.\n");
        exit(EXIT_FAILURE);
    }
}

void verifier_erreur_combattant(Combattant* a){
    if (a==NULL){
        printf("Erreur : pointeur passé en argument est NULL.\n");
        exit(EXIT_FAILURE);
    }
}

void verifier_erreur_competence(Competence* a){
    if (a==NULL){
        printf("Erreur : pointeur passé en argument est NULL.\n");
        exit(EXIT_FAILURE);
    }
}

int vivant(Combattant* combattant){
    if (combattant->pv==0){
        return 0;}
    return 1;
}

int degat(Combattant* combattant,int degat){//fonction qui inflige les dégats à un personnage mais gère aussi le fait que le personnage meurt ou non
  int n=rand()%100;
    int pv_actuel=combattant->pv;
    if (combattant->agilite<n){
        int degats_totaux=degat*(100-combattant->defense)/100;
        combattant->pv=combattant->pv-degats_totaux;
    if (combattant->pv<0){
      combattant->pv=0;}
    }
    else{
        printf("\n%s esquive !\n",combattant->nom);
    }
    return pv_actuel-combattant->pv;
}

void degat_recharge(Combattant* combattant,int degat,Competence* competence){//fonction qui inflige les dégats à un personnage mais gère aussi le fait que le personnage meurt ou non
  int n=rand()%100;
    int pv_actuel=combattant->pv;
    if (combattant->agilite<n){
        int degats_totaux=degat*(100-combattant->defense)/100;
        combattant->pv=combattant->pv-degats_totaux;
        if (combattant->pv<0){
            combattant->pv=0;}
    }
    else{
        printf("\n%s esquive !\n",combattant->nom);
    }
    competence->tour_recharge_restant=competence->tour_recharge;
}

void retirer_stats(Combattant* combattant,int index){  // Retire les effets spéciaux temporaires d'un combattant (vitesse ou défense) et elle estt appelée lorsqu'un effet expire ou est supprimé 
    if (strcmp(combattant->effet_special[index].nom,"Acceleration")==0){
        combattant->vitesse=combattant->vitesse-combattant->effet_special[index].valeur;
    }
    else if (strcmp(combattant->effet_special[index].nom,"Protection")==0){
        combattant->defense=combattant->defense-combattant->effet_special[index].valeur;
  }
}

void appliquer_effet(Combattant* combattant,Competence* competence,Effet effet){
    combattant->barre_action=0;
    for (int i=0;i<combattant->nbr_effet_actif;i++){//on regarde s'il à déjà l'effet, si oui on applique en enlevant les buffs/ malus precedent
    if (strcmp(combattant->effet_special[i].nom,effet.nom)==0){
        retirer_stats(combattant,i);
        combattant->effet_special[i]=effet;
        return;
    }}
    combattant->nbr_effet_actif++;
    combattant->effet_special[combattant->nbr_effet_actif-1]=effet;//s'il ne l'avait pas on applique l'effet
}

void soin(Combattant* combattant,int soin){ // Soigne un combattant qui ne peut pas dépasser un seuil de PV max
    combattant->pv=combattant->pv+soin;
    if(combattant->pv > combattant->pvmax){
    combattant->pv=combattant->pvmax;}
}

void soin_recharge(Combattant* combattant,int soin,Competence* competence){ // Soigne un combattant et recharge la compétence
    combattant->pv=combattant->pv+soin;
    if(combattant->pv > combattant->pvmax){
        combattant->pv=combattant->pvmax;}
    competence->tour_recharge_restant=competence->tour_recharge;
}

void acceleration(Combattant* combattant, Competence* competence){ // fonction qui permet d'Augmenter la vitesse de 30 d un allie pendant 12 tours
    Effet effet;
    strcpy(effet.nom, competence->type);
    effet.tour_restant=competence->tour_actif;
    effet.valeur=competence->valeur;
    appliquer_effet(combattant,competence,effet);
    combattant->vitesse=combattant->vitesse+competence->valeur;
    competence->tour_recharge_restant=competence->tour_recharge;
}

void protection(Combattant* combattant, Competence* competence){ //Augmente la defense d'un allie pendant plusieurs tours .
    Effet effet;
    strcpy(effet.nom, competence->type);
    effet.tour_restant=competence->tour_actif;
    effet.valeur=competence->valeur;
    appliquer_effet(combattant,competence,effet);
    combattant->defense=combattant->defense+competence->valeur;
    competence->tour_recharge_restant=competence->tour_recharge;
}

void regeneration_brulure(Combattant* combattant, Competence* competence){ // Applique un effet de type "Regeneration" ou "Brulure" selon le type de la compétence
    Effet effet;
    strcpy(effet.nom, competence->type);
    effet.valeur=competence->valeur;
    effet.tour_restant=competence->tour_actif;
    appliquer_effet(combattant,competence,effet);
    competence->tour_recharge_restant=competence->tour_recharge;
}

void frappe_impitoyable(Combattant* combattant,Competence* competence){ .
    degat(combattant,competence->valeur+combattant->defense);
    competence->tour_recharge_restant=competence->tour_recharge; // Met la compétence en recharge
}


void provocation(Combattant* combattant, Competence* competence){// Applique un effet de provocation au combattant (cible), forçant  l'adversaire à l'attaquer
    Effet effet;
    strcpy(effet.nom, competence->type);
    effet.tour_restant=competence->tour_actif;
    appliquer_effet(combattant,competence,effet);
    competence->tour_recharge_restant=competence->tour_recharge;
}

void soin_tous(Combattant* equipe[],Competence* competence){ // Soigne tous les membres vivants d'une équipe
    for (int i=0;i<TAILLE_EQUIPE;i++){
      if(vivant(equipe[i])){
          soin(equipe[i],competence->valeur);
        }
      }
      competence->tour_recharge_restant=competence->tour_recharge;
    }


void degat_tous(Combattant* equipe[],Competence* competence){// Applique des dégâts à tous les membres vivants d'une équipe
    for (int i=0;i<TAILLE_EQUIPE;i++){
        degat(equipe[i],competence->valeur);
    }
    competence->tour_recharge_restant=competence->tour_recharge;
}

void Vol_de_vie(Combattant* lanceur,Combattant* cible,Competence* competence){ // Vol de vie : inflige des dégâts à la cible et soigne le lanceur d'une partie des dégâts infligés
    int degat_inflige=degat(cible,competence->valeur);
    soin(lanceur,degat_inflige/2);
}

void retirer_effet(Combattant* combattant,int index){ // Retire un effet spécial d’un combattant à un index donné en décalant les autres effets vers la gauch
    for (; index < combattant->nbr_effet_actif - 1; index++){
        combattant->effet_special[index] = combattant->effet_special[index+ 1];
    }
    combattant->nbr_effet_actif--;
}

void maj_vitesse(Combattant* combattant){ // Met à jour la vitesse du combattant en ajoutant sa vitesse à sa barre d’action
    combattant->barre_action=combattant->barre_action+combattant->vitesse;
}

void maj_effet(Combattant* combattant){ // Met à jour tous les effets actifs sur un combattant (brûlure, régénération...) et  Applique les effets, décrémente leur durée, puis les retire s’ils expirent
    for (int i = 0; i < combattant->nbr_effet_actif; i++){
        if (strcmp(combattant->effet_special[i].nom,"Brulure")==0){
            degat(combattant,combattant->effet_special[i].valeur);
        }
        if (strcmp(combattant->effet_special[i].nom,"Regeneration")==0){
            soin(combattant,combattant->effet_special[i].valeur);
        }
        combattant->effet_special[i].tour_restant--;
        if (combattant->effet_special[i].tour_restant==0){
            retirer_stats(combattant,i);  
            retirer_effet(combattant,i);
            i--;
        }
    }
}

void maj_recharge(Combattant* combattant){// Met à jour la recharge des compétences (diminue de 1 si > 0)
    for (int i = 0; i < NOMBRE_COMPETENCE; i++) {
        if (combattant->competence[i]->tour_recharge_restant > 0) {
            combattant->competence[i]->tour_recharge_restant--;
        }
    }
}

void maj_tous_stats(Combattant* combattant){// Met à jour toutes les statistiques d’un combattant : recharge, vitesse, effets
    maj_recharge(combattant);
    maj_vitesse(combattant);
    maj_effet(combattant);
}
    
void maj_tous(Combattant* Equipe1[],Combattant* Equipe2[]){// Met à jour tous les combattants vivants des deux équipes
    for (int i=0;i<TAILLE_EQUIPE;i++){
        if (vivant(Equipe1[i])){
            maj_tous_stats(Equipe1[i]);
        }
        if (vivant(Equipe2[i])){
            maj_tous_stats(Equipe2[i]);
        }
    }
}

int charge(Combattant* combattant){// Vérifie si la barre d’action du combattant atteint le seuil pour pouvoir agir
    if (combattant->barre_action<SEUIL_ACTION){
        return 0;}
    return 1;
}

int pret(Combattant* combattant){// Vérifie si un combattant est prêt à agir (vivant et barre d’action remplie)
    return (charge(combattant)&&vivant(combattant));
}


int moinsdepvennemi(Combattant* equipe[]){// Retourne l'indice du combattant ennemi vivant avec le moins de PV
    int hp=1000;
    int numero;
    if(vivant(equipe[0]) && equipe[0]->pv<hp){
        hp=equipe[0]->pv;
        numero=0;
    }
    if(vivant(equipe[1]) && equipe[1]->pv<hp){
        hp=equipe[1]->pv;
        numero=1;
    }
    if(vivant(equipe[2]) && equipe[2]->pv<hp){
        hp=equipe[2]->pv;
        numero=2;
    }
    return numero;
  }
  
  int moinsdepvallie(Combattant* equipe[]){// Retourne l'indice du combattant allié vivant avec le moins de PV
    int hp=1000;
    int numero;
    if(vivant(equipe[0]) && equipe[0]->pv<hp){
        hp=equipe[0]->pv;
        numero=0;
    }
    if(vivant(equipe[1]) && equipe[1]->pv<hp){
        hp=equipe[1]->pv;
        numero=1;
    }
    if(vivant(equipe[2]) && equipe[2]->pv<hp){
        hp=equipe[2]->pv;
        numero=2;
    }
    return numero;
  }

  Combattant* choisir_cible(Combattant* lanceur,Combattant* equipe1[], Combattant* equipe2[], Competence* competence,int mode, int difficulte){// Détermine la cible selon le type de compétence, l'équipe, le mode et la difficulté
    int choix;
    if(lanceur->equipe==1){
        for(int i=0; i<TAILLE_EQUIPE;i++){
            for(int j=0;j<equipe2[i]->nbr_effet_actif;j++){
                if(strcmp(equipe2[i]->effet_special[j].nom,"Provocation")==0){
                    return equipe2[i];
                }
            }
        }
        if(strcmp(competence->cible, "Ennemi") == 0){
            printf("Choisissez une cible :\n");
            choix=meilleur_scan();
            while (choix<1 || choix>TAILLE_EQUIPE || !(vivant(equipe2[choix-1]))){
                printf("Cible incorrect.\n"); 
                choix=meilleur_scan();}
            return equipe2[choix-1];
            }
        else if(strcmp(competence->cible, "Allie") ==0){
        printf("Choisissez une cible :\n");
        choix=meilleur_scan();
        while(choix<1 || choix>TAILLE_EQUIPE|| !(vivant(equipe1[choix-1]))){
            choix=meilleur_scan();
            printf("Cible incorrect.\n"); 
        }
        return equipe1[choix-1];
        }
    }
    if(lanceur->equipe==2){
        if(mode==2){
            for(int i=0; i<TAILLE_EQUIPE;i++){
                for(int j=0;j<equipe1[i]->nbr_effet_actif;j++){
                    if(strcmp(equipe1[i]->effet_special[j].nom,"Provocation")==0){
                        return equipe1[i];
                    }
                }
            }
        if(strcmp(competence->cible, "Ennemi") == 0){
            printf("Choisissez une cible :\n");
            choix=meilleur_scan();
            while (choix<1 || choix>TAILLE_EQUIPE || !(vivant(equipe1[choix-1]))){
                printf("Cible incorrect.\n"); 
                choix=meilleur_scan();}
            return equipe1[choix-1];
        }
        else if(strcmp(competence->cible, "Allie") ==0){
            printf("Choisissez une cible :\n");
            choix=meilleur_scan();
            while(choix<1 || choix>TAILLE_EQUIPE || !(vivant(equipe2[choix-1]))){
                choix=meilleur_scan();
                printf("Cible incorrect.\n"); 
            }
            return equipe2[choix-1];}
      }
        else if(mode==1){
            if(difficulte==1){
                for(int i=0; i<TAILLE_EQUIPE;i++){
                    for(int j=0;j<equipe1[i]->nbr_effet_actif;j++){
                        if(strcmp(equipe1[i]->effet_special[j].nom,"Provocation")==0){
                            return equipe1[i];
                }
           }
        }
        if(strcmp(competence->cible, "Ennemi") == 0){
        choix=rand()%3+1;
        while (choix<1 || choix>TAILLE_EQUIPE || !(vivant(equipe1[choix-1]))){
          choix=rand()%TAILLE_EQUIPE+1;}
        printf("la cible choisi par l'ordinateur est %s\n", equipe1[choix-1]->nom);
        return equipe1[choix-1];}
        else if(strcmp(competence->cible, "Allie") ==0){
        choix=rand()%TAILLE_EQUIPE+1;
        while(choix<1 || choix>TAILLE_EQUIPE || !(vivant(equipe2[choix-1]))){
          choix=rand()%3+1;
       }
       printf("la cible choisi par l'ordinateur est %s\n", equipe1[choix-1]->nom);
       return equipe2[choix-1];}
        }
        else if(difficulte==2 || difficulte==3){
          for(int i=0; i<TAILLE_EQUIPE;i++){
            for(int j=0;j<equipe1[i]->nbr_effet_actif;j++){
                if(strcmp(equipe1[i]->effet_special[j].nom,"Provocation")==0){
                   return equipe1[i];
                }
           }
        }
        if(strcmp(competence->cible, "Ennemi") == 0){
          choix=moinsdepvennemi(equipe1);
          printf("la cible choisi par l'ordinateur est %s\n", equipe1[choix]->nom);
          return equipe1[choix];}
          else if(strcmp(competence->cible, "Allie") ==0){
          choix=moinsdepvallie(equipe2);
         printf("la cible choisi par l'ordinateur est %s\n", equipe1[choix]->nom);
         return equipe2[choix];}
        }
      }
  }}

void appliquer_technique(Competence* competence,Combattant* lanceur, Combattant* equipe1[], Combattant* equipe2[], int mode, int difficulte){//fonction qui va faire des if pour trouver si la technique est un degat,un soin, un endormissement... et applique la technique en conséquence
  Combattant* cible;
  lanceur->barre_action=0;
  printf("%s utilise %s \n",lanceur->nom,competence->nom);
  if (strcmp(competence->type,"Degats")==0){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence, mode, difficulte);
    degat_recharge(cible,competence->valeur,competence);
  }
  else if (strcmp(competence->type,"Soin")==0){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence, mode, difficulte);
    soin_recharge(cible,competence->valeur,competence);
  }
  else if (strcmp(competence->type,"Acceleration")==0){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence, mode, difficulte);
    acceleration(cible,competence);
  }
  else if (strcmp(competence->type,"Protection")==0){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence, mode, difficulte);
    protection(cible,competence);
  }
  else if ((strcmp(competence->type,"Regeneration")==0)||(strcmp(competence->type,"Brulure")==0)){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence, mode, difficulte);
    regeneration_brulure(cible,competence);
  }
  else if (strcmp(competence->type,"DegatsDef")==0){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence, mode, difficulte);
    frappe_impitoyable(cible,competence);
  }
  else if (strcmp(competence->type,"SoinTous")==0){
    if (lanceur->equipe==1){
        soin_tous(equipe1, competence);
    }
    else{
        soin_tous(equipe2, competence);
    }
  }
  else if (strcmp(competence->type,"DegatsTous")==0){
    if (lanceur->equipe==1){
        degat_tous(equipe2, competence);
    }
    else{
        degat_tous(equipe1, competence);
    }
  }
  else if (strcmp(competence->type,"Provocation")==0){
    provocation(lanceur,competence);
  }
  else if (strcmp(competence->type,"VolDeVie")==0){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence, mode, difficulte);
    Vol_de_vie(lanceur,cible,competence);
  }
}

Competence* charger_competence(char* fichier_competence){//fonction qui charge les données d'une technique contenues dans un fichier
    Competence* c = malloc(sizeof(Competence));
     if (c == NULL) {
       printf("Erreur d'allocation mémoire pour Competence\n");
       exit(EXIT_FAILURE);
     }
     FILE* fichier=fopen(fichier_competence,"r");
     verifier_erreur_fichier(fichier);
     fgets(c->nom,TAILLE_NOM_COMPETENCE,fichier);
     fgets(c->description,TAILLE_DESCRIPTION,fichier);
     fscanf(fichier,"%s %s %d %d %d",c->type,c->cible,&(c->valeur),&(c->tour_actif),&(c->tour_recharge));
     c->tour_recharge_restant=0;
     fclose(fichier);
     return c;
   }

Combattant* charger_combattant(char* fichier_combattant){ //fonction qui charge les données d'un combattant contenues dans un fichier
  Combattant* c = malloc(sizeof(Combattant));
  if (c == NULL) {
    printf("Erreur d'allocation mémoire pour Combattant\n");
    exit(EXIT_FAILURE);
  }
  c->nbr_effet_actif=0;
  FILE* fichier= fopen(fichier_combattant,"r");
  verifier_erreur_fichier(fichier);
  char competence1[TAILLE_NOM_COMPETENCE],competence2[TAILLE_NOM_COMPETENCE],competence3[TAILLE_NOM_COMPETENCE],competence4[TAILLE_NOM_COMPETENCE];
  c->barre_action=0;
  fscanf(fichier,"%s %s %d %d %d %d %d %d %s %s %s %s",c->nom,c->categorie,&(c->pv),&(c->pvmax),&(c->attaque),&(c->defense),&(c->agilite),&(c->vitesse),competence1,competence2,competence3,competence4);
  c->competence[0]=charger_competence(competence1);
  c->competence[1]=charger_competence(competence2);
  c->competence[2]=charger_competence(competence3);
  c->competence[3]=charger_competence(competence4);
  fclose(fichier);
  return c;
}

void afficher_personnages_disponibles(int deja_choisi[], char personnages_disponibles[NOMBRE_PERSO][TAILLE_NOM_PERSO]) {
    char chaine[TAILLE_NOM_PERSO];
    printf(" Les Personnages disponibles sont:\n");
    for (int i = 0; i < NOMBRE_PERSO; i++) {
        if (deja_choisi[i] == 0) {
            strcpy(chaine,personnages_disponibles[i]);
            chaine[strlen(chaine)-4]='\0'; // pour enlever .txt lors de l'affichage
            printf("%d. %s\n", i + 1, chaine);
        }
    }
}

void creer_equipe(Combattant* equipe1[],Combattant* equipe2[]){ //fonction qui permet aux 2 joueurs de choisir ses personnage et mets à jour les équipes
    char personnages_disponibles[NOMBRE_PERSO][TAILLE_NOM_PERSO];
    FILE* fichier=fopen("Liste_Personnage.txt","r");
  verifier_erreur_fichier(fichier);
    for (int i=0;i<NOMBRE_PERSO;i++){
      fscanf(fichier,"%s",personnages_disponibles[i]);
    };
    fclose(fichier);
// pour suivre quels personnages  sont déjà pris
int deja_choisi[NOMBRE_PERSO] = {0};
    int choix, i;

    for (i = 0; i < TAILLE_EQUIPE; i++) {
        // Joueur 1
        afficher_personnages_disponibles(deja_choisi,personnages_disponibles);
        printf("Joueur 1 - Choisissez un personnage (1-8) : \n");
        choix=meilleur_scan();
        while (choix < 1 || choix > NOMBRE_PERSO || deja_choisi[choix-1] != 0) {
            printf("Choix invalide ou deja pris. Reessayez : \n");
            choix=meilleur_scan();
        }

      equipe1[i] = charger_combattant(personnages_disponibles[choix-1]);

      equipe1[i]->equipe=1;
      deja_choisi[choix-1] = 1;  // empeche ce personnage d'être pris 
        // Joueur 2
        afficher_personnages_disponibles(deja_choisi,personnages_disponibles);
        printf("Joueur 2 - Choisissez un personnage (1-8) : \n ");
        choix=meilleur_scan();
      
        while (choix < 1 || choix > NOMBRE_PERSO ||  deja_choisi[choix-1] != 0) {
            printf("Choix invalide ou deja pris. Reessayez : ");
            choix=meilleur_scan();
        }
        deja_choisi[choix-1] = 1;
      equipe2[i] = charger_combattant(personnages_disponibles[choix-1]);
      equipe2[i]->equipe=2;
    }
  
 printf(" Composition des équipes:\n");

printf("Équipe 1 : :\n");
for (int j = 0; j < TAILLE_EQUIPE; j++) {
    printf("%s", equipe1[j]->nom);
    if (j < TAILLE_EQUIPE - 1) printf(" | ");
}
printf("\n");

printf("Équipe 2 : ");
for (int j = 0; j < TAILLE_EQUIPE; j++) {
    printf("%s", equipe2[j]->nom);
    if (j < TAILLE_EQUIPE - 1) printf(" | ");
}
printf("\n");
    printf("Equipes creees avec succes ! DEBUT DE LA PARTIE \n");
}

void creer_equipeIA(Combattant* equipe1[],Combattant* equipe2[]){ //fonction qui permet aux 2 joueurs de choisir ses personnage et mets à jour les équipes
  char personnages_disponibles[NOMBRE_PERSO][TAILLE_NOM_PERSO];
  FILE* fichier=fopen("Liste_Personnage.txt","r");
  verifier_erreur_fichier(fichier);
  for (int i=0;i<NOMBRE_PERSO;i++){
    fscanf(fichier,"%s",personnages_disponibles[i]);
  };
  fclose(fichier);

// pour suivre quels personnages  sont déjà pris
    int deja_choisi[NOMBRE_PERSO] = {0};
    int choix, i;

    for (i = 0; i < 3; i++) {
        // Joueur 1
        afficher_personnages_disponibles(deja_choisi,personnages_disponibles);
        printf("Joueur 1 - Choisissez un personnage (1-8) : \n");
        choix=meilleur_scan();
        while (choix < 1 || choix > NOMBRE_PERSO || deja_choisi[choix-1] != 0) {
            printf("Choix invalide ou deja pris. Reessayez : \n");
            choix=meilleur_scan();
        }

      equipe1[i] = charger_combattant(personnages_disponibles[choix-1]);

      equipe1[i]->equipe=1;
      deja_choisi[choix-1] = 1;  // empeche ce personnage d'être pris 
        // Joueur 2
        afficher_personnages_disponibles(deja_choisi,personnages_disponibles);
        choix=rand()%8+1;
      
        while (choix < 1 || choix > NOMBRE_PERSO ||  deja_choisi[choix-1] != 0) {
            printf("Choix invalide ou deja pris. Reessayez : ");
            choix=rand()%8+1;
        }
        printf("\n Le choix de l'ordinateur est le %d  ", choix);
        deja_choisi[choix-1] = 1;
      
      equipe2[i] = charger_combattant(personnages_disponibles[choix-1]);
      equipe2[i]->equipe=2;
    }
  
 printf(" Composition des équipes:\n");

printf("Équipe 1 : :\n");
for (int j = 0; j < TAILLE_EQUIPE; j++) {
    printf("%s", equipe1[j]->nom);
    if (j < TAILLE_EQUIPE - 1) printf(" | ");
}
printf("\n");

printf("Équipe 2 : ");
for (int j = 0; j < TAILLE_EQUIPE; j++) {
    printf("%s", equipe2[j]->nom);
    if (j < TAILLE_EQUIPE - 1) printf(" | ");
}
printf("\n");
    printf("Equipes creees avec succes ! DEBUT DE LA PARTIE \n");
}

Competence* choisir_attaque(Combattant* combattant) {//fonction qui permet de chosir l'attaque à utiliser
    int choix;
    printf("\nchoisisser une attaque\n");
    choix=meilleur_scan();
     while (choix < 1 || choix > NOMBRE_COMPETENCE || combattant->competence[choix - 1]->tour_recharge_restant > 0){
    printf("erreur choisisser une attaque valide\n");
    choix=meilleur_scan();
     }
    return combattant->competence[choix - 1];
}

// Fonction qui choisit une compétence aléatoire parmi les deux premières
// utilisée pour une IA de niveau faible ou moyen.
// Ne choisit que des compétences disponibles (pas en recharge).

Competence* choisir_attaqueIAfaiblemoyen(Combattant* combattant) {
  int choix;

  for (int i = 0; i < 2; i++) {
  }

  do {
      choix=rand()%2+1;
  } while (choix < 1 || choix > 2 || combattant->competence[choix - 1]->tour_recharge_restant > 0);
  printf("\n Le choix de l'ordinateur est l'attaque %d : ", choix);
  return combattant->competence[choix - 1];
}

// Fonction qui choisit une compétence aléatoire parmi toutes les compétences disponibles
// utilisée pour une IA de niveau fort.
// Ne choisit que des compétences disponibles (pas en recharge).

Competence* choisir_attaqueIAfort(Combattant* combattant) {
  int choix;

  for (int i = 0; i < NOMBRE_COMPETENCE; i++) {
      printf("\n");
  }

  do {
      choix=rand()%NOMBRE_COMPETENCE+1;
  } while (choix < 1 || choix > NOMBRE_COMPETENCE || combattant->competence[choix - 1]->tour_recharge_restant > 0);
  printf("\n Le choix de l'ordinateur est l'attaque %d : ", choix);
  return combattant->competence[choix - 1];
}

int compter_chiffre(int n){
  if(n==0){
    return 1;
  }
  int compteur=0;
  while (n>0){
    n/=10;
    compteur++;
  }
  return compteur;
}

void afficher_effet(Combattant* combattant){// affiche les effets actifs du combattant
  for (int i=0;i<combattant->nbr_effet_actif;i++){
    if (strcmp(combattant->effet_special[i].nom,"Brulure")==0){
      printf("🔥");
    }
    else if (strcmp(combattant->effet_special[i].nom,"Regeneration")==0){
        printf("💕");
    }
    else if (strcmp(combattant->effet_special[i].nom,"Acceleration")==0){
      printf("💨");
    }
    else if (strcmp(combattant->effet_special[i].nom,"Protection")==0){
      printf("🛡️");
    }
    else if (strcmp(combattant->effet_special[i].nom,"Provocation")==0){
      printf("😡");
    }
  }
}

void affiche_tous_perso(Combattant* Equipe1[],Combattant* Equipe2[],char* equipe1_Nom,char* equipe2_Nom,Combattant* attaquant){
  int a;
  printf("\n");
  


    printf("@[%s]",equipe1_Nom);//1
    for (int i=0;i<TAILLE_AFFICHAGE-3-strlen(equipe1_Nom);i++){
      printf("@");
    }
    printf("\n@");

    for (int i=0;i<TAILLE_AFFICHAGE-2;i++){//2
      printf(" ");
    }
    printf("@\n@");

    a=TAILLE_AFFICHAGE-1;//3
    for (int i=0;i<3;i++){
      printf(" ");
      a--;
    }
    printf("%s [1]",Equipe1[0]->nom);
    a=a-4-strlen(Equipe1[0]->nom);
    for(int i=0;i<33-4-strlen(Equipe1[0]->nom);i++){
      printf(" ");a--;
    }
    printf("%s [2]",Equipe1[1]->nom);
    a=a-4-strlen(Equipe1[1]->nom);
    for(int i=0;i<33-4-strlen(Equipe1[1]->nom);i++){
      printf(" ");a--;
    }
    printf("%s [3]",Equipe1[2]->nom);
    a=a-4-strlen(Equipe1[2]->nom);
    for (int i=0;i<a-1;i++){
      printf(" ");
    }
    printf("@\n@");

    a=TAILLE_AFFICHAGE-1;//4
    for (int i=0;i<3;i++){
      printf(" ");
      a--;
    }
    printf("%d/%d",Equipe1[0]->pv,Equipe1[0]->pvmax);
    a=a-1-compter_chiffre(Equipe1[0]->pv)-compter_chiffre(Equipe1[0]->pvmax);
    for(int i=0;i<33-1-compter_chiffre(Equipe1[0]->pv)-compter_chiffre(Equipe1[0]->pvmax);i++){
      printf(" ");a--;
    }
    printf("%d/%d",Equipe1[1]->pv,Equipe1[1]->pvmax);
    a=a-1-compter_chiffre(Equipe1[1]->pv)-compter_chiffre(Equipe1[1]->pvmax);
    for(int i=0;i<33-1-compter_chiffre(Equipe1[1]->pv)-compter_chiffre(Equipe1[1]->pvmax);i++){
      printf(" ");a--;
    }
    printf("%d/%d",Equipe1[2]->pv,Equipe1[2]->pvmax);
    a=a-1-compter_chiffre(Equipe1[2]->pv)-compter_chiffre(Equipe1[2]->pvmax);
    for (int i=0;i<a-1;i++){
      printf(" ");
    }
    printf("@\n@");

    a=TAILLE_AFFICHAGE-1;//5
    for (int i=0;i<3;i++){
      printf(" ");
      a--;
    }
    printf("%d/100",Equipe1[0]->barre_action);
    a=a-1-compter_chiffre(Equipe1[0]->barre_action)-3;
    for(int i=0;i<33-1-compter_chiffre(Equipe1[0]->barre_action)-3;i++){
      printf(" ");a--;
    }
    printf("%d/100",Equipe1[1]->barre_action);
    a=a-1-compter_chiffre(Equipe1[1]->barre_action)-3;
    for(int i=0;i<33-1-compter_chiffre(Equipe1[1]->barre_action)-3;i++){
      printf(" ");a--;
    }
    printf("%d/100",Equipe1[2]->barre_action);
    a=a-1-compter_chiffre(Equipe1[2]->barre_action)-3;
    for(int i=0;i<a-1;i++){
      printf(" ");
    }
    printf("@\n@");

    a=TAILLE_AFFICHAGE-1;//6
    for (int i=0;i<3;i++){
      printf(" ");
      a--;
    }
    afficher_effet(Equipe1[0]);
    a=a-Equipe1[0]->nbr_effet_actif*2;
    for(int i=0;i<33-(Equipe1[0]->nbr_effet_actif*2);i++){
      printf(" ");
      a--;
    }
    afficher_effet(Equipe1[1]);
    a=a-Equipe1[1]->nbr_effet_actif*2;
    for(int i=0;i<33-(Equipe1[1]->nbr_effet_actif*2);i++){
      printf(" ");
      a--;
    }
    afficher_effet(Equipe1[2]);
    a=a-(Equipe1[2]->nbr_effet_actif*2);
    for(int i=0;i<a-1;i++){
      printf(" ");
    }
    printf("@\n@");

    for (int i=0;i<TAILLE_AFFICHAGE-2;i++){//7
      printf(" ");
    }
    printf("@\n");

    for (int i=0;i<TAILLE_AFFICHAGE;i++){//8
      printf("@");
    }
    printf("\n\n");

  printf("@[%s]",equipe2_Nom);//1
  for (int i=0;i<TAILLE_AFFICHAGE-3-strlen(equipe2_Nom);i++){
    printf("@");
  }
  printf("\n@");

  for (int i=0;i<TAILLE_AFFICHAGE-2;i++){//2
    printf(" ");
  }
  printf("@\n@");

  a=TAILLE_AFFICHAGE-1;//3
  for (int i=0;i<3;i++){
    printf(" ");
    a--;
  }
  printf("%s [1]",Equipe2[0]->nom);
  a=a-4-strlen(Equipe2[0]->nom);
  for(int i=0;i<33-4-strlen(Equipe2[0]->nom);i++){
    printf(" ");a--;
  }
  printf("%s [2]",Equipe2[1]->nom);
  a=a-4-strlen(Equipe2[1]->nom);
  for(int i=0;i<33-4-strlen(Equipe2[1]->nom);i++){
    printf(" ");a--;
  }
  printf("%s [3]",Equipe2[2]->nom);
  a=a-4-strlen(Equipe2[2]->nom);
  for (int i=0;i<a-1;i++){
    printf(" ");
  }
  printf("@\n@");

  a=TAILLE_AFFICHAGE-1;//4
  for (int i=0;i<3;i++){
    printf(" ");
    a--;
  }
  printf("%d/%d",Equipe2[0]->pv,Equipe2[0]->pvmax);
  a=a-1-compter_chiffre(Equipe2[0]->pv)-compter_chiffre(Equipe2[0]->pvmax);
  for(int i=0;i<33-1-compter_chiffre(Equipe2[0]->pv)-compter_chiffre(Equipe2[0]->pvmax);i++){
    printf(" ");a--;
  }
  printf("%d/%d",Equipe2[1]->pv,Equipe2[1]->pvmax);
  a=a-1-compter_chiffre(Equipe2[1]->pv)-compter_chiffre(Equipe2[1]->pvmax);
  for(int i=0;i<33-1-compter_chiffre(Equipe2[1]->pv)-compter_chiffre(Equipe2[1]->pvmax);i++){
    printf(" ");a--;
  }
  printf("%d/%d",Equipe2[2]->pv,Equipe2[2]->pvmax);
  a=a-1-compter_chiffre(Equipe2[2]->pv)-compter_chiffre(Equipe2[2]->pvmax);
  for (int i=0;i<a-1;i++){
    printf(" ");
  }
  printf("@\n@");

  a=TAILLE_AFFICHAGE-1;//5
  for (int i=0;i<3;i++){
    printf(" ");
    a--;
  }
  printf("%d/100",Equipe2[0]->barre_action);
  a=a-1-compter_chiffre(Equipe2[0]->barre_action)-3;
  for(int i=0;i<33-1-compter_chiffre(Equipe2[0]->barre_action)-3;i++){
    printf(" ");a--;
  }
  printf("%d/100",Equipe2[1]->barre_action);
  a=a-1-compter_chiffre(Equipe2[1]->barre_action)-3;
  for(int i=0;i<33-1-compter_chiffre(Equipe2[1]->barre_action)-3;i++){
    printf(" ");a--;
  }
  printf("%d/100",Equipe2[2]->barre_action);
  a=a-1-compter_chiffre(Equipe2[2]->barre_action)-3;
  for(int i=0;i<a-1;i++){
    printf(" ");
  }
  printf("@\n@");

  a=TAILLE_AFFICHAGE-1;//6
  for (int i=0;i<3;i++){
    printf(" ");
    a--;
  }
  afficher_effet(Equipe2[0]);
  a=a-Equipe2[0]->nbr_effet_actif*2;
  for(int i=0;i<33-(Equipe2[0]->nbr_effet_actif*2);i++){
    printf(" ");
    a--;
  }
  afficher_effet(Equipe2[1]);
  a=a-Equipe2[1]->nbr_effet_actif*2;
  for(int i=0;i<33-(Equipe2[1]->nbr_effet_actif*2);i++){
    printf(" ");
    a--;
  }
  afficher_effet(Equipe2[2]);
  a=a-(Equipe2[2]->nbr_effet_actif*2);
  for(int i=0;i<a-1;i++){
    printf(" ");
  }
  printf("@\n@");

  for (int i=0;i<TAILLE_AFFICHAGE-2;i++){//7
    printf(" ");
  }
  printf("@\n");

  for (int i=0;i<TAILLE_AFFICHAGE;i++){//8
    printf("@");
  }
  printf("\n");
  
  a=TAILLE_AFFICHAGE-1;//8
    for (int i=0;i<3;i++){
      printf(" ");
      a--;
    }
  printf("\n");

    printf("-----------------%s--------------------\n",attaquant->nom);
    a=a-strlen(attaquant->nom);
    for (int i=0;i<a-1;i++){
      printf(" ");
    }

    printf("\n");
  
    a=TAILLE_AFFICHAGE-1;//9
    for (int i=0;i<5;i++){
      printf(" ");
      a--;
    }
    printf("[1] ");
    a=a-4;
    printf("%s",attaquant->competence[0]->nom);
    a=a-strlen(attaquant->competence[0]->nom);
    for (int i=0;i<a-1;i++){
      printf(" ");
    }

    printf("\n");
    a=TAILLE_AFFICHAGE-1;//10
    for (int i=0;i<7;i++){
      printf(" ");
      a--;
    }
    printf("%s",attaquant->competence[0]->description);
        if(attaquant->competence[0]->tour_recharge_restant != 0){
    for (int i=0 ; i<9;i++){
      printf(" ");
    }
    printf("[tour de recharge : %d]", attaquant->competence[0]->tour_recharge_restant);}
    a=a-strlen(attaquant->competence[0]->description);
    for (int i=0;i<a-1;i++){
      printf(" ");
    }
    printf("\n");

    a=TAILLE_AFFICHAGE-1;//11
    for (int i=0;i<5;i++){
      printf(" ");
      a--;
    }
    printf("[2] ");
    a=a-4;
    printf("%s",attaquant->competence[1]->nom);
        if(attaquant->competence[1]->tour_recharge_restant != 0){
    for (int i=0 ; i<9;i++){
      printf(" ");
    }
    printf("[tour de recharge : %d]", attaquant->competence[1]->tour_recharge_restant);}
    a=a-strlen(attaquant->competence[1]->nom);
    for (int i=0;i<a-1;i++){
      printf(" ");
    }

    printf("\n");
    a=TAILLE_AFFICHAGE-1;//12
    for (int i=0;i<7;i++){
      printf(" ");
      a--;
    }
    printf("%s",attaquant->competence[1]->description);
    a=a-strlen(attaquant->competence[1]->description);
    for (int i=0;i<a-1;i++){
      printf(" ");
    }
    printf("\n");

    a=TAILLE_AFFICHAGE-1;//13
    for (int i=0;i<5;i++){
      printf(" ");
      a--;
    }
    printf("[3] ");
    a=a-4;
    printf("%s",attaquant->competence[2]->nom);
    if(attaquant->competence[2]->tour_recharge_restant != 0){
    for (int i=0 ; i<9;i++){
      printf(" ");
    }
    printf("[tour de recharge : %d]", attaquant->competence[2]->tour_recharge_restant);}
    a=a-strlen(attaquant->competence[2]->nom);
    for (int i=0;i<a-1;i++){
      printf(" ");
    }

    printf("\n");
    a=TAILLE_AFFICHAGE-1;//14
    for (int i=0;i<7;i++){
      printf(" ");
      a--;
    }
    printf("%s",attaquant->competence[2]->description);
    a=a-strlen(attaquant->competence[2]->description);
    for (int i=0;i<a-1;i++){
      printf(" ");
    }
    printf("\n");

    a=TAILLE_AFFICHAGE-1;//15
    for (int i=0;i<5;i++){
      printf(" ");
      a--;
    }
    printf("[4] ");
    a=a-4;
    printf("%s",attaquant->competence[3]->nom);
        if(attaquant->competence[3]->tour_recharge_restant != 0){
    for (int i=0 ; i<9;i++){
      printf(" ");
    }
    printf("[tour de recharge : %d]", attaquant->competence[3]->tour_recharge_restant);}
    a=a-strlen(attaquant->competence[3]->nom);
    for (int i=0;i<a-1;i++){
      printf(" ");
    }

    printf("\n");
    a=TAILLE_AFFICHAGE-1;//16
    for (int i=0;i<7;i++){
      printf(" ");
      a--;
    }
    printf("%s",attaquant->competence[3]->description);
    a=a-strlen(attaquant->competence[3]->description);
}

// Fonction qui retourne le combattant vivant avec la plus grande valeur de "barre_action" parmi les deux équipes. Cette fonction est utilisée pour déterminer quel combattant jouera en premier lors du tour.

Combattant* plus_rapide(Combattant* equipe1[],Combattant* equipe2[]){
  Combattant c;
  int i;
  c.barre_action=-1;
  if (vivant(equipe1[0])&&equipe1[0]->barre_action>c.barre_action){
    c.barre_action=equipe1[0]->barre_action;
    i=0;
  }
  if (vivant(equipe2[0])&&equipe2[0]->barre_action>c.barre_action){
    c.barre_action=equipe2[0]->barre_action;
    i=1;
  }
  if (vivant(equipe1[1])&&equipe1[1]->barre_action>c.barre_action){
    c.barre_action=equipe1[1]->barre_action;
    i=2;
  }
  if (vivant(equipe2[1])&&equipe2[1]->barre_action>c.barre_action){
    c.barre_action=equipe2[1]->barre_action;
    i=3;
  }
  if (vivant(equipe1[2])&&equipe1[2]->barre_action>c.barre_action){
    c.barre_action=equipe1[2]->barre_action;
    i=4;
  }
  if (vivant(equipe2[2])&&equipe2[2]->barre_action>c.barre_action){
    c.barre_action=equipe2[2]->barre_action;
    i=5;
  }
  switch (i){
    case 0:
      return equipe1[0];
      break;
    case 1:
      return equipe2[0];
      break;
    case 2:
      return equipe1[1];
      break;
    case 3:
      return equipe2[1];
      break;
    case 4:
      return equipe1[2];
      break;
    case 5:
      return equipe2[2];
      break;
  }
}

void combat(Combattant* equipe1[],Combattant* equipe2[],int mode,int difficulte,char* equipe1_Nom,char* equipe2_Nom){ //fonction qui lance une boucle jusqu'à ce que le combat s'arrête
int tic=0;
Combattant* rapide;
while ((equipe1[0]->pv>0 || equipe1[1]->pv>0 || equipe1[2]->pv>0) && (equipe2[0]->pv>0 || equipe2[1]->pv>0 || equipe2[2]->pv>0)&& tic<NOMBRE_TIC_MAXIMUM){
   maj_tous(equipe1, equipe2);
  rapide=plus_rapide(equipe1,equipe2);
   affiche_tous_perso(equipe1,equipe2,equipe1_Nom,equipe2_Nom,rapide);
    if (pret(rapide)){
      if(mode==1){
        if(difficulte==1){
          if(rapide->equipe==1){
            Competence* attaque = choisir_attaque(rapide);
            appliquer_technique(attaque, rapide, equipe1, equipe2, mode, difficulte);
          }
          else if(rapide->equipe==2){
            Competence* attaque = choisir_attaqueIAfaiblemoyen(rapide);
            appliquer_technique(attaque, rapide, equipe1, equipe2, mode, difficulte);
          }
        }
        else if(difficulte==2){
          if(rapide->equipe==1){
            Competence* attaque = choisir_attaque(rapide);
            appliquer_technique(attaque, rapide, equipe1, equipe2, mode, difficulte);
          }
          else if(rapide->equipe==2){
            Competence* attaque = choisir_attaqueIAfaiblemoyen(rapide);
            appliquer_technique(attaque, rapide, equipe1, equipe2, mode, difficulte);
          }
        }
        else if(difficulte==3){
          if(rapide->equipe==1){
            Competence* attaque = choisir_attaque(rapide);
            appliquer_technique(attaque, rapide, equipe1, equipe2, mode, difficulte);
          }
          else if(rapide->equipe==2){
            Competence* attaque = choisir_attaqueIAfort(rapide);
            appliquer_technique(attaque, rapide, equipe1, equipe2, mode, difficulte);
          }
        }
      }
      else if(mode==2){
        Competence* attaque = choisir_attaque(rapide);
      appliquer_technique(attaque, rapide, equipe1, equipe2, mode, difficulte);}
      }
    tic++;
    
}}

void afficher_vainqueur(Combattant* equipe1[], Combattant* equipe2[]) { //procédure qui sert à afficher le vainqueur à l'isssu du combat
    if(vivant(equipe1[0]) || vivant(equipe1[1]) || vivant(equipe1[2])) {
        printf("L'équipe 1 a gagné !\n");}
    else{
        printf("L'équipe 2 a gagné !\n");
    }
}

void liberer_equipes(Combattant* equipe1[], Combattant* equipe2[]) {//procédure qui sert à libérer la mémoire des 2 équipes
    for(int i = 0; i < TAILLE_EQUIPE; i++) {
        for(int j = 0; j < NOMBRE_COMPETENCE; j++) {
            free(equipe1[i]->competence[j]);
        }
        free(equipe1[i]);

        for(int j = 0; j < NOMBRE_COMPETENCE; j++) {
            free(equipe2[i]->competence[j]);
        }
        free(equipe2[i]);
    }
}

int main(){

  srand(time(NULL));// obligatoire pour utiliser la fonction rand dans les autres fonctions
  int mode=-1, difficulte;
  
Combattant* equipe1[TAILLE_EQUIPE];
Combattant* equipe2[TAILLE_EQUIPE];
char equipe1_Nom[TAILLE_NOM_EQUIPE];
char equipe2_Nom[TAILLE_NOM_EQUIPE];
  
while (mode!=0){//permet de choisir le mode de jeu( solo contre ordi ou multijoeur)
    printf("Choisissez un mode:\n[0] Arrêter le programme\n[1] Solo contre l'ordinateur\n[2] Multijoueur 1V1\n");
    mode=meilleur_scan();
    while (mode!=0  && mode!=1 && mode!=2){
        printf("Erreur de saisie.\n");
        mode=meilleur_scan();
            }

            if (mode==0){
                return 0;
            }

       if(mode==1){
            printf("Choisissez le nom de l'équipe 1.\n");
            scanf(" %32[^\n]", equipe1_Nom);
                int c;
    while ((c = getchar()) != '\n' && c != EOF);

            strcpy(equipe2_Nom,"Ordinateur");
            printf("Choisissez un niveau de difficulté:\n[1] faible\n[2] intermediaire\n[3] fort\n");
            difficulte=meilleur_scan();
            while (difficulte!=1 && difficulte!=2 && difficulte!=3){
                printf("Erreur de saisie.\n");
                difficulte=meilleur_scan();
            }
            if(difficulte==1){
            creer_equipeIA(equipe1,equipe2);
            combat(equipe1,equipe2,mode,difficulte,equipe1_Nom,equipe2_Nom);
            }
            else if(difficulte==2){
            creer_equipeIA(equipe1,equipe2);
            combat(equipe1,equipe2,mode,difficulte,equipe1_Nom,equipe2_Nom);
            }
            else if(difficulte==3){
            creer_equipeIA(equipe1,equipe2);
            combat(equipe1,equipe2,mode,difficulte,equipe1_Nom,equipe2_Nom);
            }
        }


        if(mode==2){
        printf("Choisissez le nom de l'équipe 1.\n");
        scanf(" %32[^\n]", equipe1_Nom);
            int c;
    while ((c = getchar()) != '\n' && c != EOF);

        printf("Choisissez le nom de l'équipe 2.\n");
        scanf(" %32[^\n]", equipe2_Nom);
            
    while ((c = getchar()) != '\n' && c != EOF);

            creer_equipe(equipe1,equipe2);
            combat(equipe1,equipe2,mode,difficulte,equipe1_Nom,equipe2_Nom);
        }


        afficher_vainqueur(equipe1,equipe2);
        liberer_equipes(equipe1,equipe2);
}

  return 0;
}
