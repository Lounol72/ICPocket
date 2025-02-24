#ifndef RENDERICMONS_H
#define RENDERICMONS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GameEngine.h"
#include "Text.h"
#include "structPoke.h"

typedef struct IMG_ICMons {
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

IMG_ICMons *initICMonSprite(SDL_Renderer *renderer,SDL_Rect spriteRect,SDL_Rect nameRect, SDL_Rect pvRect, t_Poke *poke, TTF_Font *font, int team);
void updateICMonsSprite(t_Poke *poke, float scaleX, float scaleY);
void renderICMonsSprite(Window *win, t_Poke *poke);
void destroyICMonsSprite(t_Poke *poke);

#endif