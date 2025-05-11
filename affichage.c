#include "projet.h"
#include "affichage.h"
#include "effet.h"

void affiche_tous_perso(Combattant* Equipe1[],Combattant* Equipe2[],char* equipe1_Nom,char* equipe2_Nom,Combattant* attaquant){ //procédure permettant d'afficher tous les combattants ainsi que leurs point de vie et leur barre d'action
  int a;
  printf("\n\n\n");
  printf("@[%s]",equipe1_Nom);//1
  for (size_t i=0;i<TAILLE_AFFICHAGE-3-strlen(equipe1_Nom);i++){
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
  for(size_t i=0;i<33-4-strlen(Equipe1[0]->nom);i++){
    printf(" ");a--;
  }
  printf("%s [2]",Equipe1[1]->nom);
  a=a-4-strlen(Equipe1[1]->nom);
  for(size_t i=0;i<33-4-strlen(Equipe1[1]->nom);i++){
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
  for (size_t i=0;i<TAILLE_AFFICHAGE-3-strlen(equipe2_Nom);i++){
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
  for(size_t i=0;i<33-4-strlen(Equipe2[0]->nom);i++){
    printf(" ");a--;
  }
  printf("%s [2]",Equipe2[1]->nom);
  a=a-4-strlen(Equipe2[1]->nom);
  for(size_t i=0;i<33-4-strlen(Equipe2[1]->nom);i++){
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
  printf("\n");
  int bool;
  bool=0;
  if (attaquant->equipe==1){
    if (vivant(Equipe2[0]) || vivant(Equipe2[1]) || vivant(Equipe2[2])){
      bool=1;}
    }
  else{
    if (vivant(Equipe1[0]) || vivant(Equipe1[1]) || vivant(Equipe1[2])){
      bool=1;}
    }
  if (pret(attaquant)&&(bool==1)){
    printf("-----------------%s--------------------\n",attaquant->nom);
    printf("\n");

    printf("[1] ");
    printf("%s",attaquant->competence[0]->nom);
    printf("\n");
    printf("%s",attaquant->competence[0]->description);
    if(attaquant->competence[0]->tour_recharge_restant != 0){
      printf("  [tour de recharge : %d]", attaquant->competence[0]->tour_recharge_restant);}
  
    printf("\n");
  
    printf("[2] ");
    printf("%s",attaquant->competence[1]->nom);
    if(attaquant->competence[1]->tour_recharge_restant != 0){
      printf("  [tour de recharge : %d]", attaquant->competence[1]->tour_recharge_restant);}
    printf("\n");
    printf("%s",attaquant->competence[1]->description);
  
    printf("\n");
  
    printf("[3] ");
    printf("%s",attaquant->competence[2]->nom);
    if(attaquant->competence[2]->tour_recharge_restant != 0){
      printf("  [tour de recharge : %d]", attaquant->competence[2]->tour_recharge_restant);}
    printf("\n");
    printf("%s",attaquant->competence[2]->description);

    printf("\n");
  
    printf("[4] ");
    printf("%s",attaquant->competence[3]->nom);
    if(attaquant->competence[3]->tour_recharge_restant != 0){
      printf("  [tour de recharge : %d]", attaquant->competence[3]->tour_recharge_restant);}
    printf("\n");
    printf("%s",attaquant->competence[3]->description);

    
    printf("\n");
}}

void afficher_effet(Combattant* combattant){ //affiche les effets actifs du combattant
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

void afficher_personnages_disponibles(int deja_choisi[], char personnages_disponibles[NOMBRE_PERSO][TAILLE_NOM_PERSO]) { //procédure qui permet d'afficher les personnage encore disponible
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
