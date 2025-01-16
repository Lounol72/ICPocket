#include <stdio.h>
#include "include/menu.h"
#include "include/state.h"
#include "include/utils.h"
#include "include/boutons.h"    
extern State* currentState;
extern Bouton pageParam;
extern Bouton jouer;
extern Bouton TEST;

extern Bouton retourMenu;
void drawMenu(SDL_Surface* surface, SDL_Surface* image) {
    SDL_Rect positionImage = {0, 0, 0, 0};
    SDL_BlitSurface(image, NULL, surface, &positionImage);
    TTF_Font* font = initializeFont("assets/fonts/arial.ttf", 24);
    if (!font) {
        return;
    }
    drawButton(surface, &pageParam, font); // Fixed: passing surface to drawButton
    drawButton(surface, &jouer, font); // Fixed: passing surface to drawButton
    drawButton(surface, &TEST, font); // Fixed: passing surface to drawButton
    
}


// Draw a red highlight around a rectangle FOR DEBUGGING PURPOSES
void drawHighlight(SDL_Surface* surface, Bouton * b) {
    if (b != NULL) { // Fixed: check if b is not NULL
        SDL_FillRect(surface, &b->rect, SDL_MapRGB(surface->format, b->color.r,b->color.g,b->color.b)); // Fixed: using surface and correct color
    }
}

void changeState(void* targetState) {
    currentState = (State*)targetState;
}

void drawJeux(SDL_Surface* surface) {
    TTF_Font* font = initializeFont("assets/fonts/arial.ttf", 24);
    if (!font) {
        return;
    }
    drawButton(surface, &retourMenu, font); // Fixed: passing surface to drawButton
}



