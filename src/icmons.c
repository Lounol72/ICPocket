#include "../include/icmons.h"

void uptadeICMonsSprite(t_Poke *poke, float scaleX, float scaleY) {
    if (poke) {
        poke->rect.x = poke->initialRect.x * scaleX;
        poke->rect.y = poke->initialRect.y * scaleY;
        poke->rect.w = poke->initialRect.w * scaleX;
        poke->rect.h = poke->initialRect.h * scaleY;
    }
}
void initICMonsSprite(SDL_Renderer *renderer, const char *imagePath, t_Poke *poke, int x, int y, int w, int h) {
    //temporaire
    (void)imagePath;
    SDL_Surface *surface = IMG_Load("assets/Monsters/New Versions/calamd.png");
    if (!surface) {
        SDL_Log("❌ Erreur lors du chargement de l'image : %s", IMG_GetError());
        return ;
    }
    poke->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = NULL;
    if(!poke->texture){
        SDL_Log("❌ Erreur lors de la création de la texture : %s", SDL_GetError());
        return ;
    }
    poke->rect = (SDL_Rect){x, y, w, h};
    poke->initialRect = poke->rect;
}
void renderICMonsSprite(Window *win, t_Poke *poke) {
    if (poke && poke->texture) {
        SDL_RenderCopy(win->renderer, poke->texture, NULL, &poke->rect);
    }
}
void destroyICMonsSprite(Window *win, t_Poke *poke) {
    (void)win;
    if ( poke->texture) {
        SDL_DestroyTexture(poke->texture);
        poke->texture = NULL;
    }
    free(poke);
    poke = NULL;
}

// Functions for the ICMons selection

void handleICMonsEvent(Window *win, SDL_Event *event) {
    if (!win || !event) return;
    if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[6].buttons->size; i++) {
            ButtonClicked(game.ui[6].buttons->buttons[i], x, y, win);
        }
    }
    handleEvent(win, event);
}