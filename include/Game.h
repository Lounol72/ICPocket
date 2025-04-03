#ifndef GAME_H
#define GAME_H

#include "Audio.h"
#include "GameEngine.h"


/**
 * @file Game.h
 * @author Louis Alban
 * @brief Gestion des backgrounds, Initialisation de la structure globale
 * @date 17/02/2025
 */

/**
 * @brief Structure de jeu.
 * 
 */
extern Game game;

/**
 * @fn void initGame(Window *win);
 * @brief Initialise le jeu.
 * 
 * Cette fonction initialise les éléments nécessaires au bon fonctionnement du jeu, y compris la musique et les images.
 * 
 * @param win Pointeur sur la fenêtre.
 */
void initGame(Window *win);

/**
 * @fn void loadBackground(SDL_Texture **Background, SDL_Renderer *renderer, const char *imagePath);
 * @brief charge une image de fond.
 * 
 * @param Background utilisé pour stocker l'image de fond
 * @param renderer renderer utilisé pour charger l'image
 * @param imagePath chemin de l'image
 */
void loadBackground(SDL_Texture **Background, SDL_Renderer *renderer, const char *imagePath) ;

/**
 * @fn void loadPhrase(void);
 * @brief charge les phrases.
 * 
 * Cette fonction charge les phrases à partir d'un fichier texte et les stocke dans un tableau de chaînes de caractères.
 */
void loadPhrase(void);

/**
 * @fn void destroyGame();
 * @brief detruit le jeu.
 * 
 */
void destroyGame();

/**
 * @fn initAllButtons(Window *win);
 * @brief Initialise tous les boutons.
 * 
 * Cette fonction initialise tous les boutons de l'interface utilisateur.
 * 
 * @param win Un pointeur vers la structure Window.
 */
void initAllButtons(Window *win);

#endif /* GAME_H */