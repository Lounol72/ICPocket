#include "../include/structPoke.h"
#include "../include/duel.h"
#include "../include/trainerAI.h"
#include "../include/interDuel.h"

t_Team rouge;
t_Team bleu;

float statVariations[13]={0.25,2./7,1./3,2./5,0.5,2./3,1,1.5,2,2.5,3,3.5,4};
t_Move struggle={"Lutte",50,noType,physical,200,1,1,0};

void printTeam(t_Team * t){
	for(int i=0;i<t->nb_poke;i++){
		if (!(t->team[i].current_pv==POKE_IS_ABSCENT)){
			printPoke(&(t->team[i]));
			getchar();
			system("clear");
		}
	}
}

void teamTest(t_Team * t, int nb_poke){
	initTeam(t,nb_poke);
	for(int i=0;i<nb_poke;i++){
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

int testActionValidity(int action, t_Team * t){
	if (isStruggling(action)){
		return TRUE;
	}
	if (isAttacking(action)){
		return action < t->team[0].nb_move && ppCheck(&(t->team[0].moveList[action]));
	}
	if (isSwitching(action)){
		return t->team[action-10].current_pv > 0 && action-10 < t->nb_poke;
	}
	return FALSE;
}

int calcStatFrom(t_Poke * p, int stat) {
	if (stat==PV) return ((int)(2*p->baseStats[PV]+p->iv[PV])*p->lvl/100)+p->lvl+10;
	int value=(int)(((2*p->baseStats[stat]+p->iv[stat])*p->lvl/100)+5)*tabNature[p->nature].coeff[stat];
	return value;
}

void initTeam(t_Team * t, int nb_poke){
	t->nb_poke=nb_poke;
	for(int i=0;i<nb_poke;i++){
		t->team[i].nb_move=rand()%3+1;
		generate_poke(&(t->team[i]),"1"); //ajout du 1er poke de la BDD
		for(int j=0;j<6;j++) t->statChanges[j]=NEUTRAL_STAT_CHANGE;
		t->team[i].current_pv=calcStatFrom(&(t->team[i]),PV);//POKE_IS_ABSENT;
		for(int j=0;j<t->team[i].nb_move;j++){
			t->team[i].moveList[j]=generateRandomMove();
			t->team[i].moveList[j].current_pp=t->team[i].moveList[j].max_pp;
		}
	}
}

int calcDamage(t_Team * offender, t_Team * defender, t_Move * move){
	int coupCritique=((rand()%24)==0);
	int damage;

	int targetedStatOff=move->categ; //différenciation attaque/attaque spéciale
	int targetedStatDef=targetedStatOff==ATT?DEF:SPD;
	if (coupCritique){
			damage = (((int)(offender->team[0].lvl*0.4+2)) *
			(int)(calcStatFrom(&(offender->team[0]),targetedStatOff) * offender->statChanges[targetedStatOff]>=NEUTRAL_STAT_CHANGE?statVariations[offender->statChanges[targetedStatOff]]:statVariations[NEUTRAL_STAT_CHANGE]) 
			* move->power /
			(int)(calcStatFrom(&(defender->team[0]),targetedStatDef) * defender->statChanges[targetedStatDef]<=NEUTRAL_STAT_CHANGE?statVariations[defender->statChanges[targetedStatDef]]:statVariations[NEUTRAL_STAT_CHANGE])
			/ 50 + 2 );
	}
	else{
		damage= (((int)((int)((int)(offender->team[0].lvl*0.4+2)) *
		(int)(calcStatFrom(&(offender->team[0]),targetedStatOff) * statVariations[offender->statChanges[targetedStatOff]])
		* move->power /
		(int)(calcStatFrom(&(defender->team[0]),targetedStatDef)) * statVariations[defender->statChanges[targetedStatDef]])
		/ 50) + 2);
	}
	damage*=(rand()%16+85)/100.0;  //fourchette aléatoire de dégats

	damage*=(offender->team[0].type[0]==move->type) || (offender->team[0].type[1]==move->type && move->type!=noType)?1.5:1; // bonus STAB

	damage*=typeChart[move->type][defender->team[0].type[0]]*typeChart[move->type][defender->team[0].type[1]]; // efficacité de types

	damage*=coupCritique?1.5:1;

	if (coupCritique) printf("Coup Critique!\n");
	return damage>0?damage:1;
}

void setDefaultStatChanges(t_Team * p){
	for(int i=0;i<6;i++) p->statChanges[i]=NEUTRAL_STAT_CHANGE;
}

int isAlive(t_Poke * p){
	return p->current_pv!=0;
}

int isExisting(t_Poke * p){
	return p->current_pv!=POKE_IS_ABSCENT;
}

int isTeamAlive(t_Team * t){
	for(int i=0;i<t->nb_poke;i++){
		if(isExisting(&(t->team[i])) && isAlive(&(t->team[i]))) return TRUE;
	}
	return FALSE;
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

int PriorityForFirstPoke(t_Team * p1, t_Team * p2, t_Move * move1, t_Move * move2, int index1, int index2){
	int speedOfP1=calcStatFrom(&(p1->team[0]),SPE) * statVariations[p1->statChanges[SPE]];
	int speedOfP2=calcStatFrom(&(p2->team[0]),SPE) * statVariations[p1->statChanges[SPE]];;

	/*Cas attack only*/
	if(isAttacking(index1) && isAttacking(index2)){
		if(move1->priority_lvl>move2->priority_lvl) return 1;
		if(move2->priority_lvl>move1->priority_lvl) return 0;
		return resolveSpeedDuel(speedOfP1,speedOfP2); //same priority lvl
	}

	/*Cas switch only*/
	if(isSwitching(index1) && isSwitching(index2)){
		return resolveSpeedDuel(speedOfP1,speedOfP2);
	}
	/*Cas conflit switch vs attack*/
	if (isAttacking(index1)) return 0;
	return 1;
}

int accuracyCheck(int accuracy){
	return rand()%100<accuracy;
}

int ppCheck(t_Move * move){
	return move->current_pp>0;
}

void affectDamage(t_Team * offender, t_Team * defender, int indexMove){
	int targetedStatOff=offender->team[0].moveList[indexMove].categ; //différenciation attaque/attaque spéciale
	int targetedStatDef=targetedStatOff==ATT?DEF:SPD;

	if(!isStruggling(indexMove) && !accuracyCheck(offender->team[0].moveList[indexMove].accuracy)){
		printf("%s rate son attaque (%d precision)\n",offender->team[0].name,offender->team[0].moveList[indexMove].accuracy);
		return;
	}
	printf("Attaque subis d'une puissance de %d\n avec une attaque de %d\ncontre une defence de %d\n",offender->team[0].moveList[indexMove].power,(int)(calcStatFrom(&(offender->team[0]),targetedStatOff) * statVariations[offender->statChanges[targetedStatOff]])
	,(int)(calcStatFrom(&(defender->team[0]),targetedStatDef) * statVariations[defender->statChanges[targetedStatDef]]));
	int damage=calcDamage(offender,defender,isStruggling(indexMove)?&struggle:&(offender->team[0].moveList[indexMove]));
	printf("Dégats = %d\n",damage);
	defender->team[0].current_pv=defender->team[0].current_pv>damage?defender->team[0].current_pv - damage:0;
	if (!isStruggling(indexMove)) (offender->team[0].moveList[indexMove].current_pp)--;
}

void swapActualAttacker(t_Team * t, int swapIndex){
	t_Poke sauv=t->team[0];
	t->team[0]=t->team[swapIndex-10];
	t->team[swapIndex-10]=sauv;
	setDefaultStatChanges(t);
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

		int first = PriorityForFirstPoke(t1, t2, move1==STRUGGLE?&struggle:&(t1->team[0].moveList[move1]), move2==STRUGGLE?&struggle:&(t2->team[0].moveList[move2]), move1, move2);
		t_Team *firstTeam = first ? t1 : t2;
		t_Team *secondTeam = first ? t2 : t1;
		int firstMove = first ? move1 : move2;
		int secondMove = first ? move2 : move1;

		if (isAttacking(firstMove)) affectDamage(firstTeam, secondTeam, firstMove);
		else if (isSwitching(firstMove)) swapActualAttacker(firstTeam, firstMove);
		else return FALSE;

		if (isAlive(&(secondTeam->team[0]))) {
			if (isAttacking(secondMove)) affectDamage(secondTeam, firstTeam, secondMove);
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
	while(isTeamAlive(rouge) && isTeamAlive(bleu)){
		playATurn(rouge,rand()%rouge->team[0].nb_move,bleu,AI_move_choice(&iaTest,rouge));
		while (isTeamAlive(rouge) && !isAlive(&(rouge->team[0]))){
			int swap=rand()%5+11;
			if(testActionValidity(swap,rouge)) swapActualAttacker(rouge,swap);
		}
		while (isTeamAlive(bleu) && !isAlive(&(bleu->team[0]))){
			int swap=rand()%5+11;
			if(testActionValidity(swap,bleu)) swapActualAttacker(bleu,swap);
		}
		printf("pv rouge : %d\n\n",rouge->team[0].current_pv);
		printf("pv bleu : %d\n\n",bleu->team[0].current_pv);
	}
	if(isTeamAlive(rouge)) printf("VICTOIRE DES ROUGES!!!\n");
	else printf("VICTOIRE DES BLEUS!!!\n");
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
	for(int i=0;i<rouge->team[0].nb_move;i++){
		rouge->team[0].moveList[i].current_pp=0;
	}
	printPoke(&(rouge->team[0]));
	printPoke(&(bleu->team[0]));
	printf("test result = %d\n",playATurn(rouge,0,bleu,0));
}