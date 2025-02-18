#include "../include/text.h"
void updateTextPosition(Text *text, float scaleX, float scaleY) {
    if (text && text->texture) {
        text->rect.x = text->initialRect.x * scaleX;
        text->rect.y = text->initialRect.y * scaleY;
        text->rect.w = text->initialRect.w * scaleX;
        text->rect.h = text->initialRect.h * scaleY;
    }
}



void destroyText(Text * text){
    if (text) {
        if (text->texture) {
            SDL_DestroyTexture(text->texture);
            text->texture = NULL;
        }
        if (text->surface) {
            SDL_FreeSurface(text->surface);
            text->surface = NULL;
        }
    }
}

void initText(Window *win) {
    const char * const texts[] = {"Lancement de la Nouvelle Partie...", "ICPocket"};
    Text *textObjects[] = {&NewGameText, &title};
    SDL_Rect rects[] = {
        {win->width / 2 - 250, win->height / 2 + 250, 500, 100},
        {win->width / 2 - 250, -25, 500, 170}
    };

    for (int i = 0; i < 2; i++) {
        *textObjects[i] = (Text){texts[i], rects[i], rects[i], {255, 255, 255, 255}, win->LargeFont, NULL, NULL};
        SDL_Surface *textSurface = TTF_RenderText_Solid(win->LargeFont, texts[i], textObjects[i]->color);
        if (!textSurface) {
            SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_ERROR, "❌ Erreur de rendu du texte : %s", TTF_GetError());
            exit(EXIT_FAILURE);
        }
        textObjects[i]->texture = SDL_CreateTextureFromSurface(win->renderer, textSurface);
        if (!textObjects[i]->texture) {
            SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_ERROR, "❌ Erreur de création de la texture : %s", SDL_GetError());
            SDL_FreeSurface(textSurface);
            exit(EXIT_FAILURE);
        }
        textObjects[i]->surface = textSurface;
    }
}

Text *newText(SDL_Renderer *renderer, const char *text, TTF_Font *font, int x, int y, int w, int h){
    Text *newText = malloc(sizeof(Text));
    if (!newText) {
        SDL_Log("❌ Erreur lors de l'allocation de la structure Text.");
        return NULL;
    }
    newText->renderer = renderer;
    newText->initialRect = (SDL_Rect){x, y, w, h};
    newText->rect = newText->initialRect;
    newText->color = (SDL_Color){255, 255, 255, 255};
    newText->font = font;
    newText->surface = TTF_RenderText_Solid(font, text, newText->color);
    if (!newText->surface) {
        SDL_Log("❌ Erreur lors du rendu du texte : %s", TTF_GetError());
        free(newText);
        return NULL;
    }
    newText->texture = SDL_CreateTextureFromSurface(renderer, newText->surface);
    if (!newText->texture) {
        SDL_Log("❌ Erreur lors de la création de la texture : %s", SDL_GetError());
        SDL_FreeSurface(newText->surface);
        free(newText);
        return NULL;
    }
    return newText;
}