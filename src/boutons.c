#include "boutons.h"

// Modify the function to take an array of buttons

void InitBoutons(Bouton *b, int x, int y, int width, int height, char *text, int *color, void (*action)(void)) {
    /**
     * Initialize the button with specified parameters.
     * 
     * @param b The button to be initialized.
     * @param x The x coordinate of the top-left corner of the button.
     * @param y The y coordinate of the top-left corner of the button.
     * @param width The width of the button.
     * @param height The height of the button.
     * @param text The text to be displayed on the button.
     * @param color The color of the button.
     * @param action The function to be called when the button is clicked.
     */
    b -> rect.x = x;
    b -> rect.y = y;
    b -> rect.w = width;
    b -> rect.h = height;
    for(int i = 0; i <= 4; i++) {
        b -> color[i] = color[i];
    }
    b -> action = action;
    b -> text = text;
}

void InitSlider(Slider *s, int xStart, int xEnd, int yStart, int yEnd, char *text) {
    /**
     * Initialize the slider with specified parameters.
     * 
     * @param s The slider to be initialized.
     * @param xStart The x coordinate of the start of the slider.
     * @param xEnd The x coordinate of the end of the slider.
     * @param yStart The y coordinate of the start of the slider.
     * @param yEnd The y coordinate of the end of the slider.
     */
    s->xStart = xStart;
    s->xEnd = xEnd;
    s->yStart = yStart;
    s->yEnd = yEnd;
    s->text = text;
}

//Modify the function to take an array of buttons
//Instead of returning a boolean, return the button clicked
int Cliqued(Bouton *b, int x, int y) {
    /**
     *Verify if the mouse is in the button
     *
     * @param b The button to be checked.
     * @param x The x coordinate of the mouse.
     * @param y The y coordinate of the mouse.
     * @return 1 if the mouse is in the button, 0 otherwise.
     */
    if(x >= b->rect.x && x <= b->rect.x + b->rect.w && y >= b->rect.y && y <= b->rect.y + b->rect.h) {
        return 1;
    }
    return 0;
}