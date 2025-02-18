#ifndef GAME_H
#define GAME_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "Window.h"
#include "GameEngine.h"

extern Game game;
void initGame(Window *win);

#endif /* GAME_H */