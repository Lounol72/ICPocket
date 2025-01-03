#ifndef VOLUME_H
#define VOLUME_H

#include <SDL2/SDL.h>

extern Slider volumeSlider;

void drawVolumeControl(SDL_Surface* surface, int volume);

#endif // VOLUME_H