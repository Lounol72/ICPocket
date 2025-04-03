
#include "../include/duel.h"

/**
 * @brief Cette fonction initialise les données du jeu.
 * 
 * @return int 
 */
int main(){
	initData();
	char choix;
	printf("Charger ou nouveau jeu ?\n");
	scanf("%c",&choix);
	initBlueTeam(&bleu,&rouge);
	if(choix=='y'){
		charger("Save_1",&rouge,&bleu);
	} else {
		initTeam(&rouge,3);
	}
	//use testSwitch or testBattle to try the program's features
	testBattle(&rouge,&bleu);
	getchar();
	printf("TEAM ROUGE:\n\n");
	printTeam(&rouge);
	printf("TEAM BLEU:\n\n");
	printTeam(&bleu);
	return 0;
}