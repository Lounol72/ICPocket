#include "../include/Player.h"
#include <stdio.h>



#define ANIMATION_SPEED 0.10f // Temps entre chaque frame
#define FRAMES_PER_ANIMATION 4 // Nombre de frames par animation

Player* createPlayer(SDL_Renderer *renderer, const char *spritesheetPath, Map * map, int spawnX, int spawnY) {
    Player *player = malloc(sizeof(Player));
    if (!player) {
        printf("Erreur allocation player\n");
        return NULL;
    }

    SDL_Surface *surface = IMG_Load(spritesheetPath);
    if (!surface) {
        printf("Erreur chargement spritesheet: %s\n", IMG_GetError());
        free(player);
        return NULL;
    }

    player->spriteSheet = SDL_CreateTextureFromSurface(renderer, surface);
    if (!player->spriteSheet) {
        printf("Erreur création texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        free(player);
        return NULL;
    }

    // Utiliser les coordonnées de spawn
    player->matrixX = spawnX;
    player->matrixY = spawnY;

    int windowWidth, windowHeight;
    SDL_GetWindowSize(SDL_GetWindowFromID(1), &windowWidth, &windowHeight);

    // Calculer la position en pixels au centre de l'écran
    player->position.w = (int)(windowWidth / map->tileSizeW);
    player->position.h = (int)(windowHeight / map->tileSizeH);
    player->position.x = player->matrixX * map->tileSizeW;
    player->position.y = player->matrixY * map->tileSizeH;

    printf("Position initiale du joueur: [%d,%d] en matrice, [%d,%d] en pixels\n", 
           player->matrixX, player->matrixY, 
           player->position.x, player->position.y);

    player->currentFrame.w = FRAME_WIDTH;
    player->currentFrame.h = FRAME_HEIGHT;
    player->currentFrame.x = 0;
    player->currentFrame.y = 0;

    player->frameCount = FRAME_COUNT;
    player->currentFrameIndex = 0;
    player->animationTimer = 0;
    player->animationSpeed = ANIMATION_SPEED;
    player->isMoving = 0;
    player->state = IDLE_DOWN;

    player->mat = (int **)malloc(sizeof(int *) * map->height);
    if (!player->mat) {
        printf("Erreur allocation mémoire pour player->mat\n");
        SDL_DestroyTexture(player->spriteSheet);
        free(player);
        return NULL;
    }

    for (int i = 0; i < map->height; i++) {
        player->mat[i] = (int *)malloc(sizeof(int) * map->width);
        if (!player->mat[i]) {
            printf("Erreur allocation mémoire pour player->mat[%d]\n", i);
            for (int j = 0; j < i; j++) {
                free(player->mat[j]);
            }
            free(player->mat);
            SDL_DestroyTexture(player->spriteSheet);
            free(player);
            return NULL;
        }
    }

    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            player->mat[i][j] = 0;
        }
    }

    player->interpolationTime = 0.0f;
    player->moveSpeed = 0.4f;  // Augmenté à 0.4 secondes par case (était 0.2)
    player->targetMatrixX = player->matrixX;
    player->targetMatrixY = player->matrixY;
    player->startX = player->position.x;
    player->startY = player->position.y;
    player->targetX = player->position.x;
    player->targetY = player->position.y;
    player->isMovingToTarget = false;
    player->sizeMapH = map->height;
    player->sizeMapW = map->width;

    SDL_FreeSurface(surface);
    return player;
}

// ! DEBUG
void DEBUG_printPlayerMat(Player *player) {
    for (int i = 0; i < player->sizeMapH; i++) {
        for (int j = 0; j < player->sizeMapW; j++) {
            printf("%d ", player->mat[i][j]);
        }
        printf("\n");
    }
}

