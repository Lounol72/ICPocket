#include "include/menu.h"
#include <stdio.h>

extern Bouton retourMenu;
extern Bouton pageParam;
extern Bouton jouer;
extern Bouton TEST;
extern State* currentState;

void initFenetre(Fenetre *f, Image *images, Bouton *boutons, int nbBoutons, int nbImages, State *state, TTF_Font* font, void (*renderFenetre)(Fenetre*), void (*handleInputsFenetre)(Fenetre*, SDL_Event), void(*destroyFenetre)(Fenetre*)) {
    for(int i = 0; i < nbImages; i++) f->images[i] = &images[i];
    for(int i = 0; i < nbBoutons; i++) f->boutons[i] = boutons[i];
    f->nbBoutons = nbBoutons;
    f->nbImages = nbImages;
    f->state = state;
    f->font = font;
    f->render = renderFenetre;
    f->handleInputs = handleInputsFenetre;
    f->destroy = destroyFenetre;
}

void renderFenetre(Fenetre *f) {
    for (int i = 0; i < f->nbImages; i++) {
        SDL_BlitSurface(f->images[i]->image, NULL, SDL_GetWindowSurface(SDL_GetWindowFromID(f->images[i]->position.x)), &f->images[i]->initialPosition); // Correcting argument types
    }
    for(int i = 0; i < f->nbBoutons; i++) {
        drawButton(f->boutons[i].surface, &f->boutons[i], f->font);
    }
}

void handleInputsFenetre(Fenetre *f, SDL_Event event) {
    for(int i = 0; i < f->nbBoutons; i++) {
        if (Cliqued(&f->boutons[i], event.button.x, event.button.y)) {
            f->boutons[i].action(f->boutons[i].actionParam);
        }
    }
}

void destroyFenetre(Fenetre *f) {
    for (int i = 0; i< f->nbImages; i++) {
        SDL_FreeSurface(f->images[i]->image);
    }
    TTF_CloseFont(f->font);
    free(f->images);
    free(f->boutons);
    free(f);
}


void drawMenu(SDL_Surface* surface, SDL_Surface* image) {
    SDL_Rect positionImage = {0, 0, 0, 0};
    SDL_BlitSurface(image, NULL, surface, &positionImage);
    TTF_Font* font = initializeFont("assets/fonts/arial.ttf", 24);
    if (!font) {
        return;
    }
    drawButtonImage(surface, &pageParam); //passing surface to drawButton
    drawButton(surface, &jouer, font); //passing surface to drawButton
    drawButton(surface, &TEST, font); //passing surface to drawButton
    TTF_CloseFont(font);
}


// Draw a red highlight around a rectangle FOR DEBUGGING PURPOSES
void drawHighlight(SDL_Surface* surface, Bouton * b) {
    if (b != NULL) { //check if b is not NULL
        SDL_FillRect(surface, &b->rect, SDL_MapRGB(surface->format, b->color.r,b->color.g,b->color.b)); //using surface and correct color
    }
}

void changeState(void* targetState) {
    if (targetState == NULL) {
        SDL_Log("Invalid state transition: targetState is NULL");
        return;
    }

    State* newState = (State*)targetState;
    if (*newState != MENU && *newState != GAME && *newState != SETTINGS) {
        SDL_Log("Invalid state transition: unknown targetState");
        return;
    }

    currentState = newState;
}

void drawGame(SDL_Surface* surface) {
    TTF_Font* font = initializeFont("assets/fonts/arial.ttf", 24);
    if (!font) {
        return;
    }
    drawButton(surface, &retourMenu, font); //passing surface to drawButton
    TTF_CloseFont(font);
}
