#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

int initialize(SDL_Window** window, SDL_Surface** image, Mix_Music** music, SDL_Surface** icon);
void quitSDL(int codeError, SDL_Window* window);

#endif