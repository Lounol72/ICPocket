#include "duel.h"

/*id = 1 le joueur*/
typedef struct 
{
	int id;
	char name[100];
	t_Team * trainTeam;
}t_trainer;

void sauvegarder_Joueur(char * name,t_trainer * teamJ1,t_trainer * dresseur);