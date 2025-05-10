#include "projet.h"

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
    while ((c = getchar()) != '\n' && c != EOF);

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
    while ((c = getchar()) != '\n' && c != EOF);

        printf("Choisissez le nom de l'équipe 2.\n");
        scanf(" %32[^\n]", equipe2_Nom);
            
    while ((c = getchar()) != '\n' && c != EOF);

            creer_equipe(equipe1,equipe2);
            combat(equipe1,equipe2,mode,difficulte,equipe1_Nom,equipe2_Nom);
        }


        afficher_vainqueur(equipe1,equipe2);
        liberer_equipes(equipe1,equipe2);
}

  return 0;
}
