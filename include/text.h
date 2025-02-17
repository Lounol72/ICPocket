#ifndef TEXT_H
#define TEXT_H

#include "GameEngine.h"

void initText(Window *win);
void updateTextPosition(Text *text, float scaleX, float scaleY);
void destroyText(Text *text);

#endif /* TEXT_H */