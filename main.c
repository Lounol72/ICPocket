#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/GameEngine.h"
#include <stdbool.h>

#define WINDOWS_W 1280
#define WINDOWS_H 720

int main(int argc, char *argv[]) {
    if(argc > 1 && strcmp(argv[1], "-debug") == 0) {
        printf("🟢 Starting ICPocket in debug mode...\n\n");
        printf("📊 Debug Information:\n");
        printf("Window size: %dx%d\n", WINDOWS_W, WINDOWS_H);
        printf("Tile size: %dx%d\n", TILE_SIZE_W_SCALE, TILE_SIZE_H_SCALE);
        printf("Matrix size: %dx%d\n", MAP_WIDTH, MAP_HEIGHT);

        SDL_Init(SDL_INIT_VIDEO);
        TTF_Init();  // Initialiser SDL_ttf

        SDL_Window *window = SDL_CreateWindow("ICPocket Debug",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            WINDOWS_W, WINDOWS_H, SDL_WINDOW_SHOWN);
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        
        // Charger la police
        TTF_Font* font = TTF_OpenFont("assets/fonts/PressStart2P-Regular.ttf", 14);
        if (!font) {
            printf("Erreur chargement police: %s\n", TTF_GetError());
            return 1;
        }

        SDL_Event event;
        SDL_Color textColor = {255, 255, 255, 255}; // Blanc
        ScrollingText* dialogueText = createScrollingText(
            "Bonjour ! Je suis le Professeur PI. Bienvenue dans le monde des ICMons ! Je suis le goat de cette promo ! Ici, tu vas pouvoir apprendre à capturer des ICMons !",
            font,
            textColor,
            50,   // Délai entre caractères (ms)
            (SDL_Rect){50, WINDOWS_H - 200, 550, 100}, // Position du fond
            "assets/User Interface/Blue/button_rectangle_gradient.png",  // Image de fond
            renderer
        );

        int running = 1;
        
        while(running) {
            Uint32 frameStart = SDL_GetTicks();

            while(SDL_PollEvent(&event)) {
                if(event.type == SDL_QUIT) {
                    running = 0;
                }
                if(event.type == SDL_KEYDOWN) {
                    if(event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                        skipScrollingText(dialogueText);
                    }
                }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            if (dialogueText->isComplete && !strcmp(dialogueText->fullText, "Bonjour ! Je suis le Professeur PI. Bienvenue dans le monde des ICMons ! Je suis le goat de cette promo ! Ici, tu vas pouvoir apprendre à capturer des ICMons !")) {
                resetScrollingText(dialogueText, "Bonjour !");
                
            }else updateScrollingText(dialogueText, renderer);
            

            renderScrollingText(dialogueText, renderer);
            
            SDL_RenderPresent(renderer);
            manageFrameRate(frameStart);
        }

        printf("\n👋 Exiting debug mode...\n");
        
        // Nettoyage
        if (dialogueText) {
            destroyScrollingText(dialogueText);
        }
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
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
