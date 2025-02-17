#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#if defined(_WIN32) || defined(_WIN64)
    #define AUDIO_FREQ 44100
#elif defined(__linux__)
    #define AUDIO_FREQ 22050
#else
    #define AUDIO_FREQ 44100
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

// Inclure structPoke.h avant Buttons.h pour que t_Team soit connu
#include "structPoke.h"
#include "duel.h"
#include "Buttons.h"

#include "trainerAI.h"
#include "save.h"
#include "interDuel.h"
#include "window.h"
#include "menu.h"
#include "settings.h"
#include "newGame.h"
#include "loadGame.h"
#include "icmons.h"
#include "intermediate.h"
#include "game.h"
#include "audio.h"

// Déclarer la structure Text avant d'inclure text.h
typedef struct Text {
    const char *text;
    SDL_Rect rect;
    SDL_Rect initialRect;
    SDL_Color color;
    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;
} Text;

#include "text.h"
#include "log.h"

typedef enum AppState {
    QUIT = 0,
    SETTINGS,
    MENU,
    GAME,
    NEWGAME,
    LOADGAME,
    ICMONS,
    INTER
} AppState;

Text title = {NULL,{0,0,0,0},{0,0,0,0}, {0,0,0,0}, NULL, NULL, NULL};
Text NewGameText = {NULL,{0,0,0,0},{0,0,0,0}, {0,0,0,0}, NULL, NULL, NULL};

/* Primary window & renderer info */
typedef struct Window {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int width, height;
    int InitialWidth, InitialHeight;
    int quit;
    TTF_Font *LargeFont;
    TTF_Font *MediumFont;
    TTF_Font *SmallFont;
    TTF_Font *font;
    AppState state;
    double textSpeed;
} Window;

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

/* Main Game struct that ties everything together */
typedef struct Game {
    UI_Elements *ui;
    int nbMenu;
    BattleState battleState;
    GameState gameState;
    StateHandler *stateHandlers;
    int nbStates;
    Window *win;
    float *speeds;
    int FPS;
    int frameDelay;
    int frameStart;
    int newGameStartTime;
    int currentButton;
    int saved;
} Game;

Game game;


#endif /* GAMEENGINE_H */