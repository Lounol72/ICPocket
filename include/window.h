#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "GameEngine.h"


/* Window init/destroy */
void initWindow(Window *win, int width, int height, const char *FontPath);
void destroyWindow(Window *win);

/* Event handling */
void handleEvent(Window *win, SDL_Event *event);
void loadBackground(SDL_Texture **Background, SDL_Renderer *renderer, const char *imagePath);

void handleQuitEvent(Window *win, SDL_Event *event);

#endif /* WINDOW_H */