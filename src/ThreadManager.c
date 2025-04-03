#include "../include/GameEngine.h"
#include "../include/Player.h"

// Variables globales pour la gestion des transitions de carte
volatile int forceRender = 0;
volatile int forcePlayerMove = 0;
volatile int forcePlayerMoveX = 0;
volatile int forcePlayerMoveY = 0;

/**
 * @fn void updatePhysics(Player *player, Camera *camera, float deltaTime)
 * @brief Met à jour les aspects physiques du jeu (mouvements, collisions, etc.)
 * 
 * @param player Pointeur vers le joueur
 * @param camera Pointeur vers la caméra
 * @param deltaTime Temps écoulé depuis la dernière mise à jour
 */
static inline void updatePhysics(Player *player, Camera* camera, float deltaTime) {
    if (!player || !camera) {
        SDL_Log("ERROR: Pointeurs invalides dans updatePhysics");
        return;
    }
    
    // Effectuer les mises à jour standard
    updatePlayerPosition(player, deltaTime);
    updateCamera(camera, player->position.x, player->position.y, deltaTime);
    
    // Vérifier si le joueur a terminé son mouvement et s'il est sur une case de transition
    if (!player->isMovingToTarget) {
        // Vérifier l'état actuel du jeu
        if (game.gameState.currentState != MAP) {
            updatePlayerAnimation(player, deltaTime);
            return;
        }
        
        Map *currentMap = game.gameData.maps[player->mapIndex];
        if (!currentMap) {
            SDL_Log("ERROR: Carte actuelle NULL dans updatePhysics");
            updatePlayerAnimation(player, deltaTime);
            return;
        }
        
        // Vérifier que les coordonnées du joueur sont valides par rapport à la carte
        if (player->matrixX < 0 || player->matrixX >= currentMap->taille[0] ||
            player->matrixY < 0 || player->matrixY >= currentMap->taille[1]) {
            SDL_Log("WARNING: Coordonnées du joueur hors limites [%d,%d] - limites: [%d,%d]", 
                    player->matrixX, player->matrixY, currentMap->taille[0], currentMap->taille[1]);
            updatePlayerAnimation(player, deltaTime);
            return;
        }
        
        // Si checkAndLoadNewMap retourne 1, une transition a eu lieu
        if (checkAndLoadNewMap(player, currentMap, camera, game.win->renderer)) return;
    }
    
    // Mettre à jour l'animation indépendamment du mouvement
    updatePlayerAnimation(player, deltaTime);
}

/**
 * @fn int handleMapTransition(int mapId, Player *player, Camera *camera, SDL_Renderer *renderer)
 * @brief Gère la transition d'une carte à une autre avec un effet de fondu
 * 
 * @param mapId ID de la map à charger (0, 1, 2)
 * @param player Pointeur vers le joueur
 * @param camera Pointeur vers la caméra
 * @param renderer Renderer pour les effets visuels
 * @return int 1 si une transition a été effectuée, 0 sinon
 */
int handleMapTransition(int mapId, Player *player, Camera *camera, SDL_Renderer *renderer) {
    if (!player || !camera || !renderer || mapId < 0 || mapId >= 3) {
        SDL_Log("ERROR: Paramètres invalides pour handleMapTransition (mapId=%d)", mapId);
        return 0;
    }
    
    // Sauvegarder la position actuelle du joueur dans la carte courante
    Map *currentMap = game.gameData.maps[player->mapIndex];
    if (!currentMap) {
        SDL_Log("ERROR: Carte actuelle NULL dans handleMapTransition");
        return 0;
    }
    
    Map *newMap = game.gameData.maps[mapId];
    if (!newMap) {
        SDL_Log("ERROR: Nouvelle carte NULL (mapId=%d)", mapId);
        return 0;
    }

    
    // Chercher le 0 le plus proche de ses 4 voisins
    int x = player->matrixX;
    int y = player->matrixY;
    int i = 1;
    while (x == player->matrixX && y == player->matrixY) {
        if (x+i < currentMap->taille[0] ) {
            if (currentMap->mat[y][x+i] == 0) {
                x += i;
            }
        }
        if (x-i >= 0) {
            if (currentMap->mat[y][x-i] == 0) {
                x -= i;
            }
        }
        if (y+i < currentMap->taille[1]) {
            if (currentMap->mat[y+i][x] == 0) {
                y += i;
            }
        }
        if (y-i >= 0) {
            if (currentMap->mat[y-i][x] == 0) {
                y -= i;
            }
        }
        i++;
    }
    
    currentMap->positions[0] = x;
    currentMap->positions[1] = y;
    player->mapIndex = mapId;

    
    // Effet de fondu au noir
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    for (int i = 0; i < TRANSITION_FRAMES; i++) {
        // Calculer l'opacité pour cette frame
        Uint8 alpha = (Uint8)(255.0f * i / TRANSITION_FRAMES);
        
        // Rendre la carte et le joueur
        SDL_RenderClear(renderer);
        renderMapWithCamera(currentMap, renderer, camera);
        renderPlayerWithCamera(player, renderer, camera);
        
        // Superposer un rectangle noir avec l'opacité croissante
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
        SDL_Rect fullScreen = {0, 0, game.win->width, game.win->height};
        SDL_RenderFillRect(renderer, &fullScreen);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }
    
    // Changer de carte
    int oldMapIndex = player->mapIndex;
    
    
    // Mettre à jour la position du joueur sur la nouvelle carte
    updatePlayerSpawn(player, newMap, newMap->positions[0], newMap->positions[1]);
    
    forceUpdatePlayerAndCamera(player, camera, newMap);
    
    // Forcer la mise à jour des flags globaux pour que les autres threads sachent qu'une transition a eu lieu
    forceRender = 1;
    forcePlayerMove = 1;
    forcePlayerMoveX = newMap->positions[0];
    forcePlayerMoveY = newMap->positions[1];
    
    // Effet de fondu depuis le noir
    for (int i = TRANSITION_FRAMES; i >= 0; i--) {
        // Calculer l'opacité pour cette frame
        Uint8 alpha = (Uint8)(255.0f * i / TRANSITION_FRAMES);
        
        // Rendre la nouvelle carte et le joueur
        SDL_RenderClear(renderer);
        renderMapWithCamera(newMap, renderer, camera);
        renderPlayerWithCamera(player, renderer, camera);
        
        // Superposer un rectangle noir avec l'opacité décroissante
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
        SDL_Rect fullScreen = {0, 0, game.win->width, game.win->height};
        SDL_RenderFillRect(renderer, &fullScreen);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }
    
    SDL_Log("Transition de la carte %d à la carte %d réussie", oldMapIndex, mapId);
    return 1;
}

