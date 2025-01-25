#define NEUTRAL_STAT_CHANGE 6
#define POKE_IS_ABSCENT -1
#define STRUGGLE -10

float statVariations[13];
t_Move struggle;

typedef struct{t_Poke team[6];} t_Team;


int isAttacking(int);
int isSwitching(int);
int ppCheck(t_Move *);
int isAlive(t_Poke * p);
int testActionValidity(int action, t_Team * t);
int accuracyCheck(int accuracy);
int hasMoveLeft(t_Poke *);
int isStruggling(int);

void printTeam(t_Team * t);
int calcStatFrom(t_Poke * p,int stat,int includeVariations);
void initTeam(t_Team * t);
int calcDamage(t_Poke * offender,t_Poke * defender, t_Move *);
void setDefaultStatChanges(t_Poke * p);
int resolveSpeedDuel(int speed1, int speed2);
int PriorityForFirstPoke(t_Poke * p1, t_Poke * p2,int move1,int move2);
void affectDamage(t_Poke * offender, t_Poke * defender, int indexMove);
void swapActualAttacker(t_Team * t, int swapIndex);
int playATurn(t_Team * t1, int move1, t_Team * t2, int move2);

void testBattle(t_Team * rouge, t_Team * bleu);
void testSwitch(t_Team * rouge, t_Team * bleu);
void testPP(t_Team * rouge, t_Team * bleu);
void testStruggle(t_Team * rouge, t_Team * bleu);

int main();