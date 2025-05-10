#include "projet.h"
#include "effet.h"

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

void retirer_stats(Combattant* combattant,int index){
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
    effet.valeur=competence->valeur;
    appliquer_effet(combattant,competence,effet);
    combattant->vitesse=combattant->vitesse+competence->valeur;
    competence->tour_recharge_restant=competence->tour_recharge;
}

void protection(Combattant* combattant, Competence* competence){
    Effet effet;
    strcpy(effet.nom, competence->type);
    effet.tour_restant=competence->tour_actif;
    effet.valeur=competence->valeur;
    appliquer_effet(combattant,competence,effet);
    combattant->defense=combattant->defense+competence->valeur;
    competence->tour_recharge_restant=competence->tour_recharge;
}

void regeneration_brulure(Combattant* combattant, Competence* competence){
    Effet effet;
    strcpy(effet.nom, competence->type);
    effet.valeur=competence->valeur;
    effet.tour_restant=competence->tour_actif;
    appliquer_effet(combattant,competence,effet);
    competence->tour_recharge_restant=competence->tour_recharge;
}

void frappe_impitoyable(Combattant* combattant,Competence* competence){
    degat(combattant,competence->valeur+combattant->defense);
    competence->tour_recharge_restant=competence->tour_recharge;
}


void provocation(Combattant* combattant, Competence* competence){
    Effet effet;
    strcpy(effet.nom, competence->type);
    effet.tour_restant=competence->tour_actif;
    appliquer_effet(combattant,competence,effet);
    competence->tour_recharge_restant=competence->tour_recharge;
}

void soin_tous(Combattant* equipe[],Competence* competence){
    for (int i=0;i<TAILLE_EQUIPE;i++){
      if(vivant(equipe[i])){
          soin(equipe[i],competence->valeur);
        }
      }
      competence->tour_recharge_restant=competence->tour_recharge;
    }


void degat_tous(Combattant* equipe[],Competence* competence){
    for (int i=0;i<TAILLE_EQUIPE;i++){
        degat(equipe[i],competence->valeur);
    }
    competence->tour_recharge_restant=competence->tour_recharge;
}

void Vol_de_vie(Combattant* lanceur,Combattant* cible,Competence* competence){
    int degat_inflige=degat(cible,competence->valeur);
    soin(lanceur,degat_inflige/2);
}

void retirer_effet(Combattant* combattant,int index){
    for (; index < combattant->nbr_effet_actif - 1; index++){
        combattant->effet_special[index] = combattant->effet_special[index+ 1];
    }
    combattant->nbr_effet_actif--;
}

void maj_vitesse(Combattant* combattant){
    combattant->barre_action=combattant->barre_action+combattant->vitesse;
}

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
    for (int i = 0; i < NOMBRE_COMPETENCE; i++) {
        if (combattant->competence[i]->tour_recharge_restant > 0) {
            combattant->competence[i]->tour_recharge_restant--;
        }
    }
}

void maj_tous_stats(Combattant* combattant){
    maj_recharge(combattant);
    maj_vitesse(combattant);
    maj_effet(combattant);
}
    
void maj_tous(Combattant* Equipe1[],Combattant* Equipe2[]){
    for (int i=0;i<TAILLE_EQUIPE;i++){
        if (vivant(Equipe1[i])){
            maj_tous_stats(Equipe1[i]);
        }
        if (vivant(Equipe2[i])){
            maj_tous_stats(Equipe2[i]);
        }
    }
}

int charge(Combattant* combattant){
    if (combattant->barre_action<SEUIL_ACTION){
        return 0;}
    return 1;
}

int pret(Combattant* combattant){
    return (charge(combattant)&&vivant(combattant));
}


int moinsdepvennemi(Combattant* equipe[]){
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
  
  int moinsdepvallie(Combattant* equipe[]){
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
