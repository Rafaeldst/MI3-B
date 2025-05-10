#include "projet.h"
#include "affichage.h"
#include "combat.h"
#include "effet.h"

void combat(Combattant* equipe1[],Combattant* equipe2[],int mode,int difficulte,char* equipe1_Nom,char* equipe2_Nom){ //fonction qui lance une boucle jusqu'à ce que le combat s'arrête
int tic=0;
Combattant* rapide;
while ((equipe1[0]->pv>0 || equipe1[1]->pv>0 || equipe1[2]->pv>0) && (equipe2[0]->pv>0 || equipe2[1]->pv>0 || equipe2[2]->pv>0)&& tic<NOMBRE_TIC_MAXIMUM){
   maj_tous(equipe1, equipe2);
  rapide=plus_rapide(equipe1,equipe2);
   affiche_tous_perso(equipe1,equipe2,equipe1_Nom,equipe2_Nom,rapide);
    if (pret(rapide)){
      if(mode==1){
        if(difficulte==1){
          if(rapide->equipe==1){
            Competence* attaque = choisir_attaque(rapide);
            appliquer_technique(attaque, rapide, equipe1, equipe2, mode, difficulte);
          }
          else if(rapide->equipe==2){
            Competence* attaque = choisir_attaqueIAfaiblemoyen(rapide);
            appliquer_technique(attaque, rapide, equipe1, equipe2, mode, difficulte);
          }
        }
        else if(difficulte==2){
          if(rapide->equipe==1){
            Competence* attaque = choisir_attaque(rapide);
            appliquer_technique(attaque, rapide, equipe1, equipe2, mode, difficulte);
          }
          else if(rapide->equipe==2){
            Competence* attaque = choisir_attaqueIAfaiblemoyen(rapide);
            appliquer_technique(attaque, rapide, equipe1, equipe2, mode, difficulte);
          }
        }
        else if(difficulte==3){
          if(rapide->equipe==1){
            Competence* attaque = choisir_attaque(rapide);
            appliquer_technique(attaque, rapide, equipe1, equipe2, mode, difficulte);
          }
          else if(rapide->equipe==2){
            Competence* attaque = choisir_attaqueIAfort(rapide);
            appliquer_technique(attaque, rapide, equipe1, equipe2, mode, difficulte);
          }
        }
      }
      else if(mode==2){
        Competence* attaque = choisir_attaque(rapide);
      appliquer_technique(attaque, rapide, equipe1, equipe2, mode, difficulte);}
      }
    tic++;
    
}}

void afficher_vainqueur(Combattant* equipe1[]) {
    if(vivant(equipe1[0]) || vivant(equipe1[1]) || vivant(equipe1[2])) {
        printf("L'équipe 1 a gagné !\n");}
    else{
        printf("L'équipe 2 a gagné !\n");
    }
}



void liberer_equipes(Combattant* equipe1[], Combattant* equipe2[]) {
    for(int i = 0; i < TAILLE_EQUIPE; i++) {
        for(int j = 0; j < NOMBRE_COMPETENCE; j++) {
            free(equipe1[i]->competence[j]);
        }
        free(equipe1[i]);

        for(int j = 0; j < NOMBRE_COMPETENCE; j++) {
            free(equipe2[i]->competence[j]);
        }
        free(equipe2[i]);
    }
}



int main(){



  srand(time(NULL));
  int mode=-1, difficulte;
Combattant* equipe1[TAILLE_EQUIPE];
Combattant* equipe2[TAILLE_EQUIPE];
char equipe1_Nom[TAILLE_NOM_EQUIPE];
char equipe2_Nom[TAILLE_NOM_EQUIPE];
while (mode!=0){
    printf("Choisissez un mode:\n[0] Arrêter le programme\n[1] Solo contre l'ordinateur\n[2] Multijoueur 1V1\n");
    mode=meilleur_scan();
    while (mode!=0  && mode!=1 && mode!=2){
        printf("Erreur de saisie.\n");
        mode=meilleur_scan();
            }

            if (mode==0){
                return 0;
            }


        if(mode==1){
            printf("Choisissez le nom de l'équipe 1.\n");
            scanf(" %32[^\n]", equipe1_Nom);
                int c;
    while ((c = getchar()) != '\n' && c != EOF){
      
    }

            strcpy(equipe2_Nom,"Ordinateur");
            printf("Choisissez un niveau de difficulté:\n[1] faible\n[2] intermediaire\n[3] fort\n");
            difficulte=meilleur_scan();
            while (difficulte!=1 && difficulte!=2 && difficulte!=3){
                printf("Erreur de saisie.\n");
                difficulte=meilleur_scan();
            }
            if(difficulte==1){
            creer_equipeIA(equipe1,equipe2);
            combat(equipe1,equipe2,mode,difficulte,equipe1_Nom,equipe2_Nom);
            }
            else if(difficulte==2){
            creer_equipeIA(equipe1,equipe2);
            combat(equipe1,equipe2,mode,difficulte,equipe1_Nom,equipe2_Nom);
            }
            else if(difficulte==3){
            creer_equipeIA(equipe1,equipe2);
            combat(equipe1,equipe2,mode,difficulte,equipe1_Nom,equipe2_Nom);
            }
        }


        if(mode==2){
        printf("Choisissez le nom de l'équipe 1.\n");
        scanf(" %32[^\n]", equipe1_Nom);
            int c;
    while ((c = getchar()) != '\n' && c != EOF){

    }

        printf("Choisissez le nom de l'équipe 2.\n");
        scanf(" %32[^\n]", equipe2_Nom);
            
    while ((c = getchar()) != '\n' && c != EOF){

    }

            creer_equipe(equipe1,equipe2);
            combat(equipe1,equipe2,mode,difficulte,equipe1_Nom,equipe2_Nom);
        }


        afficher_vainqueur(equipe1);
        liberer_equipes(equipe1,equipe2);
}

  return 0;
}
