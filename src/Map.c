#include "../include/Map.h"


Map *initMap(SDL_Renderer *renderer) {
    Map *map = (Map *)malloc(sizeof(Map));
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

void destroyMap(Map *map) {
    (void)map;
    return;
}
