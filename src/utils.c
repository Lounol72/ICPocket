#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "include/utils.h"
#include "include/constants.h"

int initialize(Window* win, int w, int h) {
    if (!win) return -1; // Vérification du pointeur nul.

    // Initialisation des dimensions de la fenêtre.
    win->w = w;
    win->h = h;
    win->initialW = w;
    win->initialH = h;

    // Définir le driver audio.
    SDL_setenv("SDL_AUDIODRIVER", "pulseaudio", 1);

    // Initialiser SDL.
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        SDL_Log("Erreur initialisation SDL : %s", SDL_GetError());
        return -1;
    }

    // Initialiser SDL_image.
    if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) & (IMG_INIT_JPG | IMG_INIT_PNG))) {
        SDL_Log("Erreur initialisation SDL_image : %s", IMG_GetError());
        SDL_Quit();
        return -1;
    }

    // Initialiser SDL_mixer.
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        SDL_Log("Erreur initialisation SDL_mixer : %s", Mix_GetError());
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Initialiser SDL_ttf.
    if (TTF_Init() == -1) {
        SDL_Log("Erreur initialisation SDL_ttf : %s", TTF_GetError());
        Mix_CloseAudio();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Charger l'icône.
    win->icon = IMG_Load("assets/Iconjpg.jpg");
    if (!win->icon) {
        SDL_Log("Erreur chargement icône : %s", SDL_GetError());
        TTF_Quit();
        Mix_CloseAudio();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Créer la fenêtre.
    win->window = SDL_CreateWindow("ICPocket", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, win->w, win->h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!win->window) {
        SDL_Log("Erreur création fenêtre : %s", SDL_GetError());
        SDL_FreeSurface(win->icon);
        TTF_Quit();
        Mix_CloseAudio();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_SetWindowIcon(win->window, win->icon);

    // Charger l'image.
    win->image = IMG_Load("assets/menu.jpg");
    if (!win->image) {
        SDL_Log("Erreur chargement image : %s", SDL_GetError());
        SDL_DestroyWindow(win->window);
        SDL_FreeSurface(win->icon);
        TTF_Quit();
        Mix_CloseAudio();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Charger la musique.
    win->music = Mix_LoadMUS("assets/audio/Ulysse.mp3");
    if (!win->music) {
        SDL_Log("Erreur chargement musique : %s", Mix_GetError());
        SDL_FreeSurface(win->image);
        SDL_DestroyWindow(win->window);
        SDL_FreeSurface(win->icon);
        TTF_Quit();
        Mix_CloseAudio();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Configurer le volume de la musique.
    win->musicVolume = MIX_MAX_VOLUME * 50 / 100;
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
    TTF_Quit();
    SDL_Quit();
    exit(codeError);
}


// Function to initialize SDL_ttf and load the font
TTF_Font* initializeFont(const char* fontPath, int fontSize) {
    if (TTF_Init() == -1) {
        SDL_Log("Erreur initialisation SDL_ttf : %s", TTF_GetError());
        return NULL;
    }

    TTF_Font* font = TTF_OpenFont(fontPath, fontSize);
    if (!font) {
        SDL_Log("Erreur chargement police : %s", TTF_GetError());
        TTF_Quit();
        return NULL;
    }

    return font;
}