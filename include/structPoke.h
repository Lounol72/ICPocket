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
#include "duel.h"
#include "structures.h"

#define NEUTRAL_STAT_CHANGE 6
#define POKE_IS_ABSCENT -1
#define STRUGGLE -10
#define CONFUSED_MOVE -20

#define ATTAQUE_1 0
#define ATTAQUE_2 1
#define ATTAQUE_3 2
#define ATTAQUE_4 3

#define PV 0
#define ATT 1
#define DEF 2
#define SPA 3
#define SPD 4
#define SPE 5

#define TRUE 1
#define FALSE 0

#define typeNumber 4

/**
*@brief Generate a Icmon from a given id in the dataPoke.csv file
*@param p : the structure Icmon to generate
*@param id : the id of the Icmon to generate
 */
void generate_poke(t_Poke *p,int id);
void generate_poke_enemi(t_Poke *p, int line,t_Team * joueur);
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