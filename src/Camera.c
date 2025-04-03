#include "../include/Camera.h"


/**
 * @fn Camera* createCamera(int windowWidth, int windowHeight)
 * @brief Crée une nouvelle instance de caméra.
 * 
 * Initialise une nouvelle caméra avec les dimensions de fenêtre spécifiées.
 * 
 * @param windowWidth Largeur de la fenêtre en pixels.
 * @param windowHeight Hauteur de la fenêtre en pixels.
 * @return Un pointeur vers la nouvelle instance de Camera créée.
 */
Camera* createCamera(int windowWidth, int windowHeight) {
    Camera* camera = malloc(sizeof(Camera));
    camera->x = 0;
    camera->y = 0;
    camera->scale = 2.175f;  // Zoom initial
    camera->viewport.w = windowWidth;
    camera->viewport.h = windowHeight;
    camera->targetX = 0;
    camera->targetY = 0;
    return camera;
}

/**
 * @fn void updateCamera(Camera* camera, int playerX, int playerY, float deltaTime)
 * @brief Met à jour la position de la caméra.
 * 
 * Actualise la position de la caméra en fonction de la position du joueur
 * et du temps écoulé depuis la dernière mise à jour.
 * 
 * @param camera Pointeur vers la caméra à mettre à jour.
 * @param playerX Position X du joueur dans le monde.
 * @param playerY Position Y du joueur dans le monde.
 * @param deltaTime Temps écoulé depuis la dernière mise à jour (en secondes).
 */
void updateCamera(Camera* camera, int playerX, int playerY, float deltaTime) {
    // Position cible au centre de l'écran
    float targetCameraX = playerX - (camera->viewport.w / camera->scale) / 2;
    float targetCameraY = playerY - (camera->viewport.h / camera->scale) / 2;
    
    // Interpolation linéaire douce vers la cible
    float smoothSpeed = 5.0f;
    camera->x += (targetCameraX - camera->x) * smoothSpeed * deltaTime;
    camera->y += (targetCameraY - camera->y) * smoothSpeed * deltaTime;
}

/**
 * @fn void updateCameraViewport(Camera* camera, int windowWidth, int windowHeight);
 * @brief Met à jour la taille de la zone visible de la caméra.
 * 
 * Actualise la taille de la zone visible de la caméra en fonction des dimensions de la fenêtre.
 * 
 * @param camera Pointeur vers la caméra à mettre à jour.
 * @param windowWidth Largeur de la fenêtre en pixels.
 * @param windowHeight Hauteur de la fenêtre en pixels.
 */
void updateCameraViewport(Camera* camera, int windowWidth, int windowHeight) {
    camera->viewport.w = windowWidth;
    camera->viewport.h = windowHeight;
}

/**
 * @fn SDL_Rect getWorldToScreenRect(Camera* camera, SDL_Rect worldRect);
 * @brief Convertit les coordonnées du monde en coordonnées écran.
 * 
 * Transforme un rectangle dans les coordonnées du monde en coordonnées écran
 * en tenant compte de la position et du zoom de la caméra.
 * 
 * @param camera Pointeur vers la caméra.
 * @param worldRect Rectangle dans les coordonnées du monde à convertir.
 * @return Le rectangle converti en coordonnées écran.
 */
SDL_Rect getWorldToScreenRect(Camera* camera, SDL_Rect worldRect) {
    SDL_Rect screenRect;
    screenRect.x = (int)((worldRect.x - camera->x) * camera->scale);
    screenRect.y = (int)((worldRect.y - camera->y) * camera->scale);
    screenRect.w = (int)(worldRect.w * camera->scale);
    screenRect.h = (int)(worldRect.h * camera->scale);
    return screenRect;
}

/**
 * @fn void destroyCamera(Camera* camera);
 * @brief Libère les ressources utilisées par la caméra.
 * 
 * Détruit l'instance de la caméra et libère la mémoire associée.
 * 
 * @param camera Pointeur vers la caméra à détruire.
 */
void destroyCamera(Camera* camera) {
    free(camera);
}
