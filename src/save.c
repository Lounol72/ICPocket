#include "../include/save.h"

/**
 * @fn void sauvegarder(t_Team * teamJ1,t_Team * dresseur)
 * @brief Sauvegarde l'état du jeu dans un fichier.
 * 
 * Cette fonction enregistre l'état du jeu, y compris les informations sur les équipes et les Pokémon, dans un fichier de sauvegarde.
 * 
 * @param teamJ1 Pointeur vers l'équipe du joueur 1.
 * @param dresseur Pointeur vers l'équipe du dresseur. 
 */
void sauvegarder(t_Team * joueur,t_Team * adverse){
    char nomFichier[1024];

    snprintf(nomFichier, sizeof(nomFichier), "data/save/Save_%d.txt", joueur->id_save);
    FILE *fichier = fopen(nomFichier, "w+");
    if(fichier == NULL){
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        exit(1);
    } else {
        fprintf(fichier, "User name : SAVE1\n");
        fprintf(fichier, "nb:%d\n" , joueur->nb_poke);
        for(int i = 0; i < joueur->nb_poke; i++){
            fprintf(fichier, "id:%d\n", joueur->team[i].id);
            fprintf(fichier, "lvl:%d\n", joueur->team[i].lvl);
            fprintf(fichier, "nature:%d\n", joueur->team[i].nature);
            fprintf(fichier, "nbmove:%d\n", joueur->team[i].nb_move);
            for(int j = 0 ; j < 6 ; j++)
                fprintf(fichier, "iv:%d\n", joueur->team[i].iv[j]);
            for(int j = 0 ; j < 1 ; j++)
                fprintf(fichier, "type:%d\n", joueur->team[i].type[j]);
            for(int j = 0 ; j < joueur->team[i].nb_move ; j++){
                fprintf(fichier, "move:%d\n", joueur->team[i].moveList[j].id);
            }
            fprintf(fichier,"fichier de save_%d\n",joueur->id_save);
            fprintf(fichier,"\n\n");
        }
        
        fprintf(fichier, "Dernier dresseur battu : %s id: %d\n", adverse->trainerName, adverse->id);
        joueur->lastEnemiID = adverse->id;
        fclose(fichier);
    }
}

/**
 * @fn int charger(char * name,t_Team * teamJ1,t_Team * dresseur)
 * @brief Charge l'état du jeu à partir d'un fichier de sauvegarde.
 * 
 * Cette fonction lit un fichier de sauvegarde et restaure l'état du jeu, y compris les informations sur les équipes et les Pokémon.
 * 
 * @param name Nom du fichier de sauvegarde à charger.
 * @param teamJ1 Pointeur vers l'équipe du joueur 1.
 * @param dresseur Pointeur vers l'équipe du dresseur. 
 * @return 0 si le chargement a réussi, -1 sinon.
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
        joueur->id_save = save;
        return -1;

    } else {
        fscanf(fichier, "User name : %s\n", joueur->trainerName);
        if(strcmp(joueur->trainerName,"SAVE1")!=0){
            printf("Sauvegarde vide\n");
            sauver(joueur,save,nomSave);
            return -1;
        }
        fscanf(fichier, "nb:%d\n", &(joueur->nb_poke));
        joueur->effect = noEffect;
        for (int i = 0; i < joueur->nb_poke; i++) {
            joueur->team[i].main_effect = noEffect;
            fscanf(fichier, "id:%d\n", &(joueur->team[i].id));
            generate_poke(&(joueur->team[i]), joueur->team[i].id);
            fscanf(fichier, "lvl:%d\n", &(joueur->team[i].lvl));
            joueur->team[i].exp=expCurve(joueur->team[i].lvl);
            fscanf(fichier, "nature:%d\n", &(joueur->team[i].nature));
            fscanf(fichier, "nbmove:%d\n", &(joueur->team[i].nb_move));
            for (int j = 0; j < 6; j++)
                fscanf(fichier, "iv:%d\n", &(joueur->team[i].iv[j]));
            for (int j = 0; j < 1; j++)
                fscanf(fichier, "type:%d\n", (int *)&(joueur->team[i].type[j]));
            for (int j = 0; j < joueur->team[i].nb_move; j++) {
                fscanf(fichier, "move:%d\n", &(joueur->team[i].moveList[j].id));
                joueur->team[i].moveList[j] = generateMove(joueur->team[i].moveList[j].id);
            }
            fscanf(fichier,"fichier de save_%d\n",&(joueur->id_save));

            for(int j=0;j<6;j++) joueur->statChanges[j]=NEUTRAL_STAT_CHANGE;
		    joueur->team[i].current_pv=calcStatFrom(&(joueur->team[i]),PV);
		    joueur->team[i].initial_pv = joueur->team[i].current_pv;
            
            //Forced To generate PP because of a load bug
            for (int j = 0; j < joueur->team[i].nb_move; j++){
                joueur->team[i].moveList[j].current_pp = joueur->team[i].moveList[j].max_pp;
		    }
        }
        char temp[10];
        strcpy(temp,dresseur->trainerName);
        fscanf(fichier, "Dernier dresseur battu : %s id: %d\n", temp, &(joueur->lastEnemiID));
        fclose(fichier);

        return 1;
    }
}

/**
 * @fn void sauver(t_Team * teamJ1,int save,char * nomSave)
 * @brief Sauvegarde l'état du jeu dans un fichier.
 * 
 * Cette fonction enregistre l'état du jeu, y compris les informations sur les équipes et les Pokémon, dans un fichier de sauvegarde.
 * 
 * @param teamJ1 Pointeur vers l'équipe du joueur 1.
 * @param save Indicateur de sauvegarde (0 pour une nouvelle sauvegarde, 1 pour écraser une sauvegarde existante).
 * @param nomSave Nom du fichier de sauvegarde.
 */
void sauver(t_Team * joueur,int save,char * nomsave){
            save = atoi(nomsave);
            joueur->id_save = save;
}