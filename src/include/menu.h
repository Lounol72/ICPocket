#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include "state.h"
#include "boutons.h"

extern Bouton pageParam;
extern State* currentState;
extern Bouton retourMenu;

void drawMenu(SDL_Surface* surface, SDL_Surface* image);
void drawHighlight(SDL_Surface* surface, int x, int y, int width, int height);
void changeState(void* targetState);

#endif 