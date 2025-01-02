#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "boutons.h"

typedef enum {MENU, PARAMETRE} State;

void quitSDL(int codeError, SDL_Window* window);
void handleInputs(SDL_Window* window, int* backgroundColor, State* currentState);
void getMousePosition(SDL_Window* window);
void drawHighlight(SDL_Surface* surface, int x, int y, int width, int height);

Bouton param = {100, 100, 100, 100}; // Top-left corner at (100, 100), width and height of 100

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Surface* menu = NULL;
    SDL_Surface* parametre = NULL;
    
    int backgroundColor = 0; // 0 for black, 1 for white
    State currentState = MENU;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) quitSDL(1,NULL);

    window = SDL_CreateWindow("ICPocket", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 720, 600, SDL_WINDOW_SHOWN);

    SDL_Surface* image = IMG_Load("assets/MENU_TEST.png");
    if(!image)
    {
        printf("Erreur de chargement de l'image : %s",SDL_GetError());
        return -1;
    }
    SDL_Rect positionImage;
    positionImage.x = 0;
    positionImage.y = 0;
    if (window == NULL)quitSDL(0,window);
    while(1){
        menu = SDL_GetWindowSurface(window);
        if (backgroundColor == 0) {
            SDL_FillRect(menu, NULL, SDL_MapRGB(menu->format, 0, 0, 0));
        } else {
            SDL_FillRect(menu, NULL, SDL_MapRGB(menu->format, 255, 255, 255));
        }
        if (currentState == MENU) {
            SDL_BlitSurface(image, NULL, menu, &positionImage);
            drawHighlight(menu, param.x, param.y, param.width, param.height); // Dessiner le rectangle rouge
        } else if (currentState == PARAMETRE) {
            // Afficher les paramètres ici
            SDL_FillRect(menu, NULL, SDL_MapRGB(menu->format, 100, 100, 100)); // Exemple de fond pour les paramètres
            drawHighlight(menu, param.x, param.y, param.width, param.height); // Dessiner le rectangle rouge
        }
        SDL_UpdateWindowSurface(window);
        handleInputs(window, &backgroundColor, &currentState);
    }
    quitSDL(0,window);

    return 0;
}

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
                if (Cliqued(&param, x, y)) {
                    *backgroundColor = !(*backgroundColor); // Toggle background color
                    *currentState = (*currentState == MENU) ? PARAMETRE : MENU; // Toggle state
                }
                break;
            default:
                break;
        }
    }
}

void quitSDL(int codeError, SDL_Window* window) {
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
    exit(codeError);
}

void getMousePosition(SDL_Window* window) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    printf("Mouse position: x=%d y=%d\n", x, y);
}

void drawHighlight(SDL_Surface* surface, int x, int y, int width, int height) {
    SDL_Rect rect = { x, y, width, height };
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 255, 0, 0)); // Red highlight
}