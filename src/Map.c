#include "../include/Map.h"


Map *initMap(SDL_Renderer *renderer, const char *path) {
    Map *map = (Map *)malloc(sizeof(Map));
    map->mat = (int **)malloc(MAP_WIDTH * sizeof(int *));
    map->rect = (SDL_Rect){0, 0, MAP_WIDTH * TILE_SIZE_W_SCALE, MAP_HEIGHT * TILE_SIZE_H_SCALE};
    for (int i = 0; i < MAP_WIDTH; i++) {
        map->mat[i] = (int *)malloc(MAP_HEIGHT * sizeof(int));
    }
    map->rect = (SDL_Rect){0, 0, MAP_WIDTH * TILE_SIZE_W_SCALE, MAP_HEIGHT * TILE_SIZE_H_SCALE};
    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            if (i == 0 || i == MAP_WIDTH - 1 || j == 0 || j == MAP_HEIGHT - 1) {
                map->mat[i][j] = COLLISION;
            }
            else map->mat[i][j] = AIR;
        }
    }
    map->renderer = renderer;
    SDL_Surface *surface = IMG_Load(path);
    if (!surface) {
        printf("Erreur de chargement de l'image %s : %s\n", path, IMG_GetError());
        return NULL;
    }
    map->texture = SDL_CreateTextureFromSurface(map->renderer, surface);
    if (!map->texture) {
        printf("Erreur de création de la texture : %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return NULL;
    }
    SDL_FreeSurface(surface);
    return map;
}

void DEBUG_printMap(Map *map) {
    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            printf("%d ", map->mat[i][j]);
        }
        printf("\n");
    }
}

void renderMap(Map *map) {
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
    }
    */
}

void destroyMap(Map *map) {
    for (int i = 0; i < MAP_WIDTH; i++) {
        free(map->mat[i]);
    }
    free(map->mat);
    free(map);
}
