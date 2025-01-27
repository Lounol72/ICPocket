#ifndef INPUTS_H
#define INPUTS_H

#include <SDL2/SDL.h>
#include "state.h"

void handleInputs(SDL_Window* window, int* backgroundColor, State* currentState, SDL_Event event, int* musicVolume, int* dragging);

#endif // INPUTS_H