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
void handleButtonClick(Bouton* button, int x, int y, State* currentState, State targetState);
void handleMouseMotion(SDL_Event event, Window* win, int* dragging);
void handleMouseButtonDown(SDL_Event event, State* currentState, int* dragging);
void handleKeyDown(SDL_Keycode key, State* currentState);

#endif // INPUTS_H