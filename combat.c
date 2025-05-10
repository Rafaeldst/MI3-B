#include "projet.h"
#include "combat.h"


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


void verifier_erreur_fichier(FILE* fichier){
  if (fichier==NULL){
    printf("Erreur lors de l'ouverture du fichier.\n");
    exit(EXIT_FAILURE);}
}
  Combattant* choisir_cible(Combattant* lanceur,Combattant* equipe1[], Combattant* equipe2[], Competence* competence,int mode, int difficulte){
    if (mode!=1 && mode!=2){
        prinft("Erreur.\n");
        exit(EXIT_FAILURE);
    }
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

Competence* choisir_attaque(Combattant* combattant) {
    int choix;
    printf("\nchoisisser une attaque\n");
    choix=meilleur_scan();
     while (choix < 1 || choix > NOMBRE_COMPETENCE || combattant->competence[choix - 1]->tour_recharge_restant > 0){
    printf("erreur choisisser une attaque valide\n");
    choix=meilleur_scan();
     }
    return combattant->competence[choix - 1];
}

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

