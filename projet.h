#ifndef PROJET_H
#define PROJET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAILLE_NOM_EQUIPE 32
#define NOMBRE_TIC_MAXIMUM 5000
#define TAILLE_NOM_PERSO 50
#define TAILLE_NOM_COMPETENCE 32
#define TAILLE_NOM_EFFET 32
#define TAILLE_DESCRIPTION 512
#define TAILLE_CATEGORIE_PERSO 32
#define NOMBRE_PERSO 8
#define TAILLE_AFFICHAGE 107 //multiple de 3 (99)+8
#define NOMBRE_EFFET 6 //nombre de tous les effets possibles du jeu
#define TAILLE_EQUIPE 3
#define TAILLE_TYPE 20
#define TAILLE_CIBLE 20
#define NOMBRE_COMPETENCE 4
#define SEUIL_ACTION 100
//acceleration protection regeneration provocation et brulure
typedef struct{
  char nom[TAILLE_NOM_COMPETENCE];
  int valeur;
  char description[TAILLE_DESCRIPTION];
  char type[TAILLE_CIBLE]; // soin dégat ..?
  char cible[TAILLE_CIBLE];
  int tour_actif;
  int tour_recharge;
  int tour_recharge_restant;
} Competence;

typedef struct{
  char nom[TAILLE_NOM_EFFET];
  int tour_restant;
  int valeur;
} Effet;

typedef struct{
  char nom[TAILLE_NOM_PERSO];
  int pv;
  int pvmax;
  int attaque;
  int defense;
  int agilite;
  int vitesse;
  int barre_action;// les personnages chargent une barre d'action pour attaquer grâce a leur vitesse comme dans rclaeraid shadow legends
  char categorie[TAILLE_CATEGORIE_PERSO];
  Effet effet_special[NOMBRE_EFFET];
  int nbr_effet_actif;
  Competence* competence[NOMBRE_COMPETENCE];
  int equipe;
}  Combattant;

#endif
