#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "boutons.h"
#include "inputs.h"
#include "menu.h"
#include "state.h"
#include "utils.h"

void mainLoop(SDL_Window* window, SDL_Surface* image, int* backgroundColor, State* currentState);

Bouton param = {50, 450, 200, 100}; // x ,y largeur, hauteur

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
    
    if (window == NULL)quitSDL(0,window);
    
    mainLoop(window, image, &backgroundColor, &currentState);

    quitSDL(0,window);

    return 0;
}

void mainLoop(SDL_Window* window, SDL_Surface* image, int* backgroundColor, State* currentState) {
    SDL_Surface* menu = NULL;

    while (1) {
        menu = SDL_GetWindowSurface(window);
        if (*backgroundColor == 0) {
            SDL_FillRect(menu, NULL, SDL_MapRGB(menu->format, 0, 0, 0));
        } else {
            SDL_FillRect(menu, NULL, SDL_MapRGB(menu->format, 255, 255, 255));
        }
        if (*currentState == MENU) {
            drawMenu(menu, image);
        } else if (*currentState == PARAMETRE) {
            drawParametre(menu);
        }
        SDL_UpdateWindowSurface(window);
        handleInputs(window, backgroundColor, currentState);
    }
}
