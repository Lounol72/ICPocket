#include "../include/duel.h"

t_Team rouge;
t_Team bleu;

int (*SecEffectTab[3])(t_Team *,int,int,int)={statVarChange,applyEffect,recoilDamage};

float statVariations[13]={0.25,2./7,1./3,2./5,0.5,2./3,1,1.5,2,2.5,3,3.5,4};
t_Move struggle={-1,"lutte",50,noType,physical,200,1,1,0,1,2,100,25,0};

t_Move confusedMove={-2,"Confus",40,noType,physical,200,1,1,0,1,-1,0,0,0};

float moveEffectivenessFlag=0;
short int secondaryEffectHappenedFlag=FALSE;
short int criticalHitFlag=FALSE;

Lvl_Up_Buffer lvl_up_buffer[12];
int lvl_up_buffer_size=0;

int statVarChange(t_Team * target, int probability, int modifier, int targetedStat){
	if(rand()%100<probability) {
		if(target->statChanges[targetedStat]+modifier>12) target->statChanges[targetedStat]=12;
		else if(target->statChanges[targetedStat]+modifier<0) target->statChanges[targetedStat]=0;
		else target->statChanges[targetedStat]+=modifier;
		return TRUE;
	}
	return FALSE;
}

int applyEffect(t_Team * target, int probability, int effect, int bool_main_effect){
	if(bool_main_effect){
		if(target->team[0].main_effect==noEffect && rand()%100<probability){
			target->team[0].main_effect=effect;
			return TRUE;
		}
	}
	else{
		if(target->effect==noEffect && rand()%100<probability){
			target->effect=effect;
			return TRUE;
		}
	}
	return FALSE;
}

int recoilDamage(t_Team * target, int probability, int percentage_of_val, int indexPoke){
	if(rand()%100<probability) {
		target->team[0].current_pv-=calcStatFrom(&(target->team[indexPoke]),PV)*percentage_of_val/100;
		if(target->team[0].current_pv<0) target->team[0].current_pv=0;
		return TRUE;
	}
	return FALSE;
}

void launchSecEffect(t_Team * offender, t_Team * defender, t_Move * action){
	if(action->ind_secEffect>=0){
		if(action->target){
			if(SecEffectTab[action->ind_secEffect](offender,action->probability,action->value_effect,action->effect_modifier)){
				secondaryEffectHappenedFlag=TRUE;
				return;
			}
		}
		else{
			if(SecEffectTab[action->ind_secEffect](defender,action->probability,action->value_effect,action->effect_modifier)){
				secondaryEffectHappenedFlag=TRUE;
				return;
			}
		}
	}
	secondaryEffectHappenedFlag=FALSE;
}

