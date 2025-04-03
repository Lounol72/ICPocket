#include "../include/Text.h"
#include "../include/Window.h"
#include <string.h>
#include <stdlib.h>

/**
 * @fn void StayScaled(TTF_Font *font, Text *text, SDL_Rect *rect, SDL_Rect *initialRect)
 * @brief Met à jour la taille d'un objet texte en fonction de la mise à l'échelle.
 * 
 * Cette fonction met à jour la taille de l'objet texte spécifié en fonction de la mise à l'échelle.
 * 
 * @param font La police utilisée pour rendre le texte.
 * @param text Un pointeur vers l'objet Text.
 * @param rect Le rectangle définissant la position et la taille du texte.
 * @param initialRect Le rectangle initial pour les besoins de mise à l'échelle.
 */
void StayScaled(TTF_Font *font, Text *text, SDL_Rect *rect, SDL_Rect *initialRect) {
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

/**
 * @fn void initText(Window *win)
 * @brief Initialise les objets texte.
 * 
 * Cette fonction initialise les objets texte utilisés dans l'application. C'est une fonction temporaire.
 * 
 * @param win Un pointeur vers la structure Window.
 */
void initText(struct Window *win) {
    const char * const texts[] = {"Lancement de la Nouvelle Partie...", "ICPocket"};
    Text *textObjects[] = {&NewGameText, &title};
    SDL_Rect rects[] = {
        {win->width / 2 - 250, win->height / 2 + 250, 500, 100},
        {win->width / 2 - 250, 0, 500, 170}
    };

    for (int i = 0; i < 2; i++) {
        // Initialiser la structure
        *textObjects[i] = (Text){
            .text = texts[i],
            .rect = rects[i],
            .initialRect = rects[i],
            .color = {255, 255, 255, 255},
            .font = win->LargeFont,
            .texture = NULL,
            .is_dynamic = 0
        };

        // Créer la texture
        SDL_Surface *tempSurface = TTF_RenderText_Solid(win->LargeFont, texts[i], textObjects[i]->color);
        if (!tempSurface) {
            SDL_LogError(SDL_LOG_CATEGORY_RENDER, "❌ Erreur de rendu du texte : %s", TTF_GetError());
            continue;
        }

        textObjects[i]->texture = SDL_CreateTextureFromSurface(win->renderer, tempSurface);
        SDL_FreeSurface(tempSurface);  // Libérer la surface immédiatement

        if (!textObjects[i]->texture) {
            SDL_LogError(SDL_LOG_CATEGORY_RENDER, "❌ Erreur de création de la texture : %s", SDL_GetError());
            continue;
        }

        StayScaled(win->LargeFont, textObjects[i], &textObjects[i]->rect, &textObjects[i]->initialRect);
    }
}

/**
 * @fn Text *createText(const char *text, SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color, TTF_Font *font)
 * @brief Crée un nouvel objet texte.
 * 
 * Cette fonction crée un nouvel objet texte avec les paramètres spécifiés.
 * 
 * @param text Le contenu textuel.
 * @param renderer Le renderer SDL.
 * @param rect Le rectangle définissant la position et la taille du texte.
 * @param color La couleur du texte.
 * @param font La police utilisée pour rendre le texte.
 * @return Un pointeur vers l'objet Text créé.
 */
Text *createText(const char *text, SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color, TTF_Font *font) {
    if (!text || !renderer || !font) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "❌ Paramètres invalides pour createText");
        return NULL;
    }

    Text *newText = calloc(1, sizeof(Text));
    if (!newText) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "❌ Erreur d'allocation de mémoire pour le texte");
        return NULL;
    }

    // Initialiser tous les champs à NULL/0 d'abord
    newText->text = text;
    newText->rect = rect;
    newText->initialRect = rect;
    newText->color = color;
    newText->font = font;
    newText->is_dynamic = 1;
    newText->surface = NULL;
    newText->texture = NULL;

    // Créer la surface
    //SDL_Surface *tempSurface = TTF_RenderText_Solid(font, text, color); a feur
    SDL_Surface* tempSurface = TTF_RenderUTF8_Blended_Wrapped(
        newText->font,
        newText->text,
        newText->color,
        newText->rect.w
    );
    if (!tempSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "❌ Erreur de rendu du texte : %s", TTF_GetError());
        free(newText);
        return NULL;
    }

    // Créer la texture à partir de la surface temporaire
    SDL_Texture *tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);  // Libérer la surface immédiatement après création de la texture

    if (!tempTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "❌ Erreur de création de la texture : %s", SDL_GetError());
        free(newText);
        return NULL;
    }

    newText->texture = tempTexture;
    StayScaled(font, newText, &rect, &newText->initialRect);
    return newText;
}

