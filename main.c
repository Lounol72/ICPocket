#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/GameEngine.h"
#include <stdbool.h>

#define WINDOWS_W 1280
#define WINDOWS_H 720

int main(int argc, char *argv[]) {
    if(argc > 1 && strcmp(argv[1], "-debug") == 0) {
        printf("🟢 Starting ICPocket in debug mode...\n");
        
        // Initialisation SDL avec vérification
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            printf("❌ SDL Init failed: %s\n", SDL_GetError());
            return 1;
        }

        SDL_Window *window = SDL_CreateWindow("ICPocket Debug Mode", 
                                            SDL_WINDOWPOS_CENTERED, 
                                            SDL_WINDOWPOS_CENTERED, 
                                            WINDOWS_W, WINDOWS_H, 
                                            SDL_WINDOW_SHOWN);
        if (!window) {
            printf("❌ Window creation failed: %s\n", SDL_GetError());
            return 1;
        }

        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        if (!renderer) {
            printf("❌ Renderer creation failed: %s\n", SDL_GetError());
            SDL_DestroyWindow(window);
            return 1;
        }

        // Initialisation des composants avec vérification
        Map *map = initMap(renderer, "assets/Tileset/Map/MapFloor.png");
        if (!map) {
            printf("❌ Map initialization failed\n");
            return 1;
        }

        Player *player = createPlayer(renderer, "assets/Characters/Character 2.png");
        if (!player) {
            printf("❌ Player initialization failed\n");
            return 1;
        }

        // Debug info initiale
        printf("\n📊 Debug Information:\n");
        printf("Window size: %dx%d\n", WINDOWS_W, WINDOWS_H);
        printf("Tile size: %dx%d\n", TILE_SIZE_W_SCALE, TILE_SIZE_H_SCALE);
        printf("Matrix size: %dx%d\n", MAP_WIDTH, MAP_HEIGHT);

        SDL_Event event;
        int running = 1;
        Uint32 lastTime = SDL_GetTicks();
        float deltaTime = 0.0f;
        int frameCount = 0;
        Uint32 fpsLastTime = SDL_GetTicks();
        float fps = 0.0f;
        
        Camera* camera = createCamera(WINDOWS_W, WINDOWS_H);
        
        while(running) {
            Uint32 currentTime = SDL_GetTicks();
            deltaTime = (currentTime - lastTime) / 1000.0f;
            lastTime = currentTime;

            // Calcul FPS
            frameCount++;
            if (currentTime - fpsLastTime >= 1000) {
                fps = frameCount * 1000.0f / (currentTime - fpsLastTime);
                frameCount = 0;
                fpsLastTime = currentTime;
                
                // Afficher les infos de performance une fois par seconde
                printf("\r⚡ FPS: %.1f | Player Pos: [%d,%d] | State: %d  | DeltaTime: %.3f", 
                       fps, player->matrixX, player->matrixY, player->state, deltaTime);
                fflush(stdout);
            }

            while(SDL_PollEvent(&event)) {
                if(event.type == SDL_QUIT) {
                    running = 0;
                }
            }

            // Ne traiter les inputs que si le joueur n'est PAS en mouvement
            if (!player->isMovingToTarget) {
                const Uint8 *keyState = SDL_GetKeyboardState(NULL);
                int newMatrixX = player->matrixX;
                int newMatrixY = player->matrixY;
                PlayerState newState = player->state;
                bool shouldMove = false;

                if (keyState[SDL_SCANCODE_RIGHT]) {
                    if(newMatrixX < MAP_WIDTH - 1 && !player->isMovingToTarget) {
                        newMatrixX++;
                        newState = WALK_RIGHT;
                        shouldMove = true;
                    }
                }
                else if (keyState[SDL_SCANCODE_LEFT]) {
                    if(newMatrixX > 0 && !player->isMovingToTarget) {
                        newMatrixX--;
                        newState = WALK_LEFT;
                        shouldMove = true;
                    }
                }
                else if (keyState[SDL_SCANCODE_DOWN]) {
                    if(newMatrixY < MAP_HEIGHT - 1 && !player->isMovingToTarget) {
                        newMatrixY++;
                        newState = WALK_DOWN;
                        shouldMove = true;
                    }
                }
                else if (keyState[SDL_SCANCODE_UP]) {
                    if(newMatrixY > 0 && !player->isMovingToTarget) {
                        newMatrixY--;
                        newState = WALK_UP;
                        shouldMove = true;
                    }
                }

                if (shouldMove && map->mat[newMatrixY][newMatrixX] != COLLISION) {
                    // Sauvegarder la position de départ
                    player->startX = player->position.x;
                    player->startY = player->position.y;
                    
                    // Définir la cible
                    player->targetMatrixX = newMatrixX;
                    player->targetMatrixY = newMatrixY;
                    player->targetX = newMatrixX * TILE_SIZE_W_SCALE;
                    player->targetY = newMatrixY * TILE_SIZE_H_SCALE;
                    
                    // Initialiser le mouvement
                    player->state = newState;
                    player->isMovingToTarget = true;
                    player->interpolationTime = 0.0f;
                }
            }

            // Mettre à jour la position avec interpolation
            updatePlayerPosition(player, deltaTime);
            updatePlayerAnimation(player, deltaTime);

            // Mettre à jour la caméra pour suivre le joueur
            updateCamera(camera, player->position.x, player->position.y, deltaTime);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // Modifier les fonctions de rendu pour utiliser la caméra
            renderMapWithCamera(map, renderer, camera);
            renderPlayerWithCamera(player, renderer, camera);
            
            SDL_RenderPresent(renderer);
        }

        printf("\n👋 Exiting debug mode...\n");
        destroyMap(map);
        destroyPlayer(player);
        destroyCamera(camera);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    else {
        Window *win = (Window *)malloc(sizeof(Window));
        if (!win) {
            printf("🔴 ICPocket closed\n");
            return EXIT_FAILURE;
        }
        initWindow(win, 1280,720, "assets/fonts/PressStart2P-Regular.ttf");
        mainLoop(win);
        destroyGame();
        free(win);          
        printf("🟢 ICPocket closed\n");
    }
    return EXIT_SUCCESS;
}
