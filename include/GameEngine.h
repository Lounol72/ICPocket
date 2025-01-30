#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL_ttf.h>


// Define the state of the application
typedef enum AppState {
    GAME = 0, 
    SETTINGS ,
    MENU,
    QUIT,
    NEWGAME, 
    LOADGAME,
    PAUSE
} AppState;

typedef struct Text {
    char *text;
    SDL_Rect rect;
    SDL_Rect initialRect;
    SDL_Color color;
    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;
} Text;




// Structure of the window
typedef struct Window {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int width, height;
    int InitialWidth, InitialHeight;
    int quit;
    TTF_Font * LargeFont;
    TTF_Font * MediumFont;
    TTF_Font * SmallFont;
    TTF_Font *font;
    AppState state;
    double textSpeed;
} Window;


// Include the buttons
#include "Buttons.h"
#include "structPoke.h"
#include "duel.h"
#include "trainerAI.h"

/**
 * @brief           Initialize the window
 * 
 * @param win       struct Window
 * @param width     width
 * @param height    height
 * @param FontPath  path to the font
 */
void initWindow(Window *win, int width, int height, const char *FontPath);
/**
 * @brief       Destroy the window
 * 
 * @param win   struct Window
 */
void destroyWindow(Window *win);
/**
 * @brief           Handle the event
 * 
 * @param win       struct Window
 * @param event     SDL_Event
 */
void handleEvent(Window *win, SDL_Event *event);
/**
 * @brief           Main loop
 * 
 * @param win       struct Window
 */
void mainLoop(Window *win);


/**
 * @brief           Render the menu
 * 
 * @param win       struct Window
 */
void renderMenu(Window *win);
/**
 * @brief           Handle the event of the menu
 * 
 * @param win       struct Window
 * @param event     SDL_Event
 */
void handleMenuEvent(Window *win, SDL_Event *event);
/**
 * @brief           Render the game
 * 
 * @param win       struct Window
 */
void renderGame(Window *win);
/**
 * @brief           Handle the event of the game
 * 
 * @param win       struct Window
 * @param event     SDL_Event
 */
void handleGameEvent(Window *win, SDL_Event *event);
/**
 * @brief           Render the settings
 * 
 * @param win       struct Window
 */
void renderSettings(Window *win);
/**
 * @brief           Handle the event of the settings
 * 
 * @param win       struct Window
 * @param event     SDL_Event
 */
void handleSettingsEvent(Window *win, SDL_Event *event);
/**
 * @brief           Render the quit
 * 
 * @param win       struct Window
 */
void renderQuit(Window *win);
/**
 * @brief           Handle the event of the quit
 * 
 * @param win       struct Window
 * @param event     SDL_Event
 */
void handleQuitEvent(Window *win, SDL_Event *event);

/**
 * @brief           Handle the change of the text speed
 * 
 * @param win       struct Window
 * @param data      void
 */
void changeTextSpeed(Window *win, void *data);

/**
 * @brief           Load the background
 * 
 * @param Background    SDL_Texture
 * @param renderer      SDL_Renderer
 * @param imagePath     path to the image
 */
void loadBackground(SDL_Texture **Background, SDL_Renderer *renderer, const char *imagePath);

/**
 * @brief render the text from the struct Text
 * 
 * @param win  struct Window
 * @param text  struct Text
 */
void renderText(Window * win, Text * text);

/**
 * @brief update the position of the text
 * 
 * @param text  struct Text
 * @param scaleX  float
 * @param scaleY  float
 */
void updateTextPosition(Text *text, float scaleX, float scaleY);

/**
 * @brief Destroy the structure text and free the memory
 * 
 * @param text  struct Text
 */
void destroyText(Text * text);

/**
 * @brief Initialize every buttons we need
 * 
 * @param win  struct Window
 */
void initAllButtons(Window * win);

/**
 * @brief Create a Fic for the save
 * 
 */
void createFicGame();

/**
 * @brief Render the loading for new game
 * 
 * @param win   struct Window
 */
void renderNewGame(Window * win);
/**
 * @brief Handle the event for the new game
 * 
 * @param win   struct Window
 * @param event SDL_Event
 */
void handleNewGameEvent(Window * win, SDL_Event * event);

/**
 * @brief Read the Fic for the save
 * 
 */
void readFicGame();

/**
 * @brief Render the loading to load a game
 * 
 * @param win   struct Window
 */
void renderLoadGame(Window * win);
/**
 * @brief Handle the event to load a game
 * 
 * @param win   struct Window
 * @param event SDL_Event
 */
void handleLoadGameEvent(Window * win, SDL_Event * event);

/**
 * @brief Handle the event for the attack button
 * 
 * @param button    Button
 * @param win       Window
 */
void attqButtonClicked(Window *win, void *data);

/**
 * @brief Change the state of the window
 * 
 * @param win   struct Window
 * @param data  void
 */
void changeState(Window *win, void *data);

/**
 * @brief Initialize the text
 * 
 * @param win   struct Window
 */
void initText(Window * win);

/**
 * @brief Update the text of the attack buttons
 * 
 * @param win   struct Window
 * @param team  t_Team
 */
void updateAttackButtons(Window *win, t_Team *team);
#endif // GAMEENGINE_H