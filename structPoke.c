#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define PV 0
#define ATT 1
#define DEF 2
#define SPA 3
#define SPD 4
#define SPE 5

typedef enum{male=0,female} t_Gender;

typedef struct{
	char nature[15];
	float coeff[6];
} t_Nature;

t_Nature tabNature[25];

typedef struct{
	char name[15];
	int power;
} t_Move;


typedef struct{
	char name[20];
	t_Gender gender;
	int lvl;
	int nature;
	int baseStats[6];
	int iv[6];
	int stats[6];
	t_Move moveList[4];
} t_Poke;

void generatePoke(t_Poke *p){
	strcpy(p->name,"Dummy");
	p->gender=rand()%2;
	p->lvl=rand()%100+1;
	p->nature=rand()%25;
	for(int i=0;i<6;i++) p->baseStats[i]=rand()%256;
	for(int i=0;i<6;i++) p->iv[i]=rand()%32;
	p->stats[0]=((int)(p->baseStats[0]+p->iv[0])*2*p->lvl/100)+p->lvl+10;
	for(int i=1;i<6;i++){
		p->stats[i]=(int)(((2*p->baseStats[i]+p->iv[i])*p->lvl/100)+5)*tabNature[p->nature].coeff[i];
	}
	for(int i=0;i<4;i++){
		p->moveList[i].power=-1;
	}
}

t_Move generateRandomMove(){
	t_Move move;
	for(int i=0;i<4;i++){
		strcpy(move.name,"Move Test");
		move.power=rand()%120+30;
	}
	return move;
}

void learnMove(t_Poke * p,t_Move * m, int ind){
	strcpy(p->moveList[ind].name,m->name);
	p->moveList[ind].power=m->power;
}

void printPoke(t_Poke *p){
	printf("name=%s\n",p->name);
	printf("%d\n",p->gender);
	switch (p->gender){
		case male:printf("gender=male\n");break;
		case female:printf("gender=female\n");break;
	}
	printf("nature=%s\n",tabNature[p->nature].nature);
	printf("niveau=%d\n",p->lvl);
	printf("\nSTATS:\n");
	for(int i=0;i<6;i++){
		printf("%d\n",p->stats[i]);
	}
	printf("\n BASE STATS:\n");
	for(int i=0;i<6;i++){
		printf("%d\n",p->baseStats[i]);
	}
	printf("\nIVs:\n");
	for(int i=0;i<6;i++){
		printf("%d\n",p->iv[i]);
	}

	printf("\nMoves:\n");
	for(int i=0;i<4;i++){
		if(p->moveList[i].power>0){
			printf("n°%d\nname=%s\n",i,p->moveList[i].name);
			printf("power=%d\n",p->moveList[i].power);
		}
	}
}

void initNature(){
	for(int i=0;i<25;i++){
		for(int j=0;j<6;j++){
			tabNature[i].coeff[j]=1;
		}
	}
	strcpy(tabNature[0].nature,"Hardi");
	strcpy(tabNature[1].nature,"Solo");
	tabNature[1].coeff[ATT]=1.1;
	tabNature[1].coeff[DEF]=0.9;
	strcpy(tabNature[2].nature,"Rigide");
	tabNature[2].coeff[ATT]=1.1;
	tabNature[2].coeff[SPA]=0.9;
	strcpy(tabNature[3].nature,"Mauvais");
	tabNature[3].coeff[ATT]=1.1;
	tabNature[3].coeff[SPD]=0.9;
	strcpy(tabNature[4].nature,"Brave");
	tabNature[4].coeff[ATT]=1.1;
	tabNature[4].coeff[SPE]=0.9;

	strcpy(tabNature[5].nature,"Assuré");
	tabNature[5].coeff[DEF]=1.1;
	tabNature[5].coeff[ATT]=0.9;
	strcpy(tabNature[6].nature,"Docile");
	strcpy(tabNature[7].nature,"Malin");
	tabNature[7].coeff[DEF]=1.1;
	tabNature[7].coeff[SPA]=0.9;
	strcpy(tabNature[8].nature,"Lâche");
	tabNature[8].coeff[DEF]=1.1;
	tabNature[8].coeff[SPD]=0.9;
	strcpy(tabNature[9].nature,"Relax");
	tabNature[9].coeff[DEF]=1.1;
	tabNature[9].coeff[SPE]=0.9;

	strcpy(tabNature[10].nature,"Modeste");
	tabNature[10].coeff[SPA]=1.1;
	tabNature[10].coeff[ATT]=0.9;
	strcpy(tabNature[11].nature,"Doux");
	tabNature[11].coeff[SPA]=1.1;
	tabNature[11].coeff[DEF]=0.9;
	strcpy(tabNature[12].nature,"Pudique");
	strcpy(tabNature[13].nature,"Foufou");
	tabNature[13].coeff[SPA]=1.1;
	tabNature[13].coeff[SPD]=0.9;
	strcpy(tabNature[14].nature,"Discret");
	tabNature[14].coeff[SPA]=1.1;
	tabNature[14].coeff[SPE]=0.9;

	strcpy(tabNature[15].nature,"Calme");
	tabNature[15].coeff[SPD]=1.1;
	tabNature[15].coeff[ATT]=0.9;
	strcpy(tabNature[16].nature,"Gentil");
	tabNature[16].coeff[SPD]=1.1;
	tabNature[16].coeff[DEF]=0.9;
	strcpy(tabNature[17].nature,"Prudent");
	tabNature[17].coeff[SPD]=1.1;
	tabNature[17].coeff[SPA]=0.9;
	strcpy(tabNature[18].nature,"Bizarre");
	strcpy(tabNature[19].nature,"Malpoli");
	tabNature[19].coeff[SPD]=1.1;
	tabNature[19].coeff[SPE]=0.9;

	strcpy(tabNature[20].nature,"Timide");
	tabNature[20].coeff[SPE]=1.1;
	tabNature[20].coeff[ATT]=0.9;
	strcpy(tabNature[21].nature,"Pressé");
	tabNature[21].coeff[SPE]=1.1;
	tabNature[21].coeff[DEF]=0.9;
	strcpy(tabNature[22].nature,"Jovial");
	tabNature[22].coeff[SPE]=1.1;
	tabNature[22].coeff[SPA]=0.9;
	strcpy(tabNature[23].nature,"Naïf");
	tabNature[23].coeff[SPE]=1.1;
	tabNature[23].coeff[SPD]=0.9;
	strcpy(tabNature[24].nature,"Sérieux");
}

int main(){
	initNature();
	srand(time(NULL));
	t_Poke p;
	generatePoke(&p);
	for(int i=0;i<4;i++){
		t_Move attaque=generateRandomMove();
		learnMove(&p,&attaque,i);
	}
	printPoke(&p);
	return 0;
}