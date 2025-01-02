#ifndef BOUTONS_H
#define BOUTONS_H

typedef struct {
    int topleft;
    int topright;
    int bottomleft;
    int bottomright;
} Bouton;


int Cliqued(Bouton *b, int x ,int y);

#endif