#ifndef AFFICHAGE_H
#define AFFICHAGE_H

//tous les includes necessaires
#include "projet.h"
#include "combat.h"
#include "effet.h"

//toutes les fonctions et procédures déclarées et utilisées dans affichage.c
void afficher_personnages_disponibles(int deja_choisi[], char personnages_disponibles[NOMBRE_PERSO][TAILLE_NOM_PERSO]);
void affiche_tous_perso(Combattant* Equipe1[], Combattant* Equipe2[], char* equipe1_Nom, char* equipe2_Nom, Combattant* attaquant);
void afficher_effet(Combattant* combattant);

#endif
