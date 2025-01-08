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

Bouton pageParam;
Bouton retourMenu;
Slider volumeSlider;
State currentStateValue = MENU;
State paramState = PARAMETRE;
State menuState = MENU;
State* currentState = &currentStateValue;

// tout foutre en struct
// et enum
int main(void) {

    SDL_Window* window = NULL;
    SDL_Surface* image = NULL;
    SDL_Surface* icon = NULL;
    Mix_Music* music = NULL;
    int backgroundColor = 0;
    int musicVolume = MIX_MAX_VOLUME / 2;

    if (initialize(&window, &image, &music, &icon) != 0) {
        return -1;
    }

    // Initialize buttons
    int color[4] = {0, 0, 0, 0};
    InitBoutons(&pageParam, 50, 450, 200, 100, "Param", color, changeState, &paramState);
    InitBoutons(&retourMenu,50,450,200,100,"Menu",color,changeState,&menuState);

    // Main loop
    mainLoop(window, image, &backgroundColor, currentState, music, &musicVolume);

    // Free the memory
    if (image) SDL_FreeSurface(image);
    if (icon) SDL_FreeSurface(icon);
    if (window) SDL_DestroyWindow(window);
    if (music) Mix_FreeMusic(music);
    Mix_CloseAudio();
    quitSDL(0, window);

    return 0;
}


void audioCallback(void* userdata, Uint8* stream, int len) { // A faire
    // Delete the warning
    (void)userdata; 
    (void)len; 
    (void)stream;
    // Sending the data in our buffer...
}


void mainLoop(SDL_Window* window, SDL_Surface* image, int* backgroundColor, State* currentState, Mix_Music* music, int* musicVolume) {

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

    // Initialize the variables
    SDL_Surface* menu = NULL;
    int musicPlaying = 0;
    int dragging = 0;

    // Main loop
    while (1) {
        menu = SDL_GetWindowSurface(window);
        if ((*backgroundColor) == 0)  SDL_FillRect(menu, NULL, SDL_MapRGB(menu->format, 0, 0, 0));
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
            handleInputs(window, currentState, event, musicVolume, &dragging);
        }
    }
}
