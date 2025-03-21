#include "../include/Buttons.h"
#include <stdlib.h>




static void Scaling(const char *text, TTF_Font *font, SDL_Rect *rect, const SDL_Rect *initialRect) {
    if (!text || !font || !rect || !initialRect) return;

    int textWidth, textHeight;
    if (TTF_SizeText(font, text, &textWidth, &textHeight) != 0) return;

    float scaleW = (float)(initialRect->w - 2 * MIN_PADDING) / (float)textWidth;
    float scaleH = (float)(initialRect->h - 2 * MIN_PADDING) / (float)textHeight;
    float scale = (scaleW < scaleH) ? scaleW : scaleH;

    // Limiter l'échelle maximale pour éviter un texte trop grand
    scale = (scale > MAX_SCALE) ? MAX_SCALE : scale;

    int scaledWidth = (int)(textWidth * scale);
    int scaledHeight = (int)(textHeight * scale);

    // Centrer le texte dans le rectangle initial
    rect->x = initialRect->x + (initialRect->w - scaledWidth) / 2;
    rect->y = initialRect->y + (initialRect->h - scaledHeight) / 2;
    rect->w = scaledWidth;
    rect->h = scaledHeight;
    return;
}

static bool createTextTexture(Button *button) {
    SDL_Surface *textSurface = TTF_RenderText_Solid(button->font, 
        button->text && button->text[0] ? button->text : " ", button->textcolor);
    if (!textSurface) {
        SDL_Log("❌ Erreur lors de la création de la surface du texte : %s", TTF_GetError());
        return false;
    }

    button->textTexture = SDL_CreateTextureFromSurface(button->renderer, textSurface);
    SDL_FreeSurface(textSurface);

    if (!button->textTexture) {
        SDL_Log("❌ Erreur lors de la création de la texture du texte : %s", SDL_GetError());
        return false;
    }
    return true;
}

static bool createButtonTexture(Button *button, const char *imagePath) {
    
    SDL_Surface *surface = IMG_Load(imagePath);
    if (!surface) {
        SDL_Log("❌ Erreur lors du chargement de l'image \"%s\" : %s", imagePath, IMG_GetError());
        return false;
    }
    if (button->texture != NULL) {
        SDL_DestroyTexture(button->texture);
    }
    button->texture = SDL_CreateTextureFromSurface(button->renderer, surface);
    button->initialTexture = button->texture;
    SDL_FreeSurface(surface);

    if (!button->texture) {
        SDL_Log("❌ Erreur lors de la création de la texture : %s", SDL_GetError());
        return false;
    }
    /*
    // Exemple avec SDL_RWops
    SDL_RWops* rw = SDL_RWFromFile(imagePath, "rb");
    SDL_Surface* surface = IMG_LoadSizedSVG_RW(rw, button->rect.w, button->rect.h);
    SDL_FreeRW(rw);

    button->texture = SDL_CreateTextureFromSurface(button->renderer, surface);
    SDL_FreeSurface(surface);
    button->initialTexture = button->texture;
    */
    return true;
}

static bool createSelectedTexture(Button *button, const char *imagePath) {
    SDL_Surface *surface = IMG_Load(imagePath);
    if (!surface) {
        SDL_Log("❌ Erreur lors du chargement de l'image : %s", IMG_GetError());
        return false;
    }

    button->selectedTexture = SDL_CreateTextureFromSurface(button->renderer, surface);
    SDL_FreeSurface(surface);

    if (!button->selectedTexture) {
        SDL_Log("❌ Erreur lors de la création de la texture : %s", SDL_GetError());
        return false;
    }
    return true;
}
static void destroyButton(Button *button) {
    if (!button) return;

    // Free only the allocated textures, not the alias pointer 'texture'
    if (button->initialTexture) {
        SDL_DestroyTexture(button->initialTexture);
        button->initialTexture = NULL;
    }
    if (button->selectedTexture) {
        SDL_DestroyTexture(button->selectedTexture);
        button->selectedTexture = NULL;
    }
    if (button->textTexture) {
        SDL_DestroyTexture(button->textTexture);
        button->textTexture = NULL;
    }
    
    free(button);
    return;
}


