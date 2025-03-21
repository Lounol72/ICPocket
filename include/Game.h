#ifndef GAME_H
#define GAME_H

#include "Audio.h"
#include "GameEngine.h"


/**
 * @file Game.h
 * @author Louis Alban
 * @brief Gestion des backgrounds, Initialisation de la structure globale
 * @date 17/02/2025
 */


extern Game game;
void initGame(Window *win);
void loadBackground(SDL_Texture **Background, SDL_Renderer *renderer, const char *imagePath) ;
void destroyGame();

#endif /* GAME_H */