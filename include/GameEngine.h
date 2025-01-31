#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

/* Now we include Buttons.h to use ButtonList, SliderList, etc. */
#include "Buttons.h"

/* If you rely on these for 't_Team' or other structs, keep them here: */
#include "structPoke.h"
#include "duel.h"
#include "trainerAI.h"

/* Define the state of the application */
typedef enum AppState {
    QUIT = 0, // 0
    SETTINGS, // 1
    MENU, // 2
    GAME,  // 3
    NEWGAME,  // 4
    LOADGAME, // 5
    ICMONS, // 6
    INTER
} AppState;

/* Text rendering struct */
typedef struct Text {
    char *text;
    SDL_Rect rect;
    SDL_Rect initialRect;
    SDL_Color color;
    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;
} Text;

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
    void (*render)(Window *);
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
} Game;

/* ------------- Function Prototypes ------------- */

/* Window init/destroy */
void initWindow(Window *win, int width, int height, const char *FontPath);
void destroyWindow(Window *win);

/* Main loop */
void handleEvent(Window *win, SDL_Event *event);
void mainLoop(Window *win);

/* Menu */
void renderMenu(Window *win);
void handleMenuEvent(Window *win, SDL_Event *event);

/* Game proper */
void renderGame(Window *win);
void handleGameEvent(Window *win, SDL_Event *event);

/* Settings */
void renderSettings(Window *win);
void handleSettingsEvent(Window *win, SDL_Event *event);

/* Quit */
void renderQuit(Window *win);
void handleQuitEvent(Window *win, SDL_Event *event);

/* UI and text speed changes */
void changeTextSpeed(Window *win, void *data);
void loadBackground(SDL_Texture **Background, SDL_Renderer *renderer, const char *imagePath);

/* Text rendering, creation, destruction */
void renderText(Window *win, Text *text);
void updateTextPosition(Text *text, float scaleX, float scaleY);
void destroyText(Text *text);

/* Button initializations */
void initAllButtons(Window *win);

/* Save management */
void createFicGame();
void readFicGame();

/* Loading screens */
void renderNewGame(Window *win);
void handleNewGameEvent(Window *win, SDL_Event *event);
void renderLoadGame(Window *win);
void handleLoadGameEvent(Window *win, SDL_Event *event);

/* Attack button */
void attqButtonClicked(Window *win, void *data);

/* Generic state change button callback */
void changeState(Window *win, void *data);

/* Text-related updates for attacks */
void initText(Window *win);
void updateAttackButtons(Window *win, t_Team *team);

/* Audio */
void initAudio();
void loadMusic(Mix_Music **music, const char *musicPath);

/* ICMons selection */
void renderICMons(Window *win);
void handleICMonsEvent(Window *win, SDL_Event *event);

void initGame(Window *win) ;

#endif /* GAMEENGINE_H */