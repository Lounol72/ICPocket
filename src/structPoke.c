#include "../include/structPoke.h"
#include "../include/duel.h"

t_Nature tabNature[25];

float typeChart[typeNumber][typeNumber]={
				/*defender*/
	/*offender*//*noType	feu		plante	eau*/
	/*notype*/		{1.,		1.,		1.,		1.},
	/*feu*/			{1.,		0.5,	2.,		0.5},
	/*plante*/		{1.,		0.5,	0.5,	2.},
	/*eau*/			{1.,		2.,		0.5,	0.5}
};


void generatePoke(t_Poke *p)
{
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
		for (int i = 0; i < 4; i++)
		{
			p->moveList[i].power = -1;
		}
	}
	//fclose(dataPoke);


void generate_poke(t_Poke *p, int line)
{
	FILE *dataPoke;
	dataPoke = fopen("src/data/dataPoke.csv", "r");
	if (dataPoke == NULL){
		printf("Erreur : impossible d'ouvrir le poke.\n");
		exit(1);
	}
	else
	{
		char buffer[256];
		for (int i = 1; i < line; i++){
			if (fgets(buffer, sizeof(buffer), dataPoke) == NULL){
				printf("Erreur : ligne %d introuvable dans le fichier.\n", line);
				fclose(dataPoke);
				exit(1);
			}
		}

		/*Load Base stats*/
		fscanf(dataPoke, "%d,%[^,],%d,%d,%d,%d,%d,%d\n", &(p->id), p->name, &(p->baseStats[PV]), &(p->baseStats[ATT]), &(p->baseStats[DEF]), &(p->baseStats[SPA]), &(p->baseStats[SPD]), &(p->baseStats[SPE]));

		/*Load Types*/
		p->type[0] = rand() % (typeNumber-1) + 1;
		p->type[1] = rand() % typeNumber;

		p->lvl = rand() % 100 + 1;
		p->exp = expCurve(p->lvl); /*This is to match the start of a random generated level*/
		/*IV et Moves*/
		for (int i = 0; i < 6; i++)
			p->iv[i] = rand() % 32;
		p->nb_move = rand() % 4 + 1;
		for (int i = 0; i < p->nb_move; i++)
		{
			p->moveList[i] = generateRandomMoveBetter(p);
			p->moveList[i].current_pp = p->moveList[i].max_pp;
		}
		fclose(dataPoke);
	}
}

t_Move generateRandomMove()
{
	t_Move move;
	for (int i = 0; i < 4; i++)
	{
		strcpy(move.name, "Move Test");
		move.power = rand() % 120 + 30;
		move.categ = rand() % 2?physical:special;
		move.type = rand() % (typeNumber-1) + 1;
		move.accuracy = rand() % 71 + 30;
		move.max_pp = rand()%26 + 5;
		move.priority_lvl = rand()%8;
	}
	return move;
}

/*For now, the generation is random*/
t_Move generateRandomMoveBetter(t_Poke *p){
	FILE *dataMove;
	dataMove = fopen("src/data/dataMoves.csv", "r");
	if (dataMove == NULL){
		printf("Erreur : impossible d'ouvrir le fichier des moves.\n");
		exit(1);
	}
	else
	{
		char buffer[256];
		t_Move move;
		int isUnique = 0;
		while (!isUnique) {
			isUnique = 1;
			int line = rand() % 7 + 1; //Change when we add new moves
			rewind(dataMove); //Reset the file pointer to the beginning
			for (int i = 1; i < line; i++){
				if (fgets(buffer, sizeof(buffer), dataMove) == NULL){
					printf("Erreur : ligne %d introuvable dans le fichier.\n", line);
					fclose(dataMove);
					exit(1);
				}
			}
			fscanf(dataMove, "%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", &(move.id),move.name, &(move.power), (int*)&(move.type), &(move.categ), &(move.accuracy), &(move.max_pp), &(move.priority_lvl), &(move.target), &(move.ind_secEffect), &(move.probability), &(move.value_effect), &(move.effect_modifier));
			
			// Check if the move is unique
			for (int i = 0; i < p->nb_move; i++) {
				if (strcmp(move.name, p->moveList[i].name) == 0) {
					isUnique = 0;
					break;
				}
			}
		}
		fclose(dataMove);
		return move;
	}
}

