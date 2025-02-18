#ifndef WINDOW_H
#define WINDOW_H

/* Window init/destroy */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "GameEngine.h"

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