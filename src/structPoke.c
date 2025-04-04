#include "../include/structPoke.h"

t_Nature tabNature[25];
float typeChart[typeNumber][typeNumber] = {
    /*defender*/
    /*offender*//*noType  feu     plante   eau     electrique  malware  data    net		waifu*/
    /*notype*/  {1.,     1.,     	1.,     1.,     1.,         1.,      1.,     1.,		1.	},
    /*feu*/     {1.,     0.5,    	2.,     0.5,    1.,         2,       0.5,    2.,		2.	},
    /*plante*/  {1.,     0.5,    	0.5,    2.,     1.,         0.5,     2.,     1.,		1.	},
    /*eau*/     {1.,     2.,     	0.5,    0.5,    1.,         1.,      2.,     1.,		1.	},
    /*electrique*/ {1.,  1.,     	1.,     1.,     0.5,        2.,      2.,     2.,		0.5	},
    /*malware*/ {1.,     0.5,    	2.,     1.,     0.5,        2.,      2.,     2.,		1.	},
    /*data*/    {1.,     2.,     	0.5,    0.5,    0.5,        0.5,     0.5,     2.,		1.	},
    /*net*/     {1.,	 1,    		1,    	1,    	2,        	0.5,     0.5,    0.,		0.5	},
	/*waifu*/	{1.,	0.5,		1.,		1.,		1.,			1.,		1.,		2.,			2.	}
};

/*Fonction test*/

/**
 * @fn void generatePoke(t_Poke *)
 * @brief Génère un Pokémon avec des caractéristiques aléatoires.
 * 
 * Cette fonction remplit la structure t_Poke avec des valeurs aléatoires pour les attributs du Pokémon.
 * 
 * @param poke Pointeur vers la structure t_Poke à remplir.
 */
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
	
/**
* @fn void generate_poke(t_Poke *p,int id)
* @brief Génère un Pokémon avec des caractéristiques aléatoires.
*
* @param p Pointeur vers la structure t_Poke à remplir.
* @param line ID du Pokémon à générer.
*/
void generate_poke(t_Poke *p, int line)
{
	FILE *dataPoke;
	dataPoke = fopen("data/dataPoke.csv", "r");
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
		fscanf(dataPoke, "%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d\n", &(p->id), p->name, &(p->baseStats[PV]), &(p->baseStats[ATT]), &(p->baseStats[DEF]), &(p->baseStats[SPA]), &(p->baseStats[SPD]), &(p->baseStats[SPE]),(int*)&(p->type[0]),(int*)&(p->type[1]));
		
		p->exp = expCurve(p->lvl); /*This is to match the start of a random generated level*/
		p->nature = rand() % 25;
		/*IV et Moves*/
		p->nb_move = rand() % 3 + 2; //have minimum two moves
		for (int i = 0; i < p->nb_move; i++)
		{
			p->moveList[i] = generateRandomMoveBetter(p);
			p->moveList[i].current_pp = p->moveList[i].max_pp;
		}
		for (int i = 0; i < 6; i++)
			p->iv[i] = rand() % 32;
		fclose(dataPoke);
	}
}
/**
*@fn generatelvl(t_Team * joueur)   
*@brief Renvois un entier en fonction de la moyenne des niveaux de l'équipe du joueur
*   @param joueur struct joueur
*/
int generatelvl(t_Team * joueur){
	int sum=0;
	for(int i=0;i<joueur->nb_poke;i++){
		sum+=joueur->team[i].lvl;
	}
	int avg=sum/joueur->nb_poke;
	return avg-1+rand()%2;
}

/**
* @fn void generate_poke_enemi(t_Poke *p, int line,t_Team * joueur)
* @brief Génère un Pokémon ennemi avec des caractéristiques aléatoires.
*
*
* @param p Pointeur vers la structure t_Poke à remplir.
* @param line Ligne de la matrice de collision.
* @param joueur Pointeur vers la structure t_Team représentant le joueur.
*/

