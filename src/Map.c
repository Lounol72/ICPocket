#include "../include/Map.h"
#include "../include/Camera.h"


Map *initMap(SDL_Renderer *renderer, const char *path) {
    Map *map = (Map *)malloc(sizeof(Map));
    if (!map) return NULL;

    map->mat = (int **)malloc(MAP_HEIGHT * sizeof(int *));
    map->rect = (SDL_Rect){0, 0, MAP_WIDTH * TILE_SIZE_W_SCALE, MAP_HEIGHT * TILE_SIZE_H_SCALE};
    for (int i = 0; i < MAP_HEIGHT; i++) {
        map->mat[i] = (int *)malloc(MAP_WIDTH * sizeof(int));
    }
    map->rect = (SDL_Rect){0, 0, MAP_WIDTH * TILE_SIZE_W_SCALE, MAP_HEIGHT * TILE_SIZE_H_SCALE};
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (i == 0 || i == MAP_HEIGHT - 1 || j == 0 || j == MAP_WIDTH - 1) {
                map->mat[i][j] = COLLISION;
            }
            else map->mat[i][j] = AIR;
        }
    }
    map->renderer = renderer;

    // Charger la texture
    SDL_Surface *surface = IMG_Load(path);
    if (!surface) {
        printf("Erreur chargement texture map: %s\n", IMG_GetError());
        free(map);
        return NULL;
    }

    map->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!map->texture) {
        printf("Erreur création texture map: %s\n", SDL_GetError());
        free(map);
        return NULL;
    }

    return map;
}

void DEBUG_printMap(Map *map) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            printf("%d ", map->mat[i][j]);
        }
        printf("\n");
    }
}

void renderMapDebug(Map *map) {
    if (!map->texture) {
        printf("Erreur: Texture de la map non initialisée\n");
        return;
    }
    SDL_RenderCopy(map->renderer, map->texture, NULL, &map->rect);
    // ! DEBUG
    //* Render the grid of the map
    /*
    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            SDL_SetRenderDrawColor(map->renderer, 255, 255, 255, 255);
            SDL_RenderDrawLine(map->renderer, i * TILE_SIZE_W_SCALE, j * TILE_SIZE_H_SCALE, (i + 1) * TILE_SIZE_W_SCALE, j * TILE_SIZE_H_SCALE);
            SDL_RenderDrawLine(map->renderer, i * TILE_SIZE_W_SCALE, j * TILE_SIZE_H_SCALE, i * TILE_SIZE_W_SCALE, (j + 1) * TILE_SIZE_H_SCALE);
        }
    }*/
    
}

void renderMapWithCamera(Map* map, SDL_Renderer* renderer, Camera* camera) {
    // D'abord, rendre la texture de la map complète avec la caméra
    SDL_Rect worldRect = {
        0,
        0,
        MAP_WIDTH * TILE_SIZE_W_SCALE,
        MAP_HEIGHT * TILE_SIZE_H_SCALE
    };
    SDL_Rect screenRect = getWorldToScreenRect(camera, worldRect);
    
    // Rendre la texture de la map
    if (map->texture) {
        SDL_RenderCopy(renderer, map->texture, NULL, &screenRect);
    }

    // ! DEBUG
    /*

    // Pour le debug, rendre les collisions
    int startTileX = (int)(camera->x / TILE_SIZE_W_SCALE);
    int startTileY = (int)(camera->y / TILE_SIZE_H_SCALE);
    int endTileX = startTileX + (int)(WINDOW_WIDTH / (TILE_SIZE_W_SCALE * camera->scale)) + 2;
    int endTileY = startTileY + (int)(WINDOW_HEIGHT / (TILE_SIZE_H_SCALE * camera->scale)) + 2;

    // Limiter aux dimensions de la map
    startTileX = (startTileX < 0) ? 0 : startTileX;
    startTileY = (startTileY < 0) ? 0 : startTileY;
    endTileX = (endTileX > MAP_WIDTH) ? MAP_WIDTH : endTileX;
    endTileY = (endTileY > MAP_HEIGHT) ? MAP_HEIGHT : endTileY;

    
    // Rendre les collisions par-dessus la map
    
    for(int y = startTileY; y < endTileY; y++) {
        for(int x = startTileX; x < endTileX; x++) {
            if(map->mat[y][x] == COLLISION) {
                
                SDL_Rect collisionRect = {
                    x * TILE_SIZE_W_SCALE,
                    y * TILE_SIZE_H_SCALE,
                    TILE_SIZE_W_SCALE,
                    TILE_SIZE_H_SCALE
                };
                
                SDL_Rect screenCollisionRect = getWorldToScreenRect(camera, collisionRect);
                
                // Rendre uniquement le contour des collisions en mode debug
                //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &screenCollisionRect);
                
            }
        }
    }
    */
}

void destroyMap(Map *map) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        free(map->mat[i]);
    }
    free(map->mat);
    free(map);
}
