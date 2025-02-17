#include "../include/intermediate.h"

void handleIntermediateEvent(Window *win, SDL_Event *event) {
    if (!win || !event) return;
    if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[7].buttons->size; i++) {
            ButtonClicked(game.ui[7].buttons->buttons[i], x, y, win);
        }
    }
    if(!game.saved){

    }
    handleEvent(win, event);
}