Button *createButton(char *text, Window *win, SDL_Rect rect, SDL_Color color, SDL_Color textcolor, void (*onClick)(Window *, void *), void *data, TTF_Font *font, const char *imagePath){
    Button *button = calloc(1, sizeof(Button));
    if (!button) {
        SDL_Log("❌ Erreur d'allocation pour le bouton.");
        return NULL;
    }

    // Initialisation
    button->text = text;
    button->color = color;
    button->textcolor = textcolor;
    button->rect = button->initialRect = rect;
    button->renderer = win->renderer;
    button->onClick = onClick;
    button->data = data;
    button->font = font ? font : win->font;

    Scaling(text, button->font, &button->textRect, &rect);
    button->initialTextRect = button->textRect;
    char file[50]="\0";
    char path[80]="assets/User Interface/Blue/button_rectangle_depth_flat.png";
    if(sscanf(imagePath,"assets/User Interface/Grey/%s",file)==1)
        snprintf(path,sizeof(path),"assets/User Interface/Blue/%s",file);
    // Création des textures
    if (!createTextTexture(button) || 
        !createButtonTexture(button, imagePath) ||
        !createSelectedTexture(button, path)) {
        destroyButton(button);
        return NULL;
    }

    return button;
}

void addListButton(ButtonList *list, Button **buttons, int count) {
    if (list->buttons) {
        for (int i = 0; i < list->size; i++) destroyButton(list->buttons[i]);
        free(list->buttons);
    }
    list->buttons = malloc(count * sizeof(Button *));
    if (!list->buttons) {
        SDL_Log("❌ Erreur : allocation de mémoire pour les boutons échouée !");
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




//----------------------------------------------------------------------------------------

// Rendering functions
static void renderButton(Button *button) {
    if (!button) return;
    if (button->texture) SDL_RenderCopy(button->renderer, button->texture, NULL, &button->rect);
    if (button->textTexture)SDL_RenderCopy(button->renderer, button->textTexture, NULL, &button->textRect);
}

void renderButtonList(ButtonList *B)
{
    if (!B) return;
    for (int i = 0; i < B->size; i++)
        renderButton(B->buttons[i]);
}





//--------------------------------------------------------------------------

// Button handling functions


void ButtonClicked(Button *button, int mouseX, int mouseY, Window *win) {
    if (button && (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &button->rect)|| (mouseX == -1 && mouseY == -1) )&& button->onClick) {
        button->onClick(win, button->data);
    }
}


void updateButtonPosition(ButtonList *buttons, float Scalex, float Scaley)
{
    for (int i = 0; i < buttons->size; i++)
    {
        if(!buttons->buttons[i]){
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "❌ Invalid button at index %d", i);
            continue;
        }
        buttons->buttons[i]->rect.w = buttons->buttons[i]->initialRect.w * Scalex;
        buttons->buttons[i]->rect.h = buttons->buttons[i]->initialRect.h * Scaley;
        buttons->buttons[i]->rect.x = buttons->buttons[i]->initialRect.x * Scalex;
        buttons->buttons[i]->rect.y = buttons->buttons[i]->initialRect.y * Scaley;
        buttons->buttons[i]->textRect.w = buttons->buttons[i]->initialTextRect.w * Scalex;
        buttons->buttons[i]->textRect.h = buttons->buttons[i]->initialTextRect.h * Scaley;
        buttons->buttons[i]->textRect.x = buttons->buttons[i]->initialTextRect.x * Scalex;
        buttons->buttons[i]->textRect.y = buttons->buttons[i]->initialTextRect.y * Scaley;
        
    }
}

