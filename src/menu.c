#include "include/menu.h"
#include "include/state.h"

extern State* currentState;
extern Bouton pageParam;
extern Bouton jouer;
extern Bouton TEST;

extern Bouton retourMenu;
void drawMenu(SDL_Surface* surface, SDL_Surface* image) {
    SDL_Rect positionImage = {0, 0, 0, 0};
    SDL_BlitSurface(image, NULL, surface, &positionImage);
    drawHighlight(surface, &pageParam); // Fixed: passing surface to drawHighlight
    drawHighlight(surface, &jouer);
    drawHighlight(surface, &TEST);
    
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

void drawJeux(SDL_Surface* surface, SDL_Surface* image) {
    SDL_Rect positionImage = {0, 0, 0, 0};
    SDL_BlitSurface(image, NULL, surface, &positionImage);
    drawHighlight(surface, &retourMenu); // Fixed: passing surface to drawHighlight
}

