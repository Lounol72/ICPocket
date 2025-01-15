#include "structPoke.c"
/*PITIE ECRIT PAS LES CODES EN UNE LIGNE*/
#define NEUTRAL_STAT_CHANGE 6

typedef struct{
	t_Poke team[6];
} t_Team;

float statVariations[13]={0.25,2./7,1./3,2./5,0.5,2./3,1,1.5,2,2.5,3,3.5,4};

void printTeam(t_Team * t){
	/*
	To do:
	ADD condition if the team has less than 6 poke
	*/
	for(int i=0;i<6;i++){
		if (1){ //Insert condition here
			printPoke(&(t->team[i]));
			getchar();
			system("clear");
		}
	}
}

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

void setDefaultStatChanges(t_Poke * p){
	for(int i=0;i<6;i++) p->statChanges[i]=NEUTRAL_STAT_CHANGE;
}

int isAlive(t_Poke * p){
	return p->current_pv!=0;
}

int isAttacking(int move){
	return move<4 && move>=0;
}

int isSwitching(int move){
	return move>=11 && move<16;
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

void swapActualAttacker(t_Team * t, int swapIndex){
	t_Poke sauv=t->team[0];
	t->team[0]=t->team[swapIndex-10];
	t->team[swapIndex-10]=sauv;
	setDefaultStatChanges(&(t->team[0]));
	printf("Swapping Happened\n\n");
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
		else if (isSwitching(move1)){
			swapActualAttacker(t1,move1);
		}
		else{
			return;
		}
		if(isAlive(&(t2->team[0]))){
			if (isAttacking(move2)){
				affectDamage(&(t2->team[0]),&(t1->team[0]),move2);
			}
			else if (isSwitching(move2)){
				swapActualAttacker(t2,move2);
			}	
			else{
				return;
			}
		}
	}
	else{
		if (isAttacking(move2)){
			affectDamage(&(t2->team[0]),&(t1->team[0]),move2);
		}
		else if (isSwitching(move2)){
			swapActualAttacker(t2,move2);
		}
		else{
			return;
		}
		if (isAlive(&(t1->team[0]))){
			if(isAttacking(move1)){
				affectDamage(&(t1->team[0]),&(t2->team[0]),move1);
			}
			else if (isSwitching(move1)){
				swapActualAttacker(t1,move1);
			}
			else{
				return;
			}
		}
	}

}

void testBattle(t_Team * rouge, t_Team * bleu){
	printf("pv rouge : %d\n\n",rouge->team[0].current_pv);
	printf("pv bleu : %d\n\n",bleu->team[0].current_pv);
	while(isAlive(&(rouge->team[0])) && isAlive(&(bleu->team[0]))){
		playATurn(rouge,rand()%4,bleu,rand()%4);
		printf("pv rouge : %d\n\n",rouge->team[0].current_pv);
		printf("pv bleu : %d\n\n",bleu->team[0].current_pv);
	}
}

void testSwitch(t_Team * rouge, t_Team * bleu){
	printTeam(rouge);
	int move1=rand()%5+11;
	int move2=rand()%4;
	playATurn(rouge,move1,bleu,move2);
	printTeam(rouge);
}

int main(){
	initData();
	t_Team rouge;
	t_Team bleu;
	initTeam(&rouge);
	initTeam(&bleu);
	testSwitch(&rouge,&bleu); //use testSwitch or testBattle to try the program's features
	return 0;
}