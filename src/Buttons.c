#include "../include/Buttons.h"
#include <stdlib.h>

/**
 * @file Buttons.h
 * @brief Gestion des boutons et curseurs pour l'application.
 *
 * Ce fichier contient les définitions et les fonctions nécessaires pour créer, gérer et rendre les boutons et curseurs dans l'application SDL.
 *
 * @author Alban Louis
 * @date 27/12/2024
 */

/**
 * @fn static void Scaling(const char *text, TTF_Font *font, SDL_Rect *rect, const SDL_Rect *initialRect)
 * @brief Met à l'échelle le texte pour qu'il s'adapte au rectangle spécifié.
 * 
 * Cette fonction calcule l'échelle du texte en fonction de la taille du rectangle initial et ajuste la position et la taille du rectangle de texte.
 * 
 * @param text Le texte à afficher.
 * @param font La police utilisée pour le texte.
 * @param rect Le rectangle dans lequel le texte sera affiché.
 * @param initialRect Le rectangle initial pour le texte.
 */
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

/**
 * @fn static bool createTextTexture(Button *button)
 * @brief Crée une texture de texte pour le bouton.
 *
 * @param button Le bouton pour lequel créer la texture de texte.
 * @return true si la création de la texture a réussi, false sinon.
 */
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

/**
 * @fn static bool createButtonTexture(Button *button, const char *imagePath)
 * @brief Crée une texture pour le bouton à partir d'une image.
 *
 * @param button Le bouton pour lequel créer la texture.
 * @param imagePath Le chemin de l'image à utiliser pour la texture.
 * @return true si la création de la texture a réussi, false sinon.
 */
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

/**
 * @fn static bool createSelectedTexture(Button *button, const char *imagePath)
 * @brief Crée une texture pour le bouton sélectionné à partir d'une image.
 *
 * @param button Le bouton pour lequel créer la texture sélectionnée.
 * @param imagePath Le chemin de l'image à utiliser pour la texture sélectionnée.
 * @return true si la création de la texture a réussi, false sinon.
 */
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

/**
 * @fn static void destroyButton(Button *button)
 * @brief Détruit un bouton et libère la mémoire associée.
 *
 * @param button Le bouton à détruire.
 */
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

/**
 * @fn Button *createButton(char *text, Window *win, SDL_Rect rect, SDL_Color color, SDL_Color textcolor,
 *                         void (*onClick)(Window *win, void *data), void *data, TTF_Font *font, const char *imagePath)
 * @brief Crée et retourne un nouvel objet Button.
 *
 * @param text Le texte affiché sur le bouton.
 * @param win La fenêtre où le bouton apparaîtra.
 * @param rect La position du bouton.
 * @param color La couleur de fond du bouton.
 * @param textcolor La couleur du texte du bouton.
 * @param onClick La fonction de rappel appelée lors du clic sur le bouton.
 * @param data Les données utilisateur passées à la fonction de rappel.
 * @param font La police utilisée pour dessiner le texte du bouton.
 * @param imagePath Chemin du fichier image pour le bouton.
 * @return Un pointeur vers le nouvel objet Button créé.
 */
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

/**
 * @fn void addListButton(ButtonList *list, Button **buttons, int count)
 * @brief Ajoute une liste de boutons à la liste de boutons.
 *
 * @param list Pointeur vers la structure qui accumule les états des boutons.
 * @param buttons Pointeur vers le tableau d'informations des boutons à traiter.
 * @param count Nombre de boutons dans le tableau fourni....
 */
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

/**
 * @fn void destroyButtonList(ButtonList *list)
 * @brief Détruit la liste de boutons.
 *
 * @param list Pointeur vers la liste de boutons à détruire.
 */
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

/**
 * @fn static void renderButton(Button *button)
 * @brief Rend un bouton.
 *
 * @param button Pointeur vers le bouton à rendre.
 */
static void renderButton(Button *button) {
    if (!button) return;
    if (button->texture) SDL_RenderCopy(button->renderer, button->texture, NULL, &button->rect);
    if (button->textTexture)SDL_RenderCopy(button->renderer, button->textTexture, NULL, &button->textRect);
}

/**
 * @fn void renderButtonList(ButtonList *B)
 * @brief Rend la liste de boutons.
 *
 * @param B Pointeur vers la liste de boutons à rendre.
 */
void renderButtonList(ButtonList *B)
{
    if (!B) return;
    for (int i = 0; i < B->size; i++)
        renderButton(B->buttons[i]);
}





//--------------------------------------------------------------------------

// Button handling functions

/**
 * @fn void ButtonClicked(Button *button, int mouseX, int mouseY, Window *win)
 * @brief Gère l'interaction avec le bouton en fonction de la position actuelle de la souris et du contexte de la fenêtre.
 *
 * @param button Pointeur vers le bouton avec lequel interagir.
 * @param mouseX La coordonnée x du pointeur de la souris.
 * @param mouseY La coordonnée y du pointeur de la souris.
 * @param win La fenêtre ou le contexte graphique dans lequel l'interaction se produit.
 * @return Un statut ou un booléen indiquant si l'interaction a été traitée avec succès.
 */
void ButtonClicked(Button *button, int mouseX, int mouseY, Window *win) {
    if (button && (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &button->rect)|| (mouseX == -1 && mouseY == -1) )&& button->onClick) {
        button->onClick(win, button->data);
    }
}

