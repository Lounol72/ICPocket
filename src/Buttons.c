#include "../include/Buttons.h"
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdlib.h>

Button *createButton(char *text, Window *win, int x, int y, int w, int h, SDL_Color color, SDL_Color textcolor, void (*onClick)(Window *, void *), void *data, TTF_Font *font) {
    Button *button = malloc(sizeof(Button));
    if (!button) {
        SDL_Log("Erreur d'allocation pour le bouton.");
        return NULL;
    }
    button->text = text;
    button->textcolor = textcolor;
    button->rect = (SDL_Rect){x, y, w, h};
    button->initialRect = button->rect;
    button->color = color;
    button->texture = NULL;
    button->renderer = win->renderer;
    button->onClick = onClick;
    button->data = data;
    button->font = font ? font : win->font;

    SDL_Surface *textSurface = TTF_RenderText_Solid(button->font, text, textcolor);
    if (!textSurface) {
        SDL_Log("Erreur lors de la création de la surface du texte : %s", TTF_GetError());
        free(button);
        return NULL;
    }
    button->textTexture = SDL_CreateTextureFromSurface(win->renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!button->textTexture) {
        SDL_Log("Erreur lors de la création de la texture du texte : %s", SDL_GetError());
        free(button);
        return NULL;
    }
    return button;
}


void destroyButton(Button *button) {
    if (!button) return;
    if (button->texture) SDL_DestroyTexture(button->texture);
    if (button->textTexture) SDL_DestroyTexture(button->textTexture);
    free(button);
    button = NULL;
}

void addListButton(ButtonList *list, Button **buttons, int count) {
    if (list->buttons) {
        for (int i = 0; i < list->size; i++) destroyButton(list->buttons[i]);
        free(list->buttons);
    }
    list->buttons = malloc(count * sizeof(Button *));
    if (!list->buttons) {
        SDL_Log("Erreur : allocation de mémoire pour les boutons échouée !");
        return;
    }
    for (int i = 0; i < count; i++) list->buttons[i] = buttons[i];
    list->size = count;
}


void destroyButtonList(ButtonList *list) {
    if (list->buttons) {
        for (int i = 0; i < list->size; i++) {
            destroyButton(list->buttons[i]);
        }
        free(list->buttons);
        list->buttons = NULL; 
    }
    list->size = 0;
}



void InitTextureButton(Button *button, SDL_Renderer *renderer, const char *imagePath) {
    if (!button || !renderer || !imagePath) return;
    SDL_Surface *surface = IMG_Load(imagePath);
    if (!surface) return;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) return;
    if (button->texture) SDL_DestroyTexture(button->texture);
    button->texture = texture;
}


//----------------------------------------------------------------------------------------

// Rendering functions


void renderButtonList(ButtonList *B)
{
    if (!B) return;
    for (int i = 0; i < B->size; i++)
        renderButton(B->buttons[i]);
}

void renderButton(Button *button) {
    if (!button) return;
    if (button->texture) SDL_RenderCopy(button->renderer, button->texture, NULL, &button->rect);
    else {
        SDL_SetRenderDrawColor(button->renderer, button->color.r, button->color.g, button->color.b, button->color.a);
        SDL_RenderFillRect(button->renderer, &button->rect);
    }
    if (button->textTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(button->textTexture, NULL, NULL, &textWidth, &textHeight);
        
        // Calculs corrects pour centrer horizontalement et verticalement
        SDL_Rect textRect = {
            button->rect.x + (button->rect.w - textWidth) / 2,  // Centrage horizontal
            button->rect.y + ((button->rect.h - textHeight) / 2)* 0.8, // Ajustement vertical (ajouter un décalage)
            textWidth,
            textHeight
        };
        
        SDL_RenderCopy(button->renderer, button->textTexture, NULL, &textRect);
    }
}



//--------------------------------------------------------------------------

// Button handling functions


void ButtonClicked(Button *button, int mouseX, int mouseY, Window *win) {
    if (button && SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &button->rect) && button->onClick) {
        button->onClick(win, button->data);
    }
}


