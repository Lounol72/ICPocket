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
int initialize(SDL_Window** window, SDL_Surface** image, Mix_Music** music, SDL_Surface** icon);

Bouton pageParam = {50, 450, 200, 100}; // x ,y largeur, hauteur
Slider volumeSlider = {100, 500, 45, 65};

int main(int argc, char* argv[]) {
    // Delete the warning
    (void)argc;
    (void)argv;

    SDL_Window* window = NULL;
    SDL_Surface* image = NULL;
    SDL_Surface* icon = NULL;
    Mix_Music* music = NULL;
    int musicVolume = MIX_MAX_VOLUME / 2;
    int backgroundColor = 0;
    State currentState = MENU;

    if (initialize(&window, &image, &music, &icon) < 0) {
        return -1;
    }

    // Main loop
    mainLoop(window, image, &backgroundColor, &currentState, music, &musicVolume);

    // Free the memory
    SDL_FreeSurface(image);
    SDL_FreeSurface(icon);
    SDL_DestroyWindow(window);
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    quitSDL(0, window);

    return 0;
}

int initialize(SDL_Window** window, SDL_Surface** image, Mix_Music** music, SDL_Surface** icon) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        SDL_Log("Erreur initialisation SDL : %s", SDL_GetError());
        return -1;
    }
    
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
        SDL_Log("Erreur de chargement de l'image : %s", SDL_GetError());
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

void audioCallback(void* userdata, Uint8* stream, int len) { // A faire
    // Delete the warning
    (void)userdata; 
    (void)len; 
    (void)stream;
    // Sending the data in our buffer...
}

/**
 * The mainLoop function manages the main game loop, including updating the window, image, background
 * color, game state, music, and music volume.
 * 
 * @param window SDL_Window pointer for the main window of the application.
 * @param image The `image` parameter is a pointer to an SDL_Surface, which represents an image that
 * will be displayed on the window during the main loop of the program.
 * @param backgroundColor The `backgroundColor` parameter is a pointer to an integer array that
 * represents the RGB values of the background color for the window where the image will be displayed.
 * @param currentState The `currentState` parameter is a pointer to a `State` struct, which likely
 * contains information about the current state of the game or application. This struct may include
 * variables such as player position, game level, score, or other relevant information needed for the
 * main loop to update and render the game correctly
 * @param music A pointer to the Mix_Music structure, which represents the music that will be played in
 * the program.
 * @param musicVolume The `musicVolume` parameter is an integer pointer that stores the volume level of
 * the music being played in the main loop. This variable can be used to control the volume of the
 * music within the game.
 */
void mainLoop(SDL_Window* window, SDL_Surface* image, int* backgroundColor, State* currentState, Mix_Music* music, int* musicVolume) {
    // Initialize the variables
    SDL_Surface* menu = NULL;
    int musicPlaying = 0;
    int dragging = 0;

    // Main loop
    while (1) {
        menu = SDL_GetWindowSurface(window);
        if (*backgroundColor == 0)  SDL_FillRect(menu, NULL, SDL_MapRGB(menu->format, 0, 0, 0));
        else SDL_FillRect(menu, NULL, SDL_MapRGB(menu->format, 255, 255, 255));

        // Draw the different elements either the menu or the parameters
        if (*currentState == MENU) {
            if (!musicPlaying) {
                if (Mix_PlayMusic(music, -1) == -1) SDL_Log("Erreur lecture musique : %s", Mix_GetError());
                else musicPlaying = 1;
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

        // Update the window
        SDL_UpdateWindowSurface(window);
        
        // Handle the events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quitSDL(0, window);
            }
            handleInputs(window, backgroundColor, currentState, event, musicVolume, &dragging);
        }
    }
}
