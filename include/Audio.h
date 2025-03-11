#ifndef AUDIO_H
#define AUDIO_H
/**
 * @file Audio.h
 * @brief Gestion de l'audio pour l'application.
 * 
 * Ce fichier contient les définitions et les fonctions nécessaires pour initialiser et gérer l'audio dans l'application, en utilisant SDL_mixer.
 * 
 * @author Louis
 * @date 2/01/2025
 */
#include "Log.h"

#if defined(_WIN32) || defined(_WIN64)
    // Sur Windows
    #define AUDIO_FREQ 44100 /**< Fréquence audio pour Windows. */
#elif defined(__linux__)
    // Sur Linux
    #define AUDIO_FREQ 22050 /**< Fréquence audio pour Linux. */
#else
    // macOS, BSD, etc.
    #define AUDIO_FREQ 44100 /**< Fréquence audio par défaut pour macOS, BSD, etc. */
#endif

#include <SDL2/SDL_mixer.h>

/**
 * @brief Initialise le système audio.
 * 
 * Cette fonction initialise le système audio en configurant SDL_mixer avec les paramètres appropriés.
 */
void initAudio();

/**
 * @brief Charge un fichier de musique.
 * 
 * Cette fonction charge un fichier de musique spécifié et l'associe à un pointeur Mix_Music.
 * 
 * @param music Un pointeur vers un pointeur Mix_Music où la musique chargée sera stockée.
 * @param musicPath Le chemin du fichier de musique à charger.
 */
void loadMusic(Mix_Music **music, const char *musicPath);

#endif // AUDIO_H