
#include "Text.h"
/**
 * @file Images.h
 * @author Louis Alban
 * @brief Gestion des images et du texte associé.
 * @date 17/02/2025
 */

/**
 * @struct Image
 * @brief Représente une image avec un texte associé.
 * 
 * Cete structure contient des informations sur une image, y compris sa texture, sa position, et le texte associé.
 */
typedef struct {
    SDL_Texture *texture; /**< Texture de l'image. */
    SDL_Rect rect; /**< Rectangle définissant la position et la taille de l'image. */
    SDL_Rect initialRect; /**< Rectangle initial pour les besoins de mise à l'échelle. */
    Text *text; /**< Pointeur vers l'objet Text associé à l'image. */
} Image;

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
Image *initImage(struct Window *win, const char *imagePath, SDL_Rect rect, const char *text, SDL_Rect textRect, TTF_Font *font);

/**
 * @fn void destroyImage(Image *image)
 * @brief Détruit une image.
 * 
 * Cette fonction libère la mémoire associée à une image et à son texte.
 * 
 * @param image Pointeur vers l'objet Image à détruire.
 */
void destroyImage(Image *image);

/**
 * @fn void renderImage(struct Window *win, Image *image)
 * @brief Rend une image avec son texte associé.
 * 
 * Cette fonction dessine l'image et le texte sur la fenêtre spécifiée.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param image Pointeur vers l'objet Image à rendre.
 */
void renderImage(struct Window *win, Image *image);

/**
 * @fn void updateImageSize(Image *image, float scaleX, float scaleY)
 * @brief met à jour la taille d'une image.
 * 
 * @param image Pointeur vers l'objet Image à mettre à jour.
 * @param scaleX dimension de l'image sur l'axe X
 * @param scaleY dimension de l'image sur l'axe Y
 */
void updateImageSize(Image *image, float scaleX, float scaleY);