/**
 * @fn void destroyText(Text *text)
 * @brief Détruit un objet texte.
 * 
 * Cette fonction libère les ressources associées à l'objet texte spécifié.
 * 
 * @param text Un pointeur vers l'objet Text à détruire.
 */
void destroyText(Text *text) {
    if (!text) return;

    // Libérer la texture SDL
        if (text->texture != NULL) {
            SDL_DestroyTexture(text->texture);
            text->texture = NULL;
        }

    // Ne pas libérer le font car il est géré ailleurs
    text->font = NULL;
    text->text = NULL;  // Ne pas libérer text->text car c'est une chaîne constante

    // Libérer la structure si elle est dynamique
        if (text->is_dynamic) {
            free(text);
            text = NULL;
    }
}

/**
 * @fn void cleanupText()
 * @brief Nettoie les objets texte.
 * 
 * Cette fonction nettoie les objets texte utilisés dans l'application.
 */
// Assurez-vous que cette fonction est appelée dans votre cleanup
void cleanupText() {
    // Nettoyer NewGameText
    if (NewGameText.texture) {
        SDL_DestroyTexture(NewGameText.texture);
        NewGameText.texture = NULL;
    }

    // Nettoyer title
    if (title.texture) {
        SDL_DestroyTexture(title.texture);
        title.texture = NULL;
    }
}

/**
 * @fn void renderText(Window *win, Text *text)
 * @brief Rend un objet texte.
 * 
 * Cette fonction rend l'objet texte spécifié à l'écran.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param text Un pointeur vers l'objet Text à rendre.
 */
void renderText(struct Window * win, Text * text){
    if(!text || !text->texture) return;
    SDL_RenderCopy(win->renderer, text->texture, NULL, &text->rect);
}

/**
 * @fn void updateText(Text *text, const char *newText, SDL_Renderer *renderer)
 * @brief Met à jour le contenu d'un objet texte.
 * 
 * Cette fonction met à jour le contenu textuel de l'objet texte spécifié et le rend à nouveau.
 * 
 * @param text Un pointeur vers l'objet Text.
 * @param newText Le nouveau contenu textuel.
 * @param renderer Le renderer SDL.
 */
void updateText(Text *text, const char *newText, SDL_Renderer *renderer) {
    if (!text || !newText || !renderer) return;

        text->text = newText;

    // Libérer l'ancienne texture d'abord
    if (text->texture) {
        SDL_DestroyTexture(text->texture);
        text->texture = NULL;
    }

    // Créer une nouvelle surface temporaire
    SDL_Surface *tempSurface = TTF_RenderText_Solid(text->font, newText, text->color);
    if (!tempSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "❌ Erreur de rendu du texte : %s", TTF_GetError());
            return;
        }

    // Créer la nouvelle texture
    text->texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);  // Libérer la surface immédiatement

    if (!text->texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "❌ Erreur de création de la texture : %s", SDL_GetError());
    }
}

/**
 * @fn void updateTextPosition(Text *text, float scaleX, float scaleY)
 * @brief Met à jour la position d'un objet texte.
 * 
 * Cette fonction met à jour la position de l'objet texte spécifié en fonction des facteurs de mise à l'échelle donnés.
 * 
 * @param text Un pointeur vers l'objet Text.
 * @param scaleX Le facteur de mise à l'échelle horizontal.
 * @param scaleY Le facteur de mise à l'échelle vertical.
 */
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

/**
 * @fn ScrollingText* createScrollingText(char* text, TTF_Font* font, SDL_Color color, int charDelay, SDL_Rect backgroundPosition, const char* backgroundPath, SDL_Renderer* renderer)
 * @brief Crée un nouvel objet de texte défilant.
 * 
 * Cette fonction crée un nouvel objet de texte défilant avec les paramètres spécifiés.
 * 
 * @param text Le contenu textuel.
 * @param font La police utilisée pour rendre le texte.
 * @param color La couleur du texte.
 * @param x La position horizontale du texte.
 * @param y La position verticale du texte.
 * @param charDelay Le délai entre chaque caractère (en ms).
 * @return Un pointeur vers l'objet ScrollingText créé.
 */
