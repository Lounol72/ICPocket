#include "../include/Images.h"
#include "../include/Window.h"

Image *initImage(struct Window *win, const char *imagePath, SDL_Rect rect, const char *text, SDL_Rect textRect, TTF_Font *font) {
    Image *image = malloc(sizeof(Image));
    SDL_Surface *surface = IMG_Load(imagePath);
    image->texture = SDL_CreateTextureFromSurface(win->renderer, surface);
    SDL_FreeSurface(surface);
    image->rect = rect;
    image->initialRect = rect;
    SDL_Color color = {255, 255, 255, 255};
    image->textRect = textRect;
    image->initialTextRect = textRect;
    image->text = createText(text, win->renderer, textRect, color, font);
    return image;
}

void destroyImage(Image *image) {
    SDL_DestroyTexture(image->texture);
    destroyText(image->text);
    free(image);
}

void renderImage(struct Window *win, Image *image) {
    SDL_RenderCopy(win->renderer, image->texture, NULL, &image->rect);
    renderText(win, image->text);
}

void updateImageSize(Image *image, int scaleX, int scaleY) {
    image->rect.x = image->initialRect.x * scaleX;
    image->rect.y = image->initialRect.y * scaleY;
    image->rect.w = image->initialRect.w * scaleX;
    image->rect.h = image->initialRect.h * scaleY;

    image->text->rect.x = image->text->initialRect.x * scaleX;
    image->text->rect.y = image->text->initialRect.y * scaleY;
    image->text->rect.w = image->text->initialRect.w * scaleX;
    image->text->rect.h = image->text->initialRect.h * scaleY;
}



