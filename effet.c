//tous les includes necessaires
#include "projet.h"
#include "effet.h"

int vivant(Combattant* combattant){ //fonction qui vérifie si un combattant est toujours en vie ou non
    if (combattant->pv==0){
        return 0;}
    return 1;
}

int degat(Combattant* combattant,int degat){//fonction qui inflige les dégats à un personnage mais gère aussi le fait que le personnage meurt ou non
    int n=rand()%100;
    if (combattant->agilite<n){
        int pv_actuel=combattant->pv;
        int degats_totaux=degat*(100-combattant->defense)/100;
        combattant->pv=combattant->pv-degats_totaux;
    if (combattant->pv<0){
      combattant->pv=0;}
        return pv_actuel-combattant->pv;
    }
    else{
        printf("\n%s esquive !\n",combattant->nom);
        return 0;
    }
}

void degat_recharge(Combattant* combattant,int degat,Competence* competence){//fonction qui inflige les dégats à un personnage mais gère aussi le fait que le personnage meurt ou non
    int n=rand()%100;
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

void retirer_stats(Combattant* combattant,int index){ // retire les effets spéciaux temporaires d'un combattant (vitesse ou défense) et elle estt appelée lorsqu'un effet expire ou est supprimé
    if (strcmp(combattant->effet_special[index].nom,"Acceleration")==0){
        combattant->vitesse=combattant->vitesse-combattant->effet_special[index].valeur;
    }
    else if (strcmp(combattant->effet_special[index].nom,"Protection")==0){
        combattant->defense=combattant->defense-combattant->effet_special[index].valeur;
  }
}

void appliquer_effet(Combattant* combattant,Effet effet){//fonction qui applique un effet à un combattant
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

void soin(Combattant* combattant,int soin){ // soigne un combattant qui ne peut pas dépasser un seuil de PV max
    combattant->pv=combattant->pv+soin;
    if(combattant->pv > combattant->pvmax){
        combattant->pv=combattant->pvmax;}
}

void soin_recharge(Combattant* combattant,int soin,Competence* competence){ // soigne un combattant et recharge la compétence
    combattant->pv=combattant->pv+soin;
    if(combattant->pv > combattant->pvmax){
        combattant->pv=combattant->pvmax;}
    competence->tour_recharge_restant=competence->tour_recharge;
}

void acceleration(Combattant* combattant, Competence* competence){// fonction qui permet d'Augmenter la vitesse d un allie pendant plusieurs tours
    Effet effet;
    strcpy(effet.nom, competence->type);
    effet.tour_restant=competence->tour_actif;
    effet.valeur=competence->valeur;
    appliquer_effet(combattant,effet);
    combattant->vitesse=combattant->vitesse+competence->valeur;
    competence->tour_recharge_restant=competence->tour_recharge;
}

void protection(Combattant* combattant, Competence* competence){ //Augmente la defense d'un allie pendant plusieurs tours .
    Effet effet;
    strcpy(effet.nom, competence->type);
    effet.tour_restant=competence->tour_actif;
    effet.valeur=competence->valeur;
    appliquer_effet(combattant,effet);
    combattant->defense=combattant->defense+competence->valeur;
    competence->tour_recharge_restant=competence->tour_recharge;
}

void regeneration_brulure(Combattant* combattant, Competence* competence){ // Applique un effet de type "Regeneration" ou "Brulure" selon le type de la compétence
    Effet effet;
    strcpy(effet.nom, competence->type);
    effet.valeur=competence->valeur;
    effet.tour_restant=competence->tour_actif;
    appliquer_effet(combattant,effet);
    competence->tour_recharge_restant=competence->tour_recharge;
}

void frappe_impitoyable(Combattant* combattant,Competence* competence){ //fonction qui inflige des degats augmenter par le defense de la cible
    degat(combattant,competence->valeur+combattant->defense);
    competence->tour_recharge_restant=competence->tour_recharge;// Met la compétence en recharge
}


void provocation(Combattant* combattant, Competence* competence){ // Applique un effet de provocation au combattant (cible), forçant  l'adversaire à l'attaquer
    Effet effet;
    strcpy(effet.nom, competence->type);
    effet.tour_restant=competence->tour_actif;
    appliquer_effet(combattant,effet);
    competence->tour_recharge_restant=competence->tour_recharge;
}

void soin_tous(Combattant* equipe[],Competence* competence){// Soigne tous les membres vivants d'une équipe
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

void Vol_de_vie(Combattant* lanceur,Combattant* cible,Competence* competence){// Vol de vie : inflige des dégâts à la cible et soigne le lanceur d'une partie des dégâts infligés
    int degat_inflige=degat(cible,competence->valeur);
    soin(lanceur,degat_inflige/2);
}

void retirer_effet(Combattant* combattant,int index){// Retire un effet spécial d’un combattant à un index donné en décalant les autres effets vers la gauche
    for (; index < combattant->nbr_effet_actif - 1; index++){
        combattant->effet_special[index] = combattant->effet_special[index+ 1];
    }
    combattant->nbr_effet_actif--;
}

void maj_vitesse(Combattant* combattant){// Met à jour la vitesse du combattant en ajoutant sa vitesse à sa barre d’action
    combattant->barre_action=combattant->barre_action+combattant->vitesse;
}

void maj_effet(Combattant* combattant){// Met à jour tous les effets actifs sur un combattant (brûlure, régénération...) et  Applique les effets, décrémente leur durée, puis les retire s’ils expirent
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
