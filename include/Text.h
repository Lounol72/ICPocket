#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Window.h"


typedef struct Text {
    const char *text;
    SDL_Rect rect;
    SDL_Rect initialRect;
    SDL_Color color;
    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;
} Text;

extern Text title;
extern Text NewGameText;

void initText(Window *win); // Initialize the text objects TEMPORARY
Text *createText(const char *text,SDL_Renderer *renderer,SDL_Rect rect, SDL_Color color, TTF_Font *font);
void renderText(Window *win, Text *text);
void updateTextPosition(Text *text, float scaleX, float scaleY);
void destroyText(Text *text);

void changeTextSpeed(Window *win, void *data);
#endif /* TEXT_H */