#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_gamecontroller.h>
#include "GameEngine.h"
#include "Utils.h"
/**
 * @file Window.h
 * @author Louis Alban
 * @date 17/02/2025
 */

#if defined(SDL_RENDERER_PRESENTVSYNC)
        #define renderer_flags DEFAULT_RENDERER_FLAGS
    #else
        #define renderer_flags SDL_RENDERER_SOFTWARE
    #endif

typedef struct Game Game;
extern Game game;

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

void initWindow(Window *win, int width, int height, const char *FontPath);
void destroyWindow(Window *win);

#endif /* WINDOW_H */