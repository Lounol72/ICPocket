#include "include/boutons.h"
#include "include/state.h"

// Function to initialize a button
void InitBoutons(Bouton *b, int x, int y, int width, int height, char *text,const int *color, void (*action)(void*), void* actionParam, char *image , SDL_Surface *surface) {
    b->rect.x = x;
    b->rect.y = y;
    b->rect.w = width;
    b->rect.h = height;
    b->color.r = color[0];
    b->color.g = color[1];
    b->color.b = color[2];
    b->color.a = color[3];
    b->action = action;
    b->actionParam = actionParam;
    b->text = text;
    b->image = IMG_Load(image);
    b->surface = surface;
}

// Function to initialize a slider
void InitSlider(Slider *s, int x, int y, int width, int height, char *text,const  int *color, int volume) {
    s->bar.x = x;
    s->bar.y = y;
    s->bar.w = width;
    s->bar.h = height;
    s->text = text;
    s->cursor.x = x + (volume * width / MIX_MAX_VOLUME) - (height / 4); // Adjust cursor position based on bar width
    s->cursor.y = y - (height / 4); // Adjust cursor y position to be centered on the bar
    s->cursor.w = height / 2; // Adjust cursor width
    s->cursor.h = height + 10; // Adjust cursor height
    s->volume = volume;
    for(int i = 0; i < 4; i++) {
        s->color[i] = color[i];
    }
}

int Cliqued(Bouton *b, int x, int y) {
    return (x >= b->rect.x && x <= b->rect.x + b->rect.w &&
        y >= b->rect.y && y <= b->rect.y + b->rect.h);
}
