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

void InitSlider(Slider *s, int x, int y, int width, int height, char *text, int *color, int volume) {
    s->bar.x = x;
    s->bar.y = y;
    s->bar.w = width;
    s->bar.h = height;
    s->text = text;
    SDL_Rect cursor = {x + (volume * (width - x) / MIX_MAX_VOLUME), height - 5, 10, 30};
    s->volume = (*volume);
    for(int i = 0; i < 4; i++) {
        s->color[i] = color[i];
    }
}

int Cliqued(Bouton *b, int x, int y) {
    if (x >= b->rect.x && x <= b->rect.x + b->rect.w && y >= b->rect.y && y <= b->rect.y + b->rect.h) {
        return 1;
    }
    return 0;
}

