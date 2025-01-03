#include <SDL2/SDL.h>
#include "utils.h"

/* Quit the SDL and free the memory
 * 
 * @param codeError The `codeError` parameter is an integer that represents the error code that will be returned
 * when the program exits. This code can be used to identify the cause of the error that caused the program to
 * exit.
 * @param window SDL_Window pointer for the main window of the application.
 */
void quitSDL(int codeError, SDL_Window* window) {
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
    exit(codeError);
}