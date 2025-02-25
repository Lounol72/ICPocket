#include "../include/Player.h"

Player initPlayer(SDL_Renderer *renderer, int offsetX, int offsetY, int width, int height) {
    Player player = malloc(sizeof(Player));
    player->renderer = renderer;
    player->texture = NULL;
    player->initialRect = (SDL_Rect) {0, 0, 0, 0};
    player->positionSprite = (int *) malloc(sizeof(int) * 4);
    player->positionSprite[0] = offsetX;
    player->positionSprite[1] = offsetY;
    player->positionSprite[2] = width;
    player->positionSprite[3] = height;
    player->currentSprite = 0;
    player->maxSprite = 4;
    player->currentFrame = 0;
    player->maxFrame = 4;
    player->currentAnimation = 0;
    player->maxAnimation = 4;
    player->state = 0;
    return player;
}

void nextSprite(Player *player) {
    player->currentSprite++;
    if (player->currentSprite >= player->maxSprite) {
        player->currentSprite = 0;
    }
    player->texture = SDL_LoadBMP(player->positionSprite[player->currentSprite]);
    if (player->texture == NULL) {
        printf("Error loading texture: %s\n", SDL_GetError());
    }
    player->initialRect = (SDL_Rect) {player->positionSprite[0], player->positionSprite[1], player->positionSprite[2], player->positionSprite[3]};
}

void renderPlayer(Player *player) {
    