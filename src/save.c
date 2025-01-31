#include "../include/structPoke.h"
//#include "../include/duel.h"
#include "../include/save.h"
#include "../include/ministdlib.h"
#include "../include/trainerAI.h"


void sauvegarder_Joueur(t_trainer * joueur,t_trainer * dresseur){
    

    printf("Nom de la sauvegarde : ");
    scanf("%s", joueur->name);

    joueur->id = 1;
    
    printf("%s" , joueur->name);
    getchar();
    FILE *fichier = fopen("src/data/save/saveJoueur.txt", "w+");
    if(fichier == NULL){
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        exit(1);
    }
    
    else{
        
        fprintf(fichier, "User name : %s\n", joueur->name);
        fprintf(fichier, "Team : \n");
        for(int i = 0; i < 6; i++){
            for(int j = 0; j < 6; j++){
                calcStatFrom(&(joueur->trainTeam->team[i]),joueur->trainTeam->team[i].baseStats[j]);
            }
            fprintf(fichier, "Poke %d : %s\n", i+1, joueur->trainTeam->team[i].name);
        }

        fprintf(fichier, "Dernier dresseur battu : %s id: %d\n", dresseur->name, dresseur->id);
        
    }
    
}

int main(){
    t_trainer  *red=malloc(sizeof(t_trainer));
    red->trainTeam = malloc(sizeof(t_Team));
    t_trainer *blue = malloc(sizeof(t_trainer));
    blue->id = 48;
    strcat(blue->name,"blue");

    for(int i = 1; i < 7; i++){
        generate_poke(&(red->trainTeam->team[i]),"1");
    }
    sauvegarder_Joueur(red,blue);
    for(int i = 0; i < 6; i++){
        
            printPoke(&(red->trainTeam->team[i]));
        
    }
    
    free(red->trainTeam);

    free(red);
    return 0;

}