void setButtonText(Button *button, const char *text, SDL_Renderer *renderer) {
    if (!button || !text || !renderer) {
        SDL_Log("❌ Erreur : Paramètre NULL dans setButtonText");
        return;
    }

    // Détruire l'ancienne texture texte si elle existe
    if (button->textTexture) {
        SDL_DestroyTexture(button->textTexture);
        button->textTexture = NULL;
    }

    // Générer une nouvelle texture texte
    
    SDL_Surface *textSurface = NULL;
    if (text && text[0] == '\0') {
        textSurface = TTF_RenderText_Solid(button->font, " ", button->textcolor);
    } else {
        textSurface = TTF_RenderText_Solid(button->font, text, button->textcolor);
    }
    
    if (!textSurface) {
        SDL_Log("❌ Erreur lors de la création de la surface du texte : %s", TTF_GetError());
        return;
    }
    button->textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    if (!button->textTexture) {
        SDL_Log("❌ Erreur lors de la création de la texture du texte : %s", SDL_GetError());
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
    slider->value = 0.5f;
    slider->rect = (SDL_Rect){x, y, w, h};
    slider->initialBar = slider->rect;
    int cursorW = 25;
    slider->cursor.w = cursorW;
    slider->cursor = (SDL_Rect){slider->rect.x + (slider->value * slider->rect.w) - (slider->cursor.w / 2), y, cursorW, h+2};
    slider->initialCursor = slider->cursor;
    slider->color = color;
    slider->cursorColor = cursorColor;
    slider->renderer = renderer;
    slider->dragging = 0; // Initialiser le champ 'dragging'

    SDL_Surface *surface = IMG_Load("assets/User Interface/Grey/slide_horizontal_grey.png");
    if (!surface) {
        SDL_Log("❌ Erreur lors du chargement de l'image : %s", IMG_GetError());
        return NULL;
    }
    slider->textureBar = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!slider->textureBar) {
        SDL_Log("❌ Erreur lors de la création de la texture : %s", SDL_GetError());
        return NULL;
    }
    SDL_Surface *surfaceCursor = IMG_Load("assets/User Interface/Blue/icon_circle.png");
    if (!surfaceCursor) {
        SDL_Log("❌ Erreur lors du chargement de l'image : %s", IMG_GetError());
        return NULL;
    }
    slider->textureCursor = SDL_CreateTextureFromSurface(renderer, surfaceCursor);
    SDL_FreeSurface(surfaceCursor);

    return slider;
}


void destroySlider(Slider *slider) {
    if (slider) {
        if (slider->textureBar) SDL_DestroyTexture(slider->textureBar);
        if (slider->textureCursor) SDL_DestroyTexture(slider->textureCursor);
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
    if (slider->textureBar && slider->textureCursor) {
        SDL_RenderCopy(slider->renderer, slider->textureBar, NULL, &slider->rect);
        SDL_RenderCopy(slider->renderer, slider->textureCursor, NULL, &slider->cursor);
    }else{
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
}

// Supposez que la structure Slider contient un champ 'int dragging;' (0 ou 1).

// Optionnel : fonction inline pour clamp une valeur float
static inline float clampf(float v, float min, float max) {
    return (v < min) ? min : (v > max ? max : v);
}

int handleSliderEvent(Slider *slider, SDL_Event *event) {
    if (!slider)
        return 0;
    switch (event->type) {
        case SDL_MOUSEBUTTONDOWN:
            if (SDL_PointInRect(&(SDL_Point){ event->button.x, event->button.y }, &(slider->rect))) {
                slider->dragging = 1;
            }
            break;
        case SDL_MOUSEMOTION:
            if (slider->dragging) {
                slider->value = (float)(event->motion.x - slider->rect.x) / (slider->rect.w);
                slider->value = clampf(slider->value, 0.0f, 1.0f);
                if (slider->value > 0.88f) slider->cursor.x = slider->rect.x + slider->rect.w - slider->cursor.w;
                else slider->cursor.x = slider->rect.x + (int)(slider->value * slider->rect.w);
                //changer le volume 
                if (slider->value <= 0.05f) {
                    Mix_VolumeMusic(0);
                } else {
                    Mix_VolumeMusic(slider->value * 100);
                    
                }
            }
            break;
        case SDL_MOUSEBUTTONUP:
            slider->dragging = 0;
            break;
    }
    return 1;
}

void updateSliderPosition(SliderList *sliders, float Scalex, float Scaley) {
    if (!sliders) return;
    for (int i = 0; i < sliders->size; i++) {
        if (!sliders->sliders[i]) {
            SDL_Log("❌ Invalid slider at index %d", i);
            continue;
        }
        sliders->sliders[i]->rect.w = sliders->sliders[i]->initialBar.w * Scalex;
        sliders->sliders[i]->rect.h = sliders->sliders[i]->initialBar.h * Scaley;
        sliders->sliders[i]->rect.x = sliders->sliders[i]->initialBar.x * Scalex;
        sliders->sliders[i]->rect.y = sliders->sliders[i]->initialBar.y * Scaley;
        sliders->sliders[i]->cursor.w = 10;
        sliders->sliders[i]->cursor.h = sliders->sliders[i]->initialBar.h + (sliders->sliders[i]->initialCursor.h * Scaley) * 0.4;
        sliders->sliders[i]->cursor.x = sliders->sliders[i]->rect.x + (sliders->sliders[i]->value * sliders->sliders[i]->rect.w) - (sliders->sliders[i]->cursor.w / 2);
        sliders->sliders[i]->cursor.y = sliders->sliders[i]->rect.y - 5;
    }
}