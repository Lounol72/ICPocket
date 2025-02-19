#include "../include/GameEngine.h"
#include "../include/ministdlib.h"
#include "../include/save.h"


void sauvegarder(char * nomSave ,t_Team * joueur,t_Team * adverse){
    char nomFichier[1024];
    strcpy(nomFichier , "src/data/save/");
    strcat(nomFichier,nomSave);
    strcat(nomFichier,".txt");
    FILE *fichier = fopen(nomFichier, "w");
    if(fichier == NULL){
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        exit(1);
    } else {
        fprintf(fichier, "User name : SAVE1\n");
        fprintf(fichier, "%d\n" , joueur->nb_poke);
        for(int i = 0; i < joueur->nb_poke; i++){
            fprintf(fichier, "%d\n", joueur->team[i].id);
            fprintf(fichier, "%d\n", joueur->team[i].lvl);
            fprintf(fichier, "%d\n", joueur->team[i].nature);
            fprintf(fichier, "%d\n", joueur->team[i].nb_move);
            for(int j = 0 ; j < 6 ; j++)
                fprintf(fichier, "%d\n", joueur->team[i].iv[j]);
            for(int j = 0 ; j < 1 ; j++)
                fprintf(fichier, "%d\n", joueur->team[i].type[j]);
            for(int j = 0 ; j < joueur->team[i].nb_move ; j++){
                fprintf(fichier, "%d\n", joueur->team[i].moveList[j].id);
            }
            fprintf(fichier,"\n\n");
        }
        fprintf(fichier, "Dernier dresseur battu : %s id: %d\n", adverse->trainerName, adverse->id);
        printf("Sauvegarde effectuée\n");
        fclose(fichier);
    }
}

//Charger une sauvegarde
void charger(char *nomSave, t_Team *joueur, t_Team *dresseur) {
    char filePath[256];
    snprintf(filePath, sizeof(filePath), "src/data/save/%s.txt", nomSave);
    FILE *fichier = fopen(filePath, "r");
    if (fichier == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier de sauvegarde.\n");
        exit(1);
    } else {
        fscanf(fichier, "User name : %s\n", joueur->trainerName);
        fscanf(fichier, "%d\n", &(joueur->nb_poke));
        joueur->effect = noEffect;
        for (int i = 0; i < joueur->nb_poke; i++) {
            joueur->team[i].main_effect = noEffect;
            fscanf(fichier, "%d\n", &(joueur->team[i].id));
            generate_poke(&(joueur->team[i]), joueur->team[i].id);
            fscanf(fichier, "%d\n", &(joueur->team[i].lvl));
            fscanf(fichier, "%d\n", &(joueur->team[i].nature));
            fscanf(fichier, "%d\n", &(joueur->team[i].nb_move));
            for (int j = 0; j < 6; j++)
                fscanf(fichier, "%d\n", &(joueur->team[i].iv[j]));
            for (int j = 0; j < 1; j++)
                fscanf(fichier, "%d\n", (int *)&(joueur->team[i].type[j]));
            for (int j = 0; j < joueur->team[i].nb_move; j++) {
                fscanf(fichier, "%d\n", &(joueur->team[i].moveList[j].id));
                joueur->team[i].moveList[j] = generateMove(joueur->team[i].moveList[j].id);
            }
        }
        fscanf(fichier, "Dernier dresseur battu : %s id: %d\n", dresseur->trainerName, &(dresseur->id));
        fclose(fichier);
    }
}
