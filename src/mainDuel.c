#include "../include/structPoke.h"
#include "../include/duel.h"
#include "../include/trainerAI.h"

t_Team rouge;
t_Team bleu;

int main(){
	initData();
	teamTest(&rouge);
	teamTest(&bleu);
	testBattle(&rouge,&bleu); //use testSwitch or testBattle to try the program's features
	return 0;
}