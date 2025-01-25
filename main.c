#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "src/include/GameEngine.h"


int main(void) {
    Window *win = (Window *)malloc(sizeof(Window));
    if (!win) {
        fprintf(stderr, "Erreur : Impossible d'allouer la mémoire pour la fenêtre\n");
        return EXIT_FAILURE;
    }
    
    initWindow(win, 1280, 720, "assets/fonts/Axolotl.ttf");
    mainLoop(win);
    destroyWindow(win); 
    free(win);              
    return EXIT_SUCCESS;
}