t_Move generateMove(int line){
	FILE *dataMove;
	dataMove = fopen("src/data/dataMoves.csv", "r");
	if (dataMove == NULL){
		printf("Erreur : impossible d'ouvrir le fichier des moves.\n");
		exit(1);
	}
	else
	{
		char buffer[256];
		t_Move move;
		rewind(dataMove); //Reset the file pointer to the beginning
		for (int i = 1; i < line; i++){
			if (fgets(buffer, sizeof(buffer), dataMove) == NULL){
				printf("Erreur : ligne %d introuvable dans le fichier.\n", line);
				fclose(dataMove);
				exit(1);
			}
		}
		fscanf(dataMove, "%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", &(move.id),move.name, &(move.power), (int*)&(move.type), &(move.categ), &(move.accuracy), &(move.max_pp), &(move.priority_lvl), &(move.target), &(move.ind_secEffect), &(move.probability), &(move.value_effect), &(move.effect_modifier));
		
		fclose(dataMove);
		return move;
	}
}

void learnMove(t_Poke *p, t_Move *m, int ind)
{
	strcpy(p->moveList[ind].name, m->name);
	p->moveList[ind].power = m->power;
}

void printPoke(t_Poke * p)
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
		default:printf("Erreur\n");
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
		printf("%d\n", calcStatFrom(p,i));
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
	for (int i = 0; i < p->nb_move; i++)
	{
		if (p->moveList[i].power > 0)
		{
			printf("n°%d\nname=%s\n", i, p->moveList[i].name);
			printf("power=%d\n", p->moveList[i].power);
			switch (p->moveList[i].type){
				case feu:printf("type = feu\n");break;
				case eau:printf("type = eau\n");break;
				case plante:printf("type = plante\n");break;
				default:break;
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
			printf("precision=%d\n",p->moveList[i].accuracy);
			printf("PP = %d/%d\n",p->moveList[i].current_pp,p->moveList[i].max_pp);
			printf("priorite = %d\n\n",p->moveList[i].priority_lvl);

		}
	}
}

void initNature()
{
	const char *natures[] = {"Hardi", "Solo", "Rigide", "Mauvais", "Brave", "Assuré", "Docile", "Malin", "Lâche", "Relax", "Modeste", "Doux", "Pudique", "Foufou", "Discret", "Calme", "Gentil", "Prudent", "Bizarre", "Malpoli", "Timide", "Pressé", "Jovial", "Naïf", "Sérieux"};
	const float coeffs[25][6] = {
		{1, 1, 1, 1, 1, 1}, {1.1, 0.9, 1, 1, 1, 1}, {1.1, 1, 0.9, 1, 1, 1}, {1.1, 1, 1, 0.9, 1, 1}, {1.1, 1, 1, 1, 0.9, 1},
		{0.9, 1.1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1}, {1, 1.1, 0.9, 1, 1, 1}, {1, 1.1, 1, 0.9, 1, 1}, {1, 1.1, 1, 1, 0.9, 1},
		{0.9, 1, 1.1, 1, 1, 1}, {1, 0.9, 1.1, 1, 1, 1}, {1, 1, 1, 1, 1, 1}, {1, 1, 1.1, 0.9, 1, 1}, {1, 1, 1.1, 1, 0.9, 1},
		{0.9, 1, 1, 1.1, 1, 1}, {1, 0.9, 1, 1.1, 1, 1}, {1, 1, 0.9, 1.1, 1, 1}, {1, 1, 1, 1, 1, 1}, {1, 1, 1, 1.1, 0.9, 1},
		{0.9, 1, 1, 1, 1.1, 1}, {1, 0.9, 1, 1, 1.1, 1}, {1, 1, 0.9, 1, 1.1, 1}, {1, 1, 1, 0.9, 1.1, 1}, {1, 1, 1, 1, 1, 1}
	};
	for (int i = 0; i < 25; i++) {
		strcpy(tabNature[i].nature, natures[i]);
		for (int j = 0; j < 6; j++) {
			tabNature[i].coeff[j] = coeffs[i][j];
		}
	}
}

/*
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
*/

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
