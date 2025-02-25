#include "../include/Player.h"


#define ANIMATION_SPEED 0.15f // Temps entre chaque frame
#define FRAMES_PER_ANIMATION 4 // Nombre de frames par animation

Player* createPlayer(SDL_Renderer *renderer, const char *spritesheetPath) {
    Player *player = malloc(sizeof(Player));
    if (!player) {
        printf("Erreur allocation player\n");
        return NULL;
    }

    printf("Chargement de la spritesheet: %s\n", spritesheetPath);
    SDL_Surface *surface = IMG_Load(spritesheetPath);
    if (!surface) {
        printf("Erreur chargement spritesheet: %s\n", IMG_GetError());
        free(player);
        return NULL;
    }
    printf("Dimensions spritesheet: %dx%d\n", surface->w, surface->h);

    player->spriteSheet = SDL_CreateTextureFromSurface(renderer, surface);
    if (!player->spriteSheet) {
        printf("Erreur création texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        free(player);
        return NULL;
    }
    printf("Texture créée avec succès\n");

    // Initialisation des dimensions
    player->position.w = TILE_SIZE_W_SCALE;  // Utiliser la taille des tuiles
    player->position.h = TILE_SIZE_H_SCALE;
    player->position.x = 0;
    player->position.y = 0;

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

    player->mat = (int **)malloc(sizeof(int *) * MAP_WIDTH);
    for (int i = 0; i < MAP_WIDTH; i++) {
        player->mat[i] = (int *)malloc(sizeof(int) * MAP_HEIGHT);
    }
    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            player->mat[i][j] = 0;
        }
    }

    SDL_FreeSurface(surface);
    return player;
}

void updatePlayerAnimation(Player *player, float deltaTime) {
    if (!player) return;

    // Mise à jour de l'animation
    if (player->isMoving) {
        player->animationTimer += deltaTime;
        if (player->animationTimer >= player->animationSpeed) {
            player->animationTimer = 0;
            player->currentFrameIndex = (player->currentFrameIndex + 1) % FRAME_COUNT;
        }
    } else {
        player->currentFrameIndex = 0;
        player->animationTimer = 0;
    }

    // Calcul de la position dans la spritesheet
    player->currentFrame.x = player->currentFrameIndex * FRAME_WIDTH;
    
    // Mise à jour de la ligne selon l'état
    switch (player->state) {
        case IDLE_DOWN:  player->currentFrame.y = 0; break;
        case IDLE_UP:    player->currentFrame.y = FRAME_HEIGHT; break;
        case IDLE_LEFT:  player->currentFrame.y = 2 * FRAME_HEIGHT; break;
        case IDLE_RIGHT: player->currentFrame.y = 3 * FRAME_HEIGHT; break;
        case WALK_DOWN:  player->currentFrame.y = 4 * FRAME_HEIGHT; break;
        case WALK_UP:    player->currentFrame.y = 5 * FRAME_HEIGHT; break;
        case WALK_LEFT:  player->currentFrame.y = 6 * FRAME_HEIGHT; break;
        case WALK_RIGHT: player->currentFrame.y = 7 * FRAME_HEIGHT; break;
    }

    printf("Update - État: %d, Frame: %d, Position: (%d,%d)\n",
           player->state, player->currentFrameIndex,
           player->position.x, player->position.y);
}

void renderPlayer(SDL_Renderer *renderer, Player *player) {
    if (!player || !player->spriteSheet) {
        printf("Erreur: player ou spritesheet NULL\n");
        return;
    }

    printf("Position: x=%d, y=%d, w=%d, h=%d\n", 
           player->position.x, player->position.y, 
           player->position.w, player->position.h);
    printf("Frame: x=%d, y=%d, w=%d, h=%d\n", 
           player->currentFrame.x, player->currentFrame.y, 
           player->currentFrame.w, player->currentFrame.h);
    printf("État: %d, FrameIndex: %d\n", player->state, player->currentFrameIndex);

    if (SDL_RenderCopy(renderer, player->spriteSheet, &player->currentFrame, &player->position) != 0) {
        printf("Erreur rendu: %s\n", SDL_GetError());
    }
}

void destroyPlayer(Player *player) {
    if (player) {
        if (player->spriteSheet) {
            SDL_DestroyTexture(player->spriteSheet);
        }
        free(player);
    }
}