#include "../include/structPoke.h"
#include "../include/duel.h"
#include "../include/trainerAI.h"

t_AI iaTest={10,damageOnly,&bleu};

void insertionSort(int tabDegats[], int tabMove[], int n) {
    int i, keyDeg, keyMove, j;
    for (i = 1; i < n; i++) {
        keyDeg = tabDegats[i];
		keyMove = tabMove[i];
        j = i - 1;

        // Move elements of arr[0..i-1], that are greater than key, to one position ahead of their current position
        while (j >= 0 && tabDegats[j] > keyDeg) {
            tabDegats[j + 1] = tabDegats[j];
			tabMove[j + 1] = tabMove[j];
            j = j - 1;
        }
        tabDegats[j + 1] = keyDeg;
		tabMove[j + 1] = keyMove;
    }
}

int AI_move_choice(t_AI * ai, t_Team * p){
	int tab_damage[4]={-1};
	int tab_move[4]={0,1,2,3};
	for(int i=0;i<4;i++){
		tab_damage[i]=calcDamage(ai->AI_t_Team,p,&(ai->AI_t_Team->team[0].moveList[i]));
	}
	insertionSort(tab_damage,tab_move,4);

    int ind=0;
    if (ai->type%3==0){ // status_first AI
        ind=(rand()%ai->AI_lvl)%4;
    }
    else if(ai->type%2==0){ // damage_first AI
        while(ind<4 && rand()%20>ai->AI_lvl) ind++;
    }
    else{ //none type AI
        ind=(rand()%ai->AI_lvl)%4;
    }
	return tab_move[ind];
}