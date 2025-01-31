#include "../include/structPoke.h"
#include "../include/duel.h"
#include "../include/interDuel.h"

void healTeam(t_Team * t){
    for(int i=0;i<t->team[0].nb_poke;i++){
        t->team[i].current_pv=calcStatFrom(&(t->team[i]),PV);
        for(int j=0;j<t->team[0].nb_move;j++){
            t->team[i].moveList[j].current_pp=t->team[i].moveList[j].max_pp;
        }
    }
}