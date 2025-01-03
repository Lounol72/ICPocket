#include <SDL2/SDL.h>
#include "menu.h"
#include "boutons.h"

/* Draw differents menus on the screen
 * Each menu is represented by a different function
 * 
 * @param surface The `surface` parameter is a pointer to an SDL_Surface, which represents the window
 * where the image will be displayed.
 * @param image The `image` parameter is a pointer to an SDL_Surface, which represents an image that
 * will be displayed on the window during the main loop of the program.
*/
void drawMenu(SDL_Surface* surface, SDL_Surface* image) {
    SDL_Rect positionImage = {0, 0, 0, 0};
    SDL_BlitSurface(image, NULL, surface, &positionImage);
    drawHighlight(surface, pageParam.x, pageParam.y, pageParam.width, pageParam.height);
}

void drawParametre(SDL_Surface* surface) {
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 100, 100, 100));
    drawHighlight(surface, pageParam.x, pageParam.y, pageParam.width, pageParam.height);
}

// Draw a red highlight around a rectangle FOR DEBUGGING PURPOSES
void drawHighlight(SDL_Surface* surface, int x, int y, int width, int height) {
    SDL_Rect rect = {x, y, width, height};
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 255, 0, 0));
}