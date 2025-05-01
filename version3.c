#include <stdlib.h>

#define NOMBRE_EFFET 7//nombre de tous les effets possibles du jeu
#define TAILLE_EQUIPE 3

typedef struct{
  char nom[32];
  int valeur;
  char description[512];
  char type[20]; // soin dégat ..?
  char cible[20];
  int tour_actif;
  int tour_recharge;
  char effet_special[NOMBRE_EFFET];
} Competence;

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
  Competence competence[4];
}  Combattant;


void verifier_erreur_fichier(FILE* fichier){
  if (fichier==NULL){
    printf("Erreur lors de l'ouverture du fichier\n");
    exit(EXIT_FAILURE);}
}

void degat(Combattant combattant,int degat){//fonction qui inflige les dégats à un personnage mais gère aussi le fait que le personnage meurt ou non
  combattant.pv=combattant.pv-degat;
}

void appliquer_technique()//fonction qui va faire des if pour trouver si la technique est un degat,un soin, un endormissement... et applique la technique en conséquence

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
  fclose(f);
  return c;
}

char* personnages_disponibles[8] = {
    "Zeus", "Poseidon", "Hades", "Hermes",
    "Apollo", "Ares", "Athena", "Hera"
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

void combat() //fonction qui lance une boucle jusqu'à ce que le combat s'arrête


int main(){
  Combattant Equipe1[3];
  Combattant Equipe2[3];
  
    
  return 0;
}
