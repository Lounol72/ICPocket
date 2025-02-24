#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/GameEngine.h"
#include "include/Window.h"


int main(void) {
    printf("🟢 Starting ICPocket...\n");
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
    return EXIT_SUCCESS;
}
