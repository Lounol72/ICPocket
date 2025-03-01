#ifndef GAME_H
#define GAME_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "Audio.h"
#include "GameEngine.h"
#include "ThreadManager.h"

/**
 * @file Game.h
 * @author Louis Alban
 * @date 17/02/2025
 */

extern Game game;
void initGame(Window *win);
void loadBackground(SDL_Texture **Background, SDL_Renderer *renderer, const char *imagePath) ;
void destroyGame();

#endif /* GAME_H */