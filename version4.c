#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NOMBRE_EFFET 6 //nombre de tous les effets possibles du jeu
#define TAILLE_EQUIPE 3
//acceleration protection regeneration invisibilite provocation et brulure
typedef struct{
  char nom[32];
  int valeur;
  char description[512];
  char type[20]; // soin dégat ..?
  char cible[20];
  int tour_actif;
  int tour_recharge;
  int tour_recharge_restant;
} Competence;

typedef struct{
  char nom[32];
  int tour_restant;
  int valeur;
} Effet;

typedef struct{
  char nom[50];
  int pv;
  int pvmax;
  int attaque;
  int defense;
  int agilite;
  int vitesse;
  int barre_action;// les personnages chargent une barre d'action pour attaquer grâce a leur vitesse comme dans raid shadow legends
  char categorie[32];
  Effet effet_special[NOMBRE_EFFET];
  int nbr_effet_actif;
  Competence* competence[4];
  int equipe;
}  Combattant;

void verifier_erreur_fichier(FILE* fichier){
  if (fichier==NULL){
    printf("Erreur lors de l'ouverture du fichier\n");
    exit(EXIT_FAILURE);}
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
  return pv_actuel-combattant->pv;
}

void degat_recharge(Combattant* combattant,int degat,Competence* competence){//fonction qui inflige les dégats à un personnage mais gère aussi le fait que le personnage meurt ou non
  int n=rand()%100;
  int pv_actuel=combattant->pv;
  if (combattant->agilite>=n){
    int degats_totaux=degat*(100-combattant->defense)/100;
    combattant->pv=combattant->pv-degats_totaux;
    if (combattant->pv<0){
      combattant->pv=0;}
  }
  competence->tour_recharge_restant=competence->tour_recharge;
}


void soin(Combattant* combattant,int soin){
  combattant->pv=combattant->pv+soin;
  if(combattant->pv > combattant->pvmax){
    combattant->pv=combattant->pvmax;}
}

void soin_recharge(Combattant* combattant,int soin,Competence* competence){
  combattant->pv=combattant->pv+soin;
  if(combattant->pv > combattant->pvmax){
    combattant->pv=combattant->pvmax;}
  competence->tour_recharge_restant=competence->tour_recharge;
}

void acceleration(Combattant* combattant, Competence* competence){
  Effet effet;
  strcpy(effet.nom, competence->type);
  effet.tour_restant=competence->tour_actif;
  combattant->effet_special[combattant->nbr_effet_actif]=effet;
  combattant->nbr_effet_actif++;
  combattant->vitesse=combattant->vitesse+competence->valeur;
  competence->tour_recharge_restant=competence->tour_recharge;
}

void protection(Combattant* combattant, Competence* competence){
  Effet effet;
  strcpy(effet.nom, competence->type);
  effet.tour_restant=competence->tour_actif;
  combattant->effet_special[combattant->nbr_effet_actif]=effet;
  combattant->nbr_effet_actif++;
  combattant->defense=combattant->defense+competence->valeur;
  competence->tour_recharge_restant=competence->tour_recharge;
}

void regeneration_brulure(Combattant* combattant, Competence* competence){
  Effet effet;
  strcpy(effet.nom, competence->type);
  effet.valeur=competence->valeur;
  effet.tour_restant=competence->tour_actif;
  combattant->effet_special[combattant->nbr_effet_actif]=effet;
  combattant->nbr_effet_actif++;
  competence->tour_recharge_restant=competence->tour_recharge;
}

void frappe_impitoyable(Combattant* combattant,Competence* competence){
  degat(combattant,competence->valeur+combattant->defense);
  competence->tour_recharge_restant=competence->tour_recharge;
}


void invisibilite_provocation(Combattant* combattant, Competence* competence){
  Effet effet;
  strcpy(effet.nom, competence->type);
  effet.tour_restant=competence->tour_actif;
  combattant->effet_special[combattant->nbr_effet_actif]=effet;
  combattant->nbr_effet_actif++;
  competence->tour_recharge_restant=competence->tour_recharge;
}

void soin_tous(Combattant* equipe[],Competence* competence){
  for (int i=0;i<3;i++){
          soin(equipe[i],competence->valeur);
      }
      competence->tour_recharge_restant=competence->tour_recharge;
  }

void degat_tous(Combattant* equipe[],Competence* competence){
    for (int i=0;i<3;i++){
        degat(equipe[i],competence->valeur);
    }
    competence->tour_recharge_restant=competence->tour_recharge;
}

