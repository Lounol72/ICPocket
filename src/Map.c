#include "../include/Map.h"
#include <stdio.h>
#include <string.h>



/*
static void initCollisionMap(Map *map) {
    for (int i = 0; i < map->tileSizeH; i++) {
        for (int j = 0; j < map->tileSizeW; j++) {
            if (i == 0 || i == map->tileSizeH - 1 || j == 0 || j == map->tileSizeW - 1) {
                map->mat[i][j] = COLLISION;
            }
            else map->mat[i][j] = AIR;
        }
    }
}*/

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
//temporaire
/*
void loadNewMap(Map **map, const char *newMapPath, int mapWidth, int mapHeight, int *spawnX, int *spawnY, int playerX, int playerY) {
    // Sauvegarder la position actuelle du joueur dans l'ancienne map
    if (*map) {
        (*map)->spawnX = playerX;
        (*map)->spawnY = playerY;
    }

    // Détruire l'ancienne map
    destroyMap(*map);

    // Initialiser la nouvelle map
    *map = initMap((*map)->renderer, newMapPath, mapWidth, mapHeight, spawnX, spawnY);
    if (!*map) {
        SDL_Log("Erreur chargement nouvelle map\n");
        return;
    }

    // Si la map a déjà été visitée, utiliser la dernière position enregistrée
    if ((*map)->spawnX != -1 && (*map)->spawnY != -1) {
        *spawnX = (*map)->spawnX;
        *spawnY = (*map)->spawnY;
    }
}*/

static void initCollisionMapFromCSV(Map *map, const char *path, int *spawnX, int *spawnY) {
    FILE *file = fopen(path, "r");
    if (!file) {
        SDL_Log("Erreur ouverture fichier CSV: %s\n", path);
        return;
    }
    
    char line[1024];  // Buffer pour lire chaque ligne
    int i = 0;
    
    // Initialiser toute la matrice à 0 d'abord
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map->mat[y][x] = 1;
        }
    }
    
    // Lire le fichier ligne par ligne
    while (fgets(line, sizeof(line), file) && i < MAP_HEIGHT) {
        char *token = strtok(line, ",");
        int j = 0;
        while (token && j < MAP_WIDTH) {
            int value = atoi(token);
            map->mat[i][j] = value;
            if (value == -1) {
                *spawnX = j;
                *spawnY = i;
            }
            token = strtok(NULL, ",");
            j++;
        }
        i++;
    }
    
    fclose(file);
}
static inline void initMapMat(Map *map) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        map->mat[i] = (int *)calloc(MAP_WIDTH, sizeof(int));  // Utiliser calloc pour initialiser à 0
        if (!map->mat[i]) {
            SDL_Log("Erreur allocation mémoire pour map->mat[%d]\n", i);
            for (int j = 0; j < i; j++) {
                free(map->mat[j]);
            }
            free(map->mat);
            free(map);
            return;
        }
    }
}

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
Map *initMap(SDL_Renderer *renderer, const char *path) {
    int spawnX = -1;
    int spawnY = -1;
    Map *map = (Map *)malloc(sizeof(Map));
    if (!map) {
        SDL_Log("Erreur allocation mémoire pour map\n");
        return NULL;
    }
    
    // Initialiser les dimensions de la carte
    map->taille[0] = MAP_WIDTH;
    map->taille[1] = MAP_HEIGHT;
    
    map->mat = (int **)malloc(MAP_HEIGHT * sizeof(int *));
    if (!map->mat) {
        SDL_Log("Erreur allocation mémoire pour map->mat\n");
        free(map);
        return NULL;
    }
    initMapMat(map);
    // Ajuster le rectangle de rendu pour respecter les dimensions du CSV
    map->rect = (SDL_Rect){
        0, 
        0, 
        MAP_WIDTH * TILE_SIZE_H_SCALE,
        MAP_HEIGHT * TILE_SIZE_W_SCALE
    };
    // Charger le fichier CSV
    char *csvPath = (char *)malloc(strlen(path) + 1);
    if (!csvPath) {
        SDL_Log("Erreur allocation mémoire pour csvPath\n");
        destroyMap(map);
        return NULL;
    }
    strcpy(csvPath, path);
    char *dot = strrchr(csvPath, '.');
    if (dot) {
        strcpy(dot, ".csv");
        initCollisionMapFromCSV(map, csvPath, &spawnX, &spawnY);
        // Sauvegarder les positions de spawn dans la structure map
        map->positions[0] = spawnX;
        map->positions[1] = spawnY;
    }
    free(csvPath);

    map->renderer = renderer;
    // Charger la texture
    SDL_Surface *surface = IMG_Load(path);
    if (!surface) {
        SDL_Log("Erreur chargement texture map: %s\n", IMG_GetError());
        destroyMap(map);
        return NULL;
    }

    map->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!map->texture) {
        SDL_Log("Erreur création texture map: %s\n", SDL_GetError());
        destroyMap(map);
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

/**
 * @fn renderMapDebug(Map *map)
 * @brief cette fonction permet de rendre la carte en mode debug.
 * 
 * @param map 
 */
void renderMapDebug(Map *map) {
    if (!map->texture) {

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
void renderMapWithCamera(Map* map, SDL_Renderer* renderer, Camera* camera) {
    // D'abord, rendre la texture de la map complète avec la caméra
    SDL_Rect worldRect = {
        0,
        0,
        MAP_WIDTH * TILE_SIZE_W_SCALE,   // Largeur réelle de la carte en pixels
        MAP_HEIGHT * TILE_SIZE_H_SCALE   // Hauteur réelle de la carte en pixels
    };
    SDL_Rect screenRect = getWorldToScreenRect(camera, worldRect);
    
    // Rendre la texture de la map
    if (map->texture != NULL) {
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

/**
 * @fn void destroyMap(Map *map)
 * @brief Détruit la carte et libère la mémoire associée.
 * 
 * Cette fonction libère la mémoire allouée pour la carte et ses ressources.
 * 
 * @param map Pointeur vers la structure Map à détruire.
 */
void destroyMap(Map *map) {
    if (map) {
        if (map->mat) {
            for (int i = 0; i < MAP_HEIGHT; i++) {
                free(map->mat[i]);
                map->mat[i] = NULL;
            }
            free(map->mat);
            map->mat = NULL;
        }
        if (map->texture) {
            SDL_DestroyTexture(map->texture);
            map->texture = NULL;
        }
        free(map);
        map = NULL;
    }
}

