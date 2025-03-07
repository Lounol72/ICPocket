#include "../include/Text.h"
#include "../include/Window.h"
#include <string.h>
#include <stdlib.h>

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
            text = NULL;
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
        SDL_Rect rect = text->rect;
        rect.x = text->initialRect.x * scaleX;
        rect.y = text->initialRect.y * scaleY;
        rect.w = text->initialRect.w * scaleX;
        rect.h = text->initialRect.h * scaleY;
        StayScaled(text->font, text, &text->rect, &rect);
    }
}

void changeTextSpeed(struct Window *win, void *data) {
    float *speed = (float *)data;
    win->textSpeed = *speed;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "🚀 Vitesse du texte changée à %.2f", *speed);
}

ScrollingText* createScrollingText(char* text, TTF_Font* font, SDL_Color color, int charDelay, SDL_Rect backgroundPosition, const char* backgroundPath, SDL_Renderer* renderer) {
    ScrollingText* scrollText = malloc(sizeof(ScrollingText));
    if (!scrollText) return NULL;

    scrollText->fullText = strdup(text);
    if (!scrollText->fullText) {
        free(scrollText);
        return NULL;
    }

    scrollText->currentText = malloc(strlen(text) + 1);
    if (!scrollText->currentText) {
        free(scrollText->fullText);
        free(scrollText);
        return NULL;
    }

    scrollText->fullLength = strlen(text);
    scrollText->currentLength = 0;
    scrollText->isComplete = false;
    scrollText->lastCharTime = SDL_GetTicks();
    scrollText->charDelay = (Uint32)charDelay;
    scrollText->font = font;
    scrollText->color = color;
    scrollText->texture = NULL;

    // Stockage de la position du background
    scrollText->backgroundPosition = backgroundPosition;
    // Calcul de la zone de texte : on retire 5 pixels sur chaque côté du background
    scrollText->position = (SDL_Rect){ backgroundPosition.x + 20, backgroundPosition.y + 10, backgroundPosition.w - 20, backgroundPosition.h - 20 };
    scrollText->width = backgroundPosition.w - 20;
    SDL_Surface* surface = IMG_Load(backgroundPath);

    scrollText->background = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    scrollText->initialPosition = scrollText->position;
    scrollText->initialBackgroundPosition = scrollText->backgroundPosition;
    scrollText->currentText[0] = '\0';
    return scrollText;
}

void updateScrollingTextPosition(ScrollingText* text, float scaleX, float scaleY) {
    if (!text) return;
    text->position.x = text->initialPosition.x * scaleX;
    text->position.y = text->initialPosition.y * scaleY;
    text->position.w = text->initialPosition.w * scaleX;
    text->position.h = text->initialPosition.h * scaleY;
    text->backgroundPosition.x = text->initialBackgroundPosition.x * scaleX;
    text->backgroundPosition.y = text->initialBackgroundPosition.y * scaleY;
    text->backgroundPosition.w = text->initialBackgroundPosition.w * scaleX;
    text->backgroundPosition.h = text->initialBackgroundPosition.h * scaleY;
}

void updateScrollingText(ScrollingText* text, SDL_Renderer* renderer) {
    if (!text || !renderer || text->isComplete) return;

    Uint32 currentTime = SDL_GetTicks();
    
    if (currentTime - text->lastCharTime >= text->charDelay) {
        if (text->currentLength < text->fullLength) {
            text->currentText[text->currentLength] = text->fullText[text->currentLength];
            text->currentText[text->currentLength + 1] = '\0';
            text->currentLength++;
            text->lastCharTime = currentTime;

            if (text->texture) {
                SDL_DestroyTexture(text->texture);
                text->texture = NULL;
            }
            
            // Augmentons la largeur maximale pour éviter les retours à la ligne non désirés
            // Utilisons presque toute la largeur de la fenêtre
            SDL_Surface* surface = TTF_RenderUTF8_Blended_Wrapped(
                text->font,
                text->currentText,
                text->color,
                text->width  // Laissons une marge de 50 pixels de chaque côté
            );
            
            if (surface) {
                text->texture = SDL_CreateTextureFromSurface(renderer, surface);
                text->position.w = surface->w;
                text->position.h = surface->h;
                SDL_FreeSurface(surface);
            }
        } else {
            text->isComplete = true;
        }
    }
}

void renderScrollingText(ScrollingText* text, SDL_Renderer* renderer) {
    if (!text || !renderer || !text->texture) return;
    // On rend d'abord le background, puis le texte par-dessus
    SDL_RenderCopy(renderer, text->background, NULL, &text->backgroundPosition);
    SDL_RenderCopy(renderer, text->texture, NULL, &text->position);
}

void skipScrollingText(ScrollingText* text) {
    if (!text || text->isComplete) return;
    
    strncpy(text->currentText, text->fullText, text->fullLength);
    text->currentText[text->fullLength] = '\0';
    text->currentLength = text->fullLength;
    text->isComplete = true;
}
void resetScrollingText(ScrollingText* text, char* fullText) {
    if (!text) return;
    text->currentLength = 0;
    text->isComplete = false;
    // Libérer l'ancienne copie de fullText s'il y en a une
    text->fullText = strdup(fullText);
    if (!text->fullText) {
        // Gérer l'erreur de manière appropriée (logger, etc.)
        return;
    }
    if (text->currentText) {
        free(text->currentText);
    }
    text->currentText = malloc(strlen(fullText) + 1);
    if (!text->currentText) {
        free(text->fullText);
        text->fullText = NULL;
        return;
    }
    text->currentText[0] = '\0';
}


void destroyScrollingText(ScrollingText* text) {
    if (!text) return;
    
    if (text->texture) {
        SDL_DestroyTexture(text->texture);
        text->texture = NULL;
    }
    
    if (text->background) {
        SDL_DestroyTexture(text->background);
        text->background = NULL;
    }
    
    if (text->currentText) {
        free(text->currentText);
        text->currentText = NULL;
    }
    
    free(text);
}
