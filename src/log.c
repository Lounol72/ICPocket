#include "../include/log.h"

void LogToFile(void *userdata, int category, SDL_LogPriority priority, const char *message) {
    FILE *logFile = fopen("sdl_log.txt", "a"); // Ouvrir en mode ajout
    if (logFile) {
        fprintf(logFile, "[%d] [%d] %s\n", category, priority, message);
        fclose(logFile);
    }
}

void InitLogFile() {
    FILE *logFile = fopen("sdl_log.txt", "w"); // Ouvrir en mode écriture pour écraser le fichier
    if (logFile) {
        fclose(logFile);
    }
}