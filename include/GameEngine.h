#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL_ttf.h>


// Define the state of the application
typedef enum AppState {
    GAME = 0, 
    SETTINGS ,
    MENU,
    QUIT,
    PAUSE,
    NEWGAME, 
    LOADGAME
} AppState;




// Structure of the window
typedef struct Window {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int width, height;
    int InitialWidth, InitialHeight;
    int quit;
    TTF_Font * LargeFont;
    TTF_Font * MediumFont;
    TTF_Font * SmallFont;
    TTF_Font *font;
    AppState state;
    double textSpeed;
} Window;


// Include the buttons
#include "Buttons.h"

/**
 * @brief           Initialize the window
 * 
 * @param win       struct Window
 * @param width     width
 * @param height    height
 * @param FontPath  path to the font
 */
void initWindow(Window *win, int width, int height, const char *FontPath);
/**
 * @brief       Destroy the window
 * 
 * @param win   struct Window
 */
void destroyWindow(Window *win);
/**
 * @brief           Handle the event
 * 
 * @param win       struct Window
 * @param event     SDL_Event
 */
void handleEvent(Window *win, SDL_Event *event);
/**
 * @brief           Main loop
 * 
 * @param win       struct Window
 */
void mainLoop(Window *win);

void renderMenu(Window *win);
void handleMenuEvent(Window *win, SDL_Event *event);
void renderGame(Window *win);
void handleGameEvent(Window *win, SDL_Event *event);
void renderSettings(Window *win);
void handleSettingsEvent(Window *win, SDL_Event *event);
void renderQuit(Window *win);
void handleQuitEvent(Window *win, SDL_Event *event);
void changeTextSpeed(Window *win, void *data);

void loadBackground(SDL_Texture **Background, SDL_Renderer *renderer, const char *imagePath);
void renderText(Window * win, const char * text,SDL_Rect  * rect, SDL_Color color, TTF_Font *font);
void initAllButtons(Window * win);

void createFicGame();
void renderNewGame(Window * win);
void handleNewGameEvent(Window * win, SDL_Event * event);

void readFicGame();

void renderLoadGame(Window * win);
void handleLoadGameEvent(Window * win, SDL_Event * event);

#endif // GAMEENGINE_H