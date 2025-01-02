#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "volume.h"

void drawVolumeControl(SDL_Surface* surface, int volume) {
    // Initialiser SDL_ttf
    if (TTF_Init() == -1) {
        printf("Erreur d'initialisation de SDL_ttf : %s\n", TTF_GetError());
        return;
    }

    // Charger la police
    TTF_Font* font = TTF_OpenFont("assets/fonts/arial.ttf", 24);
    if (!font) {
        printf("Erreur de chargement de la police : %s\n", TTF_GetError());
        TTF_Quit();
        return;
    }

    // Créer la surface de texte
    SDL_Color textColor = {255, 255, 255, 255}; // Couleur blanche avec alpha
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Volume de la musique", textColor);
    if (!textSurface) {
        printf("Erreur de rendu du texte : %s\n", TTF_GetError());
        TTF_CloseFont(font);
        TTF_Quit();
        return;
    }

    // Positionner le texte
    SDL_Rect textRect = {100, 10, textSurface->w, textSurface->h};
    SDL_BlitSurface(textSurface, NULL, surface, &textRect);

    // Dessiner la barre de défilement
    SDL_Rect bar = {100, 50, 400, 20}; // Position et taille de la barre de défilement
    SDL_Rect cursor = {100 + (volume * 400 / MIX_MAX_VOLUME), 45, 10, 30}; // Position et taille du curseur

    SDL_FillRect(surface, &bar, SDL_MapRGB(surface->format, 255, 255, 255)); // Dessiner la barre de défilement
    SDL_FillRect(surface, &cursor, SDL_MapRGB(surface->format, 255, 0, 0)); // Dessiner le curseur

    // Libérer les ressources
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
    TTF_Quit();
}