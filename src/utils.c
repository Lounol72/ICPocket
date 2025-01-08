#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "include/utils.h"

int initialize(SDL_Window** window, SDL_Surface** image, Mix_Music** music, SDL_Surface** icon) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        SDL_Log("Erreur initialisation SDL : %s", SDL_GetError());
        return -1;
    }
    // Initialize Mixer
    if (Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0) {
        SDL_Log("Erreur initialisation SDL_mixer : %s", Mix_GetError());
        SDL_Quit();
        return -1;
    }

    // Load the icon
    *icon = IMG_Load("assets/Iconjpg.jpg");
    if (!*icon) {
        SDL_Log("Erreur chargement icone : %s", SDL_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return -1;
    }

    // Create the window
    *window = SDL_CreateWindow("ICPocket", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 720, 600, SDL_WINDOW_SHOWN);
    if (!*window) {
        SDL_Log("Erreur de création de la fenêtre : %s", SDL_GetError());
        SDL_FreeSurface(*icon);
        Mix_CloseAudio();
        SDL_Quit();
        return -1;
    }

    // Set the window icon
    SDL_SetWindowIcon(*window, *icon);

    // Load the image
    *image = IMG_Load("assets/MENU_TEST.png");
    if (!*image) {
        SDL_Log("Erreur chargement image : %s", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_FreeSurface(*icon);
        Mix_CloseAudio();
        SDL_Quit();
        return -1;
    }

    // Load the music
    *music = Mix_LoadMUS("assets/audio/Ulysse.mp3");
    if (!*music) {
        SDL_Log("Erreur chargement musique : %s", Mix_GetError());
        SDL_FreeSurface(*image);
        SDL_DestroyWindow(*window);
        SDL_FreeSurface(*icon);
        Mix_CloseAudio();
        SDL_Quit();
        return -1;
    }

    return 0;
}


void quitSDL(int codeError, SDL_Window* window) {
    /* Quit the SDL and free the memory
     * 
     * @param codeError The `codeError` parameter is an integer that represents the error code that will be returned
     * when the program exits. This code can be used to identify the cause of the error that caused the program to
     * exit.
     * @param window SDL_Window pointer for the main window of the application.
     */
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
    exit(codeError);
}