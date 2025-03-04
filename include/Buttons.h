#ifndef BUTTONS_H
#define BUTTONS_H

/**
 * @file Buttons.h
 * @brief Gestion des boutons et curseurs pour l'application.
 *
 * Ce fichier contient les définitions et les fonctions nécessaires pour créer, gérer et rendre les boutons et curseurs dans l'application SDL.
 *
 * @author Alban Louis
 * @date 27/12/2024
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>

typedef struct Button Button;
typedef struct ButtonList ButtonList;
typedef struct Slider Slider;
typedef struct SliderList SliderList;

#include "Utils.h"


typedef struct Window Window;
/**
 * @struct Button
 * @brief Représente un bouton dans l'application.
 *
 * Cette structure contient des informations sur un bouton, y compris son texte, sa position, sa couleur, et les fonctions associées.
 */
typedef struct Button {
    char nom[50];  /**< Nom du bouton. */
    SDL_Rect rect; /**< Rectangle définissant la position et la taille du bouton. */
    SDL_Rect initialRect; /**< Rectangle initial pour les besoins de mise à l'échelle. */
    SDL_Rect textRect; /**< Rectangle du texte du bouton. */
    SDL_Rect initialTextRect; /**< Rectangle initial pour les besoins de mise à l'échelle. */
    SDL_Color color; /**< Couleur de fond du bouton. */
    SDL_Texture *texture; /**< Texture du bouton. */
    SDL_Texture *initialTexture; /**< Texture initiale du bouton. */
    SDL_Texture *selectedTexture; /**< Texture du bouton lorsqu'il est sélectionné. */
    SDL_Renderer *renderer; /**< Renderer SDL utilisé pour dessiner le bouton. */
    char *text; /**< Texte affiché sur le bouton. */
    SDL_Texture *textTexture; /**< Texture du texte du bouton. */
    SDL_Color textcolor; /**< Couleur du texte du bouton. */
    TTF_Font *font;  /**< Police utilisée pour le texte du bouton. */
    void (*onClick)(Window *win, void *data); /**< Fonction de rappel appelée lors du clic sur le bouton. */
    void *data; /**< Données utilisateur passées à la fonction de rappel. */
} Button;

/**
 * @struct ButtonList
 * @brief Liste de boutons.
 *
 * Cette structure contient un tableau de pointeurs vers des boutons et leur nombre.
 */
typedef struct ButtonList {
    Button **buttons; /**< Tableau de pointeurs vers des boutons. */
    int size;       /**< Nombre de boutons dans la liste. */
} ButtonList;

/**
 * @struct Slider
 * @brief Représente un curseur dans l'application.
 *
 * Cette structure contient des informations sur un curseur, y compris sa position, sa couleur, et sa valeur actuelle.
 */
typedef struct Slider {
    SDL_Rect rect;           /**< Rectangle définissant la position et la taille du curseur. */
    SDL_Rect initialBar;      /**< Rectangle initial de la barre du curseur. */
    SDL_Rect cursor;         /**< Rectangle du curseur. */
    SDL_Rect initialCursor;  /**< Rectangle initial du curseur pour les besoins de mise à l'échelle. */
    SDL_Color color;         /**< Couleur de la barre du curseur. */
    SDL_Color cursorColor;   /**< Couleur du curseur. */
    SDL_Renderer *renderer;   /**< Renderer SDL utilisé pour dessiner le curseur. */
    float value;            /**< Valeur actuelle du curseur. */
    int dragging;           /**< Indicateur de glissement du curseur. */
} Slider;

/**
 * @struct SliderList
 * @brief Liste de curseurs.
 *
 * Cette structure contient un tableau de pointeurs vers des curseurs et leur nombre.
 */
typedef struct SliderList {
    Slider **sliders; /**< Tableau de pointeurs vers des curseurs. */
    int size;       /**< Nombre de curseurs dans la liste. */
} SliderList;

/**
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
Button *createButton(char *text, Window *win, SDL_Rect rect, SDL_Color color, SDL_Color textcolor,
                     void (*onClick)(Window *win, void *data), void *data, TTF_Font *font,
                     const char *imagePath);

/**
 * @brief Ajoute une liste de boutons à la liste de boutons.
 *
 * @param list Pointeur vers la structure qui accumule les états des boutons.
 * @param buttons Pointeur vers le tableau d'informations des boutons à traiter.
 * @param count Nombre de boutons dans le tableau fourni....
 */
