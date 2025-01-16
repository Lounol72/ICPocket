#include "parametre.h"
#include "boutons.h"
#include "utils.h"
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <stdio.h>

extern Slider volumeSlider;
extern Bouton retourMenu;
extern Bouton sauvegarderMenu;

// Function to draw the parameters on the given surface
void drawParametre(SDL_Surface* surface) {
    TTF_Font* font = initializeFont("assets/fonts/arial.ttf", 24);
    if (!font) {
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
    drawButton(surface, &retourMenu, font);

    // Draw the sauvegarderMenu button
    drawButton(surface, &sauvegarderMenu, font);

    // Draw the volume slider
    drawVolumeControl(surface, volumeSlider.volume);

    // Free the memory
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
    TTF_Quit();
}

// Function to draw the volume control on the given surface
void drawVolumeControl(SDL_Surface* surface, int volume) {
    TTF_Font* font = initializeFont("assets/fonts/Pokemon Solid.ttf", 24);
    if (!font) {
        return;
    }

    // Convert volume to percentage
    int volumePercentage = volume * 100 / MIX_MAX_VOLUME;
    char volumeText[20];
    snprintf(volumeText, sizeof(volumeText), "Volume: %d", volumePercentage);
    // Create the text surface
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, volumeText, textColor);
    if (!textSurface) {
        SDL_Log("Erreur création surface texte : %s", TTF_GetError());
        TTF_CloseFont(font);
        TTF_Quit();
        return;
    }

    // Clear the area where the volume text will be drawn
    SDL_Rect clearRect = {100, 50, textSurface->w, textSurface->h};
    SDL_FillRect(surface, &clearRect, SDL_MapRGB(surface->format, 192, 192, 192)); // Clear with background color

    // Position and draw the text
    SDL_Rect textRect = {100, 50, textSurface->w, textSurface->h};
    SDL_BlitSurface(textSurface, NULL, surface, &textRect);

    // Draw the volume control using volumeSlider
    SDL_FillRect(surface, &volumeSlider.bar, SDL_MapRGB(surface->format, 255, 255, 255)); 
    SDL_FillRect(surface, &volumeSlider.cursor, SDL_MapRGB(surface->format, 255, 0, 0)); 

    // Free the memory
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
    // Remove TTF_Quit() here to avoid quitting TTF prematurely
}
