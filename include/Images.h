#include "Text.h"

typedef struct {
    SDL_Texture *texture;
    SDL_Rect rect;
    SDL_Rect initialRect;
    SDL_Rect textRect;
    SDL_Rect initialTextRect;
    Text *text;
} Image;

Image *initImage(struct Window *win, const char *imagePath, SDL_Rect rect, const char *text, SDL_Rect textRect, TTF_Font *font);
void destroyImage(Image *image);
void renderImage(struct Window *win, Image *image);
void updateImageSize(Image *image, int scaleX, int scaleY);