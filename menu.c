#include <SDL2/SDL.h>
#include "menu.h"
#include "boutons.h"


void drawMenu(SDL_Surface* surface, SDL_Surface* image) {
    SDL_Rect positionImage = {0, 0};
    SDL_BlitSurface(image, NULL, surface, &positionImage);
    drawHighlight(surface, pageParam.x, pageParam.y, pageParam.width, pageParam.height);
}

void drawParametre(SDL_Surface* surface) {
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 100, 100, 100));
    drawHighlight(surface, pageParam.x, pageParam.y, pageParam.width, pageParam.height);
}

void drawHighlight(SDL_Surface* surface, int x, int y, int width, int height) {
    SDL_Rect rect = {x, y, width, height};
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 255, 0, 0));
}