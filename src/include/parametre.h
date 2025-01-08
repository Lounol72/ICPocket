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

#endif // VOLUME_H