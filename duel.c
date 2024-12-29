#include "structPoke.c"

#define NEUTRAL_STAT_CHANGE 6

typedef struct{
	t_Poke team[6];
} t_Team;

float statVariations[13]={0.25,2./7,1./3,2./5,0.5,2./3,1,1.5,2,2.5,3,3.5,4};

int calcStatFrom(t_Poke * p,int stat,int includeVariations){
	if (stat==PV) return ((int)(2*p->baseStats[PV]+p->iv[PV])*p->lvl/100)+p->lvl+10;
	int value=(int)(((2*p->baseStats[stat]+p->iv[stat])*p->lvl/100)+5)*tabNature[p->nature].coeff[stat];
	return includeVariations?value*statVariations[p->statChanges[stat]]:value;
}

void initTeam(t_Team * t){
	for(int i=0;i<6;i++){
		generatePoke(&(t->team[i]));
		for(int j=0;j<6;j++) t->team[i].statChanges[j]=NEUTRAL_STAT_CHANGE;
		t->team[i].current_pv=calcStatFrom(&(t->team[i]),PV,FALSE);
		for(int j=0;j<4;j++){
			t->team[i].moveList[j]=generateRandomMove();
		}
	}
}

int calcDamage(t_Poke * offender,t_Poke * defender,int indexMove){
	int coupCritique=(rand()%24==0);
	int damage;
	if (coupCritique){
			damage = (((int)(offender->lvl*0.4+2)) *
			calcStatFrom(offender,ATT,offender->statChanges[ATT]>=NEUTRAL_STAT_CHANGE?TRUE:FALSE) 
			* offender->moveList[indexMove].power /
			calcStatFrom(defender,DEF,defender->statChanges[DEF]<=NEUTRAL_STAT_CHANGE?TRUE:FALSE) / 50 + 2 ) * 1.5;
	}
	else{
		damage= (((int)((int)((int)(offender->lvl*0.4+2)) *
		calcStatFrom(offender,ATT,TRUE) * offender->moveList[indexMove].power /
		calcStatFrom(defender,DEF,TRUE)) / 50) + 2);
	}
	damage*=(rand()%16+85)/100.0;
	printf("Attaque subis d'une puissance de %d\n avec une attaque de %d\ncontre une defence de %d\n",offender->moveList[indexMove].power,calcStatFrom(offender,ATT,TRUE),calcStatFrom(defender,DEF,TRUE));
	printf("Dégats = %d\n",damage);
	if (coupCritique) printf("Coup Critique!\n");
	return damage;
	/*
	to add:
		Bonus STAB
		type CHART
	*/
}

int isAlive(t_Poke * p){
	return p->current_pv!=0;
}

int isAttacking(int move){
	return move<4;
}

int PriorityForFirstPoke(t_Poke * p1, t_Poke * p2,int move1,int move2){
	int speedOfP1=calcStatFrom(p1,SPE,TRUE);
	int speedOfP2=calcStatFrom(p2,SPE,TRUE);
	if(!isAttacking(move1) && !isAttacking(move2) || (isAttacking(move1) && isAttacking(move2))){
		if (speedOfP1>speedOfP2) return 1;
		if (speedOfP1<speedOfP2) return 0;
		return rand()%2;
	}
	if (isAttacking(move1)) return 0;
	return 1;
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
	if (PriorityForFirstPoke(&(t1->team[0]),&(t2->team[0]),move1,move2)){
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
	printf("pv rouge : %d\n\n",rouge.team[0].current_pv);
	printf("pv bleu : %d\n\n",bleu.team[0].current_pv);
	while(isAlive(&(rouge.team[0])) && isAlive(&(bleu.team[0]))){
		playATurn(&rouge,rand()%4,&bleu,rand()%4);
		printf("pv rouge : %d\n\n",rouge.team[0].current_pv);
		printf("pv bleu : %d\n\n",bleu.team[0].current_pv);
	}
	return 0;
}