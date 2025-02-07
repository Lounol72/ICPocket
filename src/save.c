#include "../include/GameEngine.h"
#include "../include/ministdlib.h"
#include "../include/save.h"


void sauvegarder(char * nomSave ,t_team * joueur,t_team * adverse){
    

    char nomFichier[1024];
	strcpy(nomFichier , "src/data/save/");
	strcat(nomFichier,nomSave);
	strcat(nomFichier,".txt");

    joueur->id = 1;
    
    FILE *fichier = fopen(nomFichier, "w");
    if(fichier == NULL){
        printf("Erreur : impossible d'ouvrir la save.\n");
        exit(1);
    }
    
    else{
        strcat(joueur->name,nomSave);
        fprintf(fichier, "User name : %s\n", joueur->name);
        for(int i = 0; i < 6; i++){
            fprintf(fichier, "%d\n", joueur->team[i].id);
            fprintf(fichier, "%s\n", joueur->team[i].name);
            fprintf(fichier, "%d\n", joueur->team[i].lvl);
            fprintf(fichier, "%d\n", joueur->team[i].nature);
            for(int j = 0 ; j < 6 ; j++)
                fprintf(fichier, "%d\n", joueur->team[i].iv[j]);
            fprintf(fichier, "%d\n", joueur->team[i].nb_move);
            for(int j = 0 ; j < 1 ; j++)
                fprintf(fichier, "%d\n", joueur->team[i].type[j]);
            /*for(int j = 0 ; j < 1 ; j++)
                fprintf(fichier, "%d\n", joueur->team[i].moveList[j]);*/ //Les moves ne sont pas encore init 

            fprintf(fichier,"\n\n");
        }
        fprintf(fichier, "Dernier dresseur battu : %s id: %d\n", adverse->name, adverse->id);
        
    }
    
}

void charger(char * nomSave,t_team * joueur,t_team * dresseur){
    char nomFichier[1024];
	strcpy(nomFichier , "src/data/save/");
	strcat(nomFichier,nomSave);
	strcat(nomFichier,".txt");

    FILE *fichier = fopen(nomFichier, "r");
    if(fichier == NULL){
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        exit(1);
    }else{
        fscanf(fichier, "User name : %s\n", joueur->name);
        for(int i = 0; i < 6; i++){
            fscanf(fichier, "%d\n", &(joueur->team[i].id));
            generate_poke(&(joueur->team[i]),joueur->team[i].id);
            fscanf(fichier, "%s\n", joueur->team[i].name);
            fscanf(fichier, "%d\n", &(joueur->team[i].lvl));
            fscanf(fichier, "%d\n", &(joueur->team[i].nature));
            getchar();
            for(int j = 0 ; j < 6 ; j++)
                fscanf(fichier, "%d\n", &(joueur->team[i].iv[j]));
            fscanf(fichier, "%d\n", &(joueur->team[i].nb_move));
            for(int j = 0 ; j < 1 ; j++)
                fscanf(fichier, "%d\n", (int *)&(joueur->team[i].type[j]));
            /*for(int j = 0 ; j < 1 ; j++)
                fscanf(fichier, "%d\n", joueur->team[i].moveList[j]);*/ //Les moves ne sont pas encore init 

            fscanf(fichier,"\n\n");
        }

        fscanf(fichier, "Dernier dresseur battu : %s id: %d\n", dresseur->name, &(dresseur->id));
    }

}