void generate_poke_enemi(t_Poke *p, int line,t_Team * joueur)
{
	FILE *dataPoke;
	dataPoke = fopen("data/dataPoke.csv", "r");
	if (dataPoke == NULL){
		printf("Erreur : impossible d'ouvrir le poke.\n");
		exit(1);
	}
	else
	{
		char buffer[256];
		//place the cursor at the right line
		for (int i = 1; i < line; i++){
			if (fgets(buffer, sizeof(buffer), dataPoke) == NULL){
				printf("Erreur : ligne %d introuvable dans le fichier.\n", line);
				fclose(dataPoke);
				exit(1);
			}
		}
		
		fscanf(dataPoke, "%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d\n", &(p->id), p->name, &(p->baseStats[PV]), &(p->baseStats[ATT]), &(p->baseStats[DEF]), &(p->baseStats[SPA]), &(p->baseStats[SPD]), &(p->baseStats[SPE]),(int*)&(p->type[0]),(int*)&(p->type[1]));
		
		
		p->lvl = generatelvl(joueur);
		p->nature = rand() % 25;
		//IV et Moves
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

/**
 * @fn t_Move generateRandomMove()
 * @brief Génère un mouvement aléatoire pour un Pokémon.
 * 
 * Cette fonction choisit un mouvement aléatoire parmi les mouvements disponibles du Pokémon.
 * @return t_Move 
 */
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
/**
 * @fn t_Move generateRandomMoveBetter(t_Poke *p)
 * @brief Génère un mouvement aléatoire pour un Pokémon, en tenant compte de ses caractéristiques.
 * 
 * Cette fonction choisit un mouvement aléatoire parmi les mouvements disponibles du Pokémon, en tenant compte de ses caractéristiques.
 * @param p Pointeur vers la structure t_Poke représentant le Pokémon.
 * @return t_Move 
 */
/*For now, the generation is random*/
t_Move generateRandomMoveBetter(t_Poke *p){
	FILE *dataMove;
	dataMove = fopen("data/dataMoves.csv", "r");
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
			int line = rand() % 40 + 1; //Change when we add new moves
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

/**
 * @fn t_move generateMove(int id)
 */

/*---------------------------------------------------------------------------*/
 
/**
 * @brief génere une attaque non aléatoirement, selon une id donnée, utiliser
 * @param line : l'id
 */
t_Move generateMove(int line){
	FILE *dataMove;
	dataMove = fopen("data/dataMoves.csv", "r");
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
		move.current_pp=move.max_pp;
		fclose(dataMove);
		return move;
	}
}

/**
 * @fn void learnMove(t_Poke *, t_Move *, int)
 * @brief Apprend un mouvement à un Pokémon.
 * 
 * Cette fonction remplace un mouvement existant du Pokémon par un nouveau mouvement.
 * 
 * @param poke Pointeur vers la structure t_Poke représentant le Pokémon.
 * @param move Pointeur vers la structure t_Move représentant le mouvement à apprendre.
 * @param index Indice du mouvement à remplacer (0-3).
 */
void learnMove(t_Poke *p, t_Move *m, int ind)
{
	strcpy(p->moveList[ind].name, m->name);
	p->moveList[ind].power = m->power;
}

/**
 * @fn void printPoke(t_Poke *)
 * @brief Affiche les informations d'un Pokémon.
 * 
 * Cette fonction affiche les informations du Pokémon, y compris ses caractéristiques et ses mouvements.
 * 
 * @param poke Pointeur vers la structure t_Poke à afficher.
 */
void printPoke(t_Poke * p)
{
	printf("name=%s\n", p->name);
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
		case electrique:printf("type 1 = electrique\n");break;
		case malware:printf("type 1 = malware\n");break;
		case data:printf("type 1 = data\n");break;
		case net:printf("type 1 = net\n");break;
		default:printf("Erreur\n");
	}
	switch (p->type[1]){
		case feu:printf("type 2 = feu\n");break;
		case eau:printf("type 2 = eau\n");break;
		case plante:printf("type 2 = plante\n");break;
		case electrique:printf("type 2 = electrique\n");break;
		case malware:printf("type 2 = malware\n");break;
		case data:printf("type 2 = data\n");break;
		case net:printf("type 2 = net\n");break;
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

/**
 * @fn void initNature()
 * @brief Initialise les natures des Pokémon.
 * 
 * Cette fonction remplit un tableau de natures avec des valeurs aléatoires.
 */
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

/**
 * @fn void initData()
 * @brief Initialise les données du jeu.
 * 
 * Cette fonction initialise les données du jeu, y compris les Pokémon, les mouvements et les natures.
 */
void initData()
{
	initNature();
	srand(time(NULL));
}

/**
 * @fn void printChart()
 * @brief Affiche le tableau des types de Pokémon.
 * 
 * Cette fonction affiche un tableau indiquant les forces et faiblesses des différents types de Pokémon.
 */
void printChart(){
	for(int i=0;i<typeNumber;i++){
		for(int j=0;j<typeNumber;j++){
			printf("%f\t",typeChart[i][j]);
		}
		printf("\n");
	}
}
