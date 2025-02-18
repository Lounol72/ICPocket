#ifndef 
#define

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GameEngine.h"
#include "structPoke.h"


typedef struct {
    SDL_Renderer *renderer;
    SDL_Texture *ICMonTexture;
    SDL_Rect rect;
    SDL_Rect initialRect;
    Text *PVText;
    SDL_Texture *PVbarTexture;
    SDL_Texture *PVbarTextureBack;
    SDL_Rect PVRect;
    SDL_Rect PVInitialRect;
    SDL_Texture *nameTexture;
    SDL_Rect nameRect;
    SDL_Rect nameInitialRect;
}IMG_ICMons;

IMG_ICMons *initICMonSprite(SDL_Renderer *renderer, const char *imagePath, int x, int y, int w, int h, t_Poke *poke, TTF_Font *font);

void renderICMonSprite(IMG_ICMons *poke);
void updateICMonSprite(IMG_ICMons *poke, t_Poke *pokeData);
void updateICMonPosition(IMG_ICMons *poke, int ScaleX, int ScaleY);
void destroyICMonSprite(IMG_ICMons *poke);