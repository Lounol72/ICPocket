#ifndef INTERDUEL_H
#define INTERDUEL_H

#include "duel.h"

/**
 * @file interDuel.h
 * @brief Définition et gestion des combats entre équipes.

*/

/**
 * @fn void healTeam(t_Team * t)
 * @brief cette fonction permet de soigner une équipe.
 * 
 * @param t Pointeur vers la structure de l'équipe à soigner.
 */
void healTeam(t_Team * t);

/**
 * @fn void getPokeFromTeam(t_Team * player, int index1, t_Team * adv, int index2)
 * @brief cette fonction permet de récupérer un pokémon d'une équipe.
 * 
 * @param player Pointeur vers la structure de l'équipe du joueur.
 * @param index1 Index du pokémon à récupérer dans l'équipe du joueur.
 * @param adv Pointeur vers la structure de l'équipe adverse.
 * @param index2 Index du pokémon à remplacer dans l'équipe adverse.
 */
void getPokeFromTeam(t_Team * player, int index1, t_Team * adv, int index2);

#endif /* INTERDUEL_H */
