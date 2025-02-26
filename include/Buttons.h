#ifndef BUTTONS_H
#define BUTTONS_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>


typedef struct Window Window;
typedef struct Button Button;
typedef struct ButtonList ButtonList;
typedef struct Slider Slider;
typedef struct SliderList SliderList;

#include "GameEngine.h" 

/**
 * @file Buttons.h
 * @author Alban Louis
 * @date 27/12/2024
 */

/**
 * @brief The FontSize enum represents the different font sizes that can be used for the button text.
 * 
 * The FontSize enum represents the different font sizes that can be used for the button text.
 * The font sizes are defined as LARGE, MEDIUM, and SMALL.
 * 
 * The CUSTOM value can be used to specify a custom font size.
 * 
 */

typedef enum FontSize {
    LARGE = 56,
    MEDIUM = 36,
    SMALL = 24,
    CUSTOM
} FontSize;


typedef struct Button {
    char nom[50];
    SDL_Rect rect;
    SDL_Rect initialRect;
    SDL_Color color;
    SDL_Texture *texture;
    SDL_Texture *initialTexture;
    SDL_Texture *selectedTexture;
    SDL_Renderer *renderer;
    char* text;
    SDL_Texture *textTexture;
    SDL_Color textcolor;
    TTF_Font *font; 
    void (*onClick)(Window *win, void *data);
    void *data;                             
} Button;

typedef struct ButtonList
{
    Button **buttons;
    int size;
} ButtonList;

typedef struct Slider{
    SDL_Rect rect;
    SDL_Rect initialBar;
    SDL_Rect cursor;
    SDL_Rect initialCursor;
    SDL_Color color;
    SDL_Color cursorColor;
    SDL_Renderer *renderer;
    float value;
}Slider;

typedef struct SliderList
{
    Slider **sliders;
    int size;
}SliderList;
/**
 * 
 * @fn Button *createButton(char *text, Window *win, int x, int y, int w, int h, SDL_Color color, SDL_Color textcolor, void (*onClick)(Window *, void *), void *data, TTF_Font *font)
 * 
 * @brief Creates and returns a new Button object.
 *
 *
 * @param text        The text displayed on the button
 * @param win         The window where the button will appear
 * @param x           The x-coordinate (top-left corner) of the button
 * @param y           The y-coordinate (top-left corner) of the button
 * @param w           The width of the button
 * @param h           The height of the button
 * @param color       The background color of the button
 * @param textcolor   The color to use for the button text
 * @param onClick     The callback function called when the button is clicked
 * @param data        The user-defined data passed to the callback
 * @param font        The font used to draw the button text
 *
 * @return A pointer to the newly created Button object
 */
//create all the buttons

Button *createButton(char *text, Window *win, int x, int y, int w, int h, SDL_Color color, SDL_Color textcolor, void (*onClick)(Window *, void *), void *data, TTF_Font *font);

/**
 * @brief Initializes a button with the specified image and associates it with the given renderer.
 *
 * @param button Pointer to the button structure to be initialized.
 * @param renderer Pointer to the renderer used for drawing the button.
 * @param imagePath File path to the image resource for the button.
 */
//init the texture of the button
void InitTextureButton(Button *button, SDL_Renderer *renderer, const char *imagePath);

/**
 * @brief Processes the specified button action.
 *
 * @param button The identifier of the button to process.
 */

//render the button
void renderButton(Button *button);

/**
 * @brief Processes the provided list of button structures.
 *
 * @param list    Pointer to the data structure that accumulates button states.
 * @param buttons Pointer to the array of button information to be processed.
 * @param count   Number of buttons within the provided array.
 */
//add a list of buttons to the button list
void addListButton(ButtonList *list, Button **buttons, int count);

/**
 * @brief Processes the specified list of items.
 *
 * @param list A pointer or reference to the list of items to be processed.
 */
//destroy the button list
void destroyButtonList(ButtonList *list);

/**
 * @brief Brief description of the function.
 *
 * @param B Description of the input parameter.
 */
