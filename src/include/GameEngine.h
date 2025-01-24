#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>


// Define the state of the application
typedef enum AppState {
    MENU = 0,
    GAME,
    SETTINGS,
    QUIT
} AppState;


// Structure of the window
typedef struct Window {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int width;
    int InitialWidth;
    int height;
    int InitialHeight;
    int quit;
    AppState state; 
} Window;


// Include the buttons
#include "Buttons.h"

/**
 * @brief           Initialize the window
 * 
 * @param win       struct Window
 * @param width     width
 * @param height    height
 */
void initWindow(Window *win, int width, int height);
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
void goToGameState(Window *win);
void renderSettings(Window *win);
void handleSettingsEvent(Window *win, SDL_Event *event);
void goToSettingsState(Window *win);
void renderQuit(Window *win);
void handleQuitEvent(Window *win, SDL_Event *event);

void loadBackground(SDL_Texture **Background, SDL_Renderer *renderer, const char *imagePath);
#endif // GAMEENGINE_H