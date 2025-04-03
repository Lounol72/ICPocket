#include "../include/save.h"
#include <sys/stat.h>
#include <sys/types.h>
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
        char * path = "data/save";
        mkdir(path, 0777);
        fichier = fopen(nomFichier, "w+");
        sauver(joueur,1,nomFichier);
    } else {
        fprintf(fichier,"%d\n",joueur->nb_poke);
        for(int i = 0; i < joueur->nb_poke; i++){
            fprintf(fichier,"%d;%d;%d;%d;",joueur->team[i].id,joueur->team[i].lvl,joueur->team[i].nature,joueur->team[i].nb_move);
            for(int j = 0 ; j < 6 ; j++)fprintf(fichier, "%d;", joueur->team[i].iv[j]);
            for(int j = 0 ; j < 2 ; j++)fprintf(fichier, "%d;", joueur->team[i].type[j]);
            for(int j = 0 ; j < joueur->team[i].nb_move ; j++)fprintf(fichier, "%d;", joueur->team[i].moveList[j].id);
            fprintf(fichier,"\n");
        }
        fprintf(fichier, "%d;",joueur->nb_enemiBeat);
        fprintf(fichier, "%s;%d;", adverse->trainerName, adverse->id);
        fprintf(fichier,"%d\n",joueur->id_save);
        fprintf(fichier,"check:1;");
        printf("Sauvegarde effectuée2\n");
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
        //printf("Erreur : impossible d'ouvrir le fichier de sauvegarde.\n");
        //printf("Creation d'un fichier sauvegarde\n");
        fichier = fopen(filePath, "w+");
        sauver(joueur,save,nomSave);
        //printf("sauvegarde noumero %d" , joueur->id_save);
        fclose(fichier);
        return -1;

    } else {
        fseek(fichier, 0, SEEK_END);
        if(ftell(fichier)==0){
            printf("Sauvegarde vide\n");
            sauver(joueur,save,nomSave);
            fclose(fichier);
            return -1;
        }
        fseek(fichier, 0, SEEK_SET);
        fscanf(fichier, "%d\n", &(joueur->nb_poke));
        joueur->effect = noEffect;
        for (int i = 0,nbmove; i < joueur->nb_poke; i++) {

            joueur->team[i].main_effect = noEffect;
            fscanf(fichier, "%d;%d;%d;%d;", &(joueur->team[i].id), &(joueur->team[i].lvl), &(joueur->team[i].nature), &(joueur->team[i].nb_move));
            //printf("poke id : %d , lvl : %d ,nature: %d , nb move: %d \n",joueur->team[i].id,joueur->team[i].lvl,joueur->team[i].nature,joueur->team[i].nb_move);
            nbmove = joueur->team[i].nb_move;
            generate_poke(&(joueur->team[i]), joueur->team[i].id);
            joueur->team[i].exp=expCurve(joueur->team[i].lvl);
            
            for (int j = 0; j < 6; j++) {
                fscanf(fichier, "%d;", &(joueur->team[i].iv[j]));
                //printf("iv %d : %d\n",j,joueur->team[i].iv[j]);
            }
            for (int j = 0; j < 2; j++) {
                fscanf(fichier, "%d;", (int *)&(joueur->team[i].type[j]));
                //printf("type %d : %d\n",j,joueur->team[i].type[j]);
            }
            //printf("nb_move 222: %d\n",joueur->team[i].nb_move);
            //printf("nb_move 333: %d\n",nbmove);
            for (int j = 0; j < nbmove; j++) {
                fscanf(fichier, "%d;", &(joueur->team[i].moveList[j].id));
                joueur->team[i].moveList[j] = generateMove(joueur->team[i].moveList[j].id);
                //printf("move id : %d\n",joueur->team[i].moveList[j].id);
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
        fscanf(fichier, "%d;",&(joueur->nb_enemiBeat));
        fscanf(fichier, "%[^\n];%d;", dresseur->trainerName, &(dresseur->id));
        fscanf(fichier,"%d\n",&(joueur->id_save));
        char check[10];
        fscanf(fichier, "%s;", check);
        printf("%s\n",check);

        //Si le parcours a bien été éffectué
        if(strcmp(check,"check:1;")==0){
            //printf("Chargement valide\n");
            sauver(joueur,save,nomSave);
        }
        else{
            //printf("Chargement echoué\n");
            sauver(joueur,save,nomSave);  
            return -1;
        }
        fclose(fichier);
        //printf("Chargement effectué\n");
        return 1;
    }
}
void saveDestroy(t_Team * joueur){
    char nomFichier[1024];
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

    snprintf(nomFichier, sizeof(nomFichier), "data/save/Save_%d.txt", joueur->id_save);
    FILE *fichier = fopen(nomFichier, "w+");
    if(fichier == NULL){
        exit(1);
    } else {
        //Fonction pour retirer les fichiers
        remove(nomFichier);
        //printf("Fichier de sauvegarde supprimé\n");
        fclose(fichier);
    }
}
/**
*   @brief Fonction qui sauvegarde le nom de la sauvegarde
*   @param joueur struct joueur
*   @param save id de la sauvegarde
*   @param nomsave nom de la sauvegarde
*/

void sauver(t_Team * joueur,int save,char * nomsave){
            save = atoi(nomsave);
            joueur->id_save = save;
            //printf("sauvegarde noumero %d\n" , joueur->id_save);
}