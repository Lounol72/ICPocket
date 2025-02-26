#ifndef LOG_H
#define LOG_H

/**
 * @file Log.h
 * @author Louis
 * @date 17/02/2025
 */

#include <SDL2/SDL.h>

void LogToFile(void *userdata, int category, SDL_LogPriority priority, const char *message);
void InitLogFile();

#endif // LOG_H