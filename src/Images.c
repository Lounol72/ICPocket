#include "../include/Images.h"
#include "../include/Window.h"

Image *initImage(struct Window *win, const char *imagePath, SDL_Rect rect, const char *text, SDL_Rect textRect, TTF_Font *font) {
    Image *image = malloc(sizeof(Image));
    SDL_Surface *surface = IMG_Load(imagePath);
    image->texture = SDL_CreateTextureFromSurface(win->renderer, surface);
    SDL_FreeSurface(surface);
    image->initialRect = rect;
    image->rect = rect;
    SDL_Color color = {255, 255, 255, 255};
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

void updateImageSize(Image *image, float scaleX, float scaleY) {
    if (!image) return;
    
    // Sauvegarder les valeurs initiales si ce n'est pas déjà fait
    if (image->initialRect.w == 0) {
        image->initialRect = image->rect;
    }
    // Mettre à jour en fonction des dimensions initiales
    image->rect.x = (int)(image->initialRect.x * scaleX);
    image->rect.y = (int)(image->initialRect.y * scaleY);
    image->rect.w = (int)(image->initialRect.w * scaleX);
    image->rect.h = (int)(image->initialRect.h * scaleY);
    
    // Mettre à jour le rectangle du texte s'il existe
    updateTextPosition(image->text, scaleX, scaleY);
}



