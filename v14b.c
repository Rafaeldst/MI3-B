
int meilleur_scan(){
    int retour = 0;
    int valeur;
    retour = scanf("%d", &valeur);
    while (retour != 1)
    {   
        printf("Erreur de saisie.");
        while(getchar()!='\n'){} // Ligne facultative de sécurisation
        retour = scanf("%d", &valeur);
    }
    return valeur;
}


void verifier_erreur_fichier(FILE* fichier){
  if (fichier==NULL){
    printf("Erreur lors de l'ouverture du fichier.\n");
    exit(EXIT_FAILURE);}
}

void verifier_erreur_int(int* a){
    if (a==NULL){
        printf("Erreur : pointeur passé en argument est NULL.\n");
        exit(EXIT_FAILURE);
    }
}

void verifier_erreur_char(char* a){
    if (a==NULL){
        printf("Erreur : pointeur passé en argument est NULL.\n");
        exit(EXIT_FAILURE);
    }
}

void verifier_erreur_combattant(Combattant* a){
    if (a==NULL){
        printf("Erreur : pointeur passé en argument est NULL.\n");
        exit(EXIT_FAILURE);
    }
}

void verifier_erreur_competence(Competence* a){
    if (a==NULL){
        printf("Erreur : pointeur passé en argument est NULL.\n");
        exit(EXIT_FAILURE);
    }
}

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

  Combattant* choisir_cible(Combattant* lanceur,Combattant* equipe1[], Combattant* equipe2[], Competence* competence,int mode, int difficulte){
    int choix;
    if(lanceur->equipe==1){
        for(int i=0; i<TAILLE_EQUIPE;i++){
            for(int j=0;j<equipe2[i]->nbr_effet_actif;j++){
                if(strcmp(equipe2[i]->effet_special[j].nom,"Provocation")==0){
                    return equipe2[i];
                }
            }
        }
        if(strcmp(competence->cible, "Ennemi") == 0){
            printf("Choisissez une cible :\n");
            choix=meilleur_scan();
            while (choix<1 || choix>TAILLE_EQUIPE || !(vivant(equipe2[choix-1]))){
                printf("Cible incorrect.\n"); 
                choix=meilleur_scan();}
            return equipe2[choix-1];
            }
        else if(strcmp(competence->cible, "Allie") ==0){
        printf("Choisissez une cible :\n");
        choix=meilleur_scan();
        while(choix<1 || choix>TAILLE_EQUIPE|| !(vivant(equipe1[choix-1]))){
            choix=meilleur_scan();
            printf("Cible incorrect.\n"); 
        }
        return equipe1[choix-1];
        }
    }
    if(lanceur->equipe==2){
        if(mode==2){
            for(int i=0; i<TAILLE_EQUIPE;i++){
                for(int j=0;j<equipe1[i]->nbr_effet_actif;j++){
                    if(strcmp(equipe1[i]->effet_special[j].nom,"Provocation")==0){
                        return equipe1[i];
                    }
                }
            }
        if(strcmp(competence->cible, "Ennemi") == 0){
            printf("Choisissez une cible :\n");
            choix=meilleur_scan();
            while (choix<1 || choix>TAILLE_EQUIPE || !(vivant(equipe1[choix-1]))){
                printf("Cible incorrect.\n"); 
                choix=meilleur_scan();}
            return equipe1[choix-1];
        }
        else if(strcmp(competence->cible, "Allie") ==0){
            printf("Choisissez une cible :\n");
            choix=meilleur_scan();
            while(choix<1 || choix>TAILLE_EQUIPE || !(vivant(equipe2[choix-1]))){
                choix=meilleur_scan();
                printf("Cible incorrect.\n"); 
            }
            return equipe2[choix-1];}
      }
        else if(mode==1){
            if(difficulte==1){
                for(int i=0; i<TAILLE_EQUIPE;i++){
                    for(int j=0;j<equipe1[i]->nbr_effet_actif;j++){
                        if(strcmp(equipe1[i]->effet_special[j].nom,"Provocation")==0){
                            return equipe1[i];
                }
           }
        }
        if(strcmp(competence->cible, "Ennemi") == 0){
        choix=rand()%3+1;
        while (choix<1 || choix>TAILLE_EQUIPE || !(vivant(equipe1[choix-1]))){
          choix=rand()%TAILLE_EQUIPE+1;}
        printf("la cible choisi par l'ordinateur est %s\n", equipe1[choix-1]->nom);
        return equipe1[choix-1];}
        else if(strcmp(competence->cible, "Allie") ==0){
        choix=rand()%TAILLE_EQUIPE+1;
        while(choix<1 || choix>TAILLE_EQUIPE || !(vivant(equipe2[choix-1]))){
          choix=rand()%3+1;
       }
       printf("la cible choisi par l'ordinateur est %s\n", equipe1[choix-1]->nom);
       return equipe2[choix-1];}
        }
        else if(difficulte==2 || difficulte==3){
          for(int i=0; i<TAILLE_EQUIPE;i++){
            for(int j=0;j<equipe1[i]->nbr_effet_actif;j++){
                if(strcmp(equipe1[i]->effet_special[j].nom,"Provocation")==0){
                   return equipe1[i];
                }
           }
        }
        if(strcmp(competence->cible, "Ennemi") == 0){
          choix=moinsdepvennemi(equipe1);
          printf("la cible choisi par l'ordinateur est %s\n", equipe1[choix]->nom);
          return equipe1[choix];}
          else if(strcmp(competence->cible, "Allie") ==0){
          choix=moinsdepvallie(equipe2);
         printf("la cible choisi par l'ordinateur est %s\n", equipe1[choix]->nom);
         return equipe2[choix];}
        }
      }
  }}

