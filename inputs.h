#ifndef INPUTS_H
#define INPUTS_H

#include <SDL2/SDL.h>
#include "state.h"
#include "boutons.h"

extern Bouton param;

void handleInputs(SDL_Window* window, int* backgroundColor, State* currentState);

#endif 