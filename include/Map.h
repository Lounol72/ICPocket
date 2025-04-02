#ifndef MAP_HEADER_H
#define MAP_HEADER_H

#define MAP_WIDTH 32
#define MAP_HEIGHT 20
#define TILE_SIZE_W_SCALE 40
#define TILE_SIZE_H_SCALE 36
#define TILE_SIZE_W 80
#define TILE_SIZE_H 72

// Définitions des types de cases
#define AIR 0
#define COLLISION 1
#define DUEL 6
#define MAP_TRANSITION_ID_1 2  // ID pour la première map alternative
#define MAP_TRANSITION_ID_2 3  // ID pour la deuxième map alternative
#define MAP_TRANSITION_ID_3 9  // ID pour la troisième map alternative

#include <SDL2/SDL_image.h>

#include "Camera.h"

/**
 * @file Map.h
 * @author Louis
 * @date 25/02/2025
 */

typedef struct Map{
	int **mat;
	int positions[2];
	int taille[2];
	char texturePath[256];
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Rect rect;
} Map;

typedef struct MapPosition {
	char mapPath[256];  // Chemin de la map
	int x;             // Position X du joueur
	int y;             // Position Y du joueur
} MapPosition;

Map *initMap(SDL_Renderer *renderer, const char *path);
void scaleMap(Map *map, int originalWidth, int originalHeight, int newWidth, int newHeight);
void renderMapDebug(Map *map);
void destroyMap(Map *map);
void updateMap(Map *map);
void loadMapIMG(Map *map, const char *path);
void renderMapWithCamera(Map* map, SDL_Renderer* renderer, Camera* camera);
void loadNewMap(Map **map, const char *newMapPath, int mapWidth, int mapHeight, int *spawnX, int *spawnY, int playerX, int playerY);

// ! DEBUG
void DEBUG_printMap(Map *map);

// Fonctions pour la gestion des positions sauvegardées
void saveMapPosition(Map* map, int playerX, int playerY, const char* mapPath);
MapPosition* loadMapPosition(const char* mapPath);
void freeMapPosition(MapPosition* position);

#endif
