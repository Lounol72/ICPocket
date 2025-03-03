#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_gamecontroller.h>

#include "Utils.h"
#include "Log.h"

typedef enum AppState_s {
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
 * @file Window.h
 * @brief Définition et gestion de la fenêtre principale de l'application.
 * 
 * Ce fichier définit la structure Window et les fonctions associées pour gérer la fenêtre principale de l'application et ses propriétés.
 * 
 * @author Louis
 * @date 17/02/2025
 */

#if defined(SDL_RENDERER_PRESENTVSYNC)
    #define renderer_flags DEFAULT_RENDERER_VSYNC /**< Drapeaux par défaut du renderer si VSYNC est disponible. */
//#elif defined(__linux__)
//    #define renderer_flags SDL_RENDERER_ACCELERATED /**< Drapeaux du renderer pour Linux. */
#else
    #define renderer_flags SDL_RENDERER_SOFTWARE /**< Repli sur le rendu logiciel si VSYNC n'est pas disponible. */
#endif

/** Déclaration anticipée de la structure Game. */
typedef struct Game Game;

/** Instance globale du jeu. */
extern Game game;

/**
 * @struct Window
 * @brief Représente la fenêtre principale de l'application.
 * 
 * Cette structure contient des informations sur la fenêtre principale de l'application, y compris ses dimensions, son renderer, ses polices et son état.
 */
typedef struct Window {
    SDL_Window *window;      /**< Pointeur vers la fenêtre SDL. */
    SDL_Renderer *renderer;  /**< Pointeur vers le renderer SDL. */
    int width;               /**< Largeur actuelle de la fenêtre. */
    int height;              /**< Hauteur actuelle de la fenêtre. */
    int InitialWidth;        /**< Largeur initiale de la fenêtre. */
    int InitialHeight;       /**< Hauteur initiale de la fenêtre. */
    int quit;                /**< Indicateur indiquant si l'application doit se terminer. */
    TTF_Font *LargeFont;     /**< Pointeur vers la grande police utilisée dans l'application. */
    TTF_Font *MediumFont;    /**< Pointeur vers la police moyenne utilisée dans l'application. */
    TTF_Font *SmallFont;     /**< Pointeur vers la petite police utilisée dans l'application. */
    TTF_Font *font;          /**< Pointeur vers la police par défaut utilisée dans l'application. */
    AppState state;          /**< État actuel de l'application. */
    double textSpeed;        /**< Vitesse à laquelle le texte est affiché. */
} Window;

/**
 * @brief Initialise la fenêtre principale de l'application.
 * 
 * Cette fonction initialise la fenêtre principale de l'application avec les dimensions et le chemin de police spécifiés.
 * 
 * @param win Un pointeur vers la structure Window à initialiser.
 * @param width La largeur initiale de la fenêtre.
 * @param height La hauteur initiale de la fenêtre.
 * @param FontPath Le chemin du fichier de police à utiliser.
 */
void initWindow(Window *win, int width, int height, const char *FontPath);

void destroyGame();

#endif /* WINDOW_H */