void Vol_de_vie(Combattant* lanceur,Combattant* cible,Competence* competence){
  int degat_inflige=degat(cible,competence->valeur);
  soin(lanceur,degat_inflige/2);
}

void retirer_stats(Combattant* combattant,int index){
  if (strcmp(combattant->effet_special[index].nom,"Acceleration")==0){
    combattant->vitesse=combattant->vitesse-combattant->effet_special[index].valeur;
  }
  else if (strcmp(combattant->effet_special[index].nom,"Protection")==0){
    combattant->defense=combattant->defense-combattant->effet_special[index].valeur;
  }
}


void retirer_effet(Combattant* combattant,int index){
  for (; index < combattant->nbr_effet_actif - 1; index++){
        combattant->effet_special[index] = combattant->effet_special[index+ 1];
    }
    combattant->nbr_effet_actif--;
}

void effet_deja_actif(Combattant* combattant,Competence* competence,Effet effet){
  for (int i=0;i<combattant->nbr_effet_actif;i++){
    if (strcmp(combattant->effet_special[i].nom,effet.nom)==0){
      retirer_stats(combattant,i);
      combattant->effet_special[i]=effet;
    }
  }
}

void maj_vitesse(Combattant* combattant){
  combattant->barre_action=combattant->barre_action+combattant->vitesse;}

void maj_effet(Combattant* combattant){
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

void maj_recharge(Combattant* combattant){
    for (int i = 0; i < 4; i++) {
        if (combattant->competence[i]->tour_recharge_restant > 0) {
            combattant->competence[i]->tour_recharge_restant--;
        }
    }
}

int vivant(Combattant* combattant){
    if (combattant->pv==0){
  return 0;}return 1;}

void maj_tous_stats(Combattant* combattant){
    maj_recharge(combattant);
    maj_vitesse(combattant);
    maj_effet(combattant);
}
    
    void maj_tous(Combattant* Equipe1[],Combattant* Equipe2[]){
    for (int i=0;i<3;i++){
        if (vivant(Equipe1[i])){
            maj_tous_stats(Equipe1[i]);
        }
        if (vivant(Equipe2[i])){
            maj_tous_stats(Equipe2[i]);
        }    
}}

int charge(Combattant* combattant){
  if (combattant->barre_action<99){
return 0;}return 1;}

int pret(Combattant* combattant){
return (charge(combattant)&&vivant(combattant));}

int est_invisible(Combattant* combattant){
    for(int j=0;j<combattant->nbr_effet_actif;j++){
      if(strcmp(combattant->effet_special[j].nom,"Invisibilite")==0){
                return 1;
            }}
  return 0;
}

Combattant* choisir_cible(Combattant* lanceur,Combattant* equipe1[], Combattant* equipe2[], Competence* competence){
  int choix;
  if(lanceur->equipe==1){
    for(int i=0; i<3;i++){
        for(int j=0;j<=equipe2[i]->nbr_effet_actif;j++){
            if(strcmp(equipe2[i]->effet_special[j].nom,"Provocation")==0){
                return equipe2[i];
            }
        }
    }
    if(strcmp(competence->cible, "Ennemi") == 0){
    printf("Choisissez une cible :\n");
      do{
        scanf("%d",&choix);
      }while (choix<0 || choix>2 || est_invisible(equipe2[choix]) || !(vivant(equipe2[choix])));
      return equipe2[choix];}
    else if(strcmp(competence->cible, "Allié") ==0){
    printf("Choisissez une cible :\n");
    do{
      scanf("%d",&choix);
    }while(choix<0 || choix>2);
    return equipe1[choix];}
  }
  if(lanceur->equipe==2){
     for(int i=0; i<3;i++){
        for(int j=0;j<=equipe1[i]->nbr_effet_actif;j++){
            if(strcmp(equipe1[i]->effet_special[j].nom,"Provocation")==0){
                return equipe1[i];
            }
        }
    }
    if(strcmp(competence->cible, "Ennemi") == 0){
    printf("Choisissez une cible :\n");
      do{
        scanf("%d",&choix);
      }while (choix<0 || choix>2 || est_invisible(equipe1[choix]) || !(vivant(equipe1[choix])));
    return equipe1[choix];}
    else if(strcmp(competence->cible, "Allié") ==0){
    printf("Choisissez une cible :\n");
    do{
      scanf("%d",&choix);
    }while(choix<0 || choix>2);
    return equipe2[choix];}
  }
}

void appliquer_technique(Competence* competence,Combattant* lanceur, Combattant equipe1[], Combattant equipe2[]){//fonction qui va faire des if pour trouver si la technique est un degat,un soin, un endormissement... et applique la technique en conséquence
  Combattant* cible;
  printf("%s utilise %s !\n",lanceur->nom,competence->nom);
  if (strcmp(competence->type,"Degats")==0){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence);
    degat_recharge(cible,competence->valeur,competence);
  }
  else if (strcmp(competence->type,"Soin")==0){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence);
    soin_recharge(cible,competence->valeur,competence);
  }
  else if (strcmp(competence->type,"Acceleration")==0){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence);
    acceleration(cible,competence);
  }
  else if (strcmp(competence->type,"Protection")==0){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence);
    protection(cible,competence);
  }
  else if (strcmp(competence->type,"ProtectionSoiMeme")==0){
    protection(lanceur,competence);
  }
  else if ((strcmp(competence->type,"Regeneration")==0)||(strcmp(competence->type,"Brulure")==0)){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence);
    regeneration_brulure(cible,competence);
  }
  else if (strcmp(competence->type,"DegatsDef")==0){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence);
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
    invisibilite_provocation(lanceur,competence);
  }
  else if (strcmp(competence->type,"Invisibilite")==0){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence);
    invisibilite_provocation(lanceur,competence);
    degat(cible,competence->valeur);
  }
  else if (strcmp(competence->type,"VolDeVie")==0){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence);
    Vol_de_vie(lanceur,cible,competence);
  }
}


