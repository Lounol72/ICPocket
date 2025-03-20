#ifndef SAVE_H
#define SAVE_H

#include "duel.h"

/**
*@file save.h
*@author William
*@brief Save the player's data in a file
*@param nomSave : the name of the save
*@param joueur : the player's data
*@param dresseur : the last defeated trainer
 */
void sauvegarder(t_Team * teamJ1,t_Team * dresseur);
/**
*@brief Load the player's data from a file
*@param nomSave : the name of the save
*@param joueur : the player's data
 */
int charger(char * name,t_Team * teamJ1,t_Team * dresseur);

#endif // SAVE_H