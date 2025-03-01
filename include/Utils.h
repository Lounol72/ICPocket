#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>

#include "Window.h"
#include "Buttons.h"

/**
 * @file Utils.h
 * @author Louis
 * @enum AppState
 * @brief Represents the various states of the application.
 * 
 * Cette énumération définit les différents états dans lesquels l'application peut se trouver.
 * Chaque état correspond à une partie spécifique de l'application, comme le menu principal,
 * les paramètres du jeu, ou le jeu lui-même.
 * 
 */





/* UI element container, referencing the ButtonList & SliderList defined in Buttons.h */
/**
 * @struct UI_Elements
 * @brief Conteneur pour les éléments de l'interface utilisateur.
 * 
 * Cette structure référence les listes de boutons et de curseurs définies dans Buttons.h, ainsi que la texture de fond.
 */
typedef struct {
    ButtonList *buttons;     /**< Liste des boutons de l'interface utilisateur. */
    SliderList *sliders;     /**< Liste des curseurs de l'interface utilisateur. */
    SDL_Texture *background; /**< Texture de fond de l'interface utilisateur. */
} UI_Elements;

/**
 * @brief Charge une texture à partir d'un fichier image.
 * 
 * Cette fonction charge une texture SDL à partir d'un fichier image spécifié.
 * 
 * @param filePath Le chemin du fichier image.
 * @param renderer Le renderer SDL utilisé pour créer la texture.
 * @return Un pointeur vers la texture SDL chargée, ou NULL en cas d'échec.
 */
SDL_Texture* loadTexture(const char *filePath, SDL_Renderer *renderer);

/**
 * @brief Affiche un message d'erreur SDL.
 * 
 * Cette fonction affiche un message d'erreur SDL dans la console.
 * 
 * @param message Le message d'erreur à afficher.
 */
void logSDLError(const char *message);

#endif /* UTILS_H */