#ifndef MAP_HEADER_H
#define MAP_HEADER_H

#define MAP_WIDTH 32
#define MAP_HEIGHT 20
#define TILE_SIZE_W_SCALE 40
#define TILE_SIZE_H_SCALE 36



#define AIR 0
#define COLLISION 1

#include <SDL2/SDL_image.h>
#include "Camera.h"

/**
 * @file Map.h
 * @author Louis
 * @date 25/02/2025
 */

typedef struct Map {
	SDL_Rect rect;
	SDL_Rect nameRect;
	SDL_Rect pvRect;
	SDL_Renderer *renderer;
	int **mat;
	SDL_Texture *texture;
	int width;  
	int height; 
} Map;

Map *initMap(SDL_Renderer *renderer, const char *path, int TileSizeW, int TileSizeH);
void renderMapDebug(Map *map);
void destroyMap(Map *map);
void updateMap(Map *map);
void loadMapIMG(Map *map, const char *path);
void renderMapWithCamera(Map* map, SDL_Renderer* renderer, Camera* camera);
void checkAndLoadNewMap(Map **map, int playerX, int playerY);

// ! DEBUG
void DEBUG_printMap(Map *map);

#endif
