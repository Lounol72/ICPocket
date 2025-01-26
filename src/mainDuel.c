#include "include/structPoke.h"
#include "include/duel.h"
//#include "include/trainerAI.h"

int main(){
	initData();
	t_Team rouge;
	t_Team bleu;
	initTeam(&rouge);
	initTeam(&bleu);
	testStruggle(&rouge,&bleu); //use testSwitch or testBattle to try the program's features
	return 0;
}