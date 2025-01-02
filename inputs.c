#include "inputs.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include "menu.h"
#include "boutons.h"
#include "utils.h"

extern Bouton pageParam;

void handleInputs(SDL_Window* window, int* backgroundColor, State* currentState) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quitSDL(0, window);
                break;
            case SDL_KEYDOWN:
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
            case SDL_MOUSEBUTTONDOWN:
                printf("Mouse clicked\n");
                int x, y;
                SDL_GetMouseState(&x, &y);
                printf("Mouse position: (%d, %d)\n", x, y);
                if (Cliqued(&pageParam, x, y)) {
                    *backgroundColor = !(*backgroundColor); // Toggle background color
                    *currentState = (*currentState == MENU) ? PARAMETRE : MENU; // Toggle state
                }
                break;
            default:
                break;
        }
    }
}