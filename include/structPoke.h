/**
*@file structPoke.h
*@author Nathan William
*@date 09/12/24
*/

#ifndef STRUCTPOKE_H
#define STRUCTPOKE_H
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
typedef enum{noEffect = 0,burn,poison,paralyze,flinch,confusion} t_Effect;

typedef struct{char nature[15];float coeff[6];} t_Nature;

typedef struct IMG_ICMons IMG_ICMons;

/**
*Structures des caractéristiques d'une attaque d'un ICMon
*/
typedef struct
{	
	int id;
	char name[15];
	int power;
	t_Type type;
	t_Categ categ;
	int accuracy;
	int current_pp;
	int max_pp;
	int priority_lvl;
	int target; //0: ennemi, 1: soi
	int ind_secEffect;
	int probability;
	int value_effect; //Puissance de l'effet
	int effect_modifier;//Modificateur de l'effet
} t_Move;

/**
*Structures d'un ICMon
*/
typedef struct
{
	int id;
	char name[20];
	t_Gender gender;
	t_Type type[2];
	int lvl;
	unsigned exp;
	int nature;
	int current_pv;
	int baseStats[6];
	int initial_pv;
	int iv[6];
	t_Move moveList[4];
	int nb_move;
	t_Effect main_effect;
	// Affichage
	IMG_ICMons *img;
} t_Poke;


extern t_Nature tabNature[25];
extern float typeChart[typeNumber][typeNumber];
/**
*@brief Generate a Icmon from a given id in the dataPoke.csv file
*@param p : the structure Icmon to generate
*@param id : the id of the Icmon to generate
 */
void generate_poke(t_Poke *p,int id);
//void generate_poke_enemi(t_Poke *p, int line,t_Team * joueur);
void generatePoke(t_Poke *);

/**
*@brief Fonctions pour générer des moves ( attaques de nos montres)
*@return the generated move
*/

/*---------------------------------------------------------------------------*/
 
/**
 * @brief génere une attaque non aléatoirement, selon une id donnée, utiliser
 * @param id : l'id
 */
t_Move generateMove(int id);

/**
 * @brief generer aleatoirement
 */
t_Move generateRandomMove();
t_Move generateRandomMoveBetter(t_Poke *p);
void learnMove(t_Poke *, t_Move *, int);
/*----------------------------------------------------------------------------*/
void printPoke(t_Poke *);
void initNature();
void initData();
void printChart();

#endif