void appliquer_technique(Competence* competence,Combattant* lanceur, Combattant* equipe1[], Combattant* equipe2[], int mode, int difficulte){//fonction qui va faire des if pour trouver si la technique est un degat,un soin, un endormissement... et applique la technique en conséquence
  Combattant* cible;
  lanceur->barre_action=0;
  printf("%s utilise %s \n",lanceur->nom,competence->nom);
  if (strcmp(competence->type,"Degats")==0){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence, mode, difficulte);
    degat_recharge(cible,competence->valeur,competence);
  }
  else if (strcmp(competence->type,"Soin")==0){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence, mode, difficulte);
    soin_recharge(cible,competence->valeur,competence);
  }
  else if (strcmp(competence->type,"Acceleration")==0){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence, mode, difficulte);
    acceleration(cible,competence);
  }
  else if (strcmp(competence->type,"Protection")==0){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence, mode, difficulte);
    protection(cible,competence);
  }
  else if ((strcmp(competence->type,"Regeneration")==0)||(strcmp(competence->type,"Brulure")==0)){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence, mode, difficulte);
    regeneration_brulure(cible,competence);
  }
  else if (strcmp(competence->type,"DegatsDef")==0){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence, mode, difficulte);
    frappe_impitoyable(cible,competence);
  }
  else if (strcmp(competence->type,"SoinTous")==0){
    if (lanceur->equipe==1){
        soin_tous(equipe1, competence);
    }
    else{
        soin_tous(equipe2, competence);
    }
  }
  else if (strcmp(competence->type,"DegatsTous")==0){
    if (lanceur->equipe==1){
        degat_tous(equipe2, competence);
    }
    else{
        degat_tous(equipe1, competence);
    }
  }
  else if (strcmp(competence->type,"Provocation")==0){
    provocation(lanceur,competence);
  }
  else if (strcmp(competence->type,"VolDeVie")==0){
    cible=choisir_cible(lanceur, equipe1, equipe2, competence, mode, difficulte);
    Vol_de_vie(lanceur,cible,competence);
  }
}

