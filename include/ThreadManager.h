#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include <pthread.h>
#include <SDL2/SDL.h>

/**
 * @file TheadManager.h
 * @brief Gestionnaire de threads pour le traitement parallèle
 * @author Louis
 * Ce module gère les threads parallèles pour le traitement audio, de rendu et de physique.
 * Il initialise les threads, gère leur exécution et leur terminaison.
 * 
 * 
 */

// Structure forward declarations
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
 * @param renderThread Thread dédié au rendu graphique
 * @param physicsThread Thread dédié aux calculs physiques
 * @param audioMutex Mutex pour synchroniser l'accès aux ressources audio
 * @param renderMutex Mutex pour synchroniser l'accès au rendu
 * @param physicsMutex Mutex pour synchroniser l'accès aux données physiques
 * @param isRunning Flag indiquant si les threads doivent continuer à s'exécuter
 */

typedef struct ThreadManager {
    pthread_t audioThread;
    pthread_t renderThread;
    pthread_t physicsThread;
    pthread_mutex_t audioMutex;
    pthread_mutex_t renderMutex;
    pthread_mutex_t physicsMutex;
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
 * @brief Fonction du thread de rendu
 * 
 * Cette fonction est exécutée par le thread de rendu. Elle gère le rendu graphique
 * en boucle tant que le jeu est en cours d'exécution.
 * 
 * @param arg Pointeur vers la structure Game
 * @return NULL
 */
void* renderThreadFunction(void* arg);

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

#endif /* THREAD_MANAGER_H */ 