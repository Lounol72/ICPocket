#define NEUTRAL_STAT_CHANGE 6
#define POKE_IS_ABSCENT -1
#define STRUGGLE -10
#define CONFUSED_MOVE -20

#define ATTAQUE_1 0
#define ATTAQUE_2 1
#define ATTAQUE_3 2
#define ATTAQUE_4 3

/**
 * @file duel.h
 */

extern float statVariations[13];
extern t_Move struggle;
extern t_Move confusedMove;

typedef struct{
	int id;
	char trainerName[20];
	t_Poke team[6];
	int nb_poke;
	int statChanges[6];
	t_Effect effect;	
} t_Team;

typedef struct{
	int moveId
	t_Poke * target;
} Lvl_Up_Buffer;

extern int (*SecEffectTab[3])(t_Team *,int,int,int);

/*This buffer purpose is to allow leveling up learning moves to work without blocking the SDL window*/
extern Lvl_Up_Buffer lvl_up_buffer[6];
extern int lvl_up_buffer_size;

extern t_Team rouge;
extern t_Team bleu;

int isAttacking(int);
int isSwitching(int);
int ppCheck(t_Move *);
int isExisting(t_Poke *);
int isAlive(t_Poke * p);
int isTeamAlive(t_Team *);
int testActionValidity(int action, t_Team * t);
int accuracyCheck(int accuracy);
int hasMoveLeft(t_Poke *);
int isStruggling(int);

int statVarChange(t_Team * target, int probability, int modifier, int targetedStat);
int applyEffect(t_Team * target, int probability, int effect, int bool_main_effect);
int recoilDamage(t_Team * target, int probability, int percentage_of_val, int indexPoke);
void launchSecEffect(t_Team * offender, t_Team * defender, t_Move * action);

void printTeam(t_Team * t);
int calcStatFrom(t_Poke * p,int stat);
void initTeam(t_Team * t,int);
void initBlueTeam(t_Team *t);
int calcDamage(t_Team * offender,t_Team * defender, t_Move *);
void setDefaultStatChanges(t_Team * p);
int resolveSpeedDuel(int speed1, int speed2);
int PriorityForFirstPoke(t_Team * p1, t_Team * p2, t_Move * move1, t_Move * move2, int index1, int index2);
void affectDamage(t_Team * offender, t_Team * defender, int indexMove);
void swapActualAttacker(t_Team * t, int swapIndex);
/**
 * @fn int playATurn(t_Team * t1, int move1, t_Team * t2, int move2);
 * @brief La fonction playATurn permet de jouer un tour d'un duel Pokémon classique, pour deux équipes au préalable générées et initialisées.
 * La fonction modifie l'état des équipes en paramètre et gère tout les cas particuliers (actions invalides, gestion des afflictions, cas de lutte).
 * @param t1,t2 Deux pointeurs sur structures t_Team, pointant sur deux équipes supposé initialisées (voir initTeam).
 * @param move1 Un entier repésentant l'action de l'équipe n°1, comprise entre 0 et 3. Il est recommandé d'utiliser les macros ATTAQUE_X pour renseigner cette valeur.
 * @param move1 Un entier repésentant l'action de l'équipe n°2, comprise entre 0 et 3. Il est recommandé d'utiliser les macros ATTAQUE_X pour renseigner cette valeur.
 * @return un booléen informant si un tour à été joué ou n'était pas valide pour l'être.
 */
int playATurn(t_Team * t1, int move1, t_Team * t2, int move2);

extern unsigned expCurve(int lvl);
int reachedNextLvl(t_Poke * p);
void gainExp(t_Team * target, t_Poke * source);
void checkLearningMove(t_Poke * p);

void testBattle(t_Team * rouge, t_Team * bleu);
void testSwitch(t_Team * rouge, t_Team * bleu);
void testPP(t_Team * rouge, t_Team * bleu);
void testStruggle(t_Team * rouge, t_Team * bleu);
void teamTest(t_Team * t, int);