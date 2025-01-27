#include "../include/structPoke.h"
#include "../include/duel.h"
#include "../include/trainerAI.h"
/*
t_AI iaTest={10,none,&bleu};

int AI_move_choice(t_AI * ai, t_Team * p){
	int tab_damage[4]={-1};
	for(int i=0;i<4;i++){
		tab_damage[i]=calcDamage(ai->AI_t_Team,p,&(ai->AI_t_Team->team[0].moveList[i]));
	}
	return 0;
}