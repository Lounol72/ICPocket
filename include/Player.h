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

typedef struct {
    SDL_Texture *spriteSheet;
    SDL_Rect position;
    SDL_Rect currentFrame;
    PlayerState state;
    int **mat;
    int frameCount;
    int currentFrameIndex;
    float animationTimer;
    float animationSpeed;
    int isMoving;
    float velocityX;
    float velocityY;
    int matrixX;
    int matrixY;
    float interpolationTime;    // Temps écoulé pendant l'interpolation
    float moveSpeed;           // Vitesse de déplacement (en secondes)
    int targetMatrixX;         // Position cible X dans la matrice
    int targetMatrixY;         // Position cible Y dans la matrice
    float startX;             // Position de départ X en pixels
    float startY;             // Position de départ Y en pixels
    float targetX;            // Position cible X en pixels
    float targetY;            // Position cible Y en pixels
    bool isMovingToTarget;
    int sizeMapW;
    int sizeMapH;
} Player;


Player* createPlayer(SDL_Renderer *renderer, const char *spritesheetPath, Map * map);
void updatePlayerAnimation(Player *player, float deltaTime);
void updatePlayerPosition(Player *player, float deltaTime);
void renderPlayer(SDL_Renderer *renderer, Player *player);
void movePlayer(Player *player);
void checkCollision(Player *player);
void destroyPlayer(Player *player);
void renderPlayerWithCamera(Player* player, SDL_Renderer* renderer, Camera* camera);

// ! DEBUG
void DEBUG_printPlayerMat(Player *player);

#endif