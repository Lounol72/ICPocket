#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

typedef struct Player {
    SDL_Rect rect;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Rect initialRect;

    int positionSprite[4];
    int currentSprite;
    int maxSprite;
    int currentFrame;
    int maxFrame;
    int currentAnimation;
    int maxAnimation;
    int state; //* Boolean to check if the player is moving or not
} Player;

Player initPlayer(SDL_Renderer *renderer, int offsetX, int offsetY, int width, int height);
void nextSprite(Player *player);
void renderPlayer(Player *player);
void movePlayer(Player *player);
void checkCollision(Player *player);
void destroyPlayer(Player *player);


#endif