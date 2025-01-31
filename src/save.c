#include "../include/structPoke.h"
//#include "../include/duel.h"
#include "../include/save.h"
#include "../include/ministdlib.h"
#include "../include/trainerAI.h"


void sauvegarder_Joueur(t_trainer * joueur,t_trainer * dresseur){
    

    printf("Nom de la sauvegarde : ");
    scanf("%s", joueur->name);

    joueur->id = 1;
    
    FILE *fichier = fopen("src/data/save/saveJoueur.txt", "w+");
    if(fichier == NULL){
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        exit(1);
    }
    
    else{
        
        fprintf(fichier, "User name : %s\n", joueur->name);
        for(int i = 0; i < 6; i++){
            for(int j = 0; j < 6; j++){
                calcStatFrom(&(joueur->trainTeam->team[i]),j);
            }
            fprintf(fichier, "%d\n", joueur->trainTeam->team[i].id);
            fprintf(fichier, "%s\n", joueur->trainTeam->team[i].name);
            fprintf(fichier, "%d\n", joueur->trainTeam->team[i].lvl);
            fprintf(fichier, "%d\n", joueur->trainTeam->team[i].nature);
            for(int j = 0 ; j < 6 ; j++)
                fprintf(fichier, "%d\n", joueur->trainTeam->team[i].iv[j]);
            fprintf(fichier, "%d\n", joueur->trainTeam->team[i].nb_move);
            for(int j = 0 ; j < 1 ; j++)
                fprintf(fichier, "%d\n", joueur->trainTeam->team[i].type[j]);
            /*for(int j = 0 ; j < 1 ; j++)
                fprintf(fichier, "%d\n", joueur->trainTeam->team[i].moveList[j]);*/ //Les moves ne sont pas encore init 

            fprintf(fichier,"\n\n");
        }

        fprintf(fichier, "Dernier dresseur battu : %s id: %d\n", dresseur->name, dresseur->id);
        
    }
    
}

/*
int main(){
    t_trainer  *red=malloc(sizeof(t_trainer));
    red->trainTeam = malloc(sizeof(t_Team));
    t_trainer *blue = malloc(sizeof(t_trainer));
    blue->id = 48;
    strcat(blue->name,"blue");
    for(int i = 0 ; i<6 ; i++){
        generate_poke(&(red->trainTeam->team[i]),"1");
    }
    
    sauvegarder_Joueur(red,blue);
    for(int i = 0; i < 6; i++){
        
            printPoke(&(red->trainTeam->team[i]));
        
    }
    
    free(red->trainTeam);

    free(red);
    return 0;

}*/