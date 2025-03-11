#ifndef INTERDUEL_H
#define INTERDUEL_H

#include "duel.h"

/**
 * @file interDuel.h
 * @brief Définition et gestion des combats entre équipes.

*/
void healTeam(t_Team * t);
void getPokeFromTeam(t_Team * player, int index1, t_Team * adv, int index2);

#endif /* INTERDUEL_H */
