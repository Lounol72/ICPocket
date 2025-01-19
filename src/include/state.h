#ifndef STATE_H
#define STATE_H

#include <SDL2/SDL.h>
#include "utils.h"
#include "boutons.h"
#include "parametre.h"
#include "menu.h"

/*Enumeration of the different menus we could implement
 * 
 * MENU : The main menu of the game
 * PARAMETRE : The parameters menu
 * JEU : The game itself
 * PAUSE : The pause menu
*/
typedef enum {MENU, SETTINGS, GAME} State;

extern State* currentState;

/**
 * @brief Handles the UI by drawing the current state of the game
 * 
 * @param surface surface to draw on
 * @param win structure containing the window
 */
void drawCurrentState(SDL_Surface* surface, Window* win);


#endif