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
//temporaire

void loadNewMap(Map **map, const char *newMapPath, int mapWidth, int mapHeight, int *spawnX, int *spawnY) {
    printf("Chargement de la nouvelle map: %s\n", newMapPath);

    // Détruire l'ancienne map
    destroyMap(*map);

    // Initialiser la nouvelle map
    *map = initMap((*map)->renderer, newMapPath, mapWidth, mapHeight, spawnX, spawnY);
    if (!*map) {
        printf("Erreur chargement nouvelle map\n");
        return;
    }

    // Rechercher les cases -1 dans la nouvelle map pour définir les coordonnées de spawn
    for (int i = 0; i < (*map)->tileSizeH; i++) {
        for (int j = 0; j < (*map)->tileSizeW; j++) {
            if ((*map)->mat[i][j] == -1) {
                *spawnX = j;
                *spawnY = i;
                printf("Coordonnées de spawn trouvées: [%d, %d]\n", *spawnX, *spawnY);
                return;
            }
        }
    }

    printf("Aucune case -1 trouvée, les coordonnées de spawn restent inchangées.\n");
}

static void initCollisionMapFromCSV(Map *map, const char *path, int *spawnX, int *spawnY) {
    FILE *file = fopen(path, "r");
    if (!file) {
        printf("Erreur ouverture fichier CSV: %s\n", path);
        return;
    }
    
    char line[1024];  // Buffer pour lire chaque ligne
    int i = 0;
    
    // Initialiser toute la matrice à 0 d'abord
    for (int y = 0; y < map->tileSizeH; y++) {
        for (int x = 0; x < map->tileSizeW; x++) {
            map->mat[y][x] = 0;
        }
    }
    
    // Lire le fichier ligne par ligne
    while (fgets(line, sizeof(line), file) && i < map->tileSizeH) {
        char *token = strtok(line, ",");
        int j = 0;
        while (token && j < map->tileSizeW) {
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

Map *initMap(SDL_Renderer *renderer, const char *path, int TileSizeW, int TileSizeH, int *spawnX, int *spawnY) {


    Map *map = (Map *)malloc(sizeof(Map));
    if (!map) {
        printf("Erreur allocation mémoire pour map\n");
        return NULL;
    }
    
    // Initialiser les dimensions avant d'allouer la matrice
    map->tileSizeW = TileSizeW;
    map->tileSizeH = TileSizeH;
    map->renderer = renderer;
    
    // Allouer la matrice
    map->mat = (int **)malloc(TileSizeH * sizeof(int *));
    if (!map->mat) {
        printf("Erreur allocation mémoire pour map->mat\n");
        free(map);
        return NULL;
    }
    
    for (int i = 0; i < TileSizeH; i++) {
        map->mat[i] = (int *)calloc(TileSizeW, sizeof(int));  // Utiliser calloc pour initialiser à 0
        if (!map->mat[i]) {
            printf("Erreur allocation mémoire pour map->mat[%d]\n", i);
            for (int j = 0; j < i; j++) {
                free(map->mat[j]);
            }
            free(map->mat);
            free(map);
            return NULL;
        }
    }
    
    // Configurer les dimensions et le rectangle
    map->width = TileSizeW;
    map->height = TileSizeH;
    
    // Ajuster le rectangle de rendu pour respecter les dimensions du CSV
    map->rect = (SDL_Rect){
        0, 
        0, 
        20 * TILE_SIZE_H_SCALE,
        32 * TILE_SIZE_W_SCALE
    };

    // Charger le fichier CSV
    char *csvPath = (char *)malloc(strlen(path) + 1);
    if (!csvPath) {
        printf("Erreur allocation mémoire pour csvPath\n");
        destroyMap(map);
        return NULL;
    }
    strcpy(csvPath, path);
    char *dot = strrchr(csvPath, '.');
    if (dot) {
        strcpy(dot, ".csv");
        printf("Chargement du fichier CSV: %s\n", csvPath);
        initCollisionMapFromCSV(map, csvPath, spawnX, spawnY);
    }
    free(csvPath);

    // Charger la texture
    SDL_Surface *surface = IMG_Load(path);
    if (!surface) {
        printf("Erreur chargement texture map: %s\n", IMG_GetError());
        destroyMap(map);
        return NULL;
    }

    map->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!map->texture) {
        printf("Erreur création texture map: %s\n", SDL_GetError());
        destroyMap(map);
        return NULL;
    }

    return map;
}




void DEBUG_printMap(Map *map) {
    for (int i = 0; i < map->tileSizeH; i++) {
        for (int j = 0; j < map->tileSizeW; j++) {
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
    int windowWidth, windowHeight;
    SDL_GetWindowSize(SDL_GetWindowFromID(1), &windowWidth, &windowHeight);
    SDL_Rect worldRect = {
        0,
        0,
        map->width * (int)(windowWidth / map->tileSizeW),
        map->height * (int)(windowHeight / map->tileSizeH)
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

void destroyMap(Map *map) {
    if (map) {
        if (map->mat) {
            for (int i = 0; i < map->tileSizeH; i++) {
                free(map->mat[i]);
            }
            free(map->mat);
        }
        if (map->texture) {
            SDL_DestroyTexture(map->texture);
        }
        free(map);
    }
}




