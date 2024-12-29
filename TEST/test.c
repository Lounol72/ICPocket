#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Initialize SDL video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Create the window
    SDL_Window *window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Delay for 3 seconds
    SDL_Delay(3000);

    // Destroy the window
    SDL_DestroyWindow(window);

    // Quit SDL
    SDL_Quit();

    return EXIT_SUCCESS;
}
