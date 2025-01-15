#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "include/utils.h"
#include "include/constants.h"

int initialize(Window* win) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        SDL_Log("Erreur initialisation SDL : %s", SDL_GetError());
        return -1;
    }
    
    // Initialize SDL_image
    if ((IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) & (IMG_INIT_JPG | IMG_INIT_PNG)) == 0) {
        SDL_Log("Erreur initialisation SDL_image : %s", IMG_GetError());
        SDL_Quit();
        return -1;
    }

    // Initialize Mixer
    if (Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0) {
        SDL_Log("Erreur initialisation SDL_mixer : %s", Mix_GetError());
        SDL_Quit();
        return -1;
    }

    // Load the icon
    win->icon = IMG_Load("assets/Iconjpg.jpg");
    if (!win->icon) {
        SDL_Log("Erreur chargement icône : %s", SDL_GetError());
        quitSDL(win, -1);
        return -1;
    }

    // Create the window
    win->window = SDL_CreateWindow("ICPocket", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!win->window) {
        SDL_Log("Erreur de création de la fenêtre : %s", SDL_GetError());
        quitSDL(win, -1);
        return -1;
    }

    // Set the window icon
    SDL_SetWindowIcon(win->window, win->icon);

    // Load the image
    win->image = IMG_Load("assets/MENU_TEST.png");
    if (!win->image) {
        SDL_Log("Erreur chargement image : %s", SDL_GetError());
        quitSDL(win, -1);
        return -1;
    }

    // Load the music
    win->music = Mix_LoadMUS("assets/audio/Ulysse.mp3");
    if (!win->music) {
        SDL_Log("Erreur chargement musique : %s", Mix_GetError());
        quitSDL(win, -1);
        return -1;
    }
    
    win->musicVolume = MIX_MAX_VOLUME * 50 / 100 ;
    Mix_VolumeMusic(win->musicVolume);
    win->musicPlaying = 0;

    return 0;
}

void quitSDL(Window* win, int codeError) {
    if (win->image) SDL_FreeSurface(win->image);
    if (win->icon) SDL_FreeSurface(win->icon);
    if (win->window) SDL_DestroyWindow(win->window);
    if (win->music) Mix_FreeMusic(win->music);
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
    exit(codeError);
}