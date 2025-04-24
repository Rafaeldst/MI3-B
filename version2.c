#include <stdlib.h>

typedef struct{
  char nom[32];
  int valeur;
  char description[512];
  char type[20]; // soin dégat ..?
  int tour_actif;
  int tour_recharge;
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


void degat(Combattant combattant,int degat){
  combattant.pv=combattant.pv-degat;
}

void soin(Combattant combattant, int soin){
  combattant.pv=combattant.pv+soin;
}

Combattant charger_combattant() //fonction qui charge les données d'un combattant contenues dans un fichier

Technique charger_technique() //fonction qui charge les données d'une technique contenues dans un fichier

void combat() //fonction qui lance une boucle jusqu'à ce que le combat s'arrête
  int tour=1;
int a;
while(zeus.pv>0 && hades.pv>0){
  if (tour%2==0){
    scanf("%d",&a);
    if(a==1)}
      degat(hades,CompeteneceZeus.valeur);}
    }
  else{
    scanf("%sd";&a);
    if(a==1){
      degat(zeus,Competencehades.valeur);}}
  
  tour=tour+1;//inclure un système de tour max pour éviter qu'il y ait des scénarios où le game ne peut pas se finir

int main(){

  return 0;
}
