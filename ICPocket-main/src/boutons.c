#include "boutons.h"

int Cliqued(Bouton *b, int x, int y) {
    if (x >= b->x && x <= (b->x + b->width) && y >= b->y && y <= (b->y + b->height)) return 1;
    return 0;
}