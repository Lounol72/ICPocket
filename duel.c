#include "structPoke.c"

typedef struct{
	t_Poke team[6];
} t_Team;

int calcStatFrom(t_Poke * p,int stat){
	if (stat==PV) return ((int)(p->baseStats[PV]+p->iv[PV])*2*p->lvl/100)+p->lvl+10;
	return (int)(((2*p->baseStats[stat]+p->iv[stat])*p->lvl/100)+5)*tabNature[p->nature].coeff[stat];
}

void initTeam(t_Team * t){
	for(int i=0;i<6;i++){
		generatePoke(&(t->team[i]));
		t->team[i].current_pv=calcStatFrom(&(t->team[i]),PV);
		for(int j=0;j<4;j++){
			t->team[i].moveList[j]=generateRandomMove();
		}
	}
}

int calcDamage(t_Poke * offender,t_Poke * defender,int indexMove){
	printf("Attaque subis d'une puissance de %d\n avec une attaque de %d\ncontre une defence de %d\n",offender->moveList[indexMove].power,calcStatFrom(offender,ATT),calcStatFrom(defender,DEF));
	return ((int)(offender->lvl*0.4+2)) *
	calcStatFrom(offender,ATT) * offender->moveList[indexMove].power /
	calcStatFrom(defender,DEF) / 50 + 2;
	/*
	to add:
		Bonus STAB
		type CHART
		Stat Changes (into calcStatFunction)
		Critical hit
		Random range of damage
	*/
}

int isAlive(t_Poke * p){
	return p->current_pv!=0;
}

int isAttacking(int move){
	return move<4;
}

int PriorityForFirstPoke(t_Poke * p1, t_Poke * p2,int move1,int move2){
	int speedOfP1=calcStatFrom(p1,SPE);
	int speedOfP2=calcStatFrom(p2,SPE);
	if(!isAttacking(move1) && !isAttacking(move2) || (isAttacking(move1) && isAttacking(move2))){
		if (speedOfP1>speedOfP2) return 1;
		else if (speedOfP1<speedOfP2) return 0;
		return rand()%2;
	}
	else if (isAttacking(move1)) return 1;
	return 0;
}

void affectDamage(t_Poke * offender, t_Poke * defender, int indexMove){
	int damage=calcDamage(offender,defender,indexMove);
	defender->current_pv=defender->current_pv>damage?defender->current_pv - damage:0;
}

void playATurn(t_Team * t1, int move1, t_Team * t2, int move2){
	/*
		use speed of pokemon to determine order
		attack or switch
		switch = exchanging index 0 with another index
		attack = index 0 attack index 0 
		*/
	if (PriorityForFirstPoke(&(t1->team[0]),&(t1->team[0]),move1,move2)){
		if(isAttacking(move1)){
			affectDamage(&(t1->team[0]),&(t2->team[0]),move1);
		}
		else{
			return;
		}
		if(isAlive(&(t2->team[0]))){
			if(isAttacking(move2)){
				affectDamage(&(t2->team[0]),&(t1->team[0]),move2);
			}
			else{
				return;
			}
		}
	}
	else{
		if(isAttacking(move2)){
			affectDamage(&(t2->team[0]),&(t1->team[0]),move2);
		}
		else{
			return;
		}
		if(isAlive(&(t1->team[0]))){
			if(isAttacking(move1)){
				affectDamage(&(t1->team[0]),&(t2->team[0]),move1);
			}
			else{
				return;
			}
		}
	}

}


int main(){
	initData();
	t_Team rouge;
	t_Team bleu;
	initTeam(&rouge);
	initTeam(&bleu);
	printPoke(&rouge.team[0]);
	printPoke(&bleu.team[0]);
	while(isAlive(&(rouge.team[0])) && isAlive(&(bleu.team[0]))){
		playATurn(&rouge,rand()%4,&bleu,rand()%4);
		printf("pv rouge : %d\n\n",rouge.team[0].current_pv);
		printf("pv bleu : %d\n\n",bleu.team[0].current_pv);
	}
	return 0;
}