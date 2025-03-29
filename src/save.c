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
        printf("Chargement effectué\n");
        return 1;
    }
}

void sauver(t_Team * joueur,int save,char * nomsave){
            save = atoi(nomsave);
            joueur->id_save = save;
}