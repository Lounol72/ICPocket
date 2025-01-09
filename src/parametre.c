#include "parametre.h"
#include "boutons.h"
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <stdio.h>

extern Slider volumeSlider;
extern Bouton retourMenu;

// Function to draw the parameters on the given surface
void drawParametre(SDL_Surface* surface) {
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        SDL_Log("Erreur initialisation SDL_ttf : %s", TTF_GetError());
        return;
    }

    // Load the font
    TTF_Font* font = TTF_OpenFont("assets/fonts/arial.ttf", 24);
    if (!font) {
        SDL_Log("Erreur chargement police : %s", TTF_GetError());
        TTF_Quit();
        return;
    }

    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 192, 192, 192)); // RGB for grey

    // Create the text surface
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Parametres", textColor);
    if (!textSurface) {
        SDL_Log("Erreur création surface texte : %s", TTF_GetError());
        TTF_CloseFont(font);
        TTF_Quit();
        return;
    }

    // Position and draw the text
    SDL_Rect textRect = {100, 10, textSurface->w, textSurface->h};
    SDL_BlitSurface(textSurface, NULL, surface, &textRect);

    // Draw the retourMenu button
    
    SDL_FillRect(surface, &retourMenu.rect, SDL_MapRGB(surface->format, retourMenu.color[0], retourMenu.color[1], retourMenu.color[2]));
    SDL_Surface* buttonTextSurface = TTF_RenderText_Solid(font, retourMenu.text, textColor);
    if (!buttonTextSurface) {
        SDL_Log("Erreur création surface texte bouton : %s", TTF_GetError());
    } else {
        SDL_Rect buttonTextRect = {retourMenu.rect.x + (retourMenu.rect.w - buttonTextSurface->w) / 2, retourMenu.rect.y + (retourMenu.rect.h - buttonTextSurface->h) / 2, buttonTextSurface->w, buttonTextSurface->h};
        SDL_BlitSurface(buttonTextSurface, NULL, surface, &buttonTextRect);
        SDL_FreeSurface(buttonTextSurface);
    }

    // Draw the volume slider
    
    SDL_FillRect(surface, &volumeSlider.bar, SDL_MapRGB(surface->format, 255, 255, 255)); 
    SDL_FillRect(surface, &volumeSlider.cursor, SDL_MapRGB(surface->format, 255, 0, 0)); 

    // Free the memory
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
    TTF_Quit();
}

// Function to draw the volume control on the given surface
void drawVolumeControl(SDL_Surface* surface, int volume) {
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        SDL_Log("Erreur initialisation SDL_ttf : %s", TTF_GetError());
        return;
    }

    // Load the font
    TTF_Font* font = TTF_OpenFont("assets/fonts/Pokemon Solid.ttf", 24);
    if (!font) {
        SDL_Log("Erreur chargement police : %s", TTF_GetError());
        TTF_Quit();
        return;
    }

    // Create the text surface
    char volumeText[20];
    snprintf(volumeText, sizeof(volumeText), "Volume: %d", volume);
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, volumeText, textColor);
    if (!textSurface) {
        SDL_Log("Erreur création surface texte : %s", TTF_GetError());
        TTF_CloseFont(font);
        TTF_Quit();
        return;
    }

    // Position and draw the text
    SDL_Rect textRect = {100, 50, textSurface->w, textSurface->h};
    SDL_BlitSurface(textSurface, NULL, surface, &textRect);

    // Draw the volume control using volumeSlider
    SDL_FillRect(surface, &volumeSlider.bar, SDL_MapRGB(surface->format, 255, 255, 255)); 
    SDL_FillRect(surface, &volumeSlider.cursor, SDL_MapRGB(surface->format, 255, 0, 0)); 

    // Free the memory
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
    TTF_Quit();
}