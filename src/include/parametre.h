#ifndef VOLUME_H
#define VOLUME_H

#include <SDL2/SDL.h>
#include "boutons.h"

/**
 * @brief Dessine les paramètres sur la surface donnée.
 * 
 * @param surface La surface SDL sur laquelle dessiner les paramètres.
 */
void drawParametre(SDL_Surface* surface);

/**
 * @brief Dessine le contrôle du volume sur la surface donnée.
 * 
 * @param surface La surface SDL sur laquelle dessiner le contrôle du volume.
 * @param volume Le volume actuel à afficher.
 */
void drawVolumeControl(SDL_Surface* surface, int volume);

/**
 * @brief Initialise une police de caractères avec le chemin et la taille donnés.
 * 
 * @param fontPath Le chemin du fichier de police de caractères.
 * @param fontSize La taille de la police de caractères.
 * @return TTF_Font* La police de caractères initialisée.
 */
TTF_Font* initializeFont(const char* fontPath, int fontSize);

#endif // VOLUME_H