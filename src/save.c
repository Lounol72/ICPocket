#include "../include/structPoke.h"
#include "../include/save.h"
#include "../include/ministdlib.h"
/*
void sauvegarder_Joueur(t_trainer * joueur,t_trainer * dresseur){
    

    printf("Nom de la sauvegarde : ");
    char nom[100];
    scanf("%s", nom);
    strcpy(joueur->name, nom);

    joueur->id = 1;
    dresseur->id = 2;
    

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
                calcStatFrom(&joueur->trainTeam.team[i],joueur->trainTeam.team[i]->baseStats[j]);
            }
            fprintf(fichier, "Poke %d : %s\n", i+1, joueur->trainTeam->team[i]->name);
        }

        fprintf(fichier, "Dernier dresseur battu : %s id: %d\n", dresseur->name, dresseur->id);
        
    }
    
}
*/
int main(){
    t_trainer  *red=malloc(sizeof(t_trainer));
    red->trainTeam = malloc(sizeof(t_Team));

    for(int i = 0; i < 6; i++){
        generatePoke(red->trainTeam->team[i]);
    }
    for(int i = 0; i < 6; i++){
        
            //printPoke(red->trainTeam->team[i]);
        
    }
    
    free(red->trainTeam);

    free(red);
    return 0;

}