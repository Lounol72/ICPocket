#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "GameEngine.h"
#include "Utils.h"

#ifdef USE_HW_RENDERER
    #define DEFAULT_RENDERER_FLAGS (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
#else
    #define DEFAULT_RENDERER_FLAGS SDL_RENDERER_SOFTWARE
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