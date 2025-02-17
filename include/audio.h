#ifndef AUDIO_H
#define AUDIO_H

#include "GameEngine.h"

void initAudio();
void loadMusic(Mix_Music **music, const char *musicPath);

#endif /* AUDIO_H */