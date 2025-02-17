#ifndef ICMONS_H
#define ICMONS_H

#include "GameEngine.h"

void updateICMonsSprite(t_Poke *poke, float scaleX, float scaleY);
void initICMonsSprite(SDL_Renderer *renderer, const char *imagePath, t_Poke *poke, int x, int y, int w, int h);
void renderICMonsSprite(Window *win, t_Poke *poke);
void destroyICMonsSprite(Window *win, t_Poke *poke);
void handleICMonsEvent(Window *win, SDL_Event *event);

#endif /* ICMONS_H */