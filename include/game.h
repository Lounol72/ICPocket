#ifndef GAME_H
#define GAME_H

#include "GameEngine.h"

void mainLoop(Window *win);
void initGame(Window *win);
void updateCurrentButton();
void handleGameEvent(Window *win, SDL_Event *event);
void attqButtonClicked(Window *win, void *data);
void changePokemon(Window *win, void *data);
void nextDuel(Window *win, void *data);

#endif /* GAME_H */