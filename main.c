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

const int BLACK[] = {0, 0, 0, 255};
const int WHITE[] = {255, 255, 255, 255};
const int RED[] = {255, 0, 0, 255};
const int GREEN[] = {0, 255, 0, 255};
const int BLUE[] = {0, 0, 255, 255};
const int GREY[] = {128, 128, 128, 255};

float baseWidth = 1280.0f;  // Résolution de base
float baseHeight = 720.0f;

float scale_width, scale_height;

Bouton pageParam;
Bouton retourMenu;
Bouton jouer;
Bouton sauvegarderMenu;
Bouton TEST;

Bouton* buttons[] = {&jouer, &pageParam, &retourMenu, &sauvegarderMenu, &TEST};
int buttonCount = sizeof(buttons) / sizeof(buttons[0]);

Slider volumeSlider;

State valueState = MENU;
State paramState = PARAMETRE;
State menuState = MENU;
State jouerState = JEU;

State* currentState = &valueState;

// tout foutre en struct
// et enum
int main(void) {
    float currWidth = baseWidth;
    float currHeight = baseHeight;
    Window win = {0};
    SDL_Surface* menu = NULL;
    int dragging = 0;

    if (initialize(&win, baseWidth, baseHeight) != 0) {
        return -1;
    }

    menu = SDL_GetWindowSurface(win.window);
    if (!menu) {
        SDL_Log("Erreur obtention surface de la fenêtre : %s", SDL_GetError());
        quitSDL(&win, -1);
    }
    InitBoutons(&jouer,50,540,230,120,"Jouer",RED, changeState, &jouerState, "assets/Iconjpg.jpg", menu);
    InitBoutons(&pageParam, 525, 540, 230, 120, "Param", BLUE, changeState, &paramState, "assets/Iconjpg.jpg", menu);
    InitBoutons(&retourMenu, 50, 450, 200, 100, "Menu", GREEN, changeState, &menuState, "assets/Iconjpg.jpg", menu);
    InitBoutons(&TEST, 50, 450, 200, 100, "Menu", GREEN, changeState, &menuState, "assets/Iconjpg.jpg", menu);
    InitBoutons(&sauvegarderMenu, 500, 450, 200, 100, "Save", RED, changeState, &menuState, "assets/Iconjpg.jpg", menu);   
    InitSlider(&volumeSlider, 50, 100, 400, 20, "Volume", GREY, win.musicVolume);



    SDL_Log("Bouton retourMenu initialisé à (%d, %d, %d, %d)", retourMenu.rect.x, retourMenu.rect.y, retourMenu.rect.w, retourMenu.rect.h);

    while (1) {

        int newWidth, newHeight;
        SDL_GetWindowSize(win.window, &newWidth, &newHeight);

        if (newWidth != currWidth || newHeight != currHeight) {
            scale_width = (float)newWidth / baseWidth;
            scale_height = (float)newHeight / baseHeight;

            resizeButtons(buttons, buttonCount, scale_width, scale_height);

            currWidth = newWidth;
            baseWidth = newHeight;
        }

        SDL_FillRect(menu, NULL, SDL_MapRGB(menu->format, 255, 255, 255)); 

        // Draw the different elements either the menu or the parameters
        if (*currentState == MENU) {
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
            
            if (*currentState == PARAMETRE) {
                
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
            handleInputs(&win, currentState, event, &dragging);
        }
    }

    quitSDL(&win, 0);
    return 0;
}