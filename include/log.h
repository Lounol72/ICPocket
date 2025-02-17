#ifndef LOG_H
#define LOG_H

#include "GameEngine.h"

void LogToFile(void *userdata, int category, SDL_LogPriority priority, const char *message);
void InitLogFile();

#endif /* LOG_H */