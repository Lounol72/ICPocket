#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/**
 * @file structures.h
 * @author William Nathan
 * 
 * @brief Définition des structures de données utilisées dans le jeu.
 * 
 * Ce fichier contient les définitions des structures de données utilisées pour représenter les ICmon, les attaques, les équipes et d'autres éléments du jeu.
 * Il inclut également des énumérations pour les types de ICmon, les catégories d'attaques et les effets d'état.
 */

#define typeNumber 9

/**
 * @brief Tableau de compatibilité entre les types de ICmon.
 * 
 */
extern float typeChart[typeNumber][typeNumber];

/**
 * @struct t_Categ
 * @brief Représente la catégorie d'une attaque.
 * 
 * Cette énumération définit les différentes catégories d'attaques disponibles dans le jeu.
 * 
 */
typedef enum{status=-1,physical=1,special=3} t_Categ;

/**
 * @struct t_Type
 * @brief Représente les types des ICmons.
 * 
 * Cette énumération définit les différents types de ICmon disponibles dans le jeu.
 */
typedef enum{noType=0,feu,plante,eau,electrique,malware,data,net,waifu} t_Type;

/**
 * @struct t_gender
 * @brief Représente le genre d'un ICMon.
 * 
 * Cette énumération définit les genres possibles d'un ICmon.
 */
typedef enum{male = 0,female} t_Gender;

/**
 * @struct t_Effect
 * @brief Représente les effets d'état d'un ICMon.
 * 
 * Cette énumération définit les différents effets d'état qui peuvent affecter un ICmon pendant un combat.
 */
typedef enum{noEffect = 0,burn,poison,paralyze,flinch,confusion} t_Effect;

/**
 * @struct t_Nature
 * @brief Représente la nature d'un ICMon.
 * 
 * Cette structure contient le nom de la nature et les coefficients associés aux statistiques du ICmon.
 */
typedef struct{char nature[15];float coeff[6];} t_Nature;

/**
 * @brief Tableau contenant les natures disponibles dans le jeu.
 * 
 * Ce tableau est utilisé pour stocker les différentes natures et leurs coefficients associés.
 */
extern t_Nature tabNature[25];

/**
 * @struct IMG_ICMons
 * @brief Représente un sprite d'un ICMon.
 * 
 * Cette structure contient des informations sur le sprite, la barre de vie, le nom et les sons associés à un ICMon.
 */
typedef struct IMG_ICMons IMG_ICMons;

/**
*Structures des caractéristiques d'une attaque d'un ICMon
*/

/**
 * @struct t_Move
 * @brief Représente une attaque d'un ICMon.
 * 
 * Cette structure contient des informations sur l'attaque, y compris son nom, sa puissance, son type, sa catégorie, sa précision,
 * le nombre de points de pouvoir (PP), la priorité, la cible, les effets secondaires, la probabilité et la valeur de l'effet. 
 */
typedef struct
{	
	int id; /**< id de l'attaque. */
	char name[15]; /**< nom de l'attaque. */
	int power; /**< puissance de l'attaque. */
	t_Type type; /**< type de l'attaque. */
	t_Categ categ; /**< catégorie de l'attaque. */
	int accuracy; /**< précision de l'attaque. */
	int current_pp; /**< points de pouvoir actuel de l'attaque. */
	int max_pp; /**< pp de l'attaque. */
	int priority_lvl; /**< priorité de l'attaque. */
	int target; /**< 0: ennemi, 1: soi. */
	int ind_secEffect; /**< 0: pas d'effet secondaire, 1: effet secondaire. */
	int probability; /**< probabilité de l'effet secondaire. */
	int value_effect; /**< Puissance de l'effet. */
	int effect_modifier;/**< Modificateur de l'effet. */
}t_Move;


/**
*Structures d'un ICMon
*/

/**
 * @struct t_Poke
 * @brief Représente un ICMon.
 * 
 * Cette structure contient des informations sur un ICMon, y compris son nom, son genre, son type, son niveau,
 * ses points de vie (PV), ses statistiques de base, ses IV (valeurs individuelles), sa liste de mouvements et ses effets principaux.
 */
typedef struct
{
	int id; /**< id de l'ICmon. */
	char name[20];	/**< nom du ICmon. */
	t_Gender gender; /**< genre du ICmon. */
	t_Type type[2]; /**< type du ICmon. */
	int lvl;	 /**< niveau du ICmon. */
	unsigned exp; /**< experience du ICmon. */
	int nature; /**< nature du ICmon. */
	int current_pv; /**< points de vie actuel du ICmon. */
	int baseStats[6]; /**< statistiques de base du ICmon. */
	int initial_pv; /**< points de vie initial du ICmon. */
	int iv[6]; /**< valeurs iv du ICmon. */
	t_Move moveList[4]; /**< liste des attaques du ICmon. */
	int nb_move; /**< nombre d'attaques du ICmon. */
	t_Effect main_effect; /**< effet principal du ICmon. */
	// Affichage
	IMG_ICMons *img; /**< image du ICmon. */
}t_Poke;

/**
 * @struct t_team
 * @brief La structure t_Team contient tout les attributs nécessaire à la génération et à l'utilisation d'une équipe lors d'un combat.
 * @param id l'identifiant de l'équipe, principalement utilisé pour charger une équipe d'une base de données externe.
 * @param trainerName Le nom du dresseur de l'équipe.
 * @param team Une liste de t_Poke, contenant au maximum 6 ICmons.
 * @param nb_poke le nombre de ICmon actuellement dans l'équipe.
 * @param statChanges Les coefficients de variations de chaque statistiques du ICmon en tête d'équipe, sous force d'indice vers le coefficient réel stocké dans la variable globale statVariations.
 * @param effect Un type enuméré représentant l'effet temporaire subit par le ICmon en tête d'équipe. 
 */
typedef struct{
	int id; /**< id de l'équipe. */
	char trainerName[20]; /**< nom du dresseur. */
	t_Poke team[6]; /**< liste des ICmons de l'équipe. */
	int nb_poke; /**< nombre de ICmons dans l'équipe. */
	int statChanges[6]; /**< liste des indices des coefficients de variations des statistiques du ICmon en tête d'équipe. */
	t_Effect effect; /**< effet temporaire du ICmon en tête d'équipe. */
	int lastEnemiID; /**< id du dernier ICmon ennemi. */
	int nb_enemiBeat; /**< nombre de ICmons ennemis battus. */
	int id_save; /**< id de sauvegarde du ICmon. */
}t_Team;

#endif // STRUCTURES_H