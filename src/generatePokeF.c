#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "structPoke.c"


void generate_poke(t_Poke *p,char * choixPoke)
{
	/*LA CMD DU CHOIX ICI*/
	printf("Quel starter tu veux? (int)");
	scanf("%s%*c" , choixPoke);

	FILE *dataPoke;
	char nomFichier[1024];
	strcpy(nomFichier , "dataPoke/pokeID=");
	strcat(nomFichier,choixPoke);
	strcat(nomFichier,".txt");
	
	printf("%s\n",nomFichier);
	
	dataPoke = fopen(nomFichier, "r");
	if (dataPoke == NULL)
	{
		printf("Erreur : impossible d'ouvrir le fichier.\n");
		exit(1);
	}
	else
	{
		

		/*LOAD des stats dans une VAR*/
		fscanf(dataPoke, "%d\n", p->id);
		fscanf(dataPoke, "%s\n", p->name);
		fscanf(dataPoke, "%d\n", p->baseStats[PV]);
		fscanf(dataPoke, "%d\n", p->baseStats[ATT]);
		fscanf(dataPoke, "%d\n", p->baseStats[DEF]);
		fscanf(dataPoke, "%d\n", p->baseStats[SPA]);
		fscanf(dataPoke, "%d\n", p->baseStats[SPD]);
		fscanf(dataPoke, "%d\n", p->baseStats[SPE]);

		p->gender = rand() % 2;
		p->lvl = rand() % 100 + 1;
		p->nature = rand() % 25;
		p->type[0] = rand() % (typeNumber-1) + 1;
		p->type[1] = rand() % typeNumber;
		for (int i = 0; i < 6; i++)
			p->baseStats[i] = rand() % 256;
		for (int i = 0; i < 6; i++)
			p->iv[i] = rand() % 32;
		for (int i = 0; i < 4; i++)
		{
			p->moveList[i].power = -1;
		}
		/*LOAD dans la structure*/
	fclose(dataPoke);
	}
}
/*
void mini_printpoke(t_Poke *p)
{
	printf("id=%d\n", p->id);
	printf("name=%s\n", p->name);
	printf("pv=%d\n", p->baseStats[PV]);
	printf("att=%d\n", p->baseStats[ATT]);
	printf("def=%d\n", p->baseStats[DEF]);
	printf("attSP=%d\n", p->baseStats[SPA]);
	printf("defSP=%d\n", p->baseStats[SPD]);
	printf("spd=%d\n", p->baseStats[SPE]);
	printf("test");
}*/

void main(){
    t_Poke *p;
	char oui[10];
    generate_poke(p,oui);
    printPoke(p);
}