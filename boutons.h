#ifndef BOUTONS_H
#define BOUTONS_H

typedef struct {
    int x; // Top-left x coordinate
    int y; // Top-left y coordinate
    int width;
    int height;
} Bouton;

int Cliqued(Bouton *b, int x ,int y);

#endif