Competence* charger_competence(char* fichier_competence){//fonction qui charge les données d'une technique contenues dans un fichier
    Competence* c = malloc(sizeof(Competence));
     if (c == NULL) {
       printf("Erreur d'allocation mémoire pour Competence\n");
       exit(EXIT_FAILURE);
     }
     FILE* fichier=fopen(fichier_competence,"r");
     verifier_erreur_fichier(fichier);
     fgets(c->nom,TAILLE_NOM_COMPETENCE,fichier);
     fgets(c->description,TAILLE_DESCRIPTION,fichier);
     fscanf(fichier,"%s %s %d %d %d",c->type,c->cible,&(c->valeur),&(c->tour_actif),&(c->tour_recharge));
     c->tour_recharge_restant=0;
     fclose(fichier);
     return c;
   }

Combattant* charger_combattant(char* fichier_combattant){ //fonction qui charge les données d'un combattant contenues dans un fichier
  Combattant* c = malloc(sizeof(Combattant));
  if (c == NULL) {
    printf("Erreur d'allocation mémoire pour Combattant\n");
    exit(EXIT_FAILURE);
  }
  c->nbr_effet_actif=0;
  FILE* fichier= fopen(fichier_combattant,"r");
  verifier_erreur_fichier(fichier);
  char competence1[TAILLE_NOM_COMPETENCE],competence2[TAILLE_NOM_COMPETENCE],competence3[TAILLE_NOM_COMPETENCE],competence4[TAILLE_NOM_COMPETENCE];
  c->barre_action=0;
  fscanf(fichier,"%s %s %d %d %d %d %d %d %s %s %s %s",c->nom,c->categorie,&(c->pv),&(c->pvmax),&(c->attaque),&(c->defense),&(c->agilite),&(c->vitesse),competence1,competence2,competence3,competence4);
  c->competence[0]=charger_competence(competence1);
  c->competence[1]=charger_competence(competence2);
  c->competence[2]=charger_competence(competence3);
  c->competence[3]=charger_competence(competence4);
  fclose(fichier);
  return c;
}

void afficher_personnages_disponibles(int deja_choisi[], char personnages_disponibles[NOMBRE_PERSO][TAILLE_NOM_PERSO]) {
    char chaine[TAILLE_NOM_PERSO];
    printf(" Les Personnages disponibles sont:\n");
    for (int i = 0; i < NOMBRE_PERSO; i++) {
        if (deja_choisi[i] == 0) {
            strcpy(chaine,personnages_disponibles[i]);
            chaine[strlen(chaine)-4]='\0'; // pour enlever .txt lors de l'affichage
            printf("%d. %s\n", i + 1, chaine);
        }
    }
}

