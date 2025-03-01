#include "duel.h"

#ifndef TRAINERAI_H
#define TRAINERAI_H
#define MAX_AI_LVL 20

typedef enum{none=1,statusFirst=2,damageOnly=3} t_AI_type;

typedef struct AI{
	int AI_lvl;
	t_AI_type type;
	t_Team * AI_t_Team;
} t_AI;

extern t_AI iaTest;
extern t_AI iaTest2;

void insertionSort(int tabDegats[], int tabMove[], int n);
int AI_move_choice(t_AI * ai_team, t_Team * player_team);

#endif