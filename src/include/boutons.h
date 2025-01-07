#ifndef BOUTONS_H
#define BOUTONS_H

#include <SDL2/SDL_rect.h>

typedef struct {
    SDL_Rect rect; // Rectangle representing the button
    char *text; // Text to be displayed on the button
    int color[4]; // r ,g ,b ,a
    void (*action)(void); // Function to be called when the button is clicked
} Bouton;

typedef struct {
    int xStart;
    int xEnd;
    int yStart;
    int yEnd;
    char *text;
} Slider;

void InitBoutons(Bouton *b, int x, int y, int width, int height, char *text, int *color, void (*action)(void));
void InitSlider(Slider *s, int xStart, int xEnd, int yStart, int yEnd, char *text);
int Cliqued(Bouton *b, int x ,int y);

#endif