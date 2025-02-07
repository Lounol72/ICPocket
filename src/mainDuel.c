#include "../include/structPoke.h"
#include "../include/duel.h"
#include "../include/trainerAI.h"
#include "../include/interDuel.h"

int main(){
	initData();
	initTeam(&rouge,3);
	initBlueTeam(&bleu);
	//use testSwitch or testBattle to try the program's features
	//testBattle(&rouge,&bleu);
	printf("TEAM ROUGE:\n\n");
	printTeam(&rouge);
	printf("TEAM BLEU:\n\n");
	printTeam(&bleu);

	getPokeFromTeam(&rouge,0,&bleu,0);
	printf("TEAM ROUGE:\n\n");
	printTeam(&rouge);
	return 0;
}