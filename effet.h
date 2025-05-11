#ifndef EFFET_H
#define EFFET_H

//tous les includes necessaires
#include "projet.h"
#include "affichage.h"
#include "combat.h"

//toutes les fonctions et procédures déclarées et utilisées dans effet.c
int vivant(Combattant* combattant);
int degat(Combattant* combattant,int degat);
void degat_recharge(Combattant* combattant,int degat,Competence* competence);
void retirer_stats(Combattant* combattant,int index);
void appliquer_effet(Combattant* combattant,Effet effet);
void soin(Combattant* combattant,int soin);
void soin_recharge(Combattant* combattant,int soin,Competence* competence);
void acceleration(Combattant* combattant, Competence* competence);
void protection(Combattant* combattant, Competence* competence);
void regeneration_brulure(Combattant* combattant, Competence* competence);
void frappe_impitoyable(Combattant* combattant,Competence* competence);
void provocation(Combattant* combattant, Competence* competence);
void soin_tous(Combattant* equipe[],Competence* competence);
void degat_tous(Combattant* equipe[],Competence* competence);
void Vol_de_vie(Combattant* lanceur,Combattant* cible,Competence* competence);
void retirer_effet(Combattant* combattant,int index);
void maj_vitesse(Combattant* combattant);
void maj_effet(Combattant* combattant);
void maj_recharge(Combattant* combattant);
void maj_tous_stats(Combattant* combattant);
void maj_tous(Combattant* Equipe1[],Combattant* Equipe2[]);
int charge(Combattant* combattant);
int pret(Combattant* combattant);
int moinsdepvennemi(Combattant* equipe[]);
int moinsdepvallie(Combattant* equipe[]);

#endif
