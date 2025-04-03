#include "duel.h"

#ifndef TRAINERAI_H
#define TRAINERAI_H
#define MAX_AI_LVL 20

/**
 * @file trainerAI.h
 * @brief Gestion de l'IA des dresseurs
 * 
 * Ce fichier contient la définition de la structure de l'IA des dresseurs
 * ainsi que les fonctions associées pour gérer le comportement de l'IA
 * 
 * @note L'IA est définie par un niveau et un type. Le niveau détermine la force de l'IA
 * et le type détermine son comportement.
 * 
 * @author Nathan
 */

 /**
  * @struct t_AI_type
  * @brief Types d'IA
  * 
  * Cette énumération définit les différents types d'IA disponibles pour les dresseurs.
  * 
  * @note Les types d'IA sont utilisés pour déterminer le comportement de l'IA lors des combats.
  */
typedef enum{none=1,statusFirst=2,damageOnly=3,boss=6} t_AI_type;


/**
 * @struct t_AI
 * @brief Structure de l'IA d'un dresseur
 * 
 * Cette structure contient les informations sur l'IA d'un dresseur, y compris son niveau,
 * son type et l'équipe à laquelle elle appartient.
 * 
 * @note L'IA est utilisée pour déterminer le comportement du dresseur lors des combats.
 */
typedef struct AI{
	int AI_lvl; /**< niveau de l'IA */
	t_AI_type type; /**< type de l'IA */
	t_Team * AI_t_Team; /**< équipe de l'IA */
} t_AI;



extern t_AI iaTest; /**< IA de test pour les combats */
extern t_AI iaTest2; /**< IA de test pour les combats */

/**
 * @fn void insertionSort(int tabDegats[], int tabMove[], int n)
 * @brief Trie les attaques par ordre décroissant de dégâts
 * 
 * Cette fonction trie les attaques d'un Pokémon par ordre décroissant de dégâts
 * 
 * @param tabDegats Tableau contenant les dégâts des attaques
 * @param tabMove Tableau contenant les indices des attaques
 * @param n Taille des tableaux
 */
void insertionSort(int tabDegats[], int tabMove[], int n);

/**
 * @fn int AI_move_choice(t_AI * ai_team, t_Team * player_team)
 * @brief Choisit le mouvement de l'IA
 *
 * Cette fonction choisit le mouvement de l'IA en fonction de son type et de l'équipe adverse.
 * 
 * @param ai_team Pointeur vers la structure de l'IA
 * @param player_team Pointeur vers la structure de l'équipe du joueur
 * @return int Indice du mouvement choisi
 */
int AI_move_choice(t_AI * ai_team, t_Team * player_team);

#endif