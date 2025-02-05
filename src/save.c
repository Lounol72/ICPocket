#include "../include/structPoke.h"
#include "../include/save.h"
#include "../include/ministdlib.h"
#include "../include/trainerAI.h"


void sauvegarder_Joueur(char * nomSave ,t_trainer * joueur,t_trainer * dresseur){
    

    char nomFichier[1024];
	strcpy(nomFichier , "src/data/save/");
	strcat(nomFichier,nomSave);
	strcat(nomFichier,".txt");

    joueur->id = 1;
    
    FILE *fichier = fopen(nomFichier, "w+");
    if(fichier == NULL){
        printf("Erreur : impossible d'ouvrir la save.\n");
        exit(1);
    }
    
    else{
        strcat(joueur->name,nomSave);
        fprintf(fichier, "User name : %s\n", joueur->name);
        for(int i = 0; i < 6; i++){
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

void charger_Joueur(char * nomSave,t_trainer * joueur,t_trainer * dresseur){
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
            fscanf(fichier, "%d\n", &(joueur->trainTeam->team[i].id));
            generate_poke(&(joueur->trainTeam->team[i]),joueur->trainTeam->team[i].id);
            fscanf(fichier, "%s\n", joueur->trainTeam->team[i].name);
            fscanf(fichier, "%d\n", &(joueur->trainTeam->team[i].lvl));
            fscanf(fichier, "%d\n", &(joueur->trainTeam->team[i].nature));
            getchar();
            for(int j = 0 ; j < 6 ; j++)
                fscanf(fichier, "%d\n", &(joueur->trainTeam->team[i].iv[j]));
            fscanf(fichier, "%d\n", &(joueur->trainTeam->team[i].nb_move));
            for(int j = 0 ; j < 1 ; j++)
                fscanf(fichier, "%d\n", (int *)&(joueur->trainTeam->team[i].type[j]));
            /*for(int j = 0 ; j < 1 ; j++)
                fscanf(fichier, "%d\n", joueur->trainTeam->team[i].moveList[j]);*/ //Les moves ne sont pas encore init 

            fscanf(fichier,"\n\n");
        }

        fscanf(fichier, "Dernier dresseur battu : %s id: %d\n", dresseur->name, &(dresseur->id));
    }

}
/** 
int main(){
    initData();
    
    char nom[20];
    printf("Nom joueur");
    scanf("%s" , nom);

    t_trainer  *red=malloc(sizeof(t_trainer));
    red->trainTeam = malloc(sizeof(t_Team));
    t_trainer *blue = malloc(sizeof(t_trainer));
    blue->id = 48;
    strcat(blue->name,"blue");
    initTeam(red->trainTeam,6);
    
    sauvegarder_Joueur(nom,red,blue);
    for(int i = 0; i < 6; i++){
        
            printPoke(&(red->trainTeam->team[i]));
        
    }
    
    free(red->trainTeam);

    free(red);
    

    char nom[20];
    printf("Nom de la sauvegarde à charger: ");
    scanf("%s", nom);

    t_trainer *red = malloc(sizeof(t_trainer));
    red->trainTeam = malloc(sizeof(t_Team));
    t_trainer *blue = malloc(sizeof(t_trainer));

    charger_Joueur(nom, red, blue);

    printf("Nom du joueur: %s\n", red->name);
    for(int i = 0; i < 6; i++){
        printPoke(&(red->trainTeam->team[i]));
    }
    printf("Dernier dresseur battu: %s id: %d\n", blue->name, blue->id);

    free(red->trainTeam);
    free(red);
    free(blue);
    return 0;

}
*/