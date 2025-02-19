#include "../include/Log.h"
#include <stdio.h>
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

void InitLogFile() {
    FILE *logFile = fopen("sdl_log.log", "w"); // Open in write mode
    if (logFile) {
        fclose(logFile);
    }
}