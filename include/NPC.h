#ifndef NPC_H
#define NPC_H
/**
 * @file NPC.h
 * @author Alban Louis
 * @brief Gestion des PNJ (Personnages Non Joueurs)
 * @version 0.1
 * @date 2025-03-17
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "GameEngine.h"

/**
 * @struct NPC
 * @brief Représente un personnage non joueur (PNJ) dans le jeu.
 * 
 * Cette structure contient des informations sur la position, le sprite, l'équipe, le message avant le combat,
 * l'état de défaite, et la direction du PNJ.
 */
typedef struct {
    int x, y;                    /**< position du PNJ sur la map. */
    SDL_Texture* sprite;         /**< Sprite du PNJ. */
    SDL_Rect position;           /**< rectangle de position. */
    t_Team team;                 /**< Équipe du PNJ. */
    char* message;               /**< Message avant le combat. */
    bool hasBeenDefeated;        /**< Si le PNJ a déjà été battu. */
    int direction;               /**< Direction du regard (0: bas, 1: gauche, 2: droite, 3: haut). */
} NPC;

/**
 * @struct NPCManager
 * @brief Gestionnaire de PNJ
 * 
 * Cette structure gère un tableau dynamique de PNJ, permettant d'ajouter, supprimer et mettre à jour les PNJ.
 */
typedef struct {
    NPC* npcs;                   /**< Tableau de PNJ. */
    int count;                   /**< Nombre de PNJ. */
    int capacity;                /**< Capacité maximale. */
} NPCManager;

/**
 * @fn NPCManager* createNPCManager(int initialCapacity)
 * @brief Crée un gestionnaire de PNJ avec une capacité initiale.
 * 
 * @param initialCapacity Capacité initiale du gestionnaire de PNJ.
 * @return Pointeur vers le gestionnaire de PNJ créé.
 */
NPCManager* createNPCManager(int initialCapacity);

/**
 * @fn NPC* createNPC(int x, int y, const char* spritePath, Window* win, t_Team team, const char* message)
 * @brief Crée un PNJ avec les paramètres spécifiés.
 * 
 * @param x position x du PNJ
 * @param y position y du PNJ
 * @param spritePath chemin vers le sprite du PNJ
 * @param win pointeur vers la fenêtre du jeu
 * @param team équipe du PNJ
 * @param message message à afficher avant le combat
 * @return NPC* 
 */
NPC* createNPC(int x, int y, const char* spritePath, Window* win, t_Team team, const char* message);

/**
 * @fn void updateNPCs(Window* win)
 * @brief Met à jour les PNJ dans la fenêtre spécifiée.
 * 
 * Cette fonction gère les interactions entre le joueur et les PNJ, y compris le déclenchement de combats.
 * 
 * @param win Pointeur vers la fenêtre du jeu.
 */
void updateNPCs(Window* win);

/**
 * @fn void renderNPCs(Window* win)
 * @brief Rendu des PNJ dans la fenêtre spécifiée.
 * 
 * Cette fonction dessine les PNJ sur la fenêtre du jeu.
 * 
 * @param win Pointeur vers la fenêtre du jeu.
 */
void renderNPCs(Window* win);

/**
 * @fn void startNPCBattle(NPC* npc)
 * @brief Démarre un combat avec le PNJ spécifié.
 * 
 * Cette fonction gère la logique de combat entre le joueur et le PNJ.
 * 
 * @param npc Pointeur vers le PNJ avec lequel le combat doit commencer.
 */
void startNPCBattle(NPC* npc);

/**
 * @fn void detroyNPCManager(NPCManager* manager)
 * @brief Détruit le gestionnaire de PNJ.
 * 
 * Cette fonction libère la mémoire associée au gestionnaire de PNJ et à tous les PNJ qu'il contient.
 * 
 * @param manager Pointeur vers le gestionnaire de PNJ à détruire.
 */
void destroyNPCManager(NPCManager* manager);

/**
 * @fn void destroyNPC(NPC* npc)
 * @brief Détruit un PNJ.
 * 
 * Cette fonction libère la mémoire associée à un PNJ.
 * 
 * @param npc Pointeur vers le PNJ à détruire.
 */
void destroyNPC(NPC* npc);


#endif 