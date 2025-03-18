#include "../include/save.h"

/**
*   @brief Fonction de sauvegarde
*   @param nomSave Nom de sauvegarde
*   @param joueur Equipe du joueur
*   @param adverse Dernier dresseur battu
*/
void sauvegarder(char * nomSave ,t_Team * joueur,t_Team * adverse){
    char nomFichier[1024];
    strcpy(nomFichier , "data/save/");
    strcat(nomFichier,nomSave);
    strcat(nomFichier,".txt");
    FILE *fichier = fopen(nomFichier, "w");
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
            fprintf(fichier,"\n\n");
        }
        fprintf(fichier, "Dernier dresseur battu : %s id: %d\n", adverse->trainerName, adverse->id);
        joueur->lastEnemiID = adverse->id;
        printf("Sauvegarde effectuée\n");
        fclose(fichier);
    }
}
/**
*   @brief Fonction de chargement
*   @param nomSave Nom de sauvegarde
*   @param joueur Equipe du joueur
*   @param dresseur Dernier dresseur battu
*/
int charger(char *nomSave, t_Team *joueur, t_Team *dresseur) {
    char filePath[256];
    snprintf(filePath, sizeof(filePath), "data/save/%s.txt", nomSave);
    FILE *fichier = fopen(filePath, "r");
    if (fichier == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier de sauvegarde.\n");
        return 0;
    } else {
        fscanf(fichier, "User name : %s\n", joueur->trainerName);
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
            for(int j=0;j<6;j++) joueur->statChanges[j]=NEUTRAL_STAT_CHANGE;
		    joueur->team[i].current_pv=calcStatFrom(&(joueur->team[i]),PV);
		    joueur->team[i].initial_pv = joueur->team[i].current_pv;
            
            //Forced To generate PP because of a load bug
            for (int j = 0; j < joueur->team[i].nb_move; j++){
                joueur->team[i].moveList[j].current_pp = joueur->team[i].moveList[j].max_pp;
		    }
        }
        
        char temp[10];
        strcpy(dresseur->trainerName,"oui");
        fscanf(fichier, "Dernier dresseur battu : %s id: %d\n", temp, &(joueur->lastEnemiID));
        fclose(fichier);
        return 1;
    }
}
