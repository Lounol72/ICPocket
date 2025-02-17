#include "../include/menu.h"

void render(Window *win) {
    if (game.gameState.currentState == GAME){
        SDL_Rect renderQuad = {0, 0, win->width, (int)(win->height * 0.722)};
        SDL_RenderCopy(win->renderer, game.ui[3].background, NULL, &renderQuad);
    }
    else if (game.ui[game.gameState.currentState].background) SDL_RenderCopy(win->renderer, game.ui[game.gameState.currentState].background, NULL, NULL);
    if (game.ui[game.gameState.currentState].buttons )renderButtonList(game.ui[game.gameState.currentState].buttons);
    if (game.ui[game.gameState.currentState].sliders)renderSliderList(game.ui[game.gameState.currentState].sliders);
    if(game.gameState.currentState == MENU) {
        renderText(win, &title);
    }else if(game.gameState.currentState == NEWGAME){
        renderText(win, &NewGameText);
    }
}

void handleMenuEvent(Window *win, SDL_Event *event) {
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[2].buttons->size; i++) {
            ButtonClicked(game.ui[2].buttons->buttons[i], x, y, win);
        }
    }
    handleEvent(win, event);
}