
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
    pthread_mutex_init(&game->threadManager.physicsMutex, NULL);
    //pthread_mutex_init(&game->threadManager.renderMutex, NULL);
    // Configure le drapeau d'exécution à vrai pour que les threads démarrent leurs boucles
    game->threadManager.isRunning = 1;
    
    // Crée 3 threads pour le traitement parallèle :
    // 1. Thread audio - gère les mises à jour de la musique/son
    // 2. Thread de rendu - gère le rendu graphique
    // 3. Thread physique - gère les mouvements et les collisions
    // Chaque thread reçoit le pointeur game comme argument
    pthread_create(&game->threadManager.audioThread, NULL, audioThreadFunction, game);

    pthread_create(&game->threadManager.physicsThread, NULL, physicsThreadFunction, game);
    //pthread_create(&game->threadManager.renderThread, NULL, renderThreadFunction, game);
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
        // Calcul du deltaTime avant le verrouillage pour minimiser le temps de verrouillage
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        
        if (game->gameState.currentState == MAP && game->gameData.player) {
            pthread_mutex_lock(&game->threadManager.physicsMutex);

            updatePlayerPosition(game->gameData.player, deltaTime);
            updateCamera(game->gameData.camera, 
                       game->gameData.player->position.x, 
                       game->gameData.player->position.y, 
                       deltaTime);
            
            // Mettre à jour l'animation indépendamment du mouvement
            updatePlayerAnimation(game->gameData.player, deltaTime);
            
            pthread_mutex_unlock(&game->threadManager.physicsMutex);
        }
        
        manageFrameRate(currentTime);
    }
    return NULL;
}

void cleanupThreads(Game* game) {
    if (!game) return;
    
    game->threadManager.isRunning = 0;
    pthread_join(game->threadManager.audioThread, NULL);
    pthread_join(game->threadManager.physicsThread, NULL);
    //pthread_join(game->threadManager.renderThread, NULL);
    pthread_mutex_destroy(&game->threadManager.audioMutex);
    pthread_mutex_destroy(&game->threadManager.physicsMutex);
    //pthread_mutex_destroy(&game->threadManager.renderMutex);
}