void addListButton(ButtonList *list, Button **buttons, int count);

/**
 * @brief Détruit la liste de boutons.
 *
 * @param list Pointeur vers la liste de boutons à détruire.
 */
void destroyButtonList(ButtonList *list);

/**
 * @brief Rend la liste de boutons.
 *
 * @param B Pointeur vers la liste de boutons à rendre.
 */
void renderButtonList(ButtonList *B);

/**
 * @brief Gère l'interaction avec le bouton en fonction de la position actuelle de la souris et du contexte de la fenêtre.
 *
 * @param button Pointeur vers le bouton avec lequel interagir.
 * @param mouseX La coordonnée x du pointeur de la souris.
 * @param mouseY La coordonnée y du pointeur de la souris.
 * @param win La fenêtre ou le contexte graphique dans lequel l'interaction se produit.
 * @return Un statut ou un booléen indiquant si l'interaction a été traitée avec succès.
 */
void ButtonClicked(Button *button, int mouseX, int mouseY, Window *win);

/**
 * @brief Met à jour les états des boutons et applique des facteurs de mise à l'échelle aux éléments de l'interface utilisateur.
 *
 * @param buttons Structure ou collection représentant les boutons à gérer ou à mettre à jour.
 * @param Scalex Le facteur de mise à l'échelle horizontal à appliquer.
 * @param Scaley Le facteur de mise à l'échelle vertical à appliquer.
 */
void updateButtonPosition(ButtonList *buttons, float Scalex, float Scaley);

/**
 * @brief Définit le texte d'un bouton.
 *
 * @param button Le bouton à modifier.
 * @param text Le texte à afficher sur le bouton.
 * @param renderer Le renderer à utiliser pour dessiner le bouton.
 */
void setButtonText(Button *button, const char *text, SDL_Renderer *renderer);

/**
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
Slider *createSlider(SDL_Renderer *renderer, int x, int y, int w, int h, SDL_Color color,
                     SDL_Color cursorColor);

/**
 * @brief Déplace le curseur du slider en fonction de la coordonnée X de la souris.
 *
 * @param slider Pointeur vers l'objet slider à mettre à jour.
 * @param mouseX La coordonnée X actuelle de la souris.
 */
void DragCursor(Slider *slider, int mouseX);

/**
 * @brief Rend un curseur.
 *
 * @param slider Pointeur vers le curseur à rendre.
 */
void renderSlider(Slider *slider);

/**
 * @brief Détruit un curseur.
 *
 * @param slider Pointeur vers le curseur à détruire.
 */
void destroySlider(Slider *slider);

/**
 * @brief Détruit la liste de curseurs.
 *
 * @param list Pointeur vers la liste de curseurs à détruire.
 */
void destroySliderList(SliderList *list);

/**
 * @brief Ajoute une liste de curseurs à la liste de curseurs.
 *
 * @param S Pointeur vers la structure contenant les curseurs à ajouter.
 * @param sliders Tableau de curseurs à ajouter.
 * @param size Nombre de curseurs dans le tableau.
 */
void addListSlider(SliderList *S, Slider *sliders[], int size);

/**
 * @brief Rend la liste de curseurs.
 *
 * @param S Pointeur vers la liste de curseurs à rendre.
 */
void renderSliderList(SliderList *S);

/**
 * @brief Gère l'événement de curseur.
 *
 * Cette fonction traite l'événement de curseur en fonction du curseur et des coordonnées donnés.
 *
 * @param slider Le curseur associé à l'événement.
 * @param event L'événement SDL.
 * @return int Retourne 0 en cas de succès, ou un code d'erreur en cas d'échec.
 */
int handleSliderEvent(Slider *slider, SDL_Event *event);

/**
 * @brief Met à jour la position des curseurs en fonction des coordonnées X et Y de la souris.
 *
 * @param sliders Pointeur vers la structure contenant les curseurs à mettre à jour.
 * @param Scalex Facteur de mise à l'échelle pour l'axe horizontal.
 * @param Scaley Facteur de mise à l'échelle pour l'axe vertical.
 */
void updateSliderPosition(SliderList *sliders, float Scalex, float Scaley);


#endif
