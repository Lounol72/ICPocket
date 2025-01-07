#include "include/boutons.h"
#include "include/state.h"

void InitBoutons(Bouton *b, int x, int y, int width, int height, char *text, int *color, void (*action)(void*), void* actionParam) {
    b->rect.x = x;
    b->rect.y = y;
    b->rect.w = width;
    b->rect.h = height;
    for(int i = 0; i < 4; i++) {
        b->color[i] = color[i];
    }
    b->action = action;
    b->actionParam = actionParam;
    b->text = text;
}

void InitSlider(Slider *s, int xStart, int xEnd, int yStart, int yEnd, char *text) {
    s->xStart = xStart;
    s->xEnd = xEnd;
    s->yStart = yStart;
    s->yEnd = yEnd;
    s->text = text;
}

int Cliqued(Bouton *b, int x, int y) {
    if (x >= b->rect.x && x <= b->rect.x + b->rect.w && y >= b->rect.y && y <= b->rect.y + b->rect.h) {
        return 1;
    }
    return 0;
}

