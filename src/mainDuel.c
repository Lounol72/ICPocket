#include "../include/structPoke.h"
#include "../include/duel.h"
#include "../include/trainerAI.h"

t_Team rouge;
t_Team bleu;

int main(){
	initData();
	//teamTest(&rouge);
	//teamTest(&bleu);
	initTeam(&rouge,3);
	printTeam(&rouge); //use testSwitch or testBattle to try the program's features
	return 0;
}