#ifndef MAP_HEADER_H
#define MAP_HEADER_H

#define MAP_WIDTH 32
#define MAP_HEIGHT 20
#define TILE_SIZE_W_SCALE 40
#define TILE_SIZE_H_SCALE 36
#define TILE_SIZE_W 80
#define TILE_SIZE_H 72


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
	int tileSizeW;
	int tileSizeH;
} Map;

Map *initMap(SDL_Renderer *renderer, const char *path, int TileSizeW, int TileSizeH, int *spawnX, int *spawnY);
void scaleMap(Map *map, int originalWidth, int originalHeight, int newWidth, int newHeight);
void renderMapDebug(Map *map);
void destroyMap(Map *map);
void updateMap(Map *map);
void loadMapIMG(Map *map, const char *path);
void renderMapWithCamera(Map* map, SDL_Renderer* renderer, Camera* camera);
void loadNewMap(Map **map, const char *newMapPath, int mapWidth, int mapHeight, int *spawnX, int *spawnY);

// ! DEBUG
void DEBUG_printMap(Map *map);

#endif
