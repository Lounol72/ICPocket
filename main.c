#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include "src/include/boutons.h"
#include "src/include/inputs.h"
#include "src/include/menu.h"
#include "src/include/state.h"
#include "src/include/utils.h"
#include "src/include/parametre.h"

Bouton pageParam;
Bouton retourMenu;
Slider volumeSlider;
State paramState = PARAMETRE;
State menuState = MENU;
State currentState = MENU;

// tout foutre en struct
// et enum
int main(void) {
    Window win = {0};
    SDL_Surface* menu = NULL;
    int dragging = 0;

    if (initialize(&win) != 0) {
        return -1;
    }

    menu = SDL_GetWindowSurface(win.window);
    if (!menu) {
        SDL_Log("Erreur obtention surface de la fenêtre : %s", SDL_GetError());
        quitSDL(&win, -1);
    }

    int color[4] = {0, 0, 0, 0};
    InitBoutons(&pageParam, 31, 10, 230, 120, "Param", color, changeState, &paramState);
    InitBoutons(&retourMenu, 50, 450, 200, 100, "Menu", color, changeState, &menuState);
    InitSlider(&volumeSlider, 50, 50, 400, 20, "Volume", color, win.musicVolume);

    while (1) {
        SDL_FillRect(menu, NULL, SDL_MapRGB(menu->format, 255, 255, 255));

        // Draw the different elements either the menu or the parameters
        if (currentState == MENU) {
            if (!win.musicPlaying) {
                if (Mix_PlayMusic(win.music, -1) == -1) {
                    SDL_Log("Erreur lecture musique : %s", Mix_GetError());
                } else {
                    win.musicPlaying = 1;
                }
            }
            drawMenu(menu, win.image);
        } else {
            if (win.musicPlaying) {
                Mix_HaltMusic();
                win.musicPlaying = 0;
            }
            if (currentState == PARAMETRE) {
                drawParametre(menu);
                drawVolumeControl(menu, win.musicVolume);
            }
        }

        // Update the window
        SDL_UpdateWindowSurface(win.window);

        // Handle the events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quitSDL(&win, 0);
            }
            handleInputs(&win, &currentState, event, &dragging);
        }
    }

    quitSDL(&win, 0);
    return 0;
}