void printTeam(t_Team * t){
	printf("Equipe de %s\n",t->trainerName);
	for(int i=0;i<t->nb_poke;i++){
		if (!(t->team[i].current_pv==POKE_IS_ABSCENT)){
			printPoke(&(t->team[i]));
			//getchar();
			//system("clear");
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
		return action-10 < t->nb_poke && t->team[action-10].current_pv > 0;
	}
	return FALSE;
}

int calcStatFrom(t_Poke * p, int stat) {
	if (stat==PV) return ((int)(2*p->baseStats[PV]+p->iv[PV])*p->lvl/100)+p->lvl+10;
	int value=(int)(((2*p->baseStats[stat]+p->iv[stat])*p->lvl/100)+5)*tabNature[p->nature].coeff[stat];
	if(p->main_effect==burn && stat==ATT) return value/2; //effet passif brulure
	if(p->main_effect==paralyze && stat==SPE) return value/2; //effet passif paralisie
	return value;
}

/**
*   @brief Initialise l'équipe du joueur
*   @param t structure de l'equipe du joueur
*   @param nbpoke nombre de pokemon a mettre dans l'equipe
*/
void initTeam(t_Team * t, int nb_poke){
	t->nb_poke=nb_poke;
	t->effect=noEffect;
	//t->lastEnemiID = NULL;
	t->nb_enemiBeat = 0;
	for(int i=0;i<nb_poke;i++){
		t->team[i].nb_move=rand()%3+1;
		t->team[i].main_effect=noEffect;
		generate_poke(&(t->team[i]),((i * 12335634 )%16)); // temporaire
		//generate_poke(&(t->team[i]),rand()%16+1); 
		for(int j=0;j<6;j++) t->statChanges[j]=NEUTRAL_STAT_CHANGE;
		t->team[i].current_pv=calcStatFrom(&(t->team[i]),PV);
		t->team[i].initial_pv = t->team[i].current_pv;
	}
}

/**
*   @brief Initialise l'équipe adverse en fonction du nombre d'adversaire battu par le joueur
*   @param t structure de l'equipe adverse
*   @param joueur structure de l'equipe du joueur
*/
void initBlueTeam(t_Team *t,t_Team *joueur) {
    FILE *fichierTrainer = fopen("data/dataTrainer.csv", "r");
    if (fichierTrainer == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier trainer bleu.\n");
        exit(1);
    } else {
        char buffer[256];
		//nombre random entre 1 et 8
        int id = rand() % 8+1; //Changer ici avec ID PNJ
        for (int i = 1; i < id; i++) {
            if (fgets(buffer, sizeof(buffer), fichierTrainer) == NULL) {
                printf("Erreur : ligne %d introuvable dans le fichier.\n", id);
                fclose(fichierTrainer);
                exit(1);
            }
        }
        int  id_poke;
        fscanf(fichierTrainer, "%d,%d,%[^,],", &(t->id), &(t->nb_poke), t->trainerName);
		t->nb_enemiBeat = 0;
		t->effect = noEffect;
        for (int i = 0; i < t->nb_poke; i++) {
            fscanf(fichierTrainer, "%d,", &id_poke);
            generate_poke_enemi(&(t->team[i]), id_poke,joueur);
            for (int j = 0; j < 6; j++) t->statChanges[j] = NEUTRAL_STAT_CHANGE;
            t->team[i].current_pv = calcStatFrom(&(t->team[i]), PV);
            t->team[i].initial_pv = t->team[i].current_pv;
        }
        fclose(fichierTrainer);
    }
}

int calcDamage(t_Team * offender, t_Team * defender, t_Move * move){
	/*cas des inefficacités de type*/
	criticalHitFlag=FALSE;
	if(typeChart[move->type][defender->team[0].type[0]]<0.1 || typeChart[move->type][defender->team[0].type[1]]<0.1) {
		//printf("Ca n'a aucun effet...\n");
		moveEffectivenessFlag=0;
		return 0;
	}
	/*cas des moves de status*/
	if(move->power==0) return 0;

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

	moveEffectivenessFlag=typeChart[move->type][defender->team[0].type[0]]*typeChart[move->type][defender->team[0].type[1]];
	//if (coupCritique) printf("Coup Critique!\n");
	if (coupCritique) criticalHitFlag=TRUE;
	return damage>0?damage:1;
}

void setDefaultStatChanges(t_Team * p){
	for(int i=0;i<6;i++) p->statChanges[i]=NEUTRAL_STAT_CHANGE;
}

int isAlive(t_Poke * p){
	return isExisting(p) && p->current_pv!=0;
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
	for(int i=0;i<p->nb_move;i++){
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

int affectDamage(t_Team * offender, t_Team * defender, int indexMove){
	t_Move * moveToDo;
	if(indexMove<0){
		moveToDo=isStruggling(indexMove)?&struggle:&confusedMove;
	}
	else{
		moveToDo=&(offender->team[0].moveList[indexMove]);
	}
	

	//int targetedStatOff=moveToDo->categ; //différenciation attaque/attaque spéciale
	//int targetedStatDef=targetedStatOff==ATT?DEF:SPD;

	if(!accuracyCheck(moveToDo->accuracy)){
		//printf("%s rate son attaque (%d precision)\n",offender->team[0].name,moveToDo->accuracy);
		if (!(indexMove<0)) (moveToDo->current_pp)--;
		moveEffectivenessFlag=-2;
		criticalHitFlag=0;
		secondaryEffectHappenedFlag=0;
		return FALSE;
	}
	//printf("Attaque subis d'une puissance de %d\n avec une attaque de %d\ncontre une defence de %d\n",moveToDo->power,(int)(calcStatFrom(&(offender->team[0]),targetedStatOff) * statVariations[offender->statChanges[targetedStatOff]])
	//,(int)(calcStatFrom(&(defender->team[0]),targetedStatDef) * statVariations[defender->statChanges[targetedStatDef]]));
	int damage=calcDamage(offender,defender,moveToDo);
	//if(isStruggling(indexMove)) printf("LUTTE\n");
	//printf("Dégats = %d\n",damage);
	defender->team[0].current_pv=defender->team[0].current_pv>damage?(int)(defender->team[0].current_pv - damage):0;
	if (!(indexMove<0)) (moveToDo->current_pp)--;
	launchSecEffect(offender,defender,moveToDo);
	return TRUE;
}

void swapActualAttacker(t_Team * t, int swapIndex){
	t_Poke sauv=t->team[0];
	t->team[0]=t->team[swapIndex-10];
	t->team[swapIndex-10]=sauv;
	t->effect=noEffect;
	setDefaultStatChanges(t);
	//printf("Swapping Happened\n\n");
}

int playATurn(t_Team * t1, int move1, t_Team * t2, int move2){
	/*
		use speed of pokemon to determine order
		attack or switch
		switch = exchanging index 0 with another index*/

		if(!hasMoveLeft(&(t1->team[0])) && isAttacking(move1)) move1=STRUGGLE;
		if(!hasMoveLeft(&(t2->team[0])) && isAttacking(move2)) move2=STRUGGLE;

		if(!testActionValidity(move1,t1)) {printf("\t\t\t Action Invalide rouge%d \n", move1);return FALSE;}
		if(!testActionValidity(move2,t2)) {printf("\t\t\t Action Invalide bleu %d \n", move2);return FALSE;}

		int first = PriorityForFirstPoke(t1, t2, move1==STRUGGLE?&struggle:&(t1->team[0].moveList[move1]), move2==STRUGGLE?&struggle:&(t2->team[0].moveList[move2]), move1, move2);
		t_Team *firstTeam = first ? t1 : t2;
		t_Team *secondTeam = first ? t2 : t1;
		int firstMove = first ? move1 : move2;
		int secondMove = first ? move2 : move1;

		if (isAttacking(firstMove)) {
			/*snap out confusion check*/
			if(firstTeam->effect==confusion && rand()%100<20){
				//printf("Le poké se remet de sa confusion\n");
				firstTeam->effect=noEffect;
			}
			/*flinch check*/
			if(firstTeam->effect==flinch){
				//printf("Le poke a peur! Il n'attaque pas!\n");
			}
			/*paralyze check*/
			else if(firstTeam->team[0].main_effect==paralyze && rand()%100<25){
				//printf("Le poké est paralysé!\n");
			}
			/*confusion check*/
			else if(firstTeam->effect==confusion && rand()%100<33){
				//printf("Il se blesse dans sa confusion!\n");
				affectDamage(firstTeam, firstTeam, CONFUSED_MOVE);
			}
			else{
				affectDamage(firstTeam, secondTeam, firstMove);
			}
		}
		else if (isSwitching(firstMove)) swapActualAttacker(firstTeam, firstMove);
		else return FALSE;

		if (isAlive(&(secondTeam->team[0]))) {
			if (isAttacking(secondMove)){
				/*snap out confusion check*/
				if(secondTeam->effect==confusion && rand()%100<20){
					//printf("Le poké se remet de sa confusion\n");
					secondTeam->effect=noEffect;
				}
				/*flinch check*/
				if(secondTeam->effect==flinch){
					//printf("Le poke a peur! Il n'attaque pas!\n");
				}
				/*paralyze check*/
				else if(secondTeam->team[0].main_effect==paralyze && rand()%100<25){
					//printf("Le poké est paralysé!\n");
				}
				/*confusion check*/
				else if(secondTeam->effect==confusion && rand()%100<33){
					//printf("Il se blesse dans sa confusion!\n");
					affectDamage(secondTeam, secondTeam, CONFUSED_MOVE);
				}
				else{
					affectDamage(secondTeam, firstTeam, secondMove);
				}
			}
			else if (isSwitching(secondMove)) swapActualAttacker(secondTeam, secondMove);
			else return FALSE;
		}

		/*Poison and burn damage applier*/
		if (isAlive(&(firstTeam->team[0])) && firstTeam->team[0].main_effect==burn) recoilDamage(firstTeam,100,6,0);
		if (isAlive(&(firstTeam->team[0])) && firstTeam->team[0].main_effect==poison) recoilDamage(firstTeam,100,12,0);
		if (isAlive(&(secondTeam->team[0])) && secondTeam->team[0].main_effect==burn) recoilDamage(secondTeam,100,6,0);
		if (isAlive(&(secondTeam->team[0])) && secondTeam->team[0].main_effect==poison) recoilDamage(secondTeam,100,12,0);
		
		/*flinch reset at end of turn*/
		if(firstTeam->effect==flinch) firstTeam->effect=noEffect;
		if(secondTeam->effect==flinch) secondTeam->effect=noEffect;
		return TRUE;

}


void checkLearningMove(t_Poke * p){
	FILE * movepoolFile=fopen("data/movepools.csv","r");
	if (movepoolFile!=NULL){
		char format[10];
		char line[10];
		int idMove=-1;
		/*format is 'idPoke;lvl;%d' where %d is the id of the move we search*/
		sprintf(format,"%d;%d;%%d",p->id,p->lvl);
		while (fgets(line,sizeof(line),movepoolFile)){
			if(sscanf(line,format,&idMove)==1){
				if(p->nb_move<4){
					p->moveList[p->nb_move++]=generateMove(idMove);
				}
				else{
					lvl_up_buffer[lvl_up_buffer_size].moveId=idMove;
					lvl_up_buffer[lvl_up_buffer_size++].target=p;
				}
			}
		}
	fclose(movepoolFile);
	}
}

unsigned expCurve(int lvl){
	/*Used Fast exp curve formula : lvl**3 */
	return lvl*lvl*lvl;
}

int reachedNextLvl(t_Poke * p){
	if(p->exp>=expCurve(p->lvl+1)){
		p->lvl++;
		//printf("%s monte au niveau %d\n",p->name,p->lvl);
		/*TO ADD HERE : apprendre une nouvelle attaque si disponible*/
		checkLearningMove(p);
		return TRUE;
	}
	return FALSE;
}

void gainExp(t_Team * target, t_Poke * source){
	int exp_amount;
	int pv_before_lvl_up;
	for(int i=0;i<target->nb_poke;i++){
		if(target->team[i].lvl<100){
			//* formula is IsTrainerPoke? * EXPBoost? * FaintedLvl * BaseEXP / 7
			//* isTrainerPoke and EXPBoost are always True by technical reasons therefore their values are 1.5
			//* BaseEXP has no real formula so I defined it with BasePV * 2 
			// Calculate exp gain using constants for clarity
			static const float TRAINER_BONUS = 1.5;
			static const float EXP_BOOST = 1.5;
			static const int BASE_EXP_MULTIPLIER = 2;
			static const int EXP_DIVISOR = 7;
			
			exp_amount = (TRAINER_BONUS * EXP_BOOST * source->lvl * BASE_EXP_MULTIPLIER * (source->baseStats[PV])) / EXP_DIVISOR;
			target->team[i].exp += (unsigned)exp_amount;
			//printf("%s gagne %d exp\n", target->team[i].name, exp_amount);
			
			// Store initial HP before level up
			pv_before_lvl_up = target->team[i].initial_pv;
			
			// Process any level ups
			while(reachedNextLvl(&(target->team[i])));
			
			// Update HP if pokemon was alive
			if(pv_before_lvl_up) {
				int hp_increase = calcStatFrom(&(target->team[i]), PV) - pv_before_lvl_up;
				target->team[i].current_pv += hp_increase;
				target->team[i].initial_pv += hp_increase;
			}
		}
	}
}

void testBattle(t_Team * rouge, t_Team * bleu){
	printf("Poke rouge\n");
	printPoke(&(rouge->team[0]));
	printf("Poke bleu\n");
	printPoke(&(bleu->team[0]));
	printf("pv rouge : %d\n\n",rouge->team[0].current_pv);
	printf("pv bleu : %d\n\n",bleu->team[0].current_pv);
	while(isTeamAlive(rouge) && isTeamAlive(bleu)){
		playATurn(rouge,AI_move_choice(&iaTest2,bleu),bleu,AI_move_choice(&iaTest,rouge));
		if (!isAlive(&(bleu->team[0]))){
			gainExp(rouge,&(bleu->team[0]));
		}
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
	if(isTeamAlive(rouge)){
		printf("VICTOIRE DES CHIENS!!!\n");
		sauvegarder(rouge,bleu);
	}else printf("VICTOIRE DES CHATS!!!\n");
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
	for(int i=0;i<bleu->team[0].nb_move;i++){
		bleu->team[0].moveList[i].current_pp=0;
	}
	printPoke(&(rouge->team[0]));
	printPoke(&(bleu->team[0]));
	printf("pv rouge : %d\n\n",rouge->team[0].current_pv);
	printf("pv bleu : %d\n\n",bleu->team[0].current_pv);
	printf("test result = %d\n",playATurn(rouge,0,bleu,AI_move_choice(&iaTest,rouge)));
	printf("pv rouge : %d\n\n",rouge->team[0].current_pv);
	printf("pv bleu : %d\n\n",bleu->team[0].current_pv);
}