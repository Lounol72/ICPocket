#ifndef LOG_H
#define LOG_H

/**
 * @file Log.h
 * @author Louis
 * @brief Initialisation des fonctions de logs
 * @date 17/02/2025
 */

#include <SDL2/SDL.h>

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
void LogToFile(void *userdata, int category, SDL_LogPriority priority, const char *message);

/**
 * @fn void InitLogFile()
 * @brief cette fonction permet d'initialiser le fichier de log
 * 
 * Cette fonction crée un fichier de log et configure SDL pour écrire les messages de log dans ce fichier.
 */
void InitLogFile();

#endif // LOG_H