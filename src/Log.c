#include "../include/Log.h"

void LogToFile(void *userdata, int category, SDL_LogPriority priority, const char *message) {
    (void)userdata;
    FILE *logFile = fopen("sdl_log.log", "a"); // Open in append mode
    if (logFile) {
        fprintf(logFile, "[%d] [%d] %s\n", category, priority, message);
        fclose(logFile);
    }
}

void InitLogFile() {
    FILE *logFile = fopen("sdl_log.log", "w"); // Open in write mode
    if (logFile) {
        fclose(logFile);
    }
}