Combattant* charger_combattant(const char* fichier_combattant){ //fonction qui charge les données d'un combattant contenues dans un fichier
  Combattant* c = malloc(sizeof(Combattant));
  if (c == NULL) {
    printf("Erreur d'allocation mémoire pour Combattant\n");
    exit(EXIT_FAILURE);
  }
  c->nbr_effet_actif=0;
  FILE* fichier= fopen(fichier_combattant,"r");
  verifier_erreur_fichier(fichier);
  char competence1[100],competence2[100],competence3[100],competence4[100];
  fscanf(fichier,"%s %s %d %d %d %d %d %d %s %s %s %s",c->nom,c->categorie,c->pv,c->pvmax,c->attaque,c->defense,c->agilite,c->vitesse,competence1,competence2,competence3,competence4);
  c->competence[0]=charger_competence(competence1);
  c->competence[1]=charger_competence(competence2);
  c->competence[2]=charger_competence(competence3);
  c->competence[3]=charger_competence(competence4);
  fclose(fichier);
  return c;
}

Competence* charger_competence(const char* fichier_competence){//fonction qui charge les données d'une technique contenues dans un fichier
 Competence* c = malloc(sizeof(Competence));
  if (c == NULL) {
    printf("Erreur d'allocation mémoire pour Competence\n");
    exit(EXIT_FAILURE);
  }
  FILE* fichier=fopen(fichier_competence,"r");
  verifier_erreur_fichier(fichier);
  fgets(c->nom,32,fichier);
  fgets(c->description,256,fichier);
  fscanf(fichier,"%s %s %d %d %d",c->type,c->cible,c->valeur,c->tour_actif,c->tour_recharge);
  fclose(fichier);
  return c;
}

const char* personnages_disponibles[8] = {
    "Zeus.txt", "Poseidon.txt", "Hades.txt", "Hermes.txt",
    "Apollo.txt", "Ares.txt", "Athena.txt", "Hera.txt"
};
// pour suivre quels personnages  sont déjà pris
int deja_choisi[8] = {0};

