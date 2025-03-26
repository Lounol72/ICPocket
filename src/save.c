#include "../include/save.h"

/**
*   @brief Fonction de sauvegarde
*   @param nomSave Nom de sauvegarde
*   @param joueur Equipe du joueur
*   @param adverse Dernier dresseur battu
*/

void sauvegarder(t_Team * joueur,t_Team * adverse){
    char nomFichier[1024];
    printf("save : %d\n",joueur->id_save);
    snprintf(nomFichier, sizeof(nomFichier), "data/save/Save_%d.txt", joueur->id_save);
    FILE *fichier = fopen(nomFichier, "w+");
    if(fichier == NULL){
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        exit(1);
    } else {
        fprintf(fichier,"%d\n",joueur->nb_poke);
        for(int i = 0; i < joueur->nb_poke; i++){
            fprintf(fichier,"%d;%d;%d;%d;",joueur->team[i].id,joueur->team[i].lvl,joueur->team[i].nature,joueur->team[i].nb_move);
            for(int j = 0 ; j < 6 ; j++)fprintf(fichier, "%d;", joueur->team[i].iv[j]);
            for(int j = 0 ; j < 2 ; j++)fprintf(fichier, "%d;", joueur->team[i].type[j]);
            for(int j = 0 ; j < joueur->team[i].nb_move ; j++)fprintf(fichier, "%d;", joueur->team[i].moveList[j].id);
            fprintf(fichier,"\n");
        }
        
        fprintf(fichier, "%s;%d;", adverse->trainerName, adverse->id);
        fprintf(fichier,"%d\n",joueur->id_save);
        fprintf(fichier,"check:1;");
        printf("Sauvegarde effectuée2\n");
        fclose(fichier);
    }
}


/**
*   @brief Fonction de chargement
*   @param nomSave Nom de sauvegarde
*   @param joueur Equipe du joueur
*   @param dresseur Dernier dresseur battu
*/
int charger(char *nomSave, t_Team *joueur, t_Team *dresseur){
    char filePath[256];
    int save=0;
    snprintf(filePath, sizeof(filePath), "data/save/Save_%s.txt", nomSave);
    FILE *fichier = fopen(filePath, "r");
    if (fichier == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier de sauvegarde.\n");
        printf("Creation d'un fichier sauvegarde\n");
        fichier = fopen(filePath, "w+");
        sauver(joueur,save,nomSave);
        return -1;

    } else {
        fseek(fichier, 0, SEEK_END);
        if(ftell(fichier)==0){
            printf("Sauvegarde vide\n");
            sauver(joueur,save,nomSave);
            return -1;
        }
        fseek(fichier, 0, SEEK_SET);
        fscanf(fichier, "%d\n", &(joueur->nb_poke));
        joueur->effect = noEffect;
        for (int i = 0; i < joueur->nb_poke; i++) {

            joueur->team[i].main_effect = noEffect;
            fscanf(fichier, "%d;%d;%d;%d;", &(joueur->team[i].id), &(joueur->team[i].lvl), &(joueur->team[i].nature), &(joueur->team[i].nb_move));
            printf("poke id : %d , lvl : %d ,nature: %d , nb move: %d \n",joueur->team[i].id,joueur->team[i].lvl,joueur->team[i].nature,joueur->team[i].nb_move);
            generate_poke(&(joueur->team[i]), joueur->team[i].id);
            joueur->team[i].exp=expCurve(joueur->team[i].lvl);
            
            for (int j = 0; j < 6; j++) {
                fscanf(fichier, "%d;", &(joueur->team[i].iv[j]));
                printf("iv[%d] : %d\n",j,joueur->team[i].iv[j]);
            }
            for (int j = 0; j < 2; j++) {
                fscanf(fichier, "%d;", (int *)&(joueur->team[i].type[j]));
                printf("type[%d] : %d\n",j,joueur->team[i].type[j]);
            }
            printf("nb_move 222: %d\n",joueur->team[i].nb_move);
            for (int j = 0; j < joueur->team[i].nb_move; j++) {
                fscanf(fichier, "%d;", &(joueur->team[i].moveList[j].id));
                joueur->team[i].moveList[j] = generateMove(joueur->team[i].moveList[j].id);
                printf("move[%d] : %d\n",j,joueur->team[i].moveList[j].id);
            }

            for(int j=0;j<6;j++) joueur->statChanges[j]=NEUTRAL_STAT_CHANGE;
		    joueur->team[i].current_pv=calcStatFrom(&(joueur->team[i]),PV);
		    joueur->team[i].initial_pv = joueur->team[i].current_pv;
            
            //Forced To generate PP because of a load bug
            for (int j = 0; j < joueur->team[i].nb_move; j++){
                joueur->team[i].moveList[j].current_pp = joueur->team[i].moveList[j].max_pp;
		    }
            fscanf(fichier, "\n");
        }
        fscanf(fichier, "%s;%d;", dresseur->trainerName, &(dresseur->id));
        fscanf(fichier,"%d\n",&(joueur->id_save));
        char check[10];
        fscanf(fichier,"%s",check);
        printf("check : %s\n",check);
        for(int i=0;i<joueur->nb_poke;i++){
            printPoke(&(joueur->team[i]));
        }
        if(strcmp(check,"check:1;")==0){
            printf("Chargement valide\n");
        }
        else{
            printf("Chargement echoué\n");
            sauver(joueur,save,nomSave);  
            return -1;
        }
        fclose(fichier);
        printf("Chargement effectué\n");
        for(int i = 0;i<joueur->nb_poke;i++){
            printPoke(&(joueur->team[i]));
        }
        return 1;
    }
}

void sauver(t_Team * joueur,int save,char * nomsave){
            save = atoi(nomsave);
            joueur->id_save = save;
}