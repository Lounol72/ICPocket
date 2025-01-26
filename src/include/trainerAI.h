typedef enum{none=0,statusFirst=2,damageOnly=3,boss=6} t_AI_type;

typedef struct AI{
	int AI_lvl;
	t_AI_type type;
	t_Team * AI_t_Team;
} t_AI;

extern t_AI iaTest;

int AI_move_choice(t_Team * ai_team, t_Team * player_team);