#ifndef AUDIO_H
#define AUDIO_H
/**
 * @file Audio.h
 * @author Louis
 * @date 2/01/2025
 */
#include "Log.h"

#if defined(_WIN32) || defined(_WIN64)
    // Sur Windows
    #define AUDIO_FREQ 44100
#elif defined(__linux__)
    // Sur Linux
    #define AUDIO_FREQ 22050
#else
    // macOS, BSD, etc.
    #define AUDIO_FREQ 44100
#endif

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>

void initAudio();
void loadMusic(Mix_Music **music, const char *musicPath);

#endif // AUDIO_H