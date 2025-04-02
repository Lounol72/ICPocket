
#include "../include/GameEngine.h"
static void checkAndLoadNewMap(Map **map, int *playerX, int *playerY) {
    if (!map || !*map || !(*map)->mat) {
        printf("Erreur: map ou map->mat est NULL\n");
        return;
    }

    if (*playerX < 0 || *playerX >= (*map)->tileSizeW || *playerY < 0 || *playerY >= (*map)->tileSizeH) {
        printf("Indices de joueur invalides: playerX=%d, playerY=%d\n", *playerX, *playerY);
        return;
    }

    int spawnX = *playerX;
    int spawnY = *playerY;

    if ((*map)->mat[*playerY][*playerX] == 2) {
        const char *newMapPath = "assets/Tileset/Map/2.png";
        loadNewMap(map, newMapPath, 32, 20, &spawnX, &spawnY);
    } else if ((*map)->mat[*playerY][*playerX] == 3) {
        const char *newMapPath = "assets/Tileset/Map/3.png";
        loadNewMap(map, newMapPath, 32, 20, &spawnX, &spawnY);
    } else if ((*map)->mat[*playerY][*playerX] == 9) {
        const char *newMapPath = "assets/Tileset/Map/hall.png";
        loadNewMap(map, newMapPath, 32, 20, &spawnX, &spawnY);
    }
    *playerX = spawnX;
    *playerY = spawnY;
}

static void updatePhysics(Player *player, Camera* camera, Map *map, float deltaTime) {
    updatePlayerPosition(player, deltaTime);
    updateCamera(camera, player->position.x, player->position.y, deltaTime);
    updatePlayerAnimation(player, deltaTime);
    checkAndLoadNewMap(&map, &player->matrixX, &player->matrixY);
    
}
/**
 * @brief Initialise le gestionnaire de threads
 * 
 * @param game Pointeur vers la structure Game
 * Initialise les différent verrous mutex pour les ressourcers partagées
 * et crée les threads parallèles
 */
void initThreadManager(Game* game) {
    if (!game) return;
    
    pthread_mutex_init(&game->threadManager.audioMutex, NULL);
    pthread_mutex_init(&game->threadManager.physicsMutex, NULL);
    
    game->threadManager.isRunning = 1;
    
    pthread_create(&game->threadManager.audioThread, NULL, audioThreadFunction, game);

    pthread_create(&game->threadManager.physicsThread, NULL, physicsThreadFunction, game);
}

void* audioThreadFunction(void* arg) {
    Game* game = (Game*)arg;
    while (game->threadManager.isRunning) {
        int frameStart = SDL_GetTicks();
        pthread_mutex_lock(&game->threadManager.audioMutex);
        updateMusic();
        pthread_mutex_unlock(&game->threadManager.audioMutex);
        manageFrameRate(frameStart);
    }
    return NULL;
}
/*
void* renderThreadFunction(void* arg) {
    Game* game = (Game*)arg;
    while (game->threadManager.isRunning) {
        int frameStart = SDL_GetTicks();
        pthread_mutex_lock(&game->threadManager.renderMutex);
        if (game->gameState.currentState != MAP) {
            SDL_RenderClear(game->win->renderer);
            render(game->win);
            updateCurrentButton();
            SDL_RenderPresent(game->win->renderer);
        }
        pthread_mutex_unlock(&game->threadManager.renderMutex);
        manageFrameRate(frameStart);
    }
    return NULL;
}*/

void* physicsThreadFunction(void* arg) {
    Game* game = (Game*)arg;
    Uint32 lastTime = SDL_GetTicks();
    
    while (game->threadManager.isRunning) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        
        pthread_mutex_lock(&game->threadManager.physicsMutex);
        updatePhysics(game->gameData.player, game->gameData.camera, game->gameData.map, deltaTime);
        pthread_mutex_unlock(&game->threadManager.physicsMutex);
        
        manageFrameRate(currentTime);
    }
    return NULL;
}

void cleanupThreads(Game* game) {
    if (!game) return;
    
    game->threadManager.isRunning = 0;
    pthread_join(game->threadManager.audioThread, NULL);
    pthread_join(game->threadManager.physicsThread, NULL);
    pthread_mutex_destroy(&game->threadManager.audioMutex);
    pthread_mutex_destroy(&game->threadManager.physicsMutex);
}