void afficher_personnages_disponibles() {
    printf(" Les Personnages disponibles sont:\n");
    for (int i = 0; i < 8; i++) {
        if (deja_choisi[i] == 0) {
            printf("%d. %s\n", i + 1, personnages_disponibles[i]);
        }
    }
}
void creer_equipe(Combattant* equipe1[],Combattant* equipe2[]){ //fonction qui permet aux 2 joueurs de choisir ses personnage et mets à jour les équipes

    int choix, i;

    for (i = 0; i < 3; i++) {
        // Joueur 1
        afficher_personnages_disponibles();
        printf("Joueur 1 - Choisissez un personnage (1-8) : \n");
        scanf("%d", &choix);
        while (choix < 1 || choix > 8 || deja_choisi[choix - 1] != 0) {
            printf("Choix invalide ou déjà pris. Réessayez : \n");
            scanf("%d", &choix);
        }
      equipe1[i] = charger_combattant(personnages_disponibles[choix - 1]);
      equipe1[i]->equipe=1;
      deja_choisi[choix - 1] = 1;  // empeche ce personnage d'être pris 
        // Joueur 2
        afficher_personnages_disponibles();
        printf("Joueur 2 - Choisissez un personnage (1-8) : \n ");
        scanf("%d", &choix);
      
        while (choix < 1 || choix > 8 ||  deja_choisi[choix - 1] != 0) {
            printf("Choix invalide ou déjà pris. Réessayez : ");
            scanf("%d", &choix);
        }
        deja_choisi[choix - 1] = 1;
      equipe2[i] = charger_combattant(personnages_disponibles[choix - 1]);
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
    printf("Équipes créées avec succès ! DEBUT DE LA PARTIE \n");
}

  Competence* choisir_attaque(Combattant* combattant) {
    int choix;
    printf("Choisissez une attaque pour %s :\n", combattant->nom);

    for (int i = 0; i < 4; i++) {
        printf("%d. %s - %s", i + 1, combattant->competence[i]->nom, combattant->competence[i]->description);
        if (combattant->competence[i]->tour_recharge_restant > 0) {
            printf(" (recharge %d tours restants)", combattant->competence[i]->tour_recharge_restant);
        }
        printf("\n");
    }

    do {
        printf("Entrez un nombre entre 1 et 4 correspondant à l'attaque de votre personnage :\n");
        scanf("%d", &choix);

        if (choix < 1 || choix > 4) {
            printf("Choix invalide.\n");
        } else if (combattant->competence[choix - 1]->tour_recharge_restant > 0) {
            printf("Cette compétence est encore en recharge.\n");
        }

    } while (choix < 1 || choix > 4 || combattant->competence[choix - 1]->tour_recharge_restant > 0);

    return combattant->competence[choix - 1];
}

void affiche_tous_perso(Combattant Equipe1[],Combattant Equipe2[]){
  for (int i=0;i<3;i++){
    printf("\n");
    printf("nom %d %s\n",i,Equipe1[i].nom);
    printf("%d\n",Equipe1[i].pv);
    printf("%d\n",Equipe1[i].attaque);
    printf("%d\n",Equipe1[i].defense);
    printf("%d\n",Equipe1[i].vitesse);
    printf("%d\n",Equipe1[i].barre_action);
  }
  for (int i=0;i<3;i++){
    printf("\n");
    printf("nom %d %s\n",i,Equipe1[i].nom);
    printf("%d\n",Equipe2[i].pv);
    printf("%d\n",Equipe2[i].attaque);
    printf("%d\n",Equipe2[i].defense);
    printf("%d\n",Equipe2[i].vitesse);
    printf("%d\n",Equipe2[i].barre_action);
  }
}


void combat(Combattant* equipe1[],Combattant* equipe2[]){ //fonction qui lance une boucle jusqu'à ce que le combat s'arrête
int tour=0;
while ((equipe1[0]->pv>0 || equipe1[1]->pv>0 || equipe1[2]->pv>0) && (equipe2[0]->pv>0 || equipe2[1]->pv>0 || equipe2[2]->pv>0) || tour < 100){
   maj_tous(equipe1, equipe2);
  affiche_tous_perso(equipe1,equipe2);
  if (tour % 2 == 0) { // Équipe 1 joue
            for (int i = 0; i < 3; i++) {
                if (pret(equipe1[i])) {
                    Competence* attaque = choisir_attaque(equipe1[i]);
                    appliquer_technique(attaque, equipe1[i], equipe1, equipe2);
                }
            }
        } else { // Équipe 2 joue
            for (int i = 0; i < 3; i++) {
                if (pret(equipe2[i])) {
                    Competence* attaque = choisir_attaque(equipe2[i]);
                    appliquer_technique(attaque, equipe2[i], equipe1, equipe2);
                }
            }
        }

        tour++;
    }
}

int main(){
  srand(time(NULL));
  Combattant Equipe1[TAILLE_EQUIPE];
  Combattant Equipe2[TAILLE_EQUIPE];
  creer_equipe(Equipe1,Equipe2);
  combat(Equipe1,Equipe2);
  
  return 0;
}
