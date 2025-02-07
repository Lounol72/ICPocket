#include "../include/structPoke.h"
#include "../include/duel.h"
#include "../include/trainerAI.h"
#include "../include/interDuel.h"

int main(){
	printf("stp");
	initData();
	initTeam(&rouge,3);
	initTeam(&bleu,3);
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