/**
 * @fn int checkAndLoadNewMap(Player *player, Map *map, Camera *camera, SDL_Renderer *renderer)
 * @brief Vérifie si le joueur est sur une case de transition et charge la nouvelle carte si nécessaire
 * 
 * @param player Pointeur vers le joueur
 * @param map Pointeur vers la carte actuelle
 * @param camera Pointeur vers la caméra
 * @param renderer Renderer pour les effets visuels
 * @return int 1 si une transition a été effectuée, 0 sinon
 */
int checkAndLoadNewMap(Player *player, Map *map, Camera *camera, SDL_Renderer *renderer) {
    if (!player || !map || !camera || !renderer) {
        SDL_Log("Erreur: paramètres invalides pour checkAndLoadNewMap");
        return 0;
    }
    
    // Vérifier si les coordonnées du joueur sont valides
    if (player->matrixX < 0 || player->matrixX >= map->taille[0] ||
        player->matrixY < 0 || player->matrixY >= map->taille[1]) {
        return 0;
    }
    
    // Vérifier si le joueur est sur une case de transition
    int tileValue = map->mat[player->matrixY][player->matrixX];
    int mapId = -1;
    
    // Déterminer quelle carte charger en fonction de la valeur de la case
    switch (tileValue) {
        case MAP_TRANSITION_ID_1: // Case avec ID 2
            mapId = 1;
            
            break;
        case MAP_TRANSITION_ID_2: // Case avec ID 3
            mapId = 2;
            
            break;
        case MAP_TRANSITION_ID_3: // Case avec ID 9
            mapId = 0;
            break;
    }
    
    // Si une carte valide est trouvée et différente de la carte actuelle, effectuer la transition
    if (mapId >= 0 && mapId < 3 && mapId != player->mapIndex) {
        return handleMapTransition(mapId, player, camera, renderer);
    }
    return 0;
}

/**
 * @fn void initThreadManager(Game* game)
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

/**
 * @fn void* audioThreadFunction(void* arg)
 * @brief Fonction du thread audio
 * 
 * Cette fonction est exécutée par le thread audio. Elle gère le traitement audio
 * en boucle tant que le jeu est en cours d'exécution.
 * 
 * @param arg Pointeur vers la structure Game
 * @return NULL
 */
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

/**
 * @fn void* physicsThreadFunction(void* arg)
 * @brief Fonction du thread de physique
 * 
 * Cette fonction est exécutée par le thread de physique. Elle gère les calculs physiques
 * en boucle tant que le jeu est en cours d'exécution.
 * 
 * @param arg Pointeur vers la structure Game
 * @return NULL
 */
void* physicsThreadFunction(void* arg) {
    Game* game = (Game*)arg;
    Uint32 lastTime = SDL_GetTicks();
    
    while (game->threadManager.isRunning) {
        // Calcul du deltaTime avant le verrouillage pour minimiser le temps de verrouillage
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        
        pthread_mutex_lock(&game->threadManager.physicsMutex);
        //if(game->gameState.currentState == MAP)
        updatePhysics(game->gameData.player, game->gameData.camera, deltaTime);
        pthread_mutex_unlock(&game->threadManager.physicsMutex);
        
        manageFrameRate(currentTime);
    }
    return NULL;
}

/**
 * @fn void cleanupThreads(struct Game* game)
 * @brief Nettoie et termine les threads
 * 
 * Cette fonction arrête tous les threads en cours d'exécution, attend leur terminaison
 * et détruit les mutex associés.
 * 
 * @param game Pointeur vers la structure Game
 */
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