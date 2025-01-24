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
	strcpy(nomFichier , "/dataPoke/pokeID=");
	strcat(nomFichier,choixPoke);
	
	dataPoke = fopen(nomFichier, "r");
	if (dataPoke == NULL)
	{
		printf("Erreur : impossible d'ouvrir le fichier.\n");
		exit(1);
	}
	else
	{
		int scanID;
		char namePoke[20];
		int pv,atq,def,atqSP,defSP,spd;

		printf("Quel poke choisir? (id=numero)");
		
		char id[7] = "id=";
		strcat(id , choixPoke);

		/*LOAD des stats dans une VAR*/
		fscanf(dataPoke, "id=%s",id);
		fscanf(dataPoke, "name=%s", namePoke);
		fscanf(dataPoke, "%d\n", &pv);
		fscanf(dataPoke, "%d\n", &atq);
		fscanf(dataPoke, "%d\n", &def);
		fscanf(dataPoke, "%d\n", &atqSP);
		fscanf(dataPoke, "%d\n", &defSP);
		fscanf(dataPoke, "%d\n", &spd);

		/*LOAD dans la structure*/
		p->baseStats[PV]=pv;
		p->baseStats[ATT]=atq;
		p->baseStats[DEF]=def;
		p->baseStats[SPA]=atqSP;
		p->baseStats[SPD]=defSP;
		p->baseStats[SPE]=spd;

	fclose(dataPoke);
	}
}



void main(){
    t_Poke p;
	char oui[10];
    generate_poke(&p,oui);
    printPoke(&p);
}