#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NOMBRE_EFFET 7//nombre de tous les effets possibles du jeu
#define TAILLE_EQUIPE 3
//degat soin bouclier_solide
//bouclier se protege et donne 20% des dégats pdt 2 tours
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
  Competence competence[4];
  int equipe;
}  Combattant;

void verifier_erreur_fichier(FILE* fichier){
  if (fichier==NULL){
    printf("Erreur lors de l'ouverture du fichier\n");
    exit(EXIT_FAILURE);}
}

void degat(Combattant* combattant,int degat){//fonction qui inflige les dégats à un personnage mais gère aussi le fait que le personnage meurt ou non
  int n=rand()%100;
  if (combattant.agilite>=n){
  int degats_totaux=degat*(100-combattant->defense)/100;
  combattant->pv=combattant->pv-degats_totaux;
  if (combattant->pv<0){
    combattant->pv=0;}
}}

void soin(Combattant* combattant,int soin){
  combattant->pv=combattant->pv+soin;
  if(combattant->pv > combattant->pvmax){
    combattant->pv=combattant->pvmax;}
}

void riposte_brulante(Combattant* combattant, Competence* competence){
  Effet effet;
  strcpy(effet.nom, competence->nom);
  effet.tour_restant=2;
  combattant->effet_special[nbr_effet_actif]=effet;
  combattant->nbr_effet_actif++;
  degat(combattant,competence->valeur);
  competence->tour_recharge_restant=competence->tour_recharge;
}

void acceleration(Combattant* combattant, Competence* competence){
  Effet effet;
  strcpy(effet.nom, competence->nom);
  effet.tour_restant=competence->tour_actif;
  combattant->effet_special[nbr_effet_actif]=effet;
  combattant->nbr_effet_actif++;
  combattant->vitesse=combattant->vitesse+competence->valeur;
  competence->tour_recharge_restant=competence->tour_recharge;
}

void protection(Combattant* combattant, Competence* competence){
  Effet effet;
  strcpy(effet.nom, competence->nom);
  effet.tour_restant=competence->tour_actif;
  combattant->effet_special[nbr_effet_actif]=effet;
  combattant->nbr_effet_actif++;
  combattant->vitesse=combattant->defense+competence->valeur;
  competence->tour_recharge_restant=competence->tour_recharge;
}

void benediction_divine(Combattant* combattant, Competence* competence){
  Effet effet;
  strcpy(effet.nom, competence->nom);
  effet.tour_restant=3;
  combattant->effet_special[nbr_effet_actif]=effet;
  combattant->nbr_effet_actif++;
  soin(combattant, competence->valeur);
  competence->tour_recharge_restant=competence->tour_recharge;
}

void frappe_impitoyable(Combattant* combattant,Competence* competence){
  degat(combattant,competence->valeur+combattant->defense);
  competence->tour_recharge_restant=competence->tour_recharge;
}

void lumiere_soleil(Combattant* combattant1,Combattant* combattant2,Competence* competence){
  soin(combattant1,competence->valeur);
  soin(combattant2,competence->valeur);
  competence->tour_recharge_restant=competence->tour_recharge;
}

void tsunami(Combattant equipe[],Competence* competence){
    for (int i=0;i<3;i++){
        degat(equipe[i],competence->valeur);
    }
    competence->tour_recharge_restant=competence->tour_recharge;
}

void appliquer_technique(Competence* competence,Combattant* lanceur, Combattant equipe1[], Combattant equipe2[]){//fonction qui va faire des if pour trouver si la technique est un degat,un soin, un endormissement... et applique la technique en conséquence
  combattant cible;
  cible=choisir_cible(lanceur, equipe1, equipe2, competence);
  printf("%s utilise %s sur %s !\n",lanceur->nom,competence->nom,cible.nom);
  if (strcmp(competence.type,"Degats")==0){
    degat(cible,competence.valeur);
  }
  if (strcmp(competence.type,"Soin")==0){
    soin(cible,competence.valeur);
  }
  if (strcmp(competence.type,"Acceleration")==0){
    acceleration(cible,competence);
  }
  if (strcmp(competence.type,"Protection")==0){
    protection(cible,competence);
  
  
  }
}


Combattant charger_combattant(const char* fichier_combattant){ //fonction qui charge les données d'un combattant contenues dans un fichier
  Combattant c;
  FILE* fichier= fopen(fichier_combattant,"r");
  verifier_erreur_fichier(fichier,fichier_combattant);
  char competence1[100],competence2[100],competence3[100],competence4[100];
  fscanf(fichier,"%s %s %d %d %d %d %d %d %s %s %s %s",c.nom,c.categorie,&c.pv,&c.pvmax,&c.attaque,&c.defense,&c.agilite,&c.vitesse,competence1,competence2,competence3,competence4);
  c.competence[0]=charger_competence(competence1);
  c.competence[1]=charger_competence(competence2);
  c.competence[2]=charger_competence(competence3);
  c.competence[3]=charger_competence(competence4);
  fclose(fichier);
  return c;
}

Competence charger_competence(const char* fichier_competence){//fonction qui charge les données d'une technique contenues dans un fichier
  Competence c;
  FILE* fichier=fopen(fichier_competence,"r");
  verifier_erreur_fichier(fichier,fichier_competence);
  fgets(c.nom,32,fichier);
  fgets(c.description,256,fichier);
  fscanf(fichier,%s %s %d %d %d,c.nom,c.cible,c.valeur,c.tour_actif,c.tour_recharge);
  fclose(fichier);
  return c;
}

