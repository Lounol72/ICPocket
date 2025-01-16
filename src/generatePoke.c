#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "structPoke.c"



void generatePoke(t_Poke *p)
{
    printf("La fonc se lance\n");

    char cwd[1024];
    getw(cwd, sizeof(cwd));
        
    char *pokeFile = strcat(cwd, "/PokeBDD.txt");
	FILE *dataPoke;
	dataPoke = fopen(pokeFile, "r");
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

		strcpy(p->name, namePoke);
		p->gender = rand() % 2;
		p->lvl = rand() % 100 + 1;
		p->nature = rand() % 25;
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
	fclose(dataPoke);
}


void main(){
    t_Poke p;
    generatePoke(&p);
    printPoke(&p);
}