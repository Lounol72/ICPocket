#include "../include/RenderICMons.h"

IMG_ICMons *initICMonSprite(SDL_Renderer *renderer,SDL_Rect spriteRect,SDL_Rect nameRect, SDL_Rect pvRect, t_Poke *poke, TTF_Font *font, int team) {
    if (!renderer || !poke || !font) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "❌ Invalid parameters for initICMonSprite");
        return NULL;
    }
    
    IMG_ICMons *img = malloc(sizeof(IMG_ICMons));
    if (!img) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "❌ Memory allocation failed for IMG_ICMons");
        return NULL;
    }
    
    memset(img, 0, sizeof(IMG_ICMons)); // Initialize all members to 0/NULL
    img->renderer = renderer;
    
    char path[128];
    snprintf(path, sizeof(path), "assets/Monsters/New Versions/%s.png", poke->name);
    
    SDL_Surface *surface = IMG_Load(path);
    if (team == 1) {
        // flip the surface horizontally
        SDL_Surface *flippedSurface = SDL_CreateRGBSurface(0, surface->w, surface->h, 
            surface->format->BitsPerPixel, 
            surface->format->Rmask, 
            surface->format->Gmask, 
            surface->format->Bmask, 
            surface->format->Amask);
        
        SDL_Rect srcRect = {0, 0, surface->w, surface->h};
        SDL_Rect dstRect = {0, 0, surface->w, surface->h};
        
        for (int x = 0; x < surface->w; x++) {
            srcRect.x = x;
            dstRect.x = surface->w - 1 - x;
            srcRect.w = 1;
            dstRect.w = 1;
            SDL_BlitSurface(surface, &srcRect, flippedSurface, &dstRect);
        }
        
        SDL_FreeSurface(surface);
        surface = flippedSurface;
    }
    if (!surface) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "❌ Failed to load image: %s", path);
        free(img);
        return NULL;
    }
    
    img->ICMonTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!img->ICMonTexture) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "❌ Failed to create texture");
        free(img);
        return NULL;
    }
    
    img->rect = img->initialRect = spriteRect;
    
    // Create PV text
    char pv[32];
    snprintf(pv, sizeof(pv), "%d/%d", poke->current_pv, poke->initial_pv);
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "PV : %s", pv);
    img->PVText = createText(pv, renderer, pvRect, (SDL_Color){255,255,255,255}, font);
    
    // Initialize PV bar textures
    img->PVRect = img->PVInitialRect = (SDL_Rect){pvRect.x, pvRect.y, spriteRect.w, pvRect.h};
    
    // Create name texture
    SDL_Surface *nameSurface = TTF_RenderText_Solid(font, poke->name, (SDL_Color){255, 255, 255, 255});
    if (nameSurface) {
        img->nameTexture = SDL_CreateTextureFromSurface(renderer, nameSurface);
        SDL_FreeSurface(nameSurface);
        img->nameRect = img->nameInitialRect = nameRect;
    }
    
    return img;
}

void renderICMonsSprite(Window *win, t_Poke *poke) {
    if(!poke || !poke->img) return;
    
    // Update PV text
    char pv[32];
    snprintf(pv, sizeof(pv), "%d/%d", poke->current_pv, poke->initial_pv);
    updateText(poke->img->PVText, pv, win->renderer);
    
    // Set render target back to the default
    SDL_SetRenderTarget(poke->img->renderer, NULL);
    
    // Render the ICmons texture
    SDL_RenderCopy(poke->img->renderer, poke->img->ICMonTexture, NULL, &poke->img->rect);
    
    // Render the PV bar background
    SDL_SetRenderDrawColor(poke->img->renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(poke->img->renderer, &poke->img->PVRect);
    
    // Render the PV bar foreground
    SDL_SetRenderDrawColor(poke->img->renderer, 0, 255, 0, 255);
    SDL_Rect pvForeground = poke->img->PVRect;
    float healthPercentage = ((float)poke->current_pv / poke->initial_pv);
    // Clamp health percentage between 0 and 1
    healthPercentage = healthPercentage > 1.0f ? 1.0f : (healthPercentage < 0.0f ? 0.0f : healthPercentage);
    pvForeground.w = (int)(pvForeground.w * healthPercentage);
    SDL_RenderFillRect(poke->img->renderer, &pvForeground);
    
    // Render the PV text
    if(poke->img->PVText) {
        renderText(win, poke->img->PVText);
    }
    
    // Render the name
    if(poke->img->nameTexture) {
        SDL_RenderCopy(poke->img->renderer, poke->img->nameTexture, NULL, &poke->img->nameRect);
    }
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