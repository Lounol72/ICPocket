#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>

/**
 * @file Utils.h
 * @brief Fonctions utilitaires pour l'application.
 * 
 * Ce fichier contient des fonctions utilitaires qui sont utilisées dans différentes parties de l'application SDL.
 */

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

typedef enum AppState {
    QUIT = 0, // 0
    SETTINGS, // 1
    MENU, // 2
    GAME,  // 3
    NEWGAME,  // 4
    LOADGAME, // 5
    ICMONS, // 6
    INTER, // 7
    PAUSE, // 8
    MAP, // 9
} AppState;

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