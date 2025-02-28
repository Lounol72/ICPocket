#include "../include/Camera.h"

Camera* createCamera(int windowWidth, int windowHeight) {
    Camera* camera = malloc(sizeof(Camera));
    camera->x = 0;
    camera->y = 0;
    camera->scale = 4.0f;  // Zoom initial
    camera->viewport.w = windowWidth;
    camera->viewport.h = windowHeight;
    camera->targetX = 0;
    camera->targetY = 0;
    return camera;
}

void updateCamera(Camera* camera, int playerX, int playerY, float deltaTime) {
    // Position cible au centre de l'écran
    float targetCameraX = playerX - (camera->viewport.w / camera->scale) / 2;
    float targetCameraY = playerY - (camera->viewport.h / camera->scale) / 2;
    
    // Interpolation linéaire douce vers la cible
    float smoothSpeed = 5.0f;
    camera->x += (targetCameraX - camera->x) * smoothSpeed * deltaTime;
    camera->y += (targetCameraY - camera->y) * smoothSpeed * deltaTime;
}

void updateCameraViewport(Camera* camera, int windowWidth, int windowHeight) {
    camera->viewport.w = windowWidth;
    camera->viewport.h = windowHeight;
}

SDL_Rect getWorldToScreenRect(Camera* camera, SDL_Rect worldRect) {
    SDL_Rect screenRect;
    screenRect.x = (int)((worldRect.x - camera->x) * camera->scale);
    screenRect.y = (int)((worldRect.y - camera->y) * camera->scale);
    screenRect.w = (int)(worldRect.w * camera->scale);
    screenRect.h = (int)(worldRect.h * camera->scale);
    return screenRect;
}

void destroyCamera(Camera* camera) {
    free(camera);
}
