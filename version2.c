#include <stdlib.h>

#define NOMBRE_EFFET 7//nombre de tous les effets possibles du jeu
#define TAILLE_EQUIPE 3

typedef struct{
  char nom[32];
  int valeur;
  char description[512];
  char type[20]; // soin dégat ..?
  int tour_actif;
  int tour_recharge;
  int barre_action;// les personnages chargent une barre d'action pour attaquer grâce a leur vitesse comme dans raid shadow legends
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
  char categorie[32];
  Competence competence;
}  Combattant;


void degat(Combattant combattant,int degat){//fonction qui inflige les dégats à un personnage mais gère aussi le fait que le personnage meurt ou non
  combattant.pv=combattant.pv-degat;
}

void appliquer_technique()//fonction qui va faire des if pour trouver si la technique est un degat,un soin, un endormissement... et applique la technique en conséquence

Combattant charger_combattant() //fonction qui charge les données d'un combattant contenues dans un fichier

Competence charger_competence() //fonction qui charge les données d'une technique contenues dans un fichier

void creer_equipe(Combattant* Equipe1,Combattant* Equipe2) //fonction qui permet aux 2 joueurs de choisir ses personnage et mets à jour les équipes

void combat() //fonction qui lance une boucle jusqu'à ce que le combat s'arrête


int main(){
  Combattant Equipe1[3];
  Combattant Equipe2[3];
  
    
  return 0;
}
