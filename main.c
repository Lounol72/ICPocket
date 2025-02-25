#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/GameEngine.h"
#include "include/Window.h"
#include "include/Map.h"


int main(int argc, char *argv[]) {
    if(argc > 1 && strcmp(argv[1], "-debug") == 0) {
        printf("🟢 Starting ICPocket in debug mode...\n");
        SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("ICPocket", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    
    Map *map = initMap(renderer, "assets/Tileset/Map/MapFloor.png");
    SDL_RenderClear(renderer);
    renderMap(map);
    SDL_RenderPresent(renderer);
    SDL_Delay(10000);
    
    destroyMap(map);

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
