#ifndef MAP_HEADER_H
#define MAP_HEADER_H

#define MAP_WIDTH 64
#define MAP_HEIGHT 40
#define TILE_SIZE_W_SCALE 20 
#define TILE_SIZE_H_SCALE 18

#define AIR 0
#define COLLISION 1


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Camera.h"
typedef struct Map {
    SDL_Rect rect;
    SDL_Rect nameRect;
    SDL_Rect pvRect;
    SDL_Renderer *renderer;
    int **mat;
    SDL_Texture *texture;
} Map;

Map *initMap(SDL_Renderer *renderer, const char *path);
void renderMap(Map *map);
void destroyMap(Map *map);
void updateMap(Map *map);
void loadMapIMG(Map *map, const char *path);
void renderMapWithCamera(Map* map, SDL_Renderer* renderer, Camera* camera);

// ! DEBUG
void DEBUG_printMap(Map *map);

#endif
