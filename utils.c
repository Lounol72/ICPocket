#include <SDL2/SDL.h>

void quitSDL(int codeError, SDL_Window* window) {
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
    exit(codeError);
}