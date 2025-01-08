#ifndef BOUTONS_H
#define BOUTONS_H

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_mixer.h>

typedef struct {
    SDL_Rect rect; // Rectangle representing the button
    char *text; // Text to be displayed on the button
    int color[4]; // r ,g ,b ,a
    void (*action)(void*); // Function to be called when the button is clicked
    void* actionParam; // Parameter to be passed to the action function
} Bouton;

typedef struct {
    SDL_Rect bar;
    SDL_Rect cursor;
    int volume;
    int color[4];
    char *text;
} Slider;

void InitBoutons(Bouton *b, int x, int y, int width, int height, char *text, int *color, void (*action)(void*), void* actionParam);
void InitSlider(Slider *s, int x, int y, int width, int height, char *text, int *color, int volume);
int Cliqued(Bouton *b, int x ,int y);

#endif // BOUTONS_H