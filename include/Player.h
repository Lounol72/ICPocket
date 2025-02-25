#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Map.h"

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
} Player;
#define FRAME_WIDTH 16   // Largeur d'une frame dans la spritesheet
#define FRAME_HEIGHT 20  // Hauteur d'une frame dans la spritesheet
#define FRAME_COUNT 4    // Nombre de frames par animation
#define ANIMATION_SPEED 0.15f

Player* createPlayer(SDL_Renderer *renderer, const char *spritesheetPath);
void updatePlayerAnimation(Player *player, float deltaTime);
void renderPlayer(SDL_Renderer *renderer, Player *player);
void movePlayer(Player *player);
void checkCollision(Player *player);
void destroyPlayer(Player *player);

#endif