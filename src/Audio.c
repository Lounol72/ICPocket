#include "../include/Audio.h"

/**
 * @file Audio.c
 * @brief Gestion de l'audio
 * @author Louis Subtil
 */


/**
 * @fn void initAudio()
 * @brief Initialise le système audio.
 * 
 * Cette fonction initialise le système audio en configurant SDL_mixer avec les paramètres appropriés.
 */
void initAudio() {
    SDL_LogMessage(SDL_LOG_CATEGORY_AUDIO, SDL_LOG_PRIORITY_INFO, "🔊 Initialisation de l'audio ...");

    if (Mix_Init(MIX_INIT_MP3) == 0) {  
        SDL_LogMessage(SDL_LOG_CATEGORY_AUDIO, SDL_LOG_PRIORITY_ERROR, "❌ Erreur Mix_Init: %s", Mix_GetError());
        exit(EXIT_FAILURE);
    } else {
        SDL_LogMessage(SDL_LOG_CATEGORY_AUDIO, SDL_LOG_PRIORITY_INFO, "✅ Mix_Init réussi.");
    }
    if (Mix_OpenAudio(AUDIO_FREQ, MIX_DEFAULT_FORMAT, 2, 8192) < 0) {
        SDL_LogMessage(SDL_LOG_CATEGORY_AUDIO, SDL_LOG_PRIORITY_ERROR, "❌ Erreur Mix_OpenAudio: %s", Mix_GetError());
        exit(EXIT_FAILURE);
    } else {
        SDL_LogMessage(SDL_LOG_CATEGORY_AUDIO, SDL_LOG_PRIORITY_INFO, "✅ Mix_OpenAudio réussi.");
    }
    Mix_SetMusicCMD(NULL);
}

/**
 * @fn void loadMusic(Mix_Music **music, const char *musicPath)
 * @brief Charge un fichier de musique.
 * 
 * Cette fonction charge un fichier de musique spécifié et l'associe à un pointeur Mix_Music.
 * 
 * @param music Un pointeur vers un pointeur Mix_Music où la musique chargée sera stockée.
 * @param musicPath Le chemin du fichier de musique à charger.
 */
void loadMusic(Mix_Music **music, const char *musicPath) {
    *music = Mix_LoadMUS(musicPath);
    if (!*music) {
        SDL_LogMessage(SDL_LOG_CATEGORY_AUDIO, SDL_LOG_PRIORITY_ERROR, "❌ Erreur Mix_LoadMUS: %s", Mix_GetError());
        exit(EXIT_FAILURE);
    } else {
        SDL_LogMessage(SDL_LOG_CATEGORY_AUDIO, SDL_LOG_PRIORITY_INFO, "✅ Chargement de la musique réussi.");
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
}