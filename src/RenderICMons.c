/**
 * @file RenderICMons.c
 * @brief Fonctions de gestion des sprites ICMon.
 */

#include "../include/RenderICMons.h"

/**
 * @brief Retourne horizontalement une surface.
 *
 * Cette fonction crée une nouvelle surface en retournant les pixels de la surface d'origine horizontalement.
 *
 * @param surface La SDL_Surface à retourner.
 * @return SDL_Surface* La surface retournée, ou NULL en cas d'erreur.
 */
static SDL_Surface *flipSurfaceHorizontal(SDL_Surface *surface) {
    if (!surface) return NULL;
    
    SDL_Surface *flipped = SDL_CreateRGBSurfaceWithFormat(0, surface->w, surface->h, 
                                                           surface->format->BitsPerPixel, 
                                                           surface->format->format);
    if (!flipped) {
        SDL_Log("❌ Unable to create flipped surface: %s", SDL_GetError());
        return NULL;
    }
    
    // Verrouillage des surfaces si nécessaire
    if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);
    if (SDL_MUSTLOCK(flipped)) SDL_LockSurface(flipped);
    
    int bpp = surface->format->BytesPerPixel;
    for (int y = 0; y < surface->h; y++) {
        Uint8 *src = (Uint8 *)surface->pixels + y * surface->pitch;
        Uint8 *dst = (Uint8 *)flipped->pixels + y * flipped->pitch;
        for (int x = 0; x < surface->w; x++) {
            memcpy(dst + (surface->w - 1 - x) * bpp, src + x * bpp, bpp);
        }
    }
    
    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);
    if (SDL_MUSTLOCK(flipped)) SDL_UnlockSurface(flipped);
    
    return flipped;
}

/**
 * @brief Charge un fichier audio à partir d'une ligne spécifique.
 *
 * Cette fonction charge un fichier audio WAV à partir d'une ligne spécifique d'un fichier CSV.
 *
 * @param line La ligne du fichier CSV contenant le chemin du fichier audio.
 * @return Mix_Chunk* Le pointeur vers le fichier audio chargé, ou NULL en cas d'erreur.
 */
/*
static Mix_Chunk *loadFromLine(int line) {
    char path[128];
    FILE *file = fopen("assets/audio/soundEffects/Enregistrement.csv", "r");
    if (!file) {
        SDL_Log("❌ Échec de l'ouverture du fichier : %s", SDL_GetError());
        return NULL;
    }
    
    int currentLine = 0;
    while (fgets(path, sizeof(path), file)) {
        if (currentLine == line) {
            // Suppression du caractère de fin de ligne si présent
            char *newline = strchr(path, '\n');
            if (newline)
                *newline = '\0';
            break;
        }
        currentLine++;
    }
    fclose(file);
    
    if (currentLine != line) {
        SDL_Log("❌ La ligne %d n'a pas été trouvée dans le fichier.", line);
        return NULL;
    }
    
    Mix_Chunk *chunk = Mix_LoadWAV(path);
    if (!chunk) {
        SDL_Log("❌ Échec du chargement du fichier WAV %s : %s", path, Mix_GetError());
    }
    
    return chunk;
}*/

/**
 * @brief Initialise le sprite d'un ICMon.
 *
 * Cette fonction crée et initialise le sprite associé à un poke, incluant le chargement
 * de l'image, le retournement horizontal si nécessaire, la création des textures, ainsi que
 * la configuration des textes et barres de points de vie.
 *
 * @param renderer Le renderer SDL.
 * @param spriteRect Le rectangle du sprite.
 * @param nameRect Le rectangle pour le nom.
 * @param pvRect Le rectangle pour la barre de points de vie.
 * @param poke Le pointeur vers la structure t_Poke contenant les informations du poke.
 * @param font La police TTF utilisée pour le texte.
 * @param team L'équipe (si 1, l'image est retournée horizontalement).
 * @return IMG_ICMons* Le pointeur vers le sprite initialisé, ou NULL en cas d'erreur.
 */
