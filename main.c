#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include "boutons.h"
#include "inputs.h"
#include "menu.h"
#include "state.h"
#include "utils.h"

void mainLoop(SDL_Window* window, SDL_Surface* image, int* backgroundColor, State* currentState);

Bouton param = {50, 450, 200, 100}; // x ,y largeur, hauteur

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Surface* menu = NULL;
    SDL_Surface* parametre = NULL;
    
    int backgroundColor = 0; // 0 for black, 1 for white
    State currentState = MENU;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) quitSDL(1,NULL);
    if (SDL_Init(SDL_INIT_AUDIO) < 0) quitSDL(1,NULL);
    if (Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0) // création de la configuration de la carte son
    {
        SDL_Log("Erreur initialisation SDL_mixer : %s", Mix_GetError());
        SDL_Quit();
        return -1;
    }
    window = SDL_CreateWindow("ICPocket", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 720, 600, SDL_WINDOW_SHOWN);

    SDL_Surface* image = IMG_Load("assets/MENU_TEST.png");
    if(!image)
    {
        printf("Erreur de chargement de l'image : %s",SDL_GetError());
        return -1;
    }
    
    if (window == NULL)quitSDL(0,window);
    
    mainLoop(window, image, &backgroundColor, &currentState);

    SDL_FreeSurface(menu);
    SDL_FreeSurface(parametre);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    quitSDL(0,window);

    return 0;
}

void mainLoop(SDL_Window* window, SDL_Surface* image, int* backgroundColor, State* currentState) {
    SDL_Surface* menu = NULL;

    SDL_AudioSpec spec;

    SDL_memset(&spec, 0, sizeof(spec));

    spec.freq = 96000; // 4 100 Hz, 48 000 Hz, 96 000 Hz, 192 000 Hz (standard) 
    spec.format = AUDIO_F32SYS;
    spec.channels = 1;
    spec.samples = 4096; // Oublier pas que ce sa doit être en puissance de deux 2^n
    spec.callback = [](void* param, Uint8* stream, int len)
    {
        // Envoyez les données dans notre buffer...
    };

    SDL_AudioDeviceID dev = SDL_OpenAudioDevice(nullptr, 0, &spec, &spec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);

    SDL_PauseAudioDevice(dev, SDL_FALSE);

    while (1) {
        menu = SDL_GetWindowSurface(window);
        if (*backgroundColor == 0) {
            SDL_FillRect(menu, NULL, SDL_MapRGB(menu->format, 0, 0, 0));
        } else {
            SDL_FillRect(menu, NULL, SDL_MapRGB(menu->format, 255, 255, 255));
        }
        if (*currentState == MENU) {
            drawMenu(menu, image);
        } else if (*currentState == PARAMETRE) {
            drawParametre(menu);
        }
        SDL_UpdateWindowSurface(window);
        handleInputs(window, backgroundColor, currentState);
    }
}
