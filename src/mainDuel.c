#include "../include/structPoke.h"
#include "../include/duel.h"
#include "../include/trainerAI.h"

t_Team rouge;
t_Team bleu;

int main(){
	initData();
	teamTest(&rouge,3);
	teamTest(&bleu,2);
	//use testSwitch or testBattle to try the program's features
	printf("%d\n",AI_move_choice(&iaTest,&rouge));
	return 0;
}