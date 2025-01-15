#include "include/boutons.h"
#include "include/state.h"

// Function to initialize a button
void InitBoutons(Bouton *b, int x, int y, int width, int height, char *text, int *color, void (*action)(void*), void* actionParam, char *image) {
    b->rect.x = x;
    b->rect.y = y;
    b->rect.w = width;
    b->rect.h = height;
    for (int i = 0; i < 4; i++) {
        b->color[i] = color[i];
    }
    b->action = action;
    b->actionParam = actionParam;
    b->text = text;
    b->image = IMG_Load(image);
}

// Function to initialize a slider
void InitSlider(Slider *s, int x, int y, int width, int height, char *text, int *color, int volume) {
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
    if (x >= b->rect.x && x <= b->rect.x + b->rect.w && y >= b->rect.y && y <= b->rect.y + b->rect.h) {
        return 1;
    }
    return 0;
}
