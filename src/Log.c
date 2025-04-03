#include "../include/Log.h"
#include <stdio.h>

/**
 * @fn void LogToFile(void *userdata, int category, SDL_LogPriority priority, const char *message)
 * @brief cette fonction permet d'écrire les logs dans un fichier
 * 
 * @param userdata Un pointeur vers des données définies par l'utilisateur qui peuvent être passées à la fonction.
 *                 Ce paramètre permet à l'utilisateur de fournir un contexte ou des informations supplémentaires
 *                 dont la fonction pourrait avoir besoin pendant son exécution.
 * 
 * @param category La catégorie du log, utilisée pour organiser les messages de log.
 * @param priority La priorité du log, indiquant le niveau d'importance du message.
 * @param message Le message de log à écrire dans le fichier.
 */
void LogToFile(void *userdata, int category, SDL_LogPriority priority, const char *message) {
    (void)userdata;
    FILE *logFile = fopen("sdl_log.log", "a"); // Open in append mode
    if (logFile) {
        fprintf(logFile, "[%d] [%d] %s\n", category, priority, message);
        fclose(logFile);
    }
    else {
        SDL_Log("❌ Erreur lors de l'ouverture du fichier de log : %s", SDL_GetError());
    }
}

/**
 * @fn void InitLogFile()
 * @brief cette fonction permet d'initialiser le fichier de log
 * 
 * Cette fonction crée un fichier de log et configure SDL pour écrire les messages de log dans ce fichier.
 */
void InitLogFile() {
    FILE *logFile = fopen("sdl_log.log", "w"); // Open in write mode
    if (logFile) {
        fclose(logFile);
    }
}