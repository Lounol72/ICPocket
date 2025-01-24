#include "include/Buttons.h"
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdlib.h>

Button *createButton(SDL_Renderer *renderer, int x, int y, int w, int h, SDL_Color color, void (*onClick)(Window *, void *), void *data) {
    Button *button = malloc(sizeof(Button));
    if (!button) {
        SDL_Log("Erreur d'allocation pour le bouton.");
        return NULL;
    }
    button->rect = (SDL_Rect){x, y, w, h};
    button->initialRect = button->rect;
    button->color = color;
    button->texture = NULL;
    button->renderer = renderer;
    button->onClick = onClick;
    button->data = data;

    return button;
}

void destroyButton(Button *button) {
    if (button) {
        if (button->texture) {
            SDL_DestroyTexture(button->texture);
        }
        free(button);
    }
}

void addListButton(ButtonList *list, Button **buttons, int count) {
    if (list->buttons) {
        for (int i = 0; i < list->size; i++) {
            destroyButton(list->buttons[i]);
        }
        free(list->buttons);
    }
    list->buttons = malloc(count * sizeof(Button *));
    if (!list->buttons) {
        SDL_Log("Erreur : allocation de mémoire pour les boutons échouée !");
        return;
    }
    for (int i = 0; i < count; i++) {
        list->buttons[i] = buttons[i];
    }
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
    if (!button || !renderer || !imagePath) {
        SDL_Log("InitTextureButton : Paramètres invalides.");
        return;
    }

    // Charger l'image avec SDL_image
    SDL_Surface *surface = IMG_Load(imagePath);
    if (!surface) {
        SDL_Log("Erreur : Impossible de charger l'image '%s'. Message SDL_image : %s", imagePath, IMG_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // Libérer la surface après création de la texture
    if (!texture) {
        SDL_Log("Erreur : Impossible de créer une texture pour l'image '%s'. Message SDL : %s", imagePath, SDL_GetError());
        return;
    }

    if (button->texture) {
        SDL_DestroyTexture(button->texture);
    }

    button->texture = texture;
    SDL_Log("Texture chargée avec succès pour l'image '%s'.", imagePath);
}




void renderButtonList(ButtonList *B)
{
    if (!B) return;
    for (int i = 0; i < B->size; i++)
        renderButton(B->buttons[i]);
}

void renderButton(Button *button) 
{
    if (!button) return;
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(button->renderer, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(button->renderer, button->color.r, button->color.g, button->color.b, button->color.a);
    SDL_RenderFillRect(button->renderer, &button->rect);
    if (button->texture) SDL_RenderCopy(button->renderer, button->texture, NULL, &button->rect);
    SDL_SetRenderDrawColor(button->renderer, r, g, b, a);
}

void renderButtonImage(Button *button) 
{
    if (!button) return;
    if (button->texture) SDL_RenderCopy(button->renderer, button->texture, NULL, &button->rect);
}

void renderButtonImageList(ButtonList *B)
{
    if (!B) return;
    for (int i = 0; i < B->size; i++)
        renderButtonImage(B->buttons[i]);
}



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


Slider *createSlider(SDL_Renderer *renderer, int x, int y, int w, int h, SDL_Color color, SDL_Color cursorColor) {
    Slider *slider = malloc(sizeof(Slider));
    if (!slider) {
        SDL_Log("Erreur d'allocation pour le slider.");
        return NULL;
    }
    slider->rect = (SDL_Rect){x, y, w, h};
    slider->initialBar = slider->rect;
    slider->cursor = (SDL_Rect){x, y-10, 10, h+10};
    slider->color = color;
    slider->cursorColor = cursorColor;
    slider->renderer = renderer;
    slider->value = 0;

    return slider;
}

void renderSlider(Slider *slider) {
    if (!slider) return;
    SDL_SetRenderDrawColor(slider->renderer, slider->color.r, slider->color.g, slider->color.b, slider->color.a);
    SDL_RenderFillRect(slider->renderer, &slider->rect);
    SDL_SetRenderDrawColor(slider->renderer, slider->cursorColor.r, slider->cursorColor.g, slider->cursorColor.b, slider->cursorColor.a);
    SDL_Rect cursor = {slider->rect.x + (int)(slider->value * slider->rect.w) - slider->cursor.w / 2, slider->rect.y, slider->cursor.w, slider->cursor.h};
    SDL_RenderFillRect(slider->renderer, &cursor);
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

void handleSliderEvent(Slider *slider, SDL_Event *event) {
    if (!slider) return;
    int x, y;
    SDL_GetMouseState(&x, &y);
    
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        if (SDL_PointInRect(&(SDL_Point){x, y}, &(slider->rect))) {
            slider->value = (float)(x - slider->rect.x) / slider->rect.w;
            slider->cursor.x = slider->rect.x + (slider->value * slider->rect.w) - (slider->cursor.w / 2);
        }
    } else if (event->type == SDL_MOUSEMOTION && (event->motion.state & SDL_BUTTON_LMASK)) {
        if (SDL_PointInRect(&(SDL_Point){x, y}, &(slider->rect))) {
            slider->value = (float)(x - slider->rect.x) / slider->rect.w;
            slider->cursor.x = slider->rect.x + (slider->value * slider->rect.w) - (slider->cursor.w / 2);
        }
    }
}
