#ifndef BOUTONS_H
#define BOUTONS_H

typedef struct {
    int x; // Top-left x coordinate
    int y; // Top-left y coordinate
    int width;
    int height;
    char *text;
} Bouton;

typedef struct {
    int xStart;
    int xEnd;
    int yStart;
    int yEnd;
    char *text;
} Slider;

void InitBoutons(Bouton *b, int x, int y, int width, int height, char *text);
void InitSlider(Slider *s, int xStart, int xEnd, int yStart, int yEnd, char *text);
int Cliqued(Bouton *b, int x ,int y);

#endif