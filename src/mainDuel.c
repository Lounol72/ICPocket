#include "../include/structPoke.h"
#include "../include/duel.h"
#include "../include/trainerAI.h"
#include "../include/interDuel.h"

int main(){
	initData();
	initTeam(&rouge,3);
	initTeam(&bleu,2);
	//use testSwitch or testBattle to try the program's features
	//testBattle(&rouge,&bleu);
	testBattle(&rouge,&bleu);
	return 0;
}