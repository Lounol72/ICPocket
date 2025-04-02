#ifndef PLAYER_H
#define PLAYER_H


#include "Map.h"

#include <stdbool.h>

#define FRAME_WIDTH 16   // Largeur d'une frame dans la spritesheet
#define FRAME_HEIGHT 20  // Hauteur d'une frame dans la spritesheet
#define FRAME_COUNT 4    // Nombre de frames par animation
#define ANIMATION_SPEED 0.10f
/**
 * @file Player.h
 * @author Louis
 * @date 25/02/2025
 */

/**
 * @enum PlayerState
 * @brief État du joueur.
 * 
 * Cette énumération définit les différents états possibles du joueur dans le jeu.
 */
typedef enum {
    IDLE_DOWN,
    IDLE_UP,
    IDLE_LEFT,
    IDLE_RIGHT,
    WALK_DOWN,
    WALK_UP,
    WALK_LEFT,
    WALK_RIGHT
} PlayerState;

/**
 * @struct Player
 * @brief Représente le joueur dans le jeu.
 * 
 * Cette structure contient des informations sur la position, l'état, l'animation et les mouvements du joueur.
 */
typedef struct {
    SDL_Texture *spriteSheet; /**< Texture de la spritesheet du joueur. */
    SDL_Rect position;      /**< Position actuelle du joueur. */
    SDL_Rect currentFrame; /**< Frame actuelle de l'animation. */
    PlayerState state;     /**< État actuel du joueur. */
    int **mat;         /**< Matrice de collision du joueur. */
    int frameCount;    /**< Nombre total de frames dans la spritesheet. */
    int currentFrameIndex; /**< Index de la frame actuelle. */
    float animationTimer; /**< Timer pour gérer l'animation. */
    float animationSpeed; /** Vitesse de l'animation. */ 
    int isMoving;      /**< Indicateur de mouvement du joueur. */ 
    float velocityX;   /**< Vitesse de déplacement en X. */
    float velocityY;  /**< Vitesse de déplacement en Y. */ 
    int matrixX;       /**< Position actuelle X dans la matrice. */
    int matrixY;      /**< Position actuelle Y dans la matrice. */
    float interpolationTime;    /**< Temps écoulé pendant l'interpolation. */
    float moveSpeed;           /**< Vitesse de déplacement (en secondes). */
    int targetMatrixX;         /**< Position cible X dans la matrice. */
    int targetMatrixY;         /**< Position cible Y dans la matrice. */
    float startX;             /**< Position de départ X en pixels. */
    float startY;             /**< Position de départ Y en pixels. */
    float targetX;            /**< Position cible X en pixels. */
    float targetY;            /**< Position cible Y en pixels. */
    bool isMovingToTarget;
    int sizeMapW;
    int sizeMapH;
    
} Player;

/**
 * @fn Player* createPlayer(SDL_Renderer *renderer, const char *spritesheetPath, Map * map, int spawnX, int spawnY)
 * @brief Crée un joueur avec une spritesheet, une position de départ et une matrice de collision.
 * 
 * Cette fonction initialise un joueur avec une spritesheet, une position de départ et une matrice de collision.
 * 
 * @param renderer Pointeur vers le rendu SDL.
 * @param spritesheetPath Chemin vers la spritesheet du joueur.
 * @param map Pointeur vers la carte du jeu.
 * @param spawnX Position de départ X du joueur.
 * @param spawnY Position de départ Y du joueur.
 * @return Pointeur vers le joueur créé.
 */
Player* createPlayer(SDL_Renderer *renderer, const char *spritesheetPath, Map * map, int spawnX, int spawnY);

/**
 * @fn void updatePlayerAnimation(Player *player, float deltaTime)
 * @brief Met à jour l'animation du joueur en fonction du temps écoulé.
 * 
 * Cette fonction met à jour l'animation du joueur en fonction du temps écoulé depuis la dernière mise à jour.
 * 
 * @param player Pointeur vers le joueur à mettre à jour.
 * @param deltaTime Temps écoulé depuis la dernière mise à jour.
 */
void updatePlayerAnimation(Player *player, float deltaTime);

/**
 * @fn void updatePlayerPosition(Player *player, float deltaTime)
 * @brief Met à jour la position du joueur en fonction de son état et du temps écoulé.
 * 
 * Cette fonction met à jour la position du joueur en fonction de son état (déplacement ou non) et du temps écoulé.
 * 
 * @param player Pointeur vers le joueur à mettre à jour.
 * @param deltaTime Temps écoulé depuis la dernière mise à jour.
 */
void updatePlayerPosition(Player *player, float deltaTime);

/**
 * @fn void renderPlayer(SDL_Renderer *renderer, Player *player)
 * @brief Rendu du joueur sur l'écran.
 * 
 * Cette fonction dessine le joueur sur l'écran en utilisant le rendu SDL.
 * 
 * @param renderer Pointeur vers le rendu SDL.
 * @param player Pointeur vers le joueur à dessiner.
 */
void renderPlayer(SDL_Renderer *renderer, Player *player);

/**
 * @fn void movePlayer(Player *player)
 * @brief Gère le mouvement du joueur en fonction des entrées utilisateur.
 * 
 * Cette fonction gère le mouvement du joueur en fonction des entrées utilisateur (touches fléchées ou joystick).
 * 
 * @param player Pointeur vers le joueur à déplacer.
 */
void movePlayer(Player *player);

/**
 * @fn void destroyPlayer(Player *player)
 * @brief Détruit le joueur et libère les ressources associées.
 * 
 * Cette fonction libère la mémoire associée au joueur et à sa spritesheet.
 * 
 * @param player Pointeur vers le joueur à détruire.
 */
void destroyPlayer(Player *player);

/**
 * @fn void renderPlayerWithCamera(Player* player, SDL_Renderer* renderer, Camera* camera)
 * @brief Rendu du joueur avec la caméra.
 * 
 * Cette fonction dessine le joueur sur l'écran en tenant compte de la position de la caméra.
 * 
 * @param player Pointeur vers le joueur à dessiner.
 * @param renderer Pointeur vers le rendu SDL.
 * @param camera Pointeur vers la caméra à utiliser pour le rendu.
 */
void renderPlayerWithCamera(Player* player, SDL_Renderer* renderer, Camera* camera);


// ! DEBUG
void DEBUG_printPlayerMat(Player *player);

#endif