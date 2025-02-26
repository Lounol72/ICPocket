#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include <pthread.h>
#include <SDL2/SDL.h>

// Structure forward declarations
struct Game;

typedef struct ThreadManager {
    pthread_t audioThread;
    pthread_t renderThread;
    pthread_t physicsThread;
    pthread_mutex_t audioMutex;
    pthread_mutex_t renderMutex;
    pthread_mutex_t physicsMutex;
    int isRunning;
} ThreadManager;

void initThreadManager(struct Game* game);
void cleanupThreads(struct Game* game);
void* audioThreadFunction(void* arg);
void* renderThreadFunction(void* arg);
void* physicsThreadFunction(void* arg);

#endif /* THREAD_MANAGER_H */ 