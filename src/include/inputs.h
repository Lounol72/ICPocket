#ifndef INPUTS_H
#define INPUTS_H

#include <SDL2/SDL.h>
#include "state.h"

/**
 * @brief  Handles the inputs for the game.
 * 
 * @param window            Window to handle the inputs for.
 * @param currentState      State of the game.
 * @param event             Event of the window.
 * @param musicVolume       The volume of the music.
 * @param dragging          if the slider is being dragged.
 */
void handleInputs(SDL_Window* window, State* currentState, SDL_Event event, int* musicVolume, int* dragging);

#endif // INPUTS_H