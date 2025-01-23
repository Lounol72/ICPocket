#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define PV 0
#define ATT 1
#define DEF 2
#define SPA 3
#define SPD 4
#define SPE 5

#define TRUE 1
#define FALSE 0

typedef enum{noType=0,feu,plante,eau} t_Type;
const int typeNumber=4;

typedef enum{status=-1,physical=1,special=3} t_Categ;

typedef enum
{
	male = 0,
	female
} t_Gender;

typedef struct
{
	char nature[15];
	float coeff[6];
} t_Nature;

t_Nature tabNature[25];

typedef struct
{
	char name[15];
	int power;
	t_Type type;
	t_Categ categ;
} t_Move;

typedef struct
{
	int id;
	char name[20];
	t_Gender gender;
	t_Type type[2];
	int lvl;
	int nature;
	int current_pv;
	int baseStats[6];
	int statChanges[6];
	int iv[6];
	int stats[6];
	t_Move moveList[4];
} t_Poke;

float typeChart[4][4]={
				/*defender*/
	/*offender*//*noType	feu		plante	eau*/
	/*notype*/		1.,		1.,		1.,		1.,
	/*feu*/			1.,		0.5,	2.,		0.5,
	/*plante*/		1.,		0.5,	0.5,	2.,
	/*eau*/			1.,		2.,		0.5,	0.5
};


void generatePoke(t_Poke *p)
{
	/*
	FILE *dataPoke;
	dataPoke = fopen("PokeBDD.txt", "r");
	if (dataPoke == NULL)
	{
		printf("Erreur : impossible d'ouvrir le fichier.\n");
		exit(1);
	}
	else
	{
		int scanID, jeTeChoisis = rand() % 10;
		char namePoke[20];

		fscanf(dataPoke, "%d", &scanID);
		while (!feof(dataPoke))
		{
			fscanf(dataPoke, "%d", &scanID);
			if (scanID == jeTeChoisis)
			{
				fscanf(dataPoke, "%d", &scanID);
				fscanf(dataPoke, "%s", namePoke);
				break;
			}
		}
		*/
		strcpy(p->name,"dummy");
		p->gender = rand() % 2;
		p->lvl = rand() % 100 + 1;
		p->nature = rand() % 25;
		p->type[0] = rand() % (typeNumber-1) + 1;
		p->type[1] = rand() % typeNumber;
		for (int i = 0; i < 6; i++)
			p->baseStats[i] = rand() % 256;
		for (int i = 0; i < 6; i++)
			p->iv[i] = rand() % 32;
		p->stats[PV] = ((int)(2 * p->baseStats[0] + p->iv[0]) * p->lvl / 100) + p->lvl + 10;
		for (int i = 1; i < 6; i++)
		{
			p->stats[i] = (int)(((2 * p->baseStats[i] + p->iv[i]) * p->lvl / 100) + 5) * tabNature[p->nature].coeff[i];
		}
		for (int i = 0; i < 4; i++)
		{
			p->moveList[i].power = -1;
		}
	}
	//fclose(dataPoke);

t_Move generateRandomMove()
{
	t_Move move;
	for (int i = 0; i < 4; i++)
	{
		strcpy(move.name, "Move Test");
		move.power = rand() % 120 + 30;
		move.categ = rand() % 2?physical:special;
		move.type = rand() % (typeNumber-1) + 1;
	}
	return move;
}

void learnMove(t_Poke *p, t_Move *m, int ind)
{
	strcpy(p->moveList[ind].name, m->name);
	p->moveList[ind].power = m->power;
}

void printPoke(t_Poke *p)
{
	printf("name=%s\n", p->name);
	printf("%d\n", p->gender);
	switch (p->gender)
	{
	case male:
		printf("gender=male\n");
		break;
	case female:
		printf("gender=female\n");
		break;
	}
	switch (p->type[0]){
		case feu:printf("type 1 = feu\n");break;
		case eau:printf("type 1 = eau\n");break;
		case plante:printf("type 1 = plante\n");break;
	}
	switch (p->type[1]){
		case feu:printf("type 2 = feu\n");break;
		case eau:printf("type 2 = eau\n");break;
		case plante:printf("type 2 = plante\n");break;
		default:printf("pure\n");
	}
	printf("nature=%s\n", tabNature[p->nature].nature);
	printf("niveau=%d\n", p->lvl);
	printf("\nSTATS:\n");
	for (int i = 0; i < 6; i++)
	{
		printf("%d\n", p->stats[i]);
	}
	printf("\n BASE STATS:\n");
	for (int i = 0; i < 6; i++)
	{
		printf("%d\n", p->baseStats[i]);
	}
	printf("\nIVs:\n");
	for (int i = 0; i < 6; i++)
	{
		printf("%d\n", p->iv[i]);
	}

	printf("\nMoves:\n");
	for (int i = 0; i < 4; i++)
	{
		if (p->moveList[i].power > 0)
		{
			printf("n°%d\nname=%s\n", i, p->moveList[i].name);
			printf("power=%d\n", p->moveList[i].power);
			switch (p->moveList[i].type){
				case feu:printf("type = feu\n");break;
				case eau:printf("type = eau\n");break;
				case plante:printf("type = plante\n");break;
			}
			switch (p->moveList[i].categ)
			{
			case physical:
				printf("categ = physique\n");
				break;
			case special:
				printf("categ = special\n");
				break;
			
			default:
				break;
			}
			
		}
	}
}