void creer_equipe(Combattant* equipe1[],Combattant* equipe2[]){ //fonction qui permet aux 2 joueurs de choisir ses personnage et mets à jour les équipes
    char personnages_disponibles[NOMBRE_PERSO][TAILLE_NOM_PERSO];
    FILE* fichier=fopen("Liste_Personnage.txt","r");
    for (int i=0;i<NOMBRE_PERSO;i++){
      fscanf(fichier,"%s",personnages_disponibles[i]);
    };
    fclose(fichier);
// pour suivre quels personnages  sont déjà pris
int deja_choisi[NOMBRE_PERSO] = {0};
    int choix, i;

    for (i = 0; i < TAILLE_EQUIPE; i++) {
        // Joueur 1
        afficher_personnages_disponibles(deja_choisi,personnages_disponibles);
        printf("Joueur 1 - Choisissez un personnage (1-8) : \n");
        choix=meilleur_scan();
        while (choix < 1 || choix > NOMBRE_PERSO || deja_choisi[choix-1] != 0) {
            printf("Choix invalide ou deja pris. Reessayez : \n");
            choix=meilleur_scan();
        }

      equipe1[i] = charger_combattant(personnages_disponibles[choix-1]);

      equipe1[i]->equipe=1;
      deja_choisi[choix-1] = 1;  // empeche ce personnage d'être pris 
        // Joueur 2
        afficher_personnages_disponibles(deja_choisi,personnages_disponibles);
        printf("Joueur 2 - Choisissez un personnage (1-8) : \n ");
        choix=meilleur_scan();
      
        while (choix < 1 || choix > NOMBRE_PERSO ||  deja_choisi[choix-1] != 0) {
            printf("Choix invalide ou deja pris. Reessayez : ");
            choix=meilleur_scan();
        }
        deja_choisi[choix-1] = 1;
      equipe2[i] = charger_combattant(personnages_disponibles[choix-1]);
      equipe2[i]->equipe=2;
    }
  
 printf(" Composition des équipes:\n");

printf("Équipe 1 : :\n");
for (int j = 0; j < TAILLE_EQUIPE; j++) {
    printf("%s", equipe1[j]->nom);
    if (j < TAILLE_EQUIPE - 1) printf(" | ");
}
printf("\n");

printf("Équipe 2 : ");
for (int j = 0; j < TAILLE_EQUIPE; j++) {
    printf("%s", equipe2[j]->nom);
    if (j < TAILLE_EQUIPE - 1) printf(" | ");
}
printf("\n");
    printf("Equipes creees avec succes ! DEBUT DE LA PARTIE \n");
}

void creer_equipeIA(Combattant* equipe1[],Combattant* equipe2[]){ //fonction qui permet aux 2 joueurs de choisir ses personnage et mets à jour les équipes
  char personnages_disponibles[NOMBRE_PERSO][TAILLE_NOM_PERSO];
  FILE* fichier=fopen("Liste_Personnage.txt","r");
  for (int i=0;i<NOMBRE_PERSO;i++){
    fscanf(fichier,"%s",personnages_disponibles[i]);
  };
  fclose(fichier);

// pour suivre quels personnages  sont déjà pris
    int deja_choisi[NOMBRE_PERSO] = {0};
    int choix, i;

    for (i = 0; i < 3; i++) {
        // Joueur 1
        afficher_personnages_disponibles(deja_choisi,personnages_disponibles);
        printf("Joueur 1 - Choisissez un personnage (1-8) : \n");
        choix=meilleur_scan();
        while (choix < 1 || choix > NOMBRE_PERSO || deja_choisi[choix-1] != 0) {
            printf("Choix invalide ou deja pris. Reessayez : \n");
            choix=meilleur_scan();
        }

      equipe1[i] = charger_combattant(personnages_disponibles[choix-1]);

      equipe1[i]->equipe=1;
      deja_choisi[choix-1] = 1;  // empeche ce personnage d'être pris 
        // Joueur 2
        afficher_personnages_disponibles(deja_choisi,personnages_disponibles);
        choix=rand()%8+1;
      
        while (choix < 1 || choix > NOMBRE_PERSO ||  deja_choisi[choix-1] != 0) {
            printf("Choix invalide ou deja pris. Reessayez : ");
            choix=rand()%8+1;
        }
        printf("\n Le choix de l'ordinateur est le %d  ", choix);
        deja_choisi[choix-1] = 1;
      equipe2[i] = charger_combattant(personnages_disponibles[choix-1]);
      equipe2[i]->equipe=2;
    }
  
 printf(" Composition des équipes:\n");

printf("Équipe 1 : :\n");
for (int j = 0; j < TAILLE_EQUIPE; j++) {
    printf("%s", equipe1[j]->nom);
    if (j < TAILLE_EQUIPE - 1) printf(" | ");
}
printf("\n");

printf("Équipe 2 : ");
for (int j = 0; j < TAILLE_EQUIPE; j++) {
    printf("%s", equipe2[j]->nom);
    if (j < TAILLE_EQUIPE - 1) printf(" | ");
}
printf("\n");
    printf("Equipes creees avec succes ! DEBUT DE LA PARTIE \n");
}

