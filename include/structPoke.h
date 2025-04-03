/**
*@file structPoke.h
*@author Nathan William
*@date 09/12/24
*/

#ifndef STRUCTPOKE_H
#define STRUCTPOKE_H

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


/**
* @fn void generate_poke(t_Poke *p,int id)
* @brief Génère un Pokémon avec des caractéristiques aléatoires.
*
* @param p Pointeur vers la structure t_Poke à remplir.
* @param id ID du Pokémon à générer.
*/
void generate_poke(t_Poke *p,int id);

/**
* @fn void generate_poke_enemi(t_Poke *p, int line,t_Team * joueur)
* @brief Génère un Pokémon ennemi avec des caractéristiques aléatoires.
*
*
* @param p Pointeur vers la structure t_Poke à remplir.
* @param line Ligne de la matrice de collision.
* @param joueur Pointeur vers la structure t_Team représentant le joueur.
*/
void generate_poke_enemi(t_Poke *p, int line,t_Team * joueur);

/**
 * @fn void generatePoke(t_Poke *)
 * @brief Génère un Pokémon avec des caractéristiques aléatoires.
 * 
 * Cette fonction remplit la structure t_Poke avec des valeurs aléatoires pour les attributs du Pokémon.
 * 
 * @param poke Pointeur vers la structure t_Poke à remplir.
 */
void generatePoke(t_Poke *);

/**
 * @fn t_move generateMove(int id)
 */

/*---------------------------------------------------------------------------*/
 
/**
 * @brief génere une attaque non aléatoirement, selon une id donnée, utiliser
 * @param id : l'id
 */
t_Move generateMove(int id);

/**
 * @fn t_Move generateRandomMove()
 * @brief Génère un mouvement aléatoire pour un Pokémon.
 * 
 * Cette fonction choisit un mouvement aléatoire parmi les mouvements disponibles du Pokémon.
 * @return t_Move 
 */
t_Move generateRandomMove();

/**
 * @fn t_Move generateRandomMoveBetter(t_Poke *p)
 * @brief Génère un mouvement aléatoire pour un Pokémon, en tenant compte de ses caractéristiques.
 * 
 * Cette fonction choisit un mouvement aléatoire parmi les mouvements disponibles du Pokémon, en tenant compte de ses caractéristiques.
 * @param p Pointeur vers la structure t_Poke représentant le Pokémon.
 * @return t_Move 
 */
t_Move generateRandomMoveBetter(t_Poke *p);

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
void learnMove(t_Poke *, t_Move *, int);
/*----------------------------------------------------------------------------*/

/**
 * @fn void printPoke(t_Poke *)
 * @brief Affiche les informations d'un Pokémon.
 * 
 * Cette fonction affiche les informations du Pokémon, y compris ses caractéristiques et ses mouvements.
 * 
 * @param poke Pointeur vers la structure t_Poke à afficher.
 */
void printPoke(t_Poke *);

/**
 * @fn void initNature()
 * @brief Initialise les natures des Pokémon.
 * 
 * Cette fonction remplit un tableau de natures avec des valeurs aléatoires.
 */
void initNature();

/**
 * @fn void initData()
 * @brief Initialise les données du jeu.
 * 
 * Cette fonction initialise les données du jeu, y compris les Pokémon, les mouvements et les natures.
 */
void initData();

/**
 * @fn void printChart()
 * @brief Affiche le tableau des types de Pokémon.
 * 
 * Cette fonction affiche un tableau indiquant les forces et faiblesses des différents types de Pokémon.
 */
void printChart();

#endif