ScrollingText* createScrollingText(char* text, TTF_Font* font, SDL_Color color, int charDelay, SDL_Rect backgroundPosition, const char* backgroundPath, SDL_Renderer* renderer) {
    if (!text || !font || !renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid parameters for createScrollingText");
        return NULL;
    }

    ScrollingText* scrollText = calloc(1, sizeof(ScrollingText));
    if (!scrollText) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to allocate ScrollingText");
        return NULL;
    }

    // Copier le texte
    size_t textLen = strlen(text) + 1;
    scrollText->fullText = malloc(textLen);
    if (!scrollText->fullText) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to allocate fullText");
        free(scrollText);
        return NULL;
    }
    strncpy(scrollText->fullText, text, textLen);

    // Allouer le texte courant
    scrollText->currentText = malloc(textLen);
    if (!scrollText->currentText) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to allocate currentText");
        free(scrollText->fullText);
        free(scrollText);
        return NULL;
    }
    scrollText->currentText[0] = '\0';

    // Initialiser les autres champs
    scrollText->fullLength = textLen - 1;
    scrollText->currentLength = 0;
    scrollText->isComplete = false;
    scrollText->lastCharTime = SDL_GetTicks();
    scrollText->charDelay = (Uint32)charDelay;
    scrollText->font = font;
    scrollText->color = color;
    scrollText->texture = NULL;

    // Calculate text position relative to background
    
    scrollText->backgroundPosition = backgroundPosition;
    
    // Calculer les marges en pourcentage de la taille du background
    int marginX = (int)(backgroundPosition.w * 0.05);  // 5% de la largeur
    int marginY = (int)(backgroundPosition.h * 0.15);  // 15% de la hauteur
    
    scrollText->position = (SDL_Rect){
        backgroundPosition.x + marginX,                    // marge gauche
        backgroundPosition.y + marginY,                    // marge haute
        backgroundPosition.w - (marginX * 2),             // largeur moins les marges
        backgroundPosition.h - (marginY * 2)              // hauteur moins les marges
    };
    scrollText->width = scrollText->position.w;
    scrollText->initialPosition = scrollText->position;
    scrollText->initialBackgroundPosition = backgroundPosition;

    // Load background
    SDL_Surface* surface = IMG_Load(backgroundPath);
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load background: %s", IMG_GetError());
        return NULL;
    }
    scrollText->background = SDL_CreateTextureFromSurface(renderer, surface);
    if (!scrollText->background) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create background texture: %s", SDL_GetError());
        SDL_FreeSurface(surface);
        return NULL;
    }
    SDL_FreeSurface(surface);

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "ScrollingText created successfully");
    return scrollText;
}

/**
 * @fn void updateScrollingTextPosition(ScrollingText* text, float scaleX, float scaleY)
 * @brief Met à jour la position d'un objet de texte défilant.
 * 
 * Cette fonction met à jour la position de l'objet de texte défilant spécifié en fonction des facteurs de mise à l'échelle donnés.
 * 
 * @param text Un pointeur vers l'objet ScrollingText.
 * @param scaleX Le facteur de mise à l'échelle horizontal.
 * @param scaleY Le facteur de mise à l'échelle vertical. 
 */
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

/**
 * @fn void updateScrollingText(ScrollingText* text, SDL_Renderer* renderer)
 * @brief Met à jour un objet de texte défilant.
 * 
 * Cette fonction met à jour un objet de texte défilant avec les paramètres spécifiés.
 * 
 * @param text Un pointeur vers l'objet ScrollingText.
 * @param renderer Le renderer SDL.
 */
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
            
            SDL_Surface* surface = TTF_RenderUTF8_Blended_Wrapped(
                text->font,
                text->currentText,
                text->color,
                text->width
            );
            
            if (surface) {
                text->texture = SDL_CreateTextureFromSurface(renderer, surface);
                if (!text->texture) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create texture: %s", SDL_GetError());
                }
                text->position.w = surface->w;
                text->position.h = surface->h;
                SDL_FreeSurface(surface);
            } else {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to render text: %s", TTF_GetError());
            }
        } else {
            text->isComplete = true;
        }
    }
}

/**
 * @fn void renderScrollingText(ScrollingText* text, SDL_Renderer* renderer)
 * @brief Rend un objet de texte défilant.
 * 
 * Cette fonction rend un objet de texte défilant avec les paramètres spécifiés.
 * 
 * @param text Un pointeur vers l'objet ScrollingText.
 * @param renderer Le renderer SDL.
 */
