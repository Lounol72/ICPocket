#include "boutons.h"

int Cliqued(Bouton *b, int x ,int y){
    if(x >= b->topleft && x <= b->topright && y >= b->bottomleft && y <= b->bottomright){
        return 1;
    }
}