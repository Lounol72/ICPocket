#include "../include/Images.h"
#include "../include/Window.h"

/**
 * @fn Image *initImage(struct Window *win, const char *imagePath, SDL_Rect rect, const char *text, SDL_Rect textRect, TTF_Font *font)
 * @brief Initialise une image avec un texte associé.
 * 
 * Cette fonction charge une image à partir d'un fichier et crée un objet Image avec le texte associé.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param imagePath Chemin du fichier image à charger.
 * @param rect Rectangle définissant la position et la taille de l'image.
 * @param text Le texte à afficher sur l'image.
 * @param textRect Rectangle définissant la position et la taille du texte.
 * @param font La police utilisée pour dessiner le texte.
 * @return Un pointeur vers l'objet Image créé.
 */
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

/**
 * @fn void destroyImage(Image *image)
 * @brief Détruit une image.
 * 
 * Cette fonction libère la mémoire associée à une image et à son texte.
 * 
 * @param image Pointeur vers l'objet Image à détruire.
 */
void destroyImage(Image *image) {
    SDL_DestroyTexture(image->texture);
    destroyText(image->text);
    free(image);
}

/**
 * @fn void renderImage(struct Window *win, Image *image)
 * @brief Rend une image avec son texte associé.
 * 
 * Cette fonction dessine l'image et le texte sur la fenêtre spécifiée.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param image Pointeur vers l'objet Image à rendre.
 */
void renderImage(struct Window *win, Image *image) {
    SDL_RenderCopy(win->renderer, image->texture, NULL, &image->rect);
    renderText(win, image->text);
}

/**
 * @fn void updateImageSize(Image *image, float scaleX, float scaleY)
 * @brief met à jour la taille d'une image.
 * 
 * @param image Pointeur vers l'objet Image à mettre à jour.
 * @param scaleX dimension de l'image sur l'axe X
 * @param scaleY dimension de l'image sur l'axe Y
 */
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



