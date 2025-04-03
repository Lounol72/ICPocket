#include "../include/Player.h"
#include <stdio.h>



#define ANIMATION_SPEED 0.10f // Temps entre chaque frame
#define FRAMES_PER_ANIMATION 4 // Nombre de frames par animation
/**
 * @fn void initPlayerMat(Player *player)
 * @brief Initialise la matrice de collision du joueur.
 * 
 * Cette fonction alloue de la mémoire pour la matrice de collision du joueur et initialise toutes les valeurs à 0.
 * 
 * @param player Pointeur vers le joueur à initialiser. 
 */
static inline void initPlayerMat(Player *player){
    player->mat = NULL;
    player->mat = (int **)malloc(sizeof(int *) * player->sizeMapH);
    if (!player->mat) {
        SDL_Log("Erreur allocation mémoire pour player->mat\n");
        SDL_DestroyTexture(player->spriteSheet);
        free(player);
        return;
    }

    for (int i = 0; i < player->sizeMapH; i++) {
        player->mat[i] = (int *)malloc(sizeof(int) * player->sizeMapW);
        if (!player->mat[i]) {
            SDL_Log("Erreur allocation mémoire pour player->mat[%d]\n", i);
            for (int j = 0; j < i; j++) {
                free(player->mat[j]);
            }
            free(player->mat);
            SDL_DestroyTexture(player->spriteSheet);
            free(player);
            return;
        }
        for (int j = 0; j < player->sizeMapW; j++) {
            player->mat[i][j] = 0;
        }
    }
}

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
Player* createPlayer(SDL_Renderer *renderer, char *spritesheetPath, Map * map, int mapIndex) {
    Player *player = malloc(sizeof(Player));
    if (!player) {
        SDL_Log("Erreur allocation player\n");
        return NULL;
    }

    SDL_Surface *surface = IMG_Load(spritesheetPath);
    if (!surface) {
        SDL_Log("Erreur chargement spritesheet: %s\n", IMG_GetError());
        free(player);
        return NULL;
    }

    player->spriteSheet = SDL_CreateTextureFromSurface(renderer, surface);
    if (!player->spriteSheet) {
        SDL_Log("Erreur création texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        free(player);
        return NULL;
    }


    // Utiliser les coordonnées de spawn
    player->matrixX = map->positions[0];
    player->matrixY = map->positions[1];
    player->mapIndex = mapIndex;

    // Calculer la position en pixels avec le bon facteur d'échelle
    player->position.w = TILE_SIZE_W_SCALE;
    player->position.h = TILE_SIZE_H_SCALE;
    player->position.x = player->matrixX * TILE_SIZE_W_SCALE - (player->position.w - TILE_SIZE_W_SCALE) / 2; // Centrer après agrandissement
    player->position.y = player->matrixY * TILE_SIZE_H_SCALE - (player->position.h - TILE_SIZE_H_SCALE) / 2; // Centrer après agrandissement

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

    player->interpolationTime = 0.0f;
    player->moveSpeed = 0.5f;  // Augmenté à 0.4 secondes par case (était 0.2)
    player->targetMatrixX = player->matrixX;
    player->targetMatrixY = player->matrixY;
    player->startX = player->position.x;
    player->startY = player->position.y;
    player->targetX = player->position.x;
    player->targetY = player->position.y;
    player->isMovingToTarget = false;
    player->sizeMapH = map->taille[1];
    player->sizeMapW = map->taille[0];
    initPlayerMat(player);
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

/**
 * @fn void updatePlayerAnimation(Player *player, float deltaTime)
 * @brief Met à jour l'animation du joueur en fonction du temps écoulé.
 * 
 * Cette fonction met à jour l'animation du joueur en fonction du temps écoulé depuis la dernière mise à jour.
 * 
 * @param player Pointeur vers le joueur à mettre à jour.
 * @param deltaTime Temps écoulé depuis la dernière mise à jour.
 */
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

/**
 * @fn void renderPlayer(SDL_Renderer *renderer, Player *player)
 * @brief Rendu du joueur sur l'écran.
 * 
 * Cette fonction dessine le joueur sur l'écran en utilisant le rendu SDL.
 * 
 * @param renderer Pointeur vers le rendu SDL.
 * @param player Pointeur vers le joueur à dessiner.
 */
void renderPlayer(SDL_Renderer *renderer, Player *player) {
    if (!player || !player->spriteSheet) {
        SDL_Log("Erreur: player ou spritesheet NULL\n");
        return;
    }
    
    // Calculer le rectangle de destination avec le sprite agrandi
    SDL_Rect destRect = player->position;
    
    if (SDL_RenderCopy(renderer, player->spriteSheet, &player->currentFrame, &destRect) != 0) {
        SDL_Log("Erreur rendu: %s\n", SDL_GetError());
    }
}

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
void renderPlayerWithCamera(Player* player, SDL_Renderer* renderer, Camera* camera) {
    // Récupérer le rectangle du monde pour le joueur
    SDL_Rect worldRect = player->position;
    
    // Convertir en coordonnées d'écran
    SDL_Rect screenRect = getWorldToScreenRect(camera, worldRect);
    
    if (renderer && player->spriteSheet)
        SDL_RenderCopy(renderer, player->spriteSheet, &player->currentFrame, &screenRect);
}

/**
 * @fn void destroyPlayer(Player *player)
 * @brief Détruit le joueur et libère les ressources associées.
 * 
 * Cette fonction libère la mémoire associée au joueur et à sa spritesheet.
 * 
 * @param player Pointeur vers le joueur à détruire.
 */
void destroyPlayer(Player *player) {
    if (!player) return;
    if (!player->spriteSheet) return;
    SDL_DestroyTexture(player->spriteSheet);
    player->spriteSheet = NULL;
    if (player->mat != NULL) {
        for (int i = 0; i < player->sizeMapH; i++) {
            if (player->mat[i] != NULL) {
                free(player->mat[i]);
                player->mat[i] = NULL;
            }
        }
        free(player->mat);
        player->mat = NULL;
    }
    free(player);
    player = NULL;
    
}

/**
 * @fn void updatePlayerPosition(Player *player, float deltaTime)
 * @brief Met à jour la position du joueur en fonction de son état et du temps écoulé.
 * 
 * Cette fonction met à jour la position du joueur en fonction de son état (déplacement ou non) et du temps écoulé.
 * 
 * @param player Pointeur vers le joueur à mettre à jour.
 * @param deltaTime Temps écoulé depuis la dernière mise à jour.
 */
void updatePlayerPosition(Player *player, float deltaTime) {
    if (player->isMovingToTarget) {
        player->interpolationTime += deltaTime;
        float t = player->interpolationTime / player->moveSpeed;

        if (t > 1.0f) t = 1.0f;

        float newX = player->startX + (player->targetX - player->startX) * t;
        float newY = player->startY + (player->targetY - player->startY) * t;
        
        // Mettre à jour la position avec le décalage pour centrer le sprite agrandi
        player->position.x = (int)newX - (player->position.w - TILE_SIZE_W_SCALE) / 2;
        player->position.y = (int)newY - (player->position.h - TILE_SIZE_H_SCALE) / 2;

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

/**
 * @fn void updatePlayerSpawn(Player *player, Map *map, int spawnX, int spawnY)
 * @brief Met à jour la position de départ du joueur dans la matrice de collision.
 * 
 * Cette fonction met à jour la position de départ du joueur dans la matrice de collision et réinitialise les variables de mouvement.
 * 
 * @param player Pointeur vers le joueur à mettre à jour.
 * @param map Pointeur vers la carte du jeu.
 * @param spawnX Position de départ X du joueur.
 * @param spawnY Position de départ Y du joueur. 
 */
void updatePlayerSpawn(Player *player, Map *map, int spawnX, int spawnY) {
    if (!player || !map) return;

    // Mettre à jour les coordonnées matricielles
    player->matrixX = spawnX;
    player->matrixY = spawnY;

    // Mettre à jour la position en pixels
    player->position.x = spawnX * TILE_SIZE_W_SCALE;
    player->position.y = spawnY * TILE_SIZE_H_SCALE;

    // Réinitialiser les variables de mouvement
    player->targetMatrixX = spawnX;
    player->targetMatrixY = spawnY;
    player->startX = player->position.x;
    player->startY = player->position.y;
    player->targetX = player->position.x;
    player->targetY = player->position.y;
    player->isMovingToTarget = false;
    player->interpolationTime = 0.0f;

    // Mettre à jour les dimensions de la map
    player->sizeMapH = map->taille[1];
    player->sizeMapW = map->taille[0];

    // Réinitialiser la matrice de collision
    for (int i = 0; i < player->sizeMapH; i++) {
        for (int j = 0; j < player->sizeMapW; j++) {
            player->mat[i][j] = 0;
        }
    }
}

/**
 * @fn void forceUpdatePlayerAndCamera(Player *player, Camera *camera, Map *map)
 * @brief Force la mise à jour de la position du joueur et de la caméra.
 * 
 * Cette fonction met à jour la position du joueur et de la caméra en forçant les valeurs cibles.
 * 
 * @param player Pointeur vers le joueur à mettre à jour.
 * @param camera Pointeur vers la caméra à mettre à jour.
 * @param map Pointeur vers la carte du jeu.
 */
void forceUpdatePlayerAndCamera(Player *player, Camera *camera, Map *map) {
    if (!player || !camera || !map) return;
    
    // Mettre à jour les coordonnées matricielles
    player->matrixX = player->targetMatrixX;
    player->matrixY = player->targetMatrixY;
    
    // Mettre à jour la position en pixels exactement
    player->position.x = player->matrixX * TILE_SIZE_W_SCALE;
    player->position.y = player->matrixY * TILE_SIZE_H_SCALE;
    
    // Réinitialiser tous les paramètres de mouvement
    player->startX = player->position.x;
    player->startY = player->position.y;
    player->targetX = player->position.x;
    player->targetY = player->position.y;
    player->isMovingToTarget = false;
    player->interpolationTime = 0.0f;
    
    // Forcer la position de la caméra pour être centrée sur le joueur
    camera->x = player->position.x;
    camera->y = player->position.y;
    
    // Mettre à jour l'animation en position statique
    switch (player->state) {
        case WALK_DOWN: player->state = IDLE_DOWN; break;
        case WALK_UP: player->state = IDLE_UP; break;
        case WALK_LEFT: player->state = IDLE_LEFT; break;
        case WALK_RIGHT: player->state = IDLE_RIGHT; break;
        default: break;
    }
    
    player->currentFrameIndex = 0;
    player->animationTimer = 0;
    
    // Mettre à jour la frame actuelle
    switch (player->state) {
        case IDLE_DOWN: player->currentFrame.y = 0; break;
        case IDLE_LEFT: player->currentFrame.y = 2 * FRAME_HEIGHT; break;
        case IDLE_RIGHT: player->currentFrame.y = 3 * FRAME_HEIGHT; break;
        case IDLE_UP: player->currentFrame.y = FRAME_HEIGHT; break;
        default: break;
    }
    player->currentFrame.x = 0;
}