void initNature()
{
	for (int i = 0; i < 25; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			tabNature[i].coeff[j] = 1;
		}
	}
	strcpy(tabNature[0].nature, "Hardi");
	strcpy(tabNature[1].nature, "Solo");
	tabNature[1].coeff[ATT] = 1.1;
	tabNature[1].coeff[DEF] = 0.9;
	strcpy(tabNature[2].nature, "Rigide");
	tabNature[2].coeff[ATT] = 1.1;
	tabNature[2].coeff[SPA] = 0.9;
	strcpy(tabNature[3].nature, "Mauvais");
	tabNature[3].coeff[ATT] = 1.1;
	tabNature[3].coeff[SPD] = 0.9;
	strcpy(tabNature[4].nature, "Brave");
	tabNature[4].coeff[ATT] = 1.1;
	tabNature[4].coeff[SPE] = 0.9;

	strcpy(tabNature[5].nature, "Assuré");
	tabNature[5].coeff[DEF] = 1.1;
	tabNature[5].coeff[ATT] = 0.9;
	strcpy(tabNature[6].nature, "Docile");
	strcpy(tabNature[7].nature, "Malin");
	tabNature[7].coeff[DEF] = 1.1;
	tabNature[7].coeff[SPA] = 0.9;
	strcpy(tabNature[8].nature, "Lâche");
	tabNature[8].coeff[DEF] = 1.1;
	tabNature[8].coeff[SPD] = 0.9;
	strcpy(tabNature[9].nature, "Relax");
	tabNature[9].coeff[DEF] = 1.1;
	tabNature[9].coeff[SPE] = 0.9;

	strcpy(tabNature[10].nature, "Modeste");
	tabNature[10].coeff[SPA] = 1.1;
	tabNature[10].coeff[ATT] = 0.9;
	strcpy(tabNature[11].nature, "Doux");
	tabNature[11].coeff[SPA] = 1.1;
	tabNature[11].coeff[DEF] = 0.9;
	strcpy(tabNature[12].nature, "Pudique");
	strcpy(tabNature[13].nature, "Foufou");
	tabNature[13].coeff[SPA] = 1.1;
	tabNature[13].coeff[SPD] = 0.9;
	strcpy(tabNature[14].nature, "Discret");
	tabNature[14].coeff[SPA] = 1.1;
	tabNature[14].coeff[SPE] = 0.9;

	strcpy(tabNature[15].nature, "Calme");
	tabNature[15].coeff[SPD] = 1.1;
	tabNature[15].coeff[ATT] = 0.9;
	strcpy(tabNature[16].nature, "Gentil");
	tabNature[16].coeff[SPD] = 1.1;
	tabNature[16].coeff[DEF] = 0.9;
	strcpy(tabNature[17].nature, "Prudent");
	tabNature[17].coeff[SPD] = 1.1;
	tabNature[17].coeff[SPA] = 0.9;
	strcpy(tabNature[18].nature, "Bizarre");
	strcpy(tabNature[19].nature, "Malpoli");
	tabNature[19].coeff[SPD] = 1.1;
	tabNature[19].coeff[SPE] = 0.9;

	strcpy(tabNature[20].nature, "Timide");
	tabNature[20].coeff[SPE] = 1.1;
	tabNature[20].coeff[ATT] = 0.9;
	strcpy(tabNature[21].nature, "Pressé");
	tabNature[21].coeff[SPE] = 1.1;
	tabNature[21].coeff[DEF] = 0.9;
	strcpy(tabNature[22].nature, "Jovial");
	tabNature[22].coeff[SPE] = 1.1;
	tabNature[22].coeff[SPA] = 0.9;
	strcpy(tabNature[23].nature, "Naïf");
	tabNature[23].coeff[SPE] = 1.1;
	tabNature[23].coeff[SPD] = 0.9;
	strcpy(tabNature[24].nature, "Sérieux");
}

void initData()
{
	initNature();
	srand(time(NULL));
}

void printChart(){
	for(int i=0;i<typeNumber;i++){
		for(int j=0;j<typeNumber;j++){
			printf("%f\t",typeChart[i][j]);
		}
		printf("\n");
	}
}
