#include "../include/RenderICMons.h"

IMG_ICMons *initICMonSprite(SDL_Renderer *renderer, const char *imagePath, int x, int y, int w, int h, t_Poke *poke, TTF_Font *font) {
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
    if (!img->ICMonTexture) {
        SDL_Log("❌ Erreur lors de la création de la texture : %s", SDL_GetError());
        free(img);
        return NULL;
    }
    img->rect = img->initialRect = (SDL_Rect){x, y, w, h};
    char pv[10], name[10];
    snprintf(pv, sizeof(pv), "%d/%d", poke->current_pv, poke->baseStats[PV]);
    img->PVText = initText(renderer, pv, font, x, y, w, h);
    img->PVbarTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    img->PVbarTextureBack = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    img->PVRect = img->PVInitialRect = (SDL_Rect){x, y, w, h};
    snprintf(name, sizeof(name), "%s", poke->name);
    img->nameTexture = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, name, (SDL_Color){255, 255, 255, 255}));
    img->nameRect = img->nameInitialRect = (SDL_Rect){x, y, w, h};
    return img;
}