void renderScrollingText(ScrollingText* text, SDL_Renderer* renderer) {
    if (!text || !renderer) return;
    // Render background first
    if (text->background) {
        SDL_RenderCopy(renderer, text->background, NULL, &text->backgroundPosition);
    }

    // Render text if we have content
    if (text->texture && text->currentLength > 0) {
        SDL_RenderCopy(renderer, text->texture, NULL, &text->position);
    }
}

/**
 * @fn void skipScrollingText(ScrollingText* text, SDL_Renderer* renderer)
 * @brief Passe à la suite un objet de texte défilant.
 * 
 * Cette fonction passe à la suite un objet de texte défilant avec les paramètres spécifiés.
 * 
 * @param text Un pointeur vers l'objet ScrollingText.
 * @param renderer Le renderer SDL.
 */
void skipScrollingText(ScrollingText* text, SDL_Renderer* renderer) {
    if (!text || !text->isValid || text->isComplete) return;

    // Copier tout le texte d'un coup
    strncpy(text->currentText, text->fullText, text->fullLength);
    text->currentText[text->fullLength] = '\0';
    text->currentLength = text->fullLength;

    // Libérer l'ancienne texture si elle existe
    if (text->texture) {
        SDL_DestroyTexture(text->texture);
        text->texture = NULL;
    }

    // Créer la nouvelle texture pour le texte complet
    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(
        text->font,
        text->currentText,
        text->color,
        text->width
    );
    
    if (surface) {
        text->texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (text->texture) {
            text->position.w = surface->w;
            text->position.h = surface->h;
        }
        SDL_FreeSurface(surface);
    }

    text->isComplete = true;
}

/**
 * @fn void resetScrollingText(ScrollingText* text, const char* newText)
 * @brief Réinitialise un objet de texte défilant.
 * 
 * Cette fonction réinitialise un objet de texte défilant avec les paramètres spécifiés.
 * 
 * @param text Un pointeur vers l'objet ScrollingText.
 * @param newText Le nouveau texte à afficher.
 */
void resetScrollingText(ScrollingText* text, const char* newText) {
    if (!text || !newText) return;

    // Sauvegarder les anciens pointeurs
    char* oldFullText = text->fullText;
    char* oldCurrentText = text->currentText;
    SDL_Texture* oldTexture = text->texture;

    // Allouer les nouveaux buffers
    text->fullText = strdup(newText);
    if (!text->fullText) {
        text->fullText = oldFullText;  // Restaurer l'ancien en cas d'échec
        return;
    }

    text->currentText = malloc(strlen(newText) + 1);
    if (!text->currentText) {
        free(text->fullText);
        text->fullText = oldFullText;
        text->currentText = oldCurrentText;
        return;
    }

    // Si tout s'est bien passé, libérer les anciens
    if (oldFullText) {
        free(oldFullText);
        oldFullText = NULL;
    }
    if (oldCurrentText) {
        free(oldCurrentText);
        oldCurrentText = NULL;
    }
    if (oldTexture) {
        SDL_DestroyTexture(oldTexture);
        oldTexture = NULL;
    }

    text->texture = NULL;
    text->currentText[0] = '\0';
    text->fullLength = strlen(newText);
    text->currentLength = 0;
    text->isComplete = false;
    text->lastCharTime = SDL_GetTicks();
}

/**
 * @fn void destroyScrollingText(ScrollingText* text)
 * @brief Détruit un objet de texte défilant.
 * 
 * Cette fonction détruit un objet de texte défilant avec les paramètres spécifiés.
 * 
 * @param text Un pointeur vers l'objet ScrollingText.
 */
void destroyScrollingText(ScrollingText* text) {
    if (!text) return;

    // Libérer les textures SDL
    if (text->texture) {
        SDL_DestroyTexture(text->texture);
        text->texture = NULL;
    }
    
    if (text->background) {
        SDL_DestroyTexture(text->background);
        text->background = NULL;
    }

    // Libérer les chaînes de caractères
    if (text->fullText) {
        free(text->fullText);
        text->fullText = NULL;
    }

    if (text->currentText) {
        free(text->currentText);
        text->currentText = NULL;
    }
    text->font = NULL;
    // Libérer la structure elle-même
    free(text);
    text = NULL;
}

/**
 * @fn void cleanupScrollingText(ScrollingText** text)
 * @brief Nettoie un objet de texte défilant.
 * 
 * Cette fonction nettoie un objet de texte défilant avec les paramètres spécifiés.
 * 
 * @param text Un pointeur vers l'objet ScrollingText.
 */
void cleanupScrollingText(ScrollingText** text) {
    if (!text || !*text) return;
    
    destroyScrollingText(*text);
    free(*text);
    *text = NULL;
}