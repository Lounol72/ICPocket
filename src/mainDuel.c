#include "../include/structPoke.h"
#include "../include/duel.h"
#include "../include/trainerAI.h"
#include "../include/interDuel.h"

int main(){
	initData();
	teamTest(&rouge,3);
	teamTest(&bleu,2);
	//use testSwitch or testBattle to try the program's features
	testBattle(&rouge,&bleu);
	healTeam(&rouge);
	healTeam(&bleu);
	getchar();
	testBattle(&rouge,&bleu);
	return 0;
}