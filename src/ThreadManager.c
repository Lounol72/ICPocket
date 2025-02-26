#include "../include/ThreadManager.h"
#include "../include/GameEngine.h"


/**
 * @brief Initialise le gestionnaire de threads
 * 
 * @param game Pointeur vers la structure Game
 * Initialise les différent verrous mutex pour les ressourcers partagées
 * et crée les threads parallèles
 */
void initThreadManager(Game* game) {
    // Vérifie d'abord si le pointeur game est valide, retourne si null
    if (!game) return;
    
    // Initialise les verrous mutex qui protégeront les ressources partagées :
    // - audioMutex pour les opérations audio
    // - renderMutex pour les opérations de rendu
    // - physicsMutex pour les calculs de physique/mouvement
    pthread_mutex_init(&game->threadManager.audioMutex, NULL);
    pthread_mutex_init(&game->threadManager.renderMutex, NULL);
    pthread_mutex_init(&game->threadManager.physicsMutex, NULL);
    
    // Configure le drapeau d'exécution à vrai pour que les threads démarrent leurs boucles
    game->threadManager.isRunning = 1;
    
    // Crée 3 threads pour le traitement parallèle :
    // 1. Thread audio - gère les mises à jour de la musique/son
    // 2. Thread de rendu - gère le rendu graphique
    // 3. Thread physique - gère les mouvements et les collisions
    // Chaque thread reçoit le pointeur game comme argument
    pthread_create(&game->threadManager.audioThread, NULL, audioThreadFunction, game);
    pthread_create(&game->threadManager.renderThread, NULL, renderThreadFunction, game);
    pthread_create(&game->threadManager.physicsThread, NULL, physicsThreadFunction, game);
}

void* audioThreadFunction(void* arg) {
    Game* game = (Game*)arg;
    while (game->threadManager.isRunning) {
        pthread_mutex_lock(&game->threadManager.audioMutex);
        updateMusic();
        pthread_mutex_unlock(&game->threadManager.audioMutex);
        SDL_Delay(16); // ~60fps
    }
    return NULL;
}

void* renderThreadFunction(void* arg) {
    Game* game = (Game*)arg;
    while (game->threadManager.isRunning) {
        pthread_mutex_lock(&game->threadManager.renderMutex);
        render(game->win);
        SDL_RenderPresent(game->win->renderer);
        pthread_mutex_unlock(&game->threadManager.renderMutex);
        SDL_Delay(16); // ~60fps
    }
    return NULL;
}

void* physicsThreadFunction(void* arg) {
    Game* game = (Game*)arg;
    while (game->threadManager.isRunning) {
        pthread_mutex_lock(&game->threadManager.physicsMutex);
        if (game->gameState.currentState == MAP) {
            updatePlayerPosition(game->gameData.player, game->deltaTime);
            updateCamera(game->gameData.camera, game->gameData.player->position.x, 
                        game->gameData.player->position.y, game->deltaTime);
        }
        pthread_mutex_unlock(&game->threadManager.physicsMutex);
        SDL_Delay(16); // ~60fps
    }
    return NULL;
}

void cleanupThreads(Game* game) {
    if (!game) return;
    
    game->threadManager.isRunning = 0;
    pthread_join(game->threadManager.audioThread, NULL);
    pthread_join(game->threadManager.renderThread, NULL);
    pthread_join(game->threadManager.physicsThread, NULL);

    pthread_mutex_destroy(&game->threadManager.audioMutex);
    pthread_mutex_destroy(&game->threadManager.renderMutex);
    pthread_mutex_destroy(&game->threadManager.physicsMutex);
}