Competence* choisir_attaque(Combattant* combattant) {
    int choix;
    printf("\nchoisisser une attaque\n");
    choix=meilleur_scan();
     while (choix < 1 || choix > NOMBRE_COMPETENCE || combattant->competence[choix - 1]->tour_recharge_restant > 0){
    printf("erreur choisisser une attaque valide\n");
    choix=meilleur_scan();
     }
    return combattant->competence[choix - 1];
}

Competence* choisir_attaqueIAfaiblemoyen(Combattant* combattant) {
  int choix;

  for (int i = 0; i < 2; i++) {
  }

  do {
      choix=rand()%2+1;
  } while (choix < 1 || choix > 2 || combattant->competence[choix - 1]->tour_recharge_restant > 0);
  printf("\n Le choix de l'ordinateur est l'attaque %d : ", choix);
  return combattant->competence[choix - 1];
}

Competence* choisir_attaqueIAfort(Combattant* combattant) {
  int choix;

  for (int i = 0; i < NOMBRE_COMPETENCE; i++) {
      printf("\n");
  }

  do {
      choix=rand()%NOMBRE_COMPETENCE+1;
  } while (choix < 1 || choix > NOMBRE_COMPETENCE || combattant->competence[choix - 1]->tour_recharge_restant > 0);
  printf("\n Le choix de l'ordinateur est l'attaque %d : ", choix);
  return combattant->competence[choix - 1];
}

int compter_chiffre(int n){
  if(n==0){
    return 1;
  }
  int compteur=0;
  while (n>0){
    n/=10;
    compteur++;
  }
  return compteur;
}

void afficher_effet(Combattant* combattant){
  for (int i=0;i<combattant->nbr_effet_actif;i++){
    if (strcmp(combattant->effet_special[i].nom,"Brulure")==0){
      printf("🔥");
    }
    else if (strcmp(combattant->effet_special[i].nom,"Regeneration")==0){
        printf("💕");
    }
    else if (strcmp(combattant->effet_special[i].nom,"Acceleration")==0){
      printf("💨");
    }
    else if (strcmp(combattant->effet_special[i].nom,"Protection")==0){
      printf("🛡️");
    }
    else if (strcmp(combattant->effet_special[i].nom,"Provocation")==0){
      printf("😡");
    }
  }
}


Combattant* plus_rapide(Combattant* equipe1[],Combattant* equipe2[]){
  Combattant c;
  int i;
  c.barre_action=-1;
  if (vivant(equipe1[0])&&equipe1[0]->barre_action>c.barre_action){
    c.barre_action=equipe1[0]->barre_action;
    i=0;
  }
  if (vivant(equipe2[0])&&equipe2[0]->barre_action>c.barre_action){
    c.barre_action=equipe2[0]->barre_action;
    i=1;
  }
  if (vivant(equipe1[1])&&equipe1[1]->barre_action>c.barre_action){
    c.barre_action=equipe1[1]->barre_action;
    i=2;
  }
  if (vivant(equipe2[1])&&equipe2[1]->barre_action>c.barre_action){
    c.barre_action=equipe2[1]->barre_action;
    i=3;
  }
  if (vivant(equipe1[2])&&equipe1[2]->barre_action>c.barre_action){
    c.barre_action=equipe1[2]->barre_action;
    i=4;
  }
  if (vivant(equipe2[2])&&equipe2[2]->barre_action>c.barre_action){
    c.barre_action=equipe2[2]->barre_action;
    i=5;
  }
  switch (i){
    case 0:
      return equipe1[0];
      break;
    case 1:
      return equipe2[0];
      break;
    case 2:
      return equipe1[1];
      break;
    case 3:
      return equipe2[1];
      break;
    case 4:
      return equipe1[2];
      break;
    case 5:
      return equipe2[2];
      break;
  }
}

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

void afficher_vainqueur(Combattant* equipe1[], Combattant* equipe2[]) {
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