char* personnages_disponibles[8] = {
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
void creer_equipe(Combattant* Equipe1,Combattant* Equipe2){ //fonction qui permet aux 2 joueurs de choisir ses personnage et mets à jour les équipes

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
    }
  
 printf(" Composition des équipes:\n");

printf("Équipe 1 : :\n");
for (int j = 0; j < TAILLE_EQUIPE; j++) {
    printf("%s", equipe1[j].nom);
    if (j < TAILLE_EQUIPE - 1) printf(" | ");
}
printf("\n");

printf("Équipe 2 : ");
for (int j = 0; j < TAILLE_EQUIPE; j++) {
    printf("%s", equipe2[j].nom);
    if (j < TAILLE_EQUIPE - 1) printf(" | ");
}
printf("\n");
    printf("Équipes créées avec succès ! DEBUT DE LA PARTIE \n");
}

void maj_vitesse(Combattant* combattant){
  combattant->barre_action=combattant->barre_action+combattant->vitesse;}

void maj_effet(Combattant* combattant) {
    for (int i = 0; i < combattant->nbr_effet_actif; i++) {
        combattant->effet_special[i].tour_restant--;
        if (combattant->effet_special[i]==0){
          retirer_stats(combattant,i);  
          retirer_effet(combattant,i);
            i--;
        }
    }
}

void retirer_stats(Combattant* combattant,int index){
  if (combattant.effet_special[index]=="
}

void retirer_effet(Combattant* combattant,int index){
  for (; index < combattant->nbr_effet_actif - 1; index++){
        combattant->effet_special[index] = combattant->effet_special[index+ 1];
    }
    combattant->nbr_effet_actif--;
}

void maj_recharge(Combattant* combattant){
    for (int i = 0; i < 4; i++) {
        if (combattant->competence[i].tour_recharge > 0) {
            combattant->competence[i].tour_recharge--;
        }
    }
}

void maj_tous_stats(Combattant* combattant){
    maj_recharge(combattant);
    maj_vitesse(combattant);
    maj_effet(combattant);
}
    
    void maj_tous(Combattant Equipe1[],Combattant Equipe2[]){
    for (int i=0;i<3;i++){
        if vivant(Equipe2[i]){
            maj_tous_stats(Equipe2[i]);
        }
        if vivant(Equipe2[i]){
            maj_tous_stats(Equipe2[i]);
        }    
}

int charge(Combattant combattant){
  if (combattant.barre_action<99){
return 0;}return 1;}

int vivant(Combattant combattant){
  if (combattant.pv==0){
return 0;}return 1;}

int pret(Combattant combattant){
return (charge(combattant)&&vivant(combattant));}

int choisir_attaque(){
  printf("choisir une attaque");
  int n;
  scanf("%d",&n);
  while (n<1 || n>4){
    scanf("%d",&n);}
  return n;}

combattant choisir_cible(Combattant lanceur,Combattant equipe1[], Combattant equipe2[], Competence* competence){
  int choix;
  if(lanceur.equipe==1){
    if(strcmp(competence->cible, "Ennemi") == 0){
    printf("Choisissez une cible :\n");
    do{
      scanf("%d",&choix);
    }while(choix<1 || choix>3)
    return equipe2[choix];}
    else if(strcmp(competence->cible, "Allié") ==0){
    printf("Choisissez une cible :\n");
    do{
      scanf("%d",&choix);
    }while(choix<1 || choix>3)
    return equipe1[choix];}
  }
  if(lanceur.equipe==2){
    if(strcmp(competence->cible, "Ennemi") == 0){
    printf("Choisissez une cible :\n");
    do{
      scanf("%d",&choix);
    }while(choix<1 || choix>3)
    return equipe1[choix];}
    else if(strcmp(competence->cible, "Allié") ==0){
    printf("Choisissez une cible :\n");
    do{
      scanf("%d",&choix);
    }while(choix<1 || choix>3)
    return equipe2[choix];}
  }
}

void combat(Combattant Equipe1[3],Combattant Equipe2[3]){ //fonction qui lance une boucle jusqu'à ce que le combat s'arrête
int tour=0;
Competence attaque_choisi;
Combattant cible_choisi;
while ((Equipe1[0].pv>0 && Equipe1[1].pv>0 && Equipe1[2].pv>0) && (Equipe2[0].pv>0 && Equipe2[1].pv>0 && Equipe2[2].pv>0) && tour < 500){
  maj_tous(equipe1,equipe2);
  if (tour%2==0){// joueur 1
    for (int i=0;i<3;i++){
      if (pret(equipe1[i]){
      attaque_choisi=choisir_attaque();
      cible_choisi=choisir_cible(equipe1[i],equipe1,equipe2,attaque_choisi);}}
    
  }
  

  else{//joueur 2
    for (int i=0;i<3;i++){
      if (pret(equipe2[i]){
      attaque_choisi=choisir_attaque();
      cible_choisi=choisir_cible(equipe2[i],equipe1,equipe2,attaque_choisi);}}
    
  }
  tour++;
}
}

int main(){
  srand(time(NULL));
  Combattant Equipe1[3];
  Combattant Equipe2[3];
  
    
  return 0;
}