//render the button list
void renderButtonList(ButtonList *B);

/**
 * @brief Performs the specified action for the given button.
 *
 * @param button Identifier of the button to act upon.
 */
//destroy the button
void destroyButton(Button *button);

/**
 * @brief Processes button interaction given the current mouse position and window context.
 *
 * @param button The identifier or reference to the button being interacted with.
 * @param mouseX The x-coordinate of the mouse pointer.
 * @param mouseY The y-coordinate of the mouse pointer.
 * @param win    The window or graphical context in which the interaction occurs.
 * @return A status or boolean indicating whether the interaction was handled successfully.
 */
//handle the button click
void ButtonClicked(Button *button, int mouseX, int mouseY, Window *win);

/**
 * @brief Updates button states and applies scaling factors to the user interface elements.
 *
 * @param buttons A structure or collection representing the buttons to be managed or updated.
 * @param Scalex The horizontal scaling factor to apply.
 * @param Scaley The vertical scaling factor to apply.
 */
//update the button position
void updateButtonPosition(ButtonList *buttons, float Scalex, float Scaley);


/**
 * @brief Renders a button with the specified text.
 * 
 * @param button The button to render.
 * @param text The text to display on the button.
 * @param renderer The renderer to use for drawing the button.
 */
//set the text of the button
void setButtonText(Button *button, const char *text, SDL_Renderer *renderer);

/**
 * @brief Create a Slider object
 * 
 * This function initializes a new Slider object with the specified parameters.
 * 
 * @param renderer The SDL_Renderer to use for rendering the slider.
 * @param x The x-coordinate of the slider's position.
 * @param y The y-coordinate of the slider's position.
 * @param w The width of the slider.
 * @param h The height of the slider.
 * @param color The color of the slider.
 * @param cursorColor The color of the slider's cursor.
 * @return Slider* A pointer to the newly created Slider object.
 */
//create a slider
Slider *createSlider(SDL_Renderer *renderer, int x, int y, int w, int h, SDL_Color color, SDL_Color cursorColor);

/**
 * @brief Updates the position of the slider based on the mouse's X coordinate.
 * 
 * @param slider A pointer to the slider object to be updated.
 * @param mouseX The current X coordinate of the mouse.
 */
//drag the cursor of the slider
void DragCursor(Slider *slider, int mouseX);

/**
 * @brief Handles the button press events.
 * 
 * @param slider A pointer to the slider object that the button interacts with.
 */
//render the slider
void renderSlider(Slider *slider);

/**
 * @brief 
 * 
 * @param slider 
 */
//destroy the slider
void destroySlider(Slider *slider);

/**
 * @brief Processes a list of button events.
 * 
 * @param list A pointer to the list of button events to be processed.
 */
//destroy the slider list
void destroySliderList(SliderList *list);

/**
 * @brief Processes the button inputs and updates the sliders accordingly.
 * 
 * @param S A pointer to the structure containing the button states.
 * @param sliders An array of slider values to be updated based on button inputs.
 * @param size The number of sliders in the array.
 */
//add a list of sliders to the slider list
void addListSlider(SliderList *S, Slider *sliders[], int size);

/**
 * @brief Handles button press events.
 * @param S The state of the buttons.
 */
//render the slider list
void renderSliderList(SliderList *S);

/**
 * @brief Handles the button press event.
 * 
 * This function processes the button press event based on the given slider and coordinates.
 * 
 * @param slider The slider value associated with the button press.
 * @param x The x-coordinate of the button press.
 * @param y The y-coordinate of the button press.
 * @return int Returns 0 on success, or an error code on failure.
 */
//handle the slider event
int handleSliderEvent(Slider *slider, int x, int y);
/**
 * @brief Update the position of the sliders based on the mouse's X and Y coordinates.
 * 
 * @param sliders A pointer to the structure containing the sliders to be updated. 
 * @param Scalex  Scale factor for the horizontal axis.
 * @param Scaley  Scale factor for the vertical axis.
 */
void updateSliderPosition(SliderList *sliders, float Scalex, float Scaley);


#endif