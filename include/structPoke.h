#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


#define PV 0
#define ATT 1
#define DEF 2
#define SPA 3
#define SPD 4
#define SPE 5

#define TRUE 1
#define FALSE 0

#define typeNumber 4

typedef enum{status=-1,physical=1,special=3} t_Categ;
typedef enum{noType=0,feu,plante,eau} t_Type;
typedef enum{male = 0,female} t_Gender;

typedef struct{char nature[15];float coeff[6];} t_Nature;

typedef struct
{
	char name[15];
	int power;
	t_Type type;
	t_Categ categ;
	int accuracy;
	int current_pp;
	int max_pp;
	int priority_lvl;
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
	int iv[6];
	t_Move moveList[4];
	int nb_move;
	// Affichage
	SDL_Rect rect;
	SDL_Texture *texture;
	SDL_Rect initialRect;
} t_Poke;


extern t_Nature tabNature[25];
extern float typeChart[typeNumber][typeNumber];
/**
*@brief Generate a Icmon from a given id in the dataPoke.csv file
*@param p : the structure Icmon to generate
*@param id : the id of the Icmon to generate
 */
void generate_poke(t_Poke *p,int id);
void generatePoke(t_Poke *);
t_Move generateRandomMove();
void learnMove(t_Poke *, t_Move *, int);
void printPoke(t_Poke *);
void initNature();
void initData();
void printChart();
