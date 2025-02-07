#include "../include/structPoke.h"
#include "../include/duel.h"
#include "../include/trainerAI.h"
#include "../include/interDuel.h"
#include "../include/save.h"

int main(){
	printf("stp");
	initData();
	char choix;
	printf("Charger ou nouveau jeu ?\n");
	scanf("%c",&choix);
	initTeam(&bleu,3);
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

	getPokeFromTeam(&rouge,0,&bleu,0);
	printf("TEAM ROUGE:\n\n");
	printTeam(&rouge);
	return 0;
}