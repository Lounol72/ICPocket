#ifndef INPUTS_H
#define INPUTS_H

#include <SDL2/SDL.h>
#include "state.h"
#include "utils.h"

/**
 * @brief Handle the inputs of the user
 * 
 * @param win           Window of the game
 * @param currentState  Current state of the game
 * @param event         Event of the user
 * @param dragging      Dragging state of the user
 */
void handleInputs(Window* win, State* currentState, SDL_Event event, int* dragging);

#endif // INPUTS_H