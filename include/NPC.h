#ifndef NPC_H
#define NPC_H

#include "GameEngine.h"

typedef struct {
    int x, y;                    // Position sur la carte
    SDL_Texture* sprite;         // Sprite du PNJ
    SDL_Rect position;           // Rectangle de position
    t_Team team;                 // Équipe du PNJ
    char* message;               // Message avant le combat
    bool hasBeenDefeated;        // Si le PNJ a déjà été battu
    int direction;               // Direction du regard (0: bas, 1: gauche, 2: droite, 3: haut)
} NPC;

typedef struct {
    NPC* npcs;                   // Tableau de PNJ
    int count;                   // Nombre de PNJ
    int capacity;                // Capacité maximale
} NPCManager;

NPCManager* createNPCManager(int initialCapacity);
NPC* createNPC(int x, int y, const char* spritePath, Window* win, t_Team team, const char* message);
void updateNPCs(Window* win);
void renderNPCs(Window* win);
void startNPCBattle(NPC* npc);
void destroyNPCManager(NPCManager* manager);
void destroyNPC(NPC* npc);


#endif 