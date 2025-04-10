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
 * @author Louis Alban
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

/**
 * @fn Map *initMap(SDL_Renderer *renderer, const char *path, int TileSizeW, int TileSizeH, int *spawnX, int *spawnY)
 * @brief Initialise une carte à partir d'un fichier.
 * 
 * Cette fonction charge une carte à partir d'un fichier et initialise les paramètres de la carte.
 * 
 * @param renderer Pointeur vers le renderer SDL utilisé pour dessiner la carte.
 * @param path Chemin du fichier de la carte à charger.
 * @return Un pointeur vers la structure Map initialisée.
 */
Map *initMap(SDL_Renderer *renderer, const char *path);

/**
 * @fn void scaleMap(Map *map, int originalWidth, int originalHeight, int newWidth, int newHeight)
 * @brief Met à l'échelle la carte en fonction de la taille de la fenêtre.
 * 
 * Cette fonction ajuste la taille de la carte en fonction des dimensions spécifiées.
 * 
 * @param map Pointeur vers la structure Map à mettre à l'échelle.
 * @param originalWidth Largeur d'origine de la carte.
 * @param originalHeight Hauteur d'origine de la carte.
 * @param newWidth Nouvelle largeur de la carte.
 * @param newHeight Nouvelle hauteur de la carte.
 */
void scaleMap(Map *map, int originalWidth, int originalHeight, int newWidth, int newHeight);

/**
 * @fn renderMapDebug(Map *map)
 * @brief cette fonction permet de rendre la carte en mode debug.
 * 
 * @param map 
 */
void renderMapDebug(Map *map);

/**
 * @fn void destroyMap(Map *map)
 * @brief Détruit la carte et libère la mémoire associée.
 * 
 * Cette fonction libère la mémoire allouée pour la carte et ses ressources.
 * 
 * @param map Pointeur vers la structure Map à détruire.
 */
void destroyMap(Map *map);

/**
 * @fn void updateMap(Map *map)
 * @brief Met à jour la carte.
 * 
 * Cette fonction met à jour les paramètres de la carte.
 * 
 * @param map Pointeur vers la structure Map à mettre à jour.
 */
void updateMap(Map *map);

/**
 * @fn void loadMapIMG(Map *map, const char *path)
 * @brief Charge une image de carte à partir d'un fichier.
 * 
 * Cette fonction charge une image de carte à partir d'un fichier et l'associe à la structure Map.
 * 
 * @param map Pointeur vers la structure Map où l'image sera chargée.
 * @param path Chemin du fichier image de la carte à charger.
 */
void loadMapIMG(Map *map, const char *path);

/**
 * @fn void renderMapWithCamera(Map *map, SDL_Renderer *renderer, Camera *camera)
 * @brief Rendu de la carte avec la caméra.
 * 
 * Cette fonction dessine la carte sur le renderer SDL en tenant compte de la position de la caméra.
 * 
 * @param map Pointeur vers la structure Map à dessiner.
 * @param renderer Pointeur vers le renderer SDL utilisé pour dessiner la carte.
 * @param camera Pointeur vers la structure Camera utilisée pour le rendu.
 */
void renderMapWithCamera(Map* map, SDL_Renderer* renderer, Camera* camera);

/**
 * @fn void loadNewMap(Map **map, const char *newMapPath, int mapWidth, int mapHeight, int *spawnX, int *spawnY)
 * @brief Charge une nouvelle carte à partir d'un fichier.
 * 
 * Cette fonction libère la mémoire de la carte actuelle et charge une nouvelle carte à partir d'un fichier.
 * 
 * @param map Pointeur vers un pointeur vers la structure Map à charger.
 * @param newMapPath Chemin du fichier de la nouvelle carte à charger.
 * @param mapWidth Largeur de la nouvelle carte.
 * @param mapHeight Hauteur de la nouvelle carte.
 * @param spawnX Pointeur vers la position X de spawn du joueur.
 * @param spawnY Pointeur vers la position Y de spawn du joueur.
 * @param playerX Position actuelle du joueur en X.
 * @param playerY Position actuelle du joueur en Y.
 */
void loadNewMap(Map **map, const char *newMapPath, int mapWidth, int mapHeight, int *spawnX, int *spawnY, int playerX, int playerY);


// ! DEBUG
void DEBUG_printMap(Map *map);

// Fonctions pour la gestion des positions sauvegardées
/**
 * @fn void saveMapPosition(Map* map, int playerX, int playerY, const char* mapPath)
 * @brief Sauvegarde la position du joueur sur la carte.
 * 
 * Cette fonction enregistre la position actuelle du joueur sur la carte dans un fichier.
 * 
 * @param map Pointeur vers la structure Map contenant les informations de la carte.
 * @param playerX Position X du joueur.
 * @param playerY Position Y du joueur.
 * @param mapPath Chemin du fichier de la carte.
 */
void saveMapPosition(Map* map, int playerX, int playerY, const char* mapPath);

/**
 * @fn MapPosition* loadMapPosition(const char* mapPath)
 * @brief Charge la position du joueur à partir d'un fichier.
 * 
 * Cette fonction lit la position sauvegardée du joueur à partir d'un fichier.
 * 
 * @param mapPath Chemin du fichier de la carte.
 * @return Pointeur vers une structure MapPosition contenant les informations de position.
 */
MapPosition* loadMapPosition(const char* mapPath);

/**
 * @fn void freeMapPosition(MapPosition* position)
 * @brief Libère la mémoire allouée pour une position de carte.
 * 
 * Cette fonction libère la mémoire allouée pour une structure MapPosition.
 * 
 * @param position Pointeur vers la structure MapPosition à libérer.
 */
void freeMapPosition(MapPosition* position);

#endif
