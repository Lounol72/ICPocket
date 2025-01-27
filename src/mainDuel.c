#include "../include/structPoke.h"
#include "../include/duel.h"
#include "../include/trainerAI.h"

t_Team rouge;
t_Team bleu;

void teamTest(t_Team * t){
	initTeam(t);
	for(int i=0;i<6;i++){
		t->team[i].type[1]=noType;
		t->team[i].lvl=50;
		t->team[i].nature=0;
		t->team[i].type[0]=plante;
		for(int j=0;j<7;j++){
			t->team[i].baseStats[j]=100;
		}
		for(int j=0;j<6;j++){
			t->team[i].iv[j]=31;
		}
		t->team[i].current_pv=calcStatFrom(&(t->team[i]),PV);
	}
}

int main(){
	initData();
	teamTest(&rouge);
	teamTest(&bleu);
	testBattle(&rouge,&bleu); //use testSwitch or testBattle to try the program's features
	return 0;
}