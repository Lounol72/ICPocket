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

int BLACK[] = {0, 0, 0, 255};
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
State paramState = SETTINGS;
State menuState = MENU;
State jouerState = GAME;

State* currentState = &valueState;

Window win = {0};
SDL_Surface* menu = NULL;

void initializeButtons(SDL_Surface* menu);

int main(void) {

    int now = 0;
    int ex = 0;
    int periodeFPS = 16; //16 ms pour 60fps
    int dt  = 0;
    
    
    int dragging = 0;

    initializeButtons(menu);

    if (initialize(&win, baseWidth, baseHeight) != 0) {
        return -1;
    }

    menu = SDL_GetWindowSurface(win.window);
    if (!menu) {
        SDL_Log("Erreur obtention surface de la fenêtre : %s", SDL_GetError());
        quitSDL(&win, -1);
    }

    while (1) {
        now = SDL_GetTicks();
        dt = now - ex;
        if (dt >= periodeFPS) {
            
            SDL_FillRect(menu, NULL, SDL_MapRGB(menu->format, 255, 255, 255)); 

            // Draw the different elements either the menu or the parameters
            drawCurrentState(menu, &win);

            // Update the window
            SDL_UpdateWindowSurface(win.window);

            // Handle the events
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quitSDL(&win, 0);
                }
                if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    float newWidth = event.window.data1;
                    float newHeight = event.window.data2;

                    scale_width = newWidth / win.initialW;
                    scale_height = newHeight / win.initialH;

                    updatePosButtons(buttons, buttonCount, scale_width, scale_height);

                    win.w = newWidth;
                    win.h = newHeight;

                    menu = SDL_GetWindowSurface(win.window);

                }
                handleInputs(&win, currentState, event, &dragging);
            }
            ex = now;
        } else {
            SDL_Delay(periodeFPS - dt);
        }
    }

    quitSDL(&win, 0);
    return 0;
    
}

void initializeButtons(SDL_Surface* menu) {
    InitBoutons(&jouer,50,540,230,120,"Jouer",RED, changeState, &jouerState, "assets/Iconjpg.jpg", menu, BLACK);
    InitBoutons(&pageParam, 525, 540, 230, 120, "Param", BLUE, changeState, &paramState, "assets/Iconjpg.jpg", menu, BLACK);
    InitBoutons(&retourMenu, 50, 450, 200, 100, "Menu", GREEN, changeState, &menuState, "assets/Iconjpg.jpg", menu, BLACK);
    InitBoutons(&TEST, 850, 50, 200, 100, " ", GREY, changeState, &menuState, "assets/Iconjpg.jpg", menu, BLACK);
    InitBoutons(&sauvegarderMenu, 500, 50, 200, 100, "Save", RED, changeState, &menuState, "assets/Iconjpg.jpg", menu, BLACK);   
    InitSlider(&volumeSlider, 50, 100, 400, 20, "Volume", GREY, win.musicVolume);
}