#include "../include/RenduICMons.h"

IMG_ICMons *initICMonSprite(SDL_Renderer *renderer, const char *imagePath, int x, int y, int w, int h, t_Poke *poke){
    IMG_ICMons *img = malloc(sizeof(IMG_ICMons));
    if (!img) {
        SDL_Log("❌ Erreur lors de l'allocation de la structure IMG_ICMons.");
        return NULL;
    }
    img->renderer = renderer;
    SDL_Surface *surface = IMG_Load(imagePath);
    if (!surface) {
        SDL_Log("❌ Erreur lors du chargement de l'image : %s", IMG_GetError());
        free(img);
        return NULL;
    }
    img->ICMonTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = NULL;
    if (!img->ICMonTexture) {
        SDL_Log("❌ Erreur lors de la création de la texture : %s", SDL_GetError());
        free(img);
        return NULL;
    }
    img->rect = (SDL_Rect){x, y, w, h};
    img->initialRect = img->rect;
    img->PVText = NULL;
    img->PVbarTexture = NULL;
    img->PVbarTextureBack = NULL;
    img->PVRect = (SDL_Rect){x, y, w, h};
    img->PVInitialRect = img->PVRect;
    img->nameTexture = NULL;
    img->nameRect = (SDL_Rect){x, y, w, h};
    img->nameInitialRect = img->nameRect;
    return img;
}