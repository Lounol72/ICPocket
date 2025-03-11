#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


#define typeNumber 8

extern float typeChart[typeNumber][typeNumber];

typedef enum{status=-1,physical=1,special=3} t_Categ;
typedef enum{noType=0,feu,plante,eau,electrique,malware,data,net} t_Type;
typedef enum{male = 0,female} t_Gender;
typedef enum{noEffect = 0,burn,poison,paralyze,flinch,confusion} t_Effect;

typedef struct{char nature[15];float coeff[6];} t_Nature;
extern t_Nature tabNature[25];

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
}t_Move;


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
}t_Poke;

/**
 * @brief La structure t_Team contient tout les attributs nécessaire à la génération et à l'utilisation d'une équipe lors d'un combat.
 * @param id l'identifiant de l'équipe, principalement utilisé pour charger une équipe d'une base de données externe.
 * @param trainerName Le nom du dresseur de l'équipe.
 * @param team Une liste de t_Poke, contenant au maximum 6 pokémons.
 * @param nb_poke le nombre de pokémon actuellement dans l'équipe.
 * @param statChanges Les coefficients de variations de chaque statistiques du pokémon en tête d'équipe, sous force d'indice vers le coefficient réel stocké dans la variable globale statVariations.
 * @param effect Un type enuméré représentant l'effet temporaire subit par le pokémon en tête d'équipe. 
 */
typedef struct{
	int id;
	char trainerName[20];
	t_Poke team[6];
	int nb_poke;
	int statChanges[6];
	t_Effect effect;
	int lastEnemiID;
	int nb_enemiBeat;
}t_Team;

#endif // STRUCTURES_H