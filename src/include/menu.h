#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "utils.h"
#include "state.h"
#include "boutons.h"

// Forward declaration of struct Fenetre
struct Fenetre;

/**
 * @brief Structure representing an image.
 * 
 */
typedef struct{
    SDL_Surface * image;
    SDL_Rect position;
    SDL_Rect initialPosition;

}Image;

/**
 * @brief Structure representing a window.
 * 
 */
typedef struct Fenetre {
    Image * images[20];
    Bouton boutons[20];
    int nbBoutons;
    int nbImages;
    State *state;
    TTF_Font * font; 
    void (*render)(struct Fenetre*);
    void (*handleInputs)(struct Fenetre*, SDL_Event);
    void (*update)(struct Fenetre*);
    void (*destroy)(struct Fenetre*);
} Fenetre;

/**
 * @brief Initializes a window with the given parameters.
 * 
 * @param f  Initialised window
 * @param images Images to be displayed on the window
 * @param boutons Buttons that are displayed on the window
 * @param nbBoutons Nb of buttons
 * @param nbImages Nb of Images
 * @param state State of the window
 * @param font Font to be used
 */
void initFenetre(Fenetre *f, Image *images, Bouton *boutons, int nbBoutons, int nbImages, State *state, TTF_Font* font, void (*renderFenetre)(Fenetre*), void (*handleInputsFenetre)(Fenetre*, SDL_Event), void(*destroyFenetre)(Fenetre*));

/**
 * @brief Renders the window.
 * 
 * @param f The window to be rendered.
 */
void renderFenetre(Fenetre *f);
/**
 * @brief Handles the inputs of the window.
 * 
 * @param f The window to handle the inputs.
 * @param event The event to be handled.
 */
void handleInputsFenetre(Fenetre *f, SDL_Event event);
/**
 * @brief Destroys the window.
 * 
 * @param f The window to be destroyed.
 */
void destroyFenetre(Fenetre *f);

/**
 * @brief Draws the menu.
 * 
 * @param surface The surface to draw the menu on.
 * @param image The image to be displayed on the menu.
 */
void drawMenu(SDL_Surface* surface, SDL_Surface* image);
/**
 * @brief Draws the highlight on the button.
 * 
 * @param surface The surface to draw the highlight on.
 * @param b The button to draw the highlight on.
 */
void drawHighlight(SDL_Surface * surface, Bouton * b);
/**
 * @brief Initializes the buttons.
 * 
 * @param menu The menu to initialize the buttons on.
 */
void changeState(void* targetState);
/**
 * @brief Initializes the buttons.
 * 
 * @param menu The menu to initialize the buttons on.
 */
void drawGame(SDL_Surface* surface);


#endif