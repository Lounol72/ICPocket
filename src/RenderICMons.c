#include "../include/RenderICMons.h"

IMG_ICMons *initICMonSprite(SDL_Renderer *renderer, int x, int y, int w, int h, t_Poke *poke, TTF_Font *font) {
    IMG_ICMons *img = malloc(sizeof(IMG_ICMons));
    if (!img) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "❌ Erreur lors de l'allocation de la mémoire pour l'image");
        return NULL;
    }
    img->renderer = renderer;
    char path[50];
    snprintf(path, sizeof(path), "assets/Monsters/%s.png", poke->name);
    SDL_Surface *surface = IMG_Load(path);
    if (!surface) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "❌ Erreur lors du chargement de l'image : %s", IMG_GetError());
        free(img);
        return NULL;
    }
    img->ICMonTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!img->ICMonTexture) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "❌ Erreur lors de la création de la texture : %s", SDL_GetError());
        free(img);
        return NULL;
    }
    img->rect = img->initialRect = (SDL_Rect){x, y, w, h};
    char pv[10], Name[20];
    snprintf(pv, sizeof(pv), "%d/%d", poke->current_pv, poke->baseStats[PV]);
    img->PVText = createText(pv,renderer, img->rect,(SDL_Color){255,255,255,255},font);
    img->PVbarTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    img->PVbarTextureBack = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    img->PVRect = img->PVInitialRect = (SDL_Rect){x, y, w, h};
    snprintf(Name, sizeof(Name), "%s", poke->name);
    img->nameTexture = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, Name, (SDL_Color){255, 255, 255, 255}));
    img->nameRect = img->nameInitialRect = (SDL_Rect){x, y, w, h};
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ Initialisation de l'image réussie");
    return img;
}

void renderICMonsSprite(Window *win, t_Poke *poke) {
    if(!poke->img) return;
    //render the ICmons texture
    SDL_RenderCopy(poke->img->renderer, poke->img->ICMonTexture, NULL, &poke->img->rect);
    //render the PV bar background
    SDL_SetRenderTarget(poke->img->renderer, poke->img->PVbarTextureBack);
    SDL_SetRenderDrawColor(poke->img->renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(poke->img->renderer, &poke->img->PVRect);
    //render the PV bar foreground
    SDL_SetRenderTarget(poke->img->renderer, poke->img->PVbarTexture);
    SDL_SetRenderDrawColor(poke->img->renderer, 0, 255, 0, 255);
    SDL_Rect pvForeground = poke->img->PVRect;
    pvForeground.w = (int)(pvForeground.w * ((float)poke->current_pv / poke->baseStats[PV]));
    SDL_RenderFillRect(poke->img->renderer, &pvForeground);
    //render the PV text
    renderText(win, poke->img->PVText);
    //render the name
    SDL_RenderCopy(poke->img->renderer, poke->img->nameTexture, NULL, &poke->img->nameRect);
}


void destroyICMonsSprite(t_Poke *poke) {
    if(!poke->img) return;
    SDL_DestroyTexture(poke->img->ICMonTexture);
    SDL_DestroyTexture(poke->img->PVbarTexture);
    SDL_DestroyTexture(poke->img->PVbarTextureBack);
    SDL_DestroyTexture(poke->img->nameTexture);
    destroyText(poke->img->PVText);
    free(poke->img);
    poke->img = NULL;
}

void updateICMonsSprite(t_Poke *poke, float scaleX, float scaleY) {
    if(!poke->img) return;
    poke->img->rect = (SDL_Rect){poke->img->initialRect.x * scaleX, poke->img->initialRect.y * scaleY, poke->img->initialRect.w * scaleX, poke->img->initialRect.h * scaleY};
    poke->img->PVRect = (SDL_Rect){poke->img->PVInitialRect.x * scaleX, poke->img->PVInitialRect.y * scaleY, poke->img->PVInitialRect.w * scaleX, poke->img->PVInitialRect.h * scaleY};
    poke->img->nameRect = (SDL_Rect){poke->img->nameInitialRect.x * scaleX, poke->img->nameInitialRect.y * scaleY, poke->img->nameInitialRect.w * scaleX, poke->img->nameInitialRect.h * scaleY};
    updateTextPosition(poke->img->PVText, scaleX, scaleY);
}