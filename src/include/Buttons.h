#ifndef BUTTONS_H
#define BUTTONS_H


#include <SDL2/SDL.h>
#include "GameEngine.h" 


typedef struct Button {
    char nom[50];
    SDL_Rect rect;
    SDL_Rect initialRect;
    SDL_Color color;
    SDL_Texture *texture;
    SDL_Renderer *renderer;
    char* text;
    SDL_Texture *textTexture;
    SDL_Color textcolor;
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




Button *createButton(char * text,Window * win, int x, int y, int w, int h, SDL_Color color,SDL_Color textcolor ,void (*onClick)(Window *, void *), void *data);
void InitTextureButton(Button *button, SDL_Renderer *renderer, const char *imagePath);
void renderButton(Button *button);
void addListButton(ButtonList *B, Button *buttons[], int size);
void destroyButtonList(ButtonList *list);
void renderButtonList(ButtonList *B);
void destroyButton(Button *button);
void ButtonClicked(Button *button, int mouseX, int mouseY, Window *win);
void updateButtonPosition(ButtonList *buttons, float Scalex, float Scaley);

Slider *createSlider(SDL_Renderer *renderer, int x, int y, int w, int h, SDL_Color color, SDL_Color cursorColor);
void DragCursor(Slider *slider, int mouseX);
void renderSlider(Slider *slider);
void destroySlider(Slider *slider);
void destroySliderList(SliderList *list);
void addListSlider(SliderList *S, Slider *sliders[], int size);
void renderSliderList(SliderList *S);
int handleSliderEvent(Slider *slider, int x, int y);


#endif