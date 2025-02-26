#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_gamecontroller.h>
#include <stdio.h>
#include <stdlib.h>
#include "Log.h"
#include "Audio.h"
#include "Buttons.h"
#include "structPoke.h"
#include "duel.h"
#include "trainerAI.h"
#include "save.h"
#include "interDuel.h"
#include "Utils.h"
#include "Text.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "Camera.h"
#include "ThreadManager.h"
/**
 * @file GameEngine.h
 * @author Louis Alban
 * @date 24/01/2025
 */



/* ------------- Structs ------------- */
/* UI element container, referencing the ButtonList & SliderList defined in Buttons.h */
typedef struct {
    ButtonList *buttons;
    SliderList *sliders;
    SDL_Texture *background;
} UI_Elements;

/* Battle data (assuming t_Team is defined in structPoke.h or similar) */
typedef struct {
    t_Team rouge;
    t_Team bleu;
    t_AI ia;
} BattleState;

/* General game state info */
typedef struct {
    Mix_Music *music;
    int playerTurn;
    int initialized;
    AppState currentState;
} GameState;

/* Each state has a render function and an event handler. */
typedef struct StateHandler {
    AppState state;
    void (*handleEvent)(Window *, SDL_Event *);
} StateHandler;

typedef struct {
    Player *player;
    Map *map;
    Camera *camera;
} GameData;

/* Main Game struct that ties everything together */
typedef struct Game {
    UI_Elements *ui;
    int nbMenu;
    GameData gameData;
    BattleState battleState;
    GameState gameState;
    StateHandler *stateHandlers;
    int nbStates;
    Window *win;
    float *speeds;
    int FPS;
    int frameDelay;
    int frameStart;
    int currentButton;
    int saved;
    float deltaTime;
    ThreadManager threadManager;
} Game;

#include "Game.h"
#include "Events.h"
#include "RenderICMons.h"

/* ------------- Function Prototypes ------------- */

void initAllButtons(Window *win);

/* Render */
void render(Window *win);

/* Main loop */
void mainLoop(Window *win);


/* Attack button */
void attqButtonClicked(Window *win, void *data);

/* Generic state change button callback */
void changeState(Window *win, void *data);

/* Text-related updates for attacks */
void updateICButtons(Window *win, t_Team *team);

void nextDuel(Window* win, void *data);

void updateCurrentButton();
void updateMusic();

void manageFrameRate(int frameStart);

#endif /* GAMEENGINE_H */