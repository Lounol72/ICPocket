#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include "state.h"

void drawMenu(SDL_Surface* surface, SDL_Surface* image);
void drawParametre(SDL_Surface* surface);
void drawHighlight(SDL_Surface* surface, int x, int y, int width, int height);

#endif 