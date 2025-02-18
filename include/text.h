#ifndef TEXT_H
#define TEXT_H

#include "GameEngine.h"

void initText(Window *win);
Text *newText(SDL_Renderer *renderer, const char *text, TTF_Font *font, int x, int y, int w, int h);
void updateTextPosition(Text *text, float scaleX, float scaleY);
void destroyText(Text *text);

#endif /* TEXT_H */