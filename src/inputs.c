#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include "include/inputs.h"
#include "include/menu.h"
#include "include/volume.h"
#include "include/boutons.h"
#include "utils.h"
#include "state.h"

// Set the extern variables
extern Bouton pageParam;
extern Bouton retourMenu;
extern Slider volumeSlider;


void handleInputs(SDL_Window* window, State* currentState, SDL_Event event, int* musicVolume, int* dragging) {
    /*The function handleInputs is quite long and handles multiple types of events. Consider splitting it into smaller functions for each event type*/
    switch (event.type) {
        case SDL_QUIT:
        // Quit event
            quitSDL(0, window);
            break;
        case SDL_KEYDOWN:
        // Keyboard events
            switch (event.key.keysym.sym) {
                case SDLK_a:
                    printf("A pressed\n");
                    break;
                case SDLK_z:
                    printf("Z pressed\n");
                    break;
                case SDLK_e:
                    printf("E pressed\n");
                    break;
                case SDLK_r:
                    printf("R pressed\n");
                    break;
                case SDLK_ESCAPE:
                    printf("Pause the game\n");
                    break;
                default:
                    break;
            }
            break;
        // Mouse events 
        case SDL_MOUSEBUTTONDOWN:
            printf("Mouse clicked\n");
            int x, y;
            SDL_GetMouseState(&x, &y);
            printf("Mouse position: (%d, %d)\n", x, y);
            if (event.button.button == SDL_BUTTON_LEFT) // Left button clicked
                {
                    printf("Left button clicked\n");
                    if (Cliqued(&pageParam, x, y) && *currentState == MENU) {
                        pageParam.action(pageParam.actionParam); // Call the action function with the parameter
                    }
                    if(Cliqued(&retourMenu,x,y) && *currentState == PARAMETRE)
                        retourMenu.action(retourMenu.actionParam);
                }
            if (*currentState == PARAMETRE && x >= volumeSlider.xStart && x <= volumeSlider.xEnd && y >= volumeSlider.yStart && y <= volumeSlider.yEnd) {
                *dragging = 1;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            *dragging = 0;
            break;
        case SDL_MOUSEMOTION:
            if (*dragging) {
                int x = event.motion.x;
                *musicVolume = (x - 100) * MIX_MAX_VOLUME / 400;
                if (*musicVolume < 0) *musicVolume = 0;
                if (*musicVolume > MIX_MAX_VOLUME) *musicVolume = MIX_MAX_VOLUME;
                Mix_VolumeMusic(*musicVolume);
            }
            break;
        default:
            break;
    }
}