void updateButtonPosition(ButtonList *buttons, float Scalex, float Scaley)
{
    for (int i = 0; i < buttons->size; i++)
    {
        if(!buttons->buttons[i]){
            SDL_Log("Invalid button at index %d", i);
            continue;
        }
        buttons->buttons[i]->rect.w = buttons->buttons[i]->initialRect.w * Scalex;
        buttons->buttons[i]->rect.h = buttons->buttons[i]->initialRect.h * Scaley;
        buttons->buttons[i]->rect.x = buttons->buttons[i]->initialRect.x * Scalex;
        buttons->buttons[i]->rect.y = buttons->buttons[i]->initialRect.y * Scaley;
    }
}

void setButtonText(Button *button, const char *text, SDL_Renderer *renderer) {
    if (!button || !text || !renderer) {
        SDL_Log("Erreur : Paramètre NULL dans setButtonText");
        return;
    }

    // Détruire l'ancienne texture texte si elle existe
    if (button->textTexture) {
        SDL_DestroyTexture(button->textTexture);
        button->textTexture = NULL;
    }

    // Générer une nouvelle texture texte
    SDL_Surface *textSurface = TTF_RenderText_Solid(button->font, text, button->textcolor);
    if (!textSurface) {
        SDL_Log("Erreur lors de la création de la surface du texte : %s", TTF_GetError());
        return;
    }

    button->textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    if (!button->textTexture) {
        SDL_Log("Erreur lors de la création de la texture du texte : %s", SDL_GetError());
    }
}

//-----------------------------------------------

// Slider functions

Slider *createSlider(SDL_Renderer *renderer, int x, int y, int w, int h, SDL_Color color, SDL_Color cursorColor) {
    Slider *slider = malloc(sizeof(Slider));
    if (!slider) {
        SDL_Log("Erreur d'allocation pour le slider.");
        return NULL;
    }
    slider->value = 0;
    slider->rect = (SDL_Rect){x, y, w, h};
    slider->initialBar = slider->rect;
    int cursorW = 10;
    slider->cursor = (SDL_Rect){slider->rect.x + (slider->value * cursorW) - (cursorW / 2), y-5, cursorW, h+12};
    slider->color = color;
    slider->cursorColor = cursorColor;
    slider->renderer = renderer;

    return slider;
}


void destroySlider(Slider *slider) {
    if (slider) {
        free(slider);
    }
}

void destroySliderList(SliderList *list) {
    if (list->sliders) {
        for (int i = 0; i < list->size; i++) {
            destroySlider(list->sliders[i]);
        }
        free(list->sliders);
        list->sliders = NULL;
    }
    list->size = 0;
}

void addListSlider(SliderList *S, Slider *sliders[], int size) {
    if (!S || !sliders) return;
    Slider **newSliders = (Slider **)realloc(S->sliders, (S->size + size) * sizeof(Slider *));
    if (!newSliders) return;
    S->sliders = newSliders;
    for (int i = 0; i < size; i++) S->sliders[S->size + i] = sliders[i];
    S->size += size;
}

void renderSliderList(SliderList *S) {
    if (!S) return;
    for (int i = 0; i < S->size; i++)
        renderSlider(S->sliders[i]);
}

void renderSlider(Slider *slider) {
    if (!slider) return;

    // Fonction utilitaire pour définir la couleur
    void setColor(SDL_Renderer *renderer, SDL_Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    }

    // Dessiner le slider
    setColor(slider->renderer, slider->color);
    SDL_RenderFillRect(slider->renderer, &slider->rect);

    // Dessiner le curseur
    setColor(slider->renderer, slider->cursorColor);
    
    SDL_RenderFillRect(slider->renderer, &slider->cursor);
}

int handleSliderEvent(Slider *slider, int x, int y) {
    if (!slider) return 0;
    if (SDL_PointInRect(&(SDL_Point){x, y}, &(slider->rect))) {
        slider->value = (float)(x - slider->rect.x) / slider->rect.w;
        slider->cursor.x = slider->rect.x + (slider->value * slider->rect.w) - (slider->cursor.w / 2);
        return 1; // Interaction détectée
    }
    return 0;
}