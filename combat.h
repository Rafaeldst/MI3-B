#ifndef COMBAT_H
#define COMBAT_H


#include "projet.h"
#include "affichage.h"
#include "effet.h"

int meilleur_scan();
int vivant(Combattant* c);
void soin_recharge(Combattant* c, int valeur, Competence* competence);
void degat_recharge(Combattant* c, int valeur, Competence* competence);
void acceleration(Combattant* c, Competence* competence);
void protection(Combattant* c, Competence* competence);
void verifier_erreur_fichier(FILE* fichier);
Combattant* choisir_cible(Combattant* lanceur,Combattant* equipe1[], Combattant* equipe2[], Competence* competence,int mode, int difficulte);
void appliquer_technique(Competence* competence,Combattant* lanceur, Combattant* equipe1[], Combattant* equipe2[], int mode, int difficulte);
Competence *charger_competence(char* fichier_competence);
Combattant* charger_combattant(char* fichier_combattant);
void creer_equipe(Combattant* equipe1[],Combattant* equipe2[]);
void creer_equipeIA(Combattant* equipe1[],Combattant* equipe2[]);
Competence* choisir_attaque(Combattant* combattant);
Competence* choisir_attaqueIAfaiblemoyen(Combattant* combattant);
Competence* choisir_attaqueIAfort(Combattant* combattant);
int compter_chiffre(int n);
Combattant* plus_rapide(Combattant* equipe1[],Combattant* equipe2[]);

#endif
