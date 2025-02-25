#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/GameEngine.h"
#include "include/Window.h"
#include "include/Map.h"
#include "include/Player.h"
#include <stdbool.h>

int main(int argc, char *argv[]) {
    if(argc > 1 && strcmp(argv[1], "-debug") == 0) {
        printf("🟢 Starting ICPocket in debug mode...\n");
        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_Window *window = SDL_CreateWindow("ICPocket", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

        Map *map = initMap(renderer, "assets/Tileset/Map/MapFloor.png");
        Player *player = createPlayer(renderer, "assets/Characters/Character 1.png");

        // Position initiale du joueur
        player->matrixX = MAP_WIDTH / 2;
        player->matrixY = MAP_HEIGHT / 2;
        player->position.x = player->matrixX * TILE_SIZE_W_SCALE;
        player->position.y = player->matrixY * TILE_SIZE_H_SCALE;

        SDL_Event event;
        int running = 1;
        Uint32 lastTime = SDL_GetTicks();
        float deltaTime = 0.0f;
        
        if (!player) {
            printf("Erreur création player\n");
            // Cleanup et return
        }

        while(running) {
            Uint32 currentTime = SDL_GetTicks();
            deltaTime = (currentTime - lastTime) / 1000.0f;
            lastTime = currentTime;

            if(SDL_PollEvent(&event)) {
                if(event.type == SDL_QUIT) {
                    running = 0;
                }
                else if(event.type == SDL_KEYDOWN) {
                    int newMatrixX = player->matrixX;
                    int newMatrixY = player->matrixY;
                    bool shouldMove = false;

                    switch(event.key.keysym.sym) {
                        case SDLK_UP:
                            if(newMatrixY > 0) {
                                newMatrixY--;
                                player->state = WALK_UP;
                                shouldMove = true;
                            }
                            break;
                        case SDLK_DOWN:
                            if(newMatrixY < MAP_HEIGHT - 1) {
                                newMatrixY++;
                                player->state = WALK_DOWN;
                                shouldMove = true;
                            }
                            break;
                        case SDLK_LEFT:
                            if(newMatrixX > 0) {
                                newMatrixX--;
                                player->state = WALK_LEFT;
                                shouldMove = true;
                            }
                            break;
                        case SDLK_RIGHT:
                            if(newMatrixX < MAP_WIDTH - 1) {
                                newMatrixX++;
                                player->state = WALK_RIGHT;
                                shouldMove = true;
                            }
                            break;
                        case SDLK_ESCAPE:
                            running = 0;
                            break;
                    }

                    if(shouldMove && map->mat[newMatrixY][newMatrixX] != COLLISION) {
                        player->matrixX = newMatrixX;
                        player->matrixY = newMatrixY;
                        player->position.x = player->matrixX * TILE_SIZE_W_SCALE;
                        player->position.y = player->matrixY * TILE_SIZE_H_SCALE;
                        player->isMoving = 1;
                    }
                }
                else if(event.type == SDL_KEYUP) {
                    player->isMoving = 0;
                    switch(player->state) {
                        case WALK_UP: player->state = IDLE_UP; break;
                        case WALK_DOWN: player->state = IDLE_DOWN; break;
                        case WALK_LEFT: player->state = IDLE_LEFT; break;
                        case WALK_RIGHT: player->state = IDLE_RIGHT; break;
                        default: break;
                    }
                }
            }

            // Debug: afficher la position du joueur
            printf("Position joueur: [%d,%d], Collision: %d\n", 
                   player->matrixX, player->matrixY, 
                   map->mat[player->matrixY][player->matrixX]);

            printf("\n--- Nouveau frame ---\n");
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            updatePlayerAnimation(player, deltaTime);
            renderMap(map);
            renderPlayer(renderer, player);
            
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }

        destroyMap(map);
        destroyPlayer(player);
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
        initWindow(win, 1280,720, "assets/fonts/Axolotl.ttf");
        mainLoop(win);
        destroyWindow(win); 
        free(win);          
        printf("🟢 ICPocket closed\n");
    }
    return EXIT_SUCCESS;
}
