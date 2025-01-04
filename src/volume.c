#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "volume.h"
#include "boutons.h"

// The volume slider


void drawVolumeControl(SDL_Surface* surface, int volume) {
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("Erreur d'initialisation de SDL_ttf : %s\n", TTF_GetError());
        return;
    }

    // Load the font
    TTF_Font* font = TTF_OpenFont("assets/fonts/Pokemon Solid.ttf", 18);
    if (!font) {
        printf("Erreur de chargement de la police : %s\n", TTF_GetError());
        TTF_Quit();
        return;
    }

    // Create the text surface
    SDL_Color textColor = {0,0,0,0}; // Black color
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Volume de la musique :", textColor);
    if (!textSurface) {
        printf("Erreur de rendu du texte : %s\n", TTF_GetError());
        TTF_CloseFont(font);
        TTF_Quit();
        return;
    }

    // Position and draw the text
    SDL_Rect textRect = {100, 10, textSurface->w, textSurface->h};
    SDL_BlitSurface(textSurface, NULL, surface, &textRect);

    // Draw the volume control using volumeSlider
    SDL_Rect bar = {volumeSlider.xStart, volumeSlider.yStart, volumeSlider.xEnd - volumeSlider.xStart, volumeSlider.yEnd - volumeSlider.yStart};
    SDL_Rect cursor = {volumeSlider.xStart + (volume * (volumeSlider.xEnd - volumeSlider.xStart) / MIX_MAX_VOLUME), volumeSlider.yStart - 5, 10, 30};

    SDL_FillRect(surface, &bar, SDL_MapRGB(surface->format, 255, 255, 255)); 
    SDL_FillRect(surface, &cursor, SDL_MapRGB(surface->format, 255, 0, 0)); 

    // Free the memory
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
    TTF_Quit();
}