void updatePlayerAnimation(Player *player, float deltaTime) {
    if (player->isMovingToTarget) {
        // Mettre à jour le timer d'animation pendant le déplacement
        player->animationTimer += deltaTime;
        
        // Calculer le nombre de frames en fonction de la durée du déplacement
        float animationProgress = player->interpolationTime / player->moveSpeed;
        int totalFrames = player->frameCount - 1; // -1 car on commence à 0
        
        // Calculer l'index de frame actuel
        player->currentFrameIndex = (int)(animationProgress * totalFrames);
        
        // S'assurer que l'index reste dans les limites
        if (player->currentFrameIndex > totalFrames) {
            player->currentFrameIndex = totalFrames;
        }
        else {
            player->currentFrameIndex ++;
        }

        // Calculer la position dans la spritesheet
        player->currentFrame.x = player->currentFrameIndex * FRAME_WIDTH;
        
        // Mettre à jour la ligne selon l'état de déplacement
        switch (player->state) {
            case WALK_DOWN:  player->currentFrame.y = 0; break;
            case WALK_LEFT:  player->currentFrame.y = 2* FRAME_HEIGHT; break;
            case WALK_RIGHT: player->currentFrame.y = 3 * FRAME_HEIGHT; break;
            case WALK_UP:    player->currentFrame.y = FRAME_HEIGHT; break;
            default: break;
        }
    } else {
        // En état idle, rester sur la première frame de la direction correspondante
        player->currentFrameIndex = 0;
        player->animationTimer = 0;
        
        switch (player->state) {
            case IDLE_DOWN:  player->currentFrame.y = 0; break;
            case IDLE_LEFT:  player->currentFrame.y = 2 * FRAME_HEIGHT; break;
            case IDLE_RIGHT: player->currentFrame.y = 3 * FRAME_HEIGHT; break;
            case IDLE_UP:    player->currentFrame.y = FRAME_HEIGHT; break;
            default: break;
        }
        player->currentFrame.x = 0;
    }

    // S'assurer que les dimensions de la frame sont correctes
    player->currentFrame.w = FRAME_WIDTH;
    player->currentFrame.h = FRAME_HEIGHT;
}

void renderPlayer(SDL_Renderer *renderer, Player *player) {
    if (!player || !player->spriteSheet) {
        printf("Erreur: player ou spritesheet NULL\n");
        return;
    }
    if (SDL_RenderCopy(renderer, player->spriteSheet, &player->currentFrame, &player->position) != 0) {
        printf("Erreur rendu: %s\n", SDL_GetError());
    }
}

void renderPlayerWithCamera(Player* player, SDL_Renderer* renderer, Camera* camera) {
    SDL_Rect worldRect = player->position;
    SDL_Rect screenRect = getWorldToScreenRect(camera, worldRect);

    if (renderer && player->spriteSheet)
        SDL_RenderCopy(renderer, player->spriteSheet, &player->currentFrame, &screenRect);
}

void destroyPlayer(Player *player) {
    if (player) {
        if (player->spriteSheet) {
            SDL_DestroyTexture(player->spriteSheet);
        }
        for (int i = 0; i < player->sizeMapH; i++) {
            free(player->mat[i]);
        }
        free(player->mat);
        free(player);
    }
}

//void uptadeSpawnPlayer(Player *player, )


void updatePlayerPosition(Player *player, float deltaTime) {
    if (player->isMovingToTarget) {
        player->interpolationTime += deltaTime;
        float t = player->interpolationTime / player->moveSpeed;

        if (t > 1.0f) t = 1.0f;

        float newX = player->startX + (player->targetX - player->startX) * t;
        float newY = player->startY + (player->targetY - player->startY) * t;

        player->position.x = (int)newX;
        player->position.y = (int)newY;

        if (t >= 1.0f) {
            player->matrixX = player->targetMatrixX;
            player->matrixY = player->targetMatrixY;
            player->isMovingToTarget = false;
            player->interpolationTime = 0.0f;

            switch (player->state) {
                case WALK_UP: player->state = IDLE_UP; break;
                case WALK_DOWN: player->state = IDLE_DOWN; break;
                case WALK_LEFT: player->state = IDLE_LEFT; break;
                case WALK_RIGHT: player->state = IDLE_RIGHT; break;
                default: break;
            }
        }
    }
}



