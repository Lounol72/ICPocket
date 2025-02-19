#ifndef EVENTS_H
#define EVENTS_H

#include "GameEngine.h"
#include "Window.h"
#include "Buttons.h"
#include "Text.h"
#include "structPoke.h"
#include <SDL2/SDL.h>


/* Handeling Events */
void handleEvent(Window *win, SDL_Event *event);
void handleIntermediateEvent(Window *win, SDL_Event *event);
void handleQuitEvent(Window *win, SDL_Event *event);
void handleSettingsEvent(Window *win, SDL_Event *event);
void handleMenuEvent(Window *win, SDL_Event *event);
void handleGameEvent(Window *win, SDL_Event *event);
void handleNewGameEvent(Window *win, SDL_Event *event);
void handleLoadGameEvent(Window *win, SDL_Event *event);
void handleICMonsEvent(Window *win, SDL_Event *event);

#endif /* HANDLEEVENTS_H */