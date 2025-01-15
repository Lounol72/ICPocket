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
    //ajouter une image avec les coodonnées du rect
} Bouton;

typedef struct {
    SDL_Rect bar;
    SDL_Rect cursor;
    int volume;
    int color[4];
    char *text;
} Slider;

/**
 * @brief Initializes a button with the given parameters.
 * 
 * @param b The button to be initialized.
 * @param x The x-coordinate of the button.
 * @param y The y-coordinate of the button.
 * @param width The width of the button.
 * @param height The height of the button.
 * @param text The text to be displayed on the button.
 * @param color The color of the button in the format {r, g, b, a}.
 * @param action The function to be called when the button is clicked.
 * @param actionParam The parameter to be passed to the action function.
 */
void InitBoutons(Bouton *b, int x, int y, int width, int height, char *text, int *color, void (*action)(void*), void* actionParam);

/**
 * @brief Initializes a slider with the given parameters.
 * 
 * @param s The slider to be initialized.
 * @param x The x-coordinate of the slider.
 * @param y The y-coordinate of the slider.
 * @param width The width of the slider.
 * @param height The height of the slider.
 * @param text The text to be displayed on the slider.
 * @param color The color of the slider in the format {r, g, b, a}.
 * @param volume The initial volume level of the slider.
 */
void InitSlider(Slider *s, int x, int y, int width, int height, char *text, int *color, int volume);

/**
 * @brief Checks if a button was clicked.
 * 
 * @param b The button to check.
 * @param x The x-coordinate of the click.
 * @param y The y-coordinate of the click.
 * @return int Returns 1 if the button was clicked, 0 otherwise.
 */
int Cliqued(Bouton *b, int x ,int y);

#endif // BOUTONS_H