/**
 * @fn uptadeButtonPosition(ButtonList *buttons, float Scalex, float Scaley)
 * @brief Met à jour les états des boutons et applique des facteurs de mise à l'échelle aux éléments de l'interface utilisateur.
 *
 * @param buttons Structure ou collection représentant les boutons à gérer ou à mettre à jour.
 * @param Scalex Le facteur de mise à l'échelle horizontal à appliquer.
 * @param Scaley Le facteur de mise à l'échelle vertical à appliquer.
 */
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

/**
 * @fn void setButtonText(Button *button, const char *text, SDL_Renderer *renderer)
 * @brief Définit le texte d'un bouton.
 *
 * @param button Le bouton à modifier.
 * @param text Le texte à afficher sur le bouton.
 * @param renderer Le renderer à utiliser pour dessiner le bouton.
 */
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
/**
 * @fn Slider *createSlider(SDL_Renderer *renderer, int x, int y, int w, int h, SDL_Color color,
 *                         SDL_Color cursorColor)
 * @brief Crée un objet Slider.
 *
 * Cette fonction initialise un nouvel objet Slider avec les paramètres spécifiés.
 *
 * @param renderer Le SDL_Renderer à utiliser pour le rendu du curseur.
 * @param x La coordonnée x de la position du curseur.
 * @param y La coordonnée y de la position du curseur.
 * @param w La largeur du curseur.
 * @param h La hauteur du curseur.
 * @param color La couleur du curseur.
 * @param cursorColor La couleur du curseur.
 * @return Slider* Un pointeur vers le nouvel objet Slider créé.
 */
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
    slider->cursor = (SDL_Rect){slider->rect.x + (slider->value * slider->rect.w) - (slider->cursor.w / 2), y-2, cursorW+3, h+2};
    slider->initialCursor = slider->cursor;
    slider->color = color;
    slider->cursorColor = cursorColor;
    slider->renderer = renderer;
    slider->dragging = 0;

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

/**
 * @fn void destroySlider(Slider *slider)
 * @brief Détruit un curseur.
 *
 * @param slider Pointeur vers le curseur à détruire.
 */
void destroySlider(Slider *slider) {
    if (slider) {
        if (slider->textureBar) SDL_DestroyTexture(slider->textureBar);
        if (slider->textureCursor) SDL_DestroyTexture(slider->textureCursor);
        free(slider);
    }
}

/**
 * @fn void destroySliderList(SliderList *list)
 * @brief Détruit la liste de curseurs.
 *
 * @param list Pointeur vers la liste de curseurs à détruire.
 */
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

/**
 * @fn void addListSlider(SliderList *S, Slider *sliders[], int size)
 * @brief Ajoute une liste de curseurs à la liste de curseurs.
 *
 * @param S Pointeur vers la structure contenant les curseurs à ajouter.
 * @param sliders Tableau de curseurs à ajouter.
 * @param size Nombre de curseurs dans le tableau.
 */
void addListSlider(SliderList *S, Slider *sliders[], int size) {
    if (!S || !sliders) return;
    Slider **newSliders = (Slider **)realloc(S->sliders, (S->size + size) * sizeof(Slider *));
    if (!newSliders) return;
    S->sliders = newSliders;
    for (int i = 0; i < size; i++) S->sliders[S->size + i] = sliders[i];
    S->size += size;
}

/**
 * @fn void renderSliderList(SliderList *S)
 * @brief Rend la liste de curseurs.
 *
 * @param S Pointeur vers la liste de curseurs à rendre.
 */
void renderSliderList(SliderList *S) {
    if (!S) return;
    for (int i = 0; i < S->size; i++)
        renderSlider(S->sliders[i]);
}

/**
 * @fn void renderSlider(Slider *slider)
 * @brief Rend un curseur.
 *
 * @param slider Pointeur vers le curseur à rendre.
 */
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

static inline float clampf(float v, float min, float max) {return (v < min) ? min : (v > max ? max : v);} //

/**
 * @fn int handleSliderEvent(Slider *slider, SDL_Event *event)
 * @brief Gère l'événement de curseur.
 *
 * Cette fonction traite l'événement de curseur en fonction du curseur et des coordonnées donnés.
 *
 * @param slider Le curseur associé à l'événement.
 * @param event L'événement SDL.
 * @return int Retourne 0 en cas de succès, ou un code d'erreur en cas d'échec.
 */
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

/**
 * @fn void updateSliderPosition(SliderList *sliders, float Scalex, float Scaley)
 * @brief Met à jour la position des curseurs en fonction des coordonnées X et Y de la souris.
 *
 * @param sliders Pointeur vers la structure contenant les curseurs à mettre à jour.
 * @param Scalex Facteur de mise à l'échelle pour l'axe horizontal.
 * @param Scaley Facteur de mise à l'échelle pour l'axe vertical.
 */
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
        sliders->sliders[i]->cursor.w = sliders->sliders[i]->initialCursor.w * Scalex;
        sliders->sliders[i]->cursor.h = sliders->sliders[i]->initialCursor.h * Scaley;
        sliders->sliders[i]->cursor.x = sliders->sliders[i]->initialCursor.x * Scalex;
        sliders->sliders[i]->cursor.y = sliders->sliders[i]->initialCursor.y * Scaley;
    }
}