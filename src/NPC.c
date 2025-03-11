#include "../include/NPC.h"
/*
NPCManager* createNPCManager(int initialCapacity) {
    NPCManager* manager = malloc(sizeof(NPCManager));
    manager->npcs = malloc(sizeof(NPC) * initialCapacity);
    manager->count = 0;
    manager->capacity = initialCapacity;
    return manager;
}

NPC* createNPC(int x, int y, const char* spritePath, Window* win, t_Team team, const char* message) {
    NPC* npc = &game.npcManager->npcs[game.npcManager->count++];
    
    npc->x = x;
    npc->y = y;
    npc->team = team;
    npc->message = strdup(message);
    npc->hasBeenDefeated = false;
    npc->direction = 0;
    
    // Charger le sprite
    SDL_Surface* surface = IMG_Load(spritePath);
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Impossible de charger le sprite du PNJ: %s", IMG_GetError());
        return NULL;
    }
    
    npc->sprite = SDL_CreateTextureFromSurface(win->renderer, surface);
    SDL_FreeSurface(surface);
    
    // Définir la position du sprite
    npc->position = (SDL_Rect){x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    
    return npc;
}

void updateNPCs(Window* win) {
    for (int i = 0; i < game.npcManager->count; i++) {
        NPC* npc = &game.npcManager->npcs[i];
        
        // Mettre à jour la position relative à la caméra
        npc->position.x = (npc->x * TILE_SIZE) - game.gameData.camera->x;
        npc->position.y = (npc->y * TILE_SIZE) - game.gameData.camera->y;
        
        // Vérifier si le joueur est adjacent au PNJ
        int playerX = game.gameData.player->matrixX;
        int playerY = game.gameData.player->matrixY;
        
        if (!npc->hasBeenDefeated && 
            ((abs(playerX - npc->x) == 1 && playerY == npc->y) ||
             (abs(playerY - npc->y) == 1 && playerX == npc->x))) {
            
            // Faire face au joueur
            if (playerX < npc->x) npc->direction = 1;      // Gauche
            else if (playerX > npc->x) npc->direction = 2; // Droite
            else if (playerY < npc->y) npc->direction = 3; // Haut
            else npc->direction = 0;                       // Bas
            
            // Si le joueur appuie sur la touche d'interaction
            if (game.gameData.player->isInteracting) {
                startNPCBattle(npc);
            }
        }
    }
}

void renderNPCs(Window* win) {
    for (int i = 0; i < game.npcManager->count; i++) {
        NPC* npc = &game.npcManager->npcs[i];
        
        // Source rectangle pour l'animation du sprite
        SDL_Rect srcRect = {
            0,                    // x dans la spritesheet
            npc->direction * TILE_SIZE, // y dans la spritesheet selon la direction
            TILE_SIZE,           // largeur
            TILE_SIZE            // hauteur
        };
        
        SDL_RenderCopy(win->renderer, npc->sprite, &srcRect, &npc->position);
    }
}

void startNPCBattle(NPC* npc) {
    // Sauvegarder l'équipe bleue actuelle
    t_Team tempBlueTeam = game.battleState.bleu;
    
    // Remplacer par l'équipe du PNJ
    game.battleState.bleu = npc->team;
    
    // Afficher le message du PNJ
    if (game.battleState.text) {
        cleanupScrollingText(&game.battleState.text);
    }
    game.battleState.text = createScrollingText(
        npc->message,
        game.win->LargeFont,
        (SDL_Color){255, 255, 255, 255},
        game.speed,
        (SDL_Rect){game.win->width * 0.4531, game.win->height * 0.5,
                   game.win->width * 0.5, game.win->height * 0.2},
        "assets/User Interface/Grey/button_rectangle_depth_flat.png",
        game.win->renderer
    );
    
    // Changer l'état du jeu
    changeState(game.win, &game.stateHandlers[3].state);
} 

void destroyNPCManager(NPCManager* manager) {
    free(manager->npcs);
    free(manager);
}

void destroyNPC(NPC* npc) {
    SDL_DestroyTexture(npc->sprite);
    free(npc->message);
    free(npc);
}

*/