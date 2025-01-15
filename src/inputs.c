#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include "include/inputs.h"
#include "include/menu.h"
#include "include/parametre.h"
#include "include/boutons.h"
#include "utils.h"
#include "state.h"

// Set the extern variables
extern Bouton pageParam;
extern Bouton retourMenu;
extern Slider volumeSlider;


void handleInputs(Window* win, State* currentState, SDL_Event event, int* dragging) {
    /*The function handleInputs is quite long and handles multiple types of events. Consider splitting it into smaller functions for each event type*/
    switch (event.type) {
        case SDL_QUIT:
        // Quit event
            quitSDL(win, 0);
            break;
        case SDL_KEYDOWN:
        // Keyboard events
            switch (event.key.keysym.sym) {
                case SDLK_z:
                    printf("Z pressed\n");//Go up
                    break;
                case SDLK_q:
                    printf("Q pressed\n");//go to the left
                    break;
                case SDLK_s:
                    printf("S pressed\n");//go down
                    break;
                case SDLK_d:
                    printf("D pressed\n");//go to the right
                    break;
                case SDLK_a:
                    printf("A pressed\n");//click buton
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
                    if (Cliqued(&pageParam, x, y))
                        pageParam.action(pageParam.actionParam); // Call the action function with the parameter
                    
                    if(Cliqued(&retourMenu,x,y))
                        retourMenu.action(retourMenu.actionParam);
                }
            if (*currentState == PARAMETRE && SDL_PointInRect(&(SDL_Point){x, y}, &(volumeSlider.bar)))
                *dragging = 1;
            break;
        case SDL_MOUSEBUTTONUP:
            *dragging = 0;
            break;
        case SDL_MOUSEMOTION:
            if (*dragging) {
                int x = event.motion.x;
                win->musicVolume = SDL_clamp((x - volumeSlider.bar.x) * MIX_MAX_VOLUME / volumeSlider.bar.w, 0, MIX_MAX_VOLUME);
                Mix_VolumeMusic(win->musicVolume);

                // Update the cursor position
                volumeSlider.cursor.x = volumeSlider.bar.x + (win->musicVolume * volumeSlider.bar.w / MIX_MAX_VOLUME) - (volumeSlider.cursor.w / 2);
            }
            break;
        default:
            break;
    }
}