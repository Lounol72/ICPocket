#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include <pthread.h>
#include "Player.h"
#include "Map.h"
#include "Camera.h"

/**
 * @file TheadManager.h
 * @brief Gestionnaire de threads pour le traitement parallèle
 * @author Louis
 * Ce module gère les threads parallèles pour le traitement audio, de rendu et de physique.
 * Il initialise les threads, gère leur exécution et leur terminaison, et gère également 
 * les transitions entre cartes.
 * 
 */

// Nombre de frames pour la transition entre cartes
#define TRANSITION_FRAMES 30

// Variables globales pour la gestion des transitions de carte
extern volatile int forceRender;
extern volatile int forcePlayerMove;
extern volatile int forcePlayerMoveX;
extern volatile int forcePlayerMoveY;

struct Game;

/**
 * @brief Structure de gestion des threads
 * 
 * Cette structure contient les threads et mutex nécessaires pour gérer :
 * - Le rendu graphique (renderThread)
 * - La physique du jeu (physicsThread) 
 * - L'audio (audioThread)
 * 
 * Les mutex permettent de synchroniser l'accès aux ressources partagées
 * entre les différents threads.
 * 
 * @param audioThread Thread dédié au traitement audio
 * @param renderThread Thread dédié au traitement du render lorsque l'on est pas sur la map
 * @param physicsThread Thread dédié aux calculs physiques
 * @param audioMutex Mutex pour synchroniser l'accès aux ressources audio
 * @param renderMutex Mutex pour synchroniser l'accès au render
 * @param physicsMutex Mutex pour synchroniser l'accès aux données physiques
 * @param isRunning Flag indiquant si les threads doivent continuer à s'exécuter
 */

typedef struct ThreadManager {
    pthread_t audioThread;
    pthread_t physicsThread;
    pthread_t renderThread;
    pthread_mutex_t audioMutex;
    pthread_mutex_t physicsMutex;
    pthread_mutex_t renderMutex;
    int isRunning;
} ThreadManager;

/**
 * @brief Initialise le gestionnaire de threads
 * 
 * @param game Pointeur vers la structure Game
 * Initialise les différent verrous mutex pour les ressourcers partagées
 * et crée les threads parallèles
 */
void initThreadManager(struct Game* game);

/**
 * @brief Nettoie et termine les threads
 * 
 * Cette fonction arrête tous les threads en cours d'exécution, attend leur terminaison
 * et détruit les mutex associés.
 * 
 * @param game Pointeur vers la structure Game
 */
void cleanupThreads(struct Game* game);

/**
 * @brief Fonction du thread audio
 * 
 * Cette fonction est exécutée par le thread audio. Elle gère le traitement audio
 * en boucle tant que le jeu est en cours d'exécution.
 * 
 * @param arg Pointeur vers la structure Game
 * @return NULL
 */
void* audioThreadFunction(void* arg);

/**
 * @brief Fonction du thread de physique
 * 
 * Cette fonction est exécutée par le thread de physique. Elle gère les calculs physiques
 * en boucle tant que le jeu est en cours d'exécution.
 * 
 * @param arg Pointeur vers la structure Game
 * @return NULL
 */
void* physicsThreadFunction(void* arg);

/**
 * @brief Fonction du thread de rendu
 * 
 * Cette fonction est exécutée par le thread de rendu. Elle gère le rendu graphique
 * en boucle tant que le jeu est en cours d'exécution.
 * 
 */
void* renderThreadFunction(void* arg);

/**
 * @brief Gère la transition d'une carte à une autre avec un effet de fondu
 * 
 * @param mapId ID de la map à charger (0, 1, 2)
 * @param player Pointeur vers le joueur
 * @param camera Pointeur vers la caméra
 * @param renderer Renderer pour les effets visuels
 * @return int 1 si une transition a été effectuée, 0 sinon
 */
int handleMapTransition(int mapId, Player *player, Camera *camera, SDL_Renderer *renderer);

/**
 * @brief Vérifie si le joueur est sur une case de transition et charge la nouvelle carte si nécessaire
 * 
 * @param player Pointeur vers le joueur
 * @param map Pointeur vers la carte actuelle
 * @param camera Pointeur vers la caméra
 * @param renderer Renderer pour les effets visuels 
 * @return int 1 si une transition a été effectuée, 0 sinon
 */
int checkAndLoadNewMap(Player *player, Map *map, Camera *camera, SDL_Renderer *renderer);

#endif /* THREAD_MANAGER_H */ 