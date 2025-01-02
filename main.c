#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include "src/include/boutons.h"
#include "src/include/inputs.h"
#include "src/include/menu.h"
#include "src/include/state.h"
#include "src/include/utils.h"
#include "src/include/volume.h"

void mainLoop(SDL_Window* window, SDL_Surface* image, int* backgroundColor, State* currentState, Mix_Music* music, int* musicVolume);

Bouton pageParam = {50, 450, 200, 100}; // x ,y largeur, hauteur

int main(int argc, char* argv[]) {
    (void)argc; // Supprimer le warning
    (void)argv; // Supprimer le warning
    SDL_Window* window = NULL;
    SDL_Surface* menu = NULL;
    SDL_Surface* pageParametre = NULL;
    Mix_Music* music = NULL; // Déclaration de la variable pour la musique
    int musicVolume = MIX_MAX_VOLUME / 2; // Volume initial de la musique (50%)
    
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

    // Charger la musique
    music = Mix_LoadMUS("assets/audio/Ulysse.mp3");
    if (!music) {
        SDL_Log("Erreur chargement musique : %s", Mix_GetError());
        Mix_CloseAudio();
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
    
    if (window == NULL) quitSDL(0,window);
    
    mainLoop(window, image, &backgroundColor, &currentState, music, &musicVolume);

    SDL_FreeSurface(menu);
    SDL_FreeSurface(pageParametre);
    SDL_DestroyWindow(window);
    Mix_FreeMusic(music); // Libérer la musique
    Mix_CloseAudio();
    quitSDL(0,window);

    return 0;
}

void audioCallback(void* userdata, Uint8* stream, int len) {
    (void)userdata; // Supprimer le warning
    (void)len; // Supprimer le warning
    (void)stream; // Supprimer le warning
    // Envoyez les données dans notre buffer...
}

void mainLoop(SDL_Window* window, SDL_Surface* image, int* backgroundColor, State* currentState, Mix_Music* music, int* musicVolume) {
    SDL_Surface* menu = NULL;
    int musicPlaying = 0;
    int dragging = 0;

    while (1) {
        menu = SDL_GetWindowSurface(window);
        if (*backgroundColor == 0) {
            SDL_FillRect(menu, NULL, SDL_MapRGB(menu->format, 0, 0, 0));
        } else {
            SDL_FillRect(menu, NULL, SDL_MapRGB(menu->format, 255, 255, 255));
        }
        if (*currentState == MENU) {
            if (!musicPlaying) {
                if (Mix_PlayMusic(music, -1) == -1) {
                    SDL_Log("Erreur lecture musique : %s", Mix_GetError());
                } else {
                    musicPlaying = 1;
                }
            }
            drawMenu(menu, image);
        } else {
            if (musicPlaying) {
                Mix_HaltMusic();
                musicPlaying = 0;
            }
            if (*currentState == PARAMETRE) {
                drawParametre(menu);
                drawVolumeControl(menu, *musicVolume);
            }
        }
        SDL_UpdateWindowSurface(window);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quitSDL(0, window);
            }
            handleInputs(window, backgroundColor, currentState, event, musicVolume, &dragging);
        }
    }
}