IMG_ICMons *initICMonSprite(SDL_Renderer *renderer, SDL_Rect spriteRect, SDL_Rect nameRect, SDL_Rect pvRect, t_Poke *poke, TTF_Font *font, int team) {
    if (!renderer || !poke || !font) {
        SDL_Log("❌ Invalid parameters for initICMonSprite");
        return NULL;
    }
    
    // Allocation et initialisation de la structure
    IMG_ICMons *img = calloc(1, sizeof(IMG_ICMons));
    if (!img) {
        SDL_Log("❌ Memory allocation failed for IMG_ICMons");
        return NULL;
    }
    
    img->renderer = renderer;
    
    // Construction du chemin de l'image
    char path[128];
    snprintf(path, sizeof(path), "assets/Monsters/New Versions/%s.png", poke->name);
    
    SDL_Surface *surface = IMG_Load(path);
    if (!surface) {
        SDL_Log("❌ Failed to load image: %s", path);
        free(img);
        return NULL;
    }
    
    // Si l'équipe est 1, retourne l'image horizontalement
    if (team == 1) {
        SDL_Surface *flippedSurface = flipSurfaceHorizontal(surface);
        SDL_FreeSurface(surface);
        if (!flippedSurface) {
            SDL_Log("❌ Failed to flip image for team %d", team);
            free(img);
            return NULL;
        }
        surface = flippedSurface;
    }
    
    // Création de la texture depuis la surface
    img->ICMonTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!img->ICMonTexture) {
        SDL_Log("❌ Failed to create texture from image");
        free(img);
        return NULL;
    }
    
    img->rect = img->initialRect = spriteRect;
    
    // Ajout affichage lvl
    char lvlTextBuffer[32];
    snprintf(lvlTextBuffer, sizeof(lvlTextBuffer), "N.%d", poke->lvl);
    img->LvlText = createText(lvlTextBuffer, renderer, (SDL_Rect){spriteRect.x + spriteRect.h - 25, nameRect.y, nameRect.w-5, nameRect.h}, (SDL_Color){255,255,255,255}, font);
    
    
    // Création du texte de points de vie (PV)
    char pvTextBuffer[32];
    snprintf(pvTextBuffer, sizeof(pvTextBuffer), "%d/%d", poke->current_pv, poke->initial_pv);
    SDL_Log("PV : %s", pvTextBuffer);
    img->PVText = createText(pvTextBuffer, renderer, pvRect, (SDL_Color){255,255,255,255}, font);
    
    // Initialisation de la barre de PV (la largeur est définie par celle du sprite)
    img->PVRect = img->PVInitialRect = (SDL_Rect){pvRect.x, pvRect.y, spriteRect.w, pvRect.h};
    
    // Création de la texture du nom
    SDL_Surface *nameSurface = TTF_RenderText_Solid(font, poke->name, (SDL_Color){255, 255, 255, 255});
    if (nameSurface) {
        img->nameTexture = SDL_CreateTextureFromSurface(renderer, nameSurface);
        SDL_FreeSurface(nameSurface);
        img->nameRect = img->nameInitialRect = nameRect;
    } else {
        SDL_Log("❌ Failed to render name text for %s", poke->name);
    }

    if (team == 1) {
        free(img->PVText);
        img->PVText = NULL;
    }
    Mix_Chunk *ICMonSound = Mix_LoadWAV("assets/audio/soundEffects/Enregistrement.mp3");
    if (!ICMonSound) {
        SDL_Log("❌ Failed to load ICMonSound: %s", Mix_GetError());
    }
    img->ICMonSound = ICMonSound;

    return img;
}
static void updatePVBar(t_Poke *poke) {
    if (!poke || !poke->img) return;
    
    // Calcul du pourcentage de vie, en s'assurant qu'il est entre 0 et 1
    float healthPercentage = (float)poke->current_pv / poke->initial_pv;
    healthPercentage = (healthPercentage > 1.0f) ? 1.0f : ((healthPercentage < 0.0f) ? 0.0f : healthPercentage);
    
    // Calcul de la largeur cible de la barre de HP
    int targetWidth = (int)(poke->img->PVInitialRect.w * healthPercentage);
    
    // Animation progressive : on rapproche currentHPWidth de targetWidth
    // Le facteur 0.1f détermine la vitesse d'interpolation (à ajuster selon vos préférences)
    if (fabs(targetWidth - poke->img->currentHPWidth) < 1.0f) {
        poke->img->currentHPWidth = targetWidth;
    } else {
        poke->img->currentHPWidth += (targetWidth - poke->img->currentHPWidth) * 0.1f;
    }
    
    // Dessiner le fond rouge de la barre de HP
    SDL_SetRenderDrawColor(poke->img->renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(poke->img->renderer, &poke->img->PVRect);
    
    // Dessiner la barre de HP en vert avec la largeur interpolée
    SDL_Rect pvForeground = poke->img->PVRect;
    pvForeground.w = (int)poke->img->currentHPWidth;
    SDL_SetRenderDrawColor(poke->img->renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(poke->img->renderer, &pvForeground);
}


/**
 * @brief Rendu du sprite ICMon sur la fenêtre.
 *
 * Cette fonction met à jour et affiche le sprite du poke, incluant le sprite principal,
 * la barre de points de vie (PV) avec son arrière-plan et le texte des PV, ainsi que le nom.
 *
 * @param win La fenêtre contenant le renderer.
 * @param poke Le poke dont le sprite sera rendu.
 */
void renderICMonsSprite(Window *win, t_Poke *poke) {
    if (!poke || !poke->img) return;
    
    // Mise à jour du texte de PV avec les valeurs actuelles
    static char pvBuffer[32];
    snprintf(pvBuffer, sizeof(pvBuffer), "%d/%d", poke->current_pv, poke->initial_pv);
    updateText(poke->img->PVText, pvBuffer, win->renderer);
    
    // Réinitialisation de la cible de rendu
    SDL_SetRenderTarget(poke->img->renderer, NULL);
    
    // Rendu de la texture principale
    SDL_RenderCopy(poke->img->renderer, poke->img->ICMonTexture, NULL, &poke->img->rect);
    
    // Rendu de l'arrière-plan de la barre de PV en rouge
    SDL_SetRenderDrawColor(poke->img->renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(poke->img->renderer, &poke->img->PVRect);
    
    updatePVBar(poke);
    
    // Rendu du texte de PV si disponible
    if (poke->img->PVText) {
        renderText(win, poke->img->PVText);
    }
    
    // Rendu du nom
    if (poke->img->nameTexture) {
        SDL_RenderCopy(poke->img->renderer, poke->img->nameTexture, NULL, &poke->img->nameRect);
    }

    // Rendu du lvl
    if (poke->img->LvlText) {
        renderText(win, poke->img->LvlText);
    }
}

/**
 * @brief Détruit et libère les ressources associées au sprite d'un ICMon.
 *
 * Cette fonction détruit les textures, libère la mémoire allouée pour le sprite et réinitialise
 * le pointeur associé dans la structure du poke.
 *
 * @param poke Le poke dont le sprite sera détruit.
 */
void destroyICMonsSprite(t_Poke *poke) {
    if (!poke->img) return;
    
    if (poke->img->ICMonTexture)
        SDL_DestroyTexture(poke->img->ICMonTexture);
    if (poke->img->PVbarTexture)
        SDL_DestroyTexture(poke->img->PVbarTexture);
    if (poke->img->PVbarTextureBack)
        SDL_DestroyTexture(poke->img->PVbarTextureBack);
    if (poke->img->nameTexture)
        SDL_DestroyTexture(poke->img->nameTexture);
    if (poke->img->LvlText)
        destroyText(poke->img->LvlText);
    destroyText(poke->img->PVText);
    Mix_FreeChunk(poke->img->ICMonSound);
    free(poke->img);
    poke->img = NULL;
}

/**
 * @brief Met à jour le sprite ICMon en appliquant une échelle sur les dimensions et positions.
 *
 * Cette fonction met à jour la taille et la position du sprite, du nom et de la barre de PV
 * en fonction des coefficients d'échelle passés en paramètre.
 *
 * @param poke Le poke dont le sprite sera mis à jour.
 * @param scaleX L'échelle appliquée sur l'axe horizontal.
 * @param scaleY L'échelle appliquée sur l'axe vertical.
 */
void updateICMonsSprite(t_Poke *poke, float scaleX, float scaleY) {
    if (!poke->img) return;
    
    // Mise à jour des dimensions et positions du sprite
    poke->img->rect.w = (int)(poke->img->initialRect.w * scaleX);
    poke->img->rect.h = (int)(poke->img->initialRect.h * scaleY);
    poke->img->rect.x = (int)(poke->img->initialRect.x * scaleX);
    poke->img->rect.y = (int)(poke->img->initialRect.y * scaleY);
    
    // Mise à jour des dimensions et positions du nom
    poke->img->nameRect.w = (int)(poke->img->nameInitialRect.w * scaleX);
    poke->img->nameRect.h = (int)(poke->img->nameInitialRect.h * scaleY);
    poke->img->nameRect.x = (int)(poke->img->nameInitialRect.x * scaleX);
    poke->img->nameRect.y = (int)(poke->img->nameInitialRect.y * scaleY);
    
    // Mise à jour des dimensions et positions de la barre de PV
    poke->img->PVRect.w = (int)(poke->img->PVInitialRect.w * scaleX);
    poke->img->PVRect.h = (int)(poke->img->PVInitialRect.h * scaleY);
    poke->img->PVRect.x = (int)(poke->img->PVInitialRect.x * scaleX);
    poke->img->PVRect.y = (int)(poke->img->PVInitialRect.y * scaleY);
    
    if (poke->img->PVText)
        updateTextPosition(poke->img->PVText, scaleX, scaleY);
    if (poke->img->LvlText)
        updateTextPosition(poke->img->LvlText, scaleX, scaleY);
}
