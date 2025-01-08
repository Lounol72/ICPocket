#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>


typedef struct {
    SDL_Window* window;
    SDL_Surface* image;
    SDL_Surface* icon;
    Mix_Music* music;
    int musicVolume;
    int musicPlaying;
} Window;

int initialize(Window* win);
void quitSDL(Window* win, int codeError);

#endif