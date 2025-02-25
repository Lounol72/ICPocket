#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

// Définir les constantes de la fenêtre
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

typedef struct {
    float x, y;           // Position de la caméra
    float scale;          // Niveau de zoom
    SDL_Rect viewport;    // Rectangle de vue
    int targetX, targetY; // Position cible (joueur)
} Camera;

Camera* createCamera(int windowWidth, int windowHeight);
void updateCamera(Camera* camera, int playerX, int playerY, float deltaTime);
SDL_Rect getWorldToScreenRect(Camera* camera, SDL_Rect worldRect);

#endif
