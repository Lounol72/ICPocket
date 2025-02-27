#ifndef CAMERA_H
#define CAMERA_H

/**
 * @file Camera.h
 * @brief Gestion de la caméra pour le défilement de la vue du jeu.
 * 
 * Ce fichier contient les définitions et les fonctions nécessaires pour gérer la caméra du jeu,
 * permettant le suivi du joueur et la gestion du viewport.
 * 
 * @author Louis
 * @date 25/02/2025
 */

#include <SDL2/SDL.h>

/** @brief Largeur par défaut de la fenêtre. */
#define WINDOW_WIDTH 1280

/** @brief Hauteur par défaut de la fenêtre. */
#define WINDOW_HEIGHT 720

/**
 * @struct Camera
 * @brief Structure représentant la caméra du jeu.
 * 
 * Cette structure contient toutes les informations nécessaires pour gérer la vue du jeu,
 * incluant la position de la caméra, le niveau de zoom et la zone visible.
 */
typedef struct {
    float x, y;           /**< Position actuelle de la caméra dans le monde. */
    float scale;          /**< Niveau de zoom de la caméra. */
    SDL_Rect viewport;    /**< Rectangle définissant la zone visible à l'écran. */
    int targetX, targetY; /**< Position cible (généralement celle du joueur) que la caméra doit suivre. */
} Camera;

/**
 * @brief Crée une nouvelle instance de caméra.
 * 
 * Initialise une nouvelle caméra avec les dimensions de fenêtre spécifiées.
 * 
 * @param windowWidth Largeur de la fenêtre en pixels.
 * @param windowHeight Hauteur de la fenêtre en pixels.
 * @return Un pointeur vers la nouvelle instance de Camera créée.
 */
Camera* createCamera(int windowWidth, int windowHeight);

/**
 * @brief Met à jour la position de la caméra.
 * 
 * Actualise la position de la caméra en fonction de la position du joueur
 * et du temps écoulé depuis la dernière mise à jour.
 * 
 * @param camera Pointeur vers la caméra à mettre à jour.
 * @param playerX Position X du joueur dans le monde.
 * @param playerY Position Y du joueur dans le monde.
 * @param deltaTime Temps écoulé depuis la dernière mise à jour (en secondes).
 */
void updateCamera(Camera* camera, int playerX, int playerY, float deltaTime);

/**
 * @brief Convertit les coordonnées du monde en coordonnées écran.
 * 
 * Transforme un rectangle dans les coordonnées du monde en coordonnées écran
 * en tenant compte de la position et du zoom de la caméra.
 * 
 * @param camera Pointeur vers la caméra.
 * @param worldRect Rectangle dans les coordonnées du monde à convertir.
 * @return Le rectangle converti en coordonnées écran.
 */
SDL_Rect getWorldToScreenRect(Camera* camera, SDL_Rect worldRect);

/**
 * @brief Libère les ressources utilisées par la caméra.
 * 
 * Détruit l'instance de la caméra et libère la mémoire associée.
 * 
 * @param camera Pointeur vers la caméra à détruire.
 */
void destroyCamera(Camera* camera);

#endif /* CAMERA_H */
