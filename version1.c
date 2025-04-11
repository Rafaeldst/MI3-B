#include <stdlib.h>

typedef struct{
  char nom[32];
  int valeur;
  char description[512];
  int tour_actif;
  int tour_recharge;
} Competence;


typedef struct{
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

int main(){
  
  Combattant hades;
  hades.pvmax=100;
  hades.pv=hades.pvmax;
  hades.attaque=25;
  hades.defense=10;
  hades.agilite=2;
  hades.vitesse=100;
  hades.categorie="sorcier";

  Competence CompetenceHades1;
  Comp.nom="boule de feu";
  comp.valeur=hades.attaque*2;
  comp.description="envoie une boule de feu";
  comp.tour_recharge=0;
  
  Combattant zeus;
  zeus.pvmax=100;
  zeus.pv=zeus.pvmax;
  zeus.attaque=35;
  zeus.defense=15;
  zeus.agilite=2;
  zeus.vitesse=50;
  zeus.categorie="attaquant";

  Competence CompetenceZeus1;
  CompetenceZeus.nom="eclair";
  CompetenceZeus1.valeur=2;
  CompetenceZeus1.description="lance un eclair";
  CompetenceZeus1.tour_recharge=0;

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
  
  tour=tour+1;
  return 0;
}
