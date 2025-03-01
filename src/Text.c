#include "../include/Text.h"
#include "../include/Window.h"

static void StayScaled(TTF_Font *font, Text *text, SDL_Rect *rect, SDL_Rect *initialRect) {
    if (!text || !font || !rect || !initialRect) return;

    int textWidth, textHeight;
    if (TTF_SizeText(font, text->text, &textWidth, &textHeight) != 0) return;

    float scaleW = (float)(initialRect->w ) / (float)textWidth;
    float scaleH = (float)(initialRect->h) / (float)textHeight;
    float scale = (scaleW < scaleH) ? scaleW : scaleH;

    int scaledWidth = (int)(textWidth * scale);
    int scaledHeight = (int)(textHeight * scale);

    // Centrer le texte dans le rectangle initial
    rect->x = initialRect->x + (initialRect->w - scaledWidth) / 2;
    rect->y = initialRect->y + (initialRect->h - scaledHeight) / 2;
    rect->w = scaledWidth;
    rect->h = scaledHeight;
    return;
}
void initText(struct Window *win) {
    
    const char * const texts[] = {"Lancement de la Nouvelle Partie...", "ICPocket"};
    Text *textObjects[] = {&NewGameText, &title};
    SDL_Rect rects[] = {
        {win->width / 2 - 250, win->height / 2 + 250, 500, 100},
        {win->width / 2 - 250, 0, 500, 170}
    };

    for (int i = 0; i < 2; i++) {
        *textObjects[i] = (Text){texts[i], rects[i], rects[i], {255, 255, 255, 255}, win->LargeFont, NULL, NULL, 0};
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
    
    StayScaled(win->LargeFont, &NewGameText, &NewGameText.rect, &NewGameText.initialRect);
    StayScaled(win->LargeFont, &title, &title.rect, &title.initialRect);


}

Text *createText(const char *text,SDL_Renderer *renderer,SDL_Rect rect, SDL_Color color, TTF_Font *font){
    Text *newText = malloc(sizeof(Text));
    if (!newText) {
        SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_ERROR, "❌ Erreur d'allocation de mémoire pour le texte.");
        return NULL;
    }
    newText->text = text;
    newText->rect = rect;
    newText->initialRect = rect;
    newText->color = color;
    newText->font = font;
    newText->surface = TTF_RenderText_Solid(font, text, color);
    if (!newText->surface) {
        SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_ERROR, "❌ Erreur de rendu du texte : %s", TTF_GetError());
        free(newText);
        return NULL;
    }
    newText->texture = SDL_CreateTextureFromSurface(renderer, newText->surface);
    if (!newText->texture) {
        SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_ERROR, "❌ Erreur de création de la texture : %s", SDL_GetError());
        SDL_FreeSurface(newText->surface);
        free(newText);
        return NULL;
    }
    newText->is_dynamic = 1;
    return newText;
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
        if (text->is_dynamic) {
            free(text);
        }
    }
}

void renderText(struct Window * win, Text * text){
    if(!text || !text->texture) return;
    SDL_RenderCopy(win->renderer, text->texture, NULL, &text->rect);
}

void updateText(Text *text, const char *newText, SDL_Renderer *renderer) {
    if (text && newText) {
        text->text = newText;
        SDL_Surface *newSurface = TTF_RenderText_Solid(text->font, newText, text->color);
        if (!newSurface) {
            SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_ERROR, "❌ Erreur de rendu du texte : %s", TTF_GetError());
            return;
        }
        SDL_DestroyTexture(text->texture);
        text->texture = SDL_CreateTextureFromSurface(renderer, newSurface);
        if (!text->texture) {
            SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_ERROR, "❌ Erreur de création de la texture : %s", SDL_GetError());
            SDL_FreeSurface(newSurface);
            return;
        }
        SDL_FreeSurface(text->surface);
        text->surface = newSurface;
    }
}


void updateTextPosition(Text *text, float scaleX, float scaleY) {
    if (text && text->texture) {
        text->rect.x = text->initialRect.x * scaleX;
        text->rect.y = text->initialRect.y * scaleY;
        text->rect.w = text->initialRect.w * scaleX;
        text->rect.h = text->initialRect.h * scaleY;
    }
}

void changeTextSpeed(struct Window *win, void *data) {
    float *speed = (float *)data;
    win->textSpeed = *speed;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "🚀 Vitesse du texte changée à %.2f", *speed);
}
