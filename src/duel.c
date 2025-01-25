#include "structPoke.c"
#include "include/duel.h"

float statVariations[13]={0.25,2./7,1./3,2./5,0.5,2./3,1,1.5,2,2.5,3,3.5,4};

t_Move struggle={"Lutte",50,noType,physical,200,1,1,0};

void printTeam(t_Team * t){
	for(int i=0;i<6;i++){
		if (!(t->team[i].current_pv==POKE_IS_ABSCENT)){
			printPoke(&(t->team[i]));
			getchar();
			system("clear");
		}
	}
}

int testActionValidity(int action, t_Team * t){
	if (isStruggling(action)){
		return TRUE;
	}
	if (isAttacking(action)){
		return ppCheck(&(t->team[0].moveList[action]));
	}
	if (isSwitching(action)){
		return t->team[action-10].current_pv>0?TRUE:FALSE;
	}
	return FALSE;
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
			t->team[i].moveList[j].current_pp=t->team[i].moveList[j].max_pp;
		}
	}
}

int calcDamage(t_Poke * offender,t_Poke * defender, t_Move * move){
	int coupCritique=(rand()%24==0);
	int damage;

	int targetedStatOff=move->categ; //différenciation attaque/attaque spéciale
	int targetedStatDef=targetedStatOff==ATT?DEF:SPD;
	if (coupCritique){
			damage = (((int)(offender->lvl*0.4+2)) *
			calcStatFrom(offender,targetedStatOff,offender->statChanges[ATT]>=NEUTRAL_STAT_CHANGE?TRUE:FALSE) 
			* move->power /
			calcStatFrom(defender,targetedStatDef,defender->statChanges[DEF]<=NEUTRAL_STAT_CHANGE?TRUE:FALSE) / 50 + 2 ) * 1.5;
	}
	else{
		damage= (((int)((int)((int)(offender->lvl*0.4+2)) *
		calcStatFrom(offender,targetedStatOff,TRUE) * move->power /
		calcStatFrom(defender,targetedStatDef,TRUE)) / 50) + 2);
	}
	damage*=(rand()%16+85)/100.0;  //fourchette aléatoire de dégats

	damage*=offender->type[0]==move->type || offender->type[1]==move->type?1.5:1; // bonus STAB

	damage*=typeChart[move->type][defender->type[0]]*typeChart[move->type][defender->type[1]]; // efficacité de types


	printf("Attaque subis d'une puissance de %d\n avec une attaque de %d\ncontre une defence de %d\n",move->power,calcStatFrom(offender,targetedStatOff,TRUE),calcStatFrom(defender,targetedStatDef,TRUE));
	printf("Dégats = %d\n",damage);
	if (coupCritique) printf("Coup Critique!\n");
	return damage>0?damage:1;
}

void setDefaultStatChanges(t_Poke * p){
	for(int i=0;i<6;i++) p->statChanges[i]=NEUTRAL_STAT_CHANGE;
}

int isAlive(t_Poke * p){
	return p->current_pv!=0;
}

int isAttacking(int move){
	return (move<4 && move>=0) || isStruggling(move);
}

int isSwitching(int move){
	return move>=11 && move<16;
}

int isStruggling(int move){
	return move==STRUGGLE;
}

int hasMoveLeft(t_Poke * p){
	int somme=0;
	for(int i=0;i<4;i++){
		somme+=p->moveList[i].current_pp;
	}
	return somme;
}

int resolveSpeedDuel(int speed1, int speed2){
	if (speed1>speed2) return 1;
	if (speed1<speed2) return 0;
	return rand()%2; //speed tie : choose winner randomly
}

int PriorityForFirstPoke(t_Poke * p1, t_Poke * p2,int move1,int move2){
	int speedOfP1=calcStatFrom(p1,SPE,TRUE);
	int speedOfP2=calcStatFrom(p2,SPE,TRUE);

	/*Cas attack only*/
	if(isAttacking(move1) && isAttacking(move2)){
		if(p1->moveList[move1].priority_lvl>p2->moveList[move2].priority_lvl) return 1;
		if(p2->moveList[move2].priority_lvl>p1->moveList[move1].priority_lvl) return 0;
		return resolveSpeedDuel(speedOfP1,speedOfP2); //same priority lvl
	}

	/*Cas switch only*/
	if(isSwitching(move1) && isSwitching(move2)){
		return resolveSpeedDuel(speedOfP1,speedOfP2);
	}
	/*Cas conflit switch vs attack*/
	if (isAttacking(move1)) return 0;
	return 1;
}

int accuracyCheck(int accuracy){
	return rand()%100<accuracy;
}

int ppCheck(t_Move * move){
	return move->current_pp>0;
}

void affectDamage(t_Poke * offender, t_Poke * defender, int indexMove){
	if(!isStruggling(indexMove) && !accuracyCheck(offender->moveList[indexMove].accuracy)){
		printf("%s rate son attaque (%d precision)\n",offender->name,offender->moveList[indexMove].accuracy);
		return;
	}
	int damage=calcDamage(offender,defender,isStruggling(indexMove)?&struggle:&(offender->moveList[indexMove]));
	defender->current_pv=defender->current_pv>damage?defender->current_pv - damage:0;
	if (!isStruggling(indexMove)) (offender->moveList[indexMove].current_pp)--;
}

void swapActualAttacker(t_Team * t, int swapIndex){
	t_Poke sauv=t->team[0];
	t->team[0]=t->team[swapIndex-10];
	t->team[swapIndex-10]=sauv;
	setDefaultStatChanges(&(t->team[0]));
	printf("Swapping Happened\n\n");
}

int playATurn(t_Team * t1, int move1, t_Team * t2, int move2){
	/*
		use speed of pokemon to determine order
		attack or switch
		switch = exchanging index 0 with another index*/

		if(!hasMoveLeft(&(t1->team[0])) && isAttacking(move1)) move1=STRUGGLE;
		if(!hasMoveLeft(&(t2->team[0])) && isAttacking(move2)) move2=STRUGGLE;

		if(!testActionValidity(move1,t1) || !testActionValidity(move2,t2)) return FALSE;

		int first = PriorityForFirstPoke(&(t1->team[0]), &(t2->team[0]), move1, move2);
		t_Team *firstTeam = first ? t1 : t2;
		t_Team *secondTeam = first ? t2 : t1;
		int firstMove = first ? move1 : move2;
		int secondMove = first ? move2 : move1;

		if (isAttacking(firstMove)) affectDamage(&(firstTeam->team[0]), &(secondTeam->team[0]), firstMove);
		else if (isSwitching(firstMove)) swapActualAttacker(firstTeam, firstMove);
		else return FALSE;

		if (isAlive(&(secondTeam->team[0]))) {
			if (isAttacking(secondMove)) affectDamage(&(secondTeam->team[0]), &(firstTeam->team[0]), secondMove);
			else if (isSwitching(secondMove)) swapActualAttacker(secondTeam, secondMove);
			else return FALSE;
		}
		return TRUE;

}

void testBattle(t_Team * rouge, t_Team * bleu){
	printPoke(&(rouge->team[0]));
	printPoke(&(bleu->team[0]));
	printf("pv rouge : %d\n\n",rouge->team[0].current_pv);
	printf("pv bleu : %d\n\n",bleu->team[0].current_pv);
	while(isAlive(&(rouge->team[0])) && isAlive(&(bleu->team[0]))){
		playATurn(rouge,rand()%4,bleu,rand()%4);
		printf("pv rouge : %d\n\n",rouge->team[0].current_pv);
		printf("pv bleu : %d\n\n",bleu->team[0].current_pv);
	}
	//printPoke(&(rouge->team[0]));
}

void testPP(t_Team * rouge, t_Team * bleu){
	rouge->team[0].moveList[0].current_pp=0;
	int test=playATurn(rouge,0,bleu,0);
	if(test==FALSE){
		printf("TEST REUSSI!\n");
	}
	else{
		printf("ECHEC TEST!\n");
	}
}

void testSwitch(t_Team * rouge, t_Team * bleu){
	printTeam(rouge);
	int move1=rand()%5+11;
	int move2=rand()%4;
	playATurn(rouge,move1,bleu,move2);
	printTeam(rouge);
}

void testStruggle(t_Team * rouge, t_Team * bleu){
	for(int i=0;i<4;i++){
		rouge->team[0].moveList[i].current_pp=0;
	}
	printPoke(&(rouge->team[0]));
	printPoke(&(bleu->team[0]));
	printf("test result = %d\n",playATurn(rouge,0,bleu,0));
}

int main(){
	initData();
	t_Team rouge;
	t_Team bleu;
	initTeam(&rouge);
	initTeam(&bleu);
	testStruggle(&rouge,&bleu); //use testSwitch or testBattle to try the program's features
	return 0;
}