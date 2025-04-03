#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

/**
 * @file Text.h
 * @brief Définition et gestion des éléments de texte.
 * 
 * Ce fichier définit la structure Text et les fonctions associées pour la gestion du rendu de texte dans une application SDL.
 */

typedef struct Window Window;

/**
 * @struct Text
 * @brief Représente un élément de texte dans l'application.
 * 
 * Cette structure contient des informations sur un élément de texte, y compris son contenu, sa position, sa couleur, sa police et les détails de rendu.
 */
typedef struct Text {
    const char *text;        /**< Le contenu textuel à afficher. */
    SDL_Rect rect;           /**< Le rectangle définissant la position et la taille du texte. */
    SDL_Rect initialRect;    /**< Le rectangle initial pour les besoins de mise à l'échelle. */
    SDL_Color color;         /**< La couleur du texte. */
    TTF_Font *font;          /**< La police utilisée pour rendre le texte. */
    SDL_Surface *surface;    /**< La surface contenant le texte rendu. */
    SDL_Texture *texture;    /**< La texture utilisée pour rendre le texte à l'écran. */
    int is_dynamic;          /**< Indique si le texte est dynamique (temporaire). */
} Text;

typedef struct {
    char* fullText;           /**< Texte complet à afficher*/
    char* currentText;        /**< Texte actuellement affiché*/
    int currentLength;       /**< Longueur actuelle du texte affiché*/
    int fullLength;          /**< Longueur totale du texte complet*/
    int isComplete;          /**< Indique si le texte est complètement affiché*/
    Uint32 lastCharTime;     /**< Temps écoulé depuis le dernier caractère affiché*/
    Uint32 charDelay;        /**< Délai entre chaque caractère (en ms)*/
    SDL_Texture* texture;    /**< Texture du texte*/
    SDL_Texture* background; /**< Texture de fond*/
    SDL_Rect position;       /**< Position du texte*/
    SDL_Rect initialPosition; /**< Position initiale du texte*/
    SDL_Rect backgroundPosition; /**< Position de fond*/
    SDL_Rect initialBackgroundPosition; /**< Position initiale de fond*/
    TTF_Font* font;          /**< Police utilisée pour le texte*/
    SDL_Color color;         /**< Couleur du texte*/
    int width;               /**< Largeur du texte*/
    int isValid;       /**< Indique si le texte est valide*/
} ScrollingText;

/** Éléments de texte globaux utilisés dans l'application. */
extern Text title;           /**< L'élément de texte pour le titre. */
extern Text NewGameText;     /**< L'élément de texte pour le nouveau jeu. */

/**
 * @fn void initText(Window *win)
 * @brief Initialise les objets texte.
 * 
 * Cette fonction initialise les objets texte utilisés dans l'application. C'est une fonction temporaire.
 * 
 * @param win Un pointeur vers la structure Window.
 */
void initText( Window *win);

/**
 * @fn Text *createText(const char *text, SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color, TTF_Font *font)
 * @brief Crée un nouvel objet texte.
 * 
 * Cette fonction crée un nouvel objet texte avec les paramètres spécifiés.
 * 
 * @param text Le contenu textuel.
 * @param renderer Le renderer SDL.
 * @param rect Le rectangle définissant la position et la taille du texte.
 * @param color La couleur du texte.
 * @param font La police utilisée pour rendre le texte.
 * @return Un pointeur vers l'objet Text créé.
 */
Text *createText(const char *text, SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color, TTF_Font *font);

/**
 * @fn void renderText(Window *win, Text *text)
 * @brief Rend un objet texte.
 * 
 * Cette fonction rend l'objet texte spécifié à l'écran.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param text Un pointeur vers l'objet Text à rendre.
 */
void renderText( Window *win, Text *text);

/**
 * @fn void updateTextPosition(Text *text, float scaleX, float scaleY)
 * @brief Met à jour la position d'un objet texte.
 * 
 * Cette fonction met à jour la position de l'objet texte spécifié en fonction des facteurs de mise à l'échelle donnés.
 * 
 * @param text Un pointeur vers l'objet Text.
 * @param scaleX Le facteur de mise à l'échelle horizontal.
 * @param scaleY Le facteur de mise à l'échelle vertical.
 */
void updateTextPosition(Text *text, float scaleX, float scaleY);

/**
 * @fn void destroyText(Text *text)
 * @brief Détruit un objet texte.
 * 
 * Cette fonction libère les ressources associées à l'objet texte spécifié.
 * 
 * @param text Un pointeur vers l'objet Text à détruire.
 */
void destroyText(Text *text);

/**
 * @fn void updateText(Text *text, const char *newText, SDL_Renderer *renderer)
 * @brief Met à jour le contenu d'un objet texte.
 * 
 * Cette fonction met à jour le contenu textuel de l'objet texte spécifié et le rend à nouveau.
 * 
 * @param text Un pointeur vers l'objet Text.
 * @param newText Le nouveau contenu textuel.
 * @param renderer Le renderer SDL.
 */
void updateText(Text *text, const char *newText, SDL_Renderer *renderer);

/**
 * @fn void changeTextSpeed(Window *win, void *data)
 * @brief Change la vitesse du texte.
 * 
 * Cette fonction change la vitesse à laquelle le texte est affiché. C'est une fonction temporaire.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param data Données supplémentaires pour changer la vitesse du texte.
 */
void changeTextSpeed(Window *win, void *data);

/**
 * @fn void cleanupText()
 * @brief Nettoie les objets texte.
 * 
 * Cette fonction nettoie les objets texte utilisés dans l'application.
 */
void cleanupText();

/**
 * @fn void StayScaled(TTF_Font *font, Text *text, SDL_Rect *rect, SDL_Rect *initialRect)
 * @brief Met à jour la taille d'un objet texte en fonction de la mise à l'échelle.
 * 
 * Cette fonction met à jour la taille de l'objet texte spécifié en fonction de la mise à l'échelle.
 * 
 * @param font La police utilisée pour rendre le texte.
 * @param text Un pointeur vers l'objet Text.
 * @param rect Le rectangle définissant la position et la taille du texte.
 * @param initialRect Le rectangle initial pour les besoins de mise à l'échelle.
 */
void StayScaled(TTF_Font *font, Text *text, SDL_Rect *rect, SDL_Rect *initialRect);

/**
 * @fn ScrollingText* createScrollingText(char* text, TTF_Font* font, SDL_Color color, int charDelay, SDL_Rect backgroundPosition, const char* backgroundPath, SDL_Renderer* renderer)
 * @brief Crée un nouvel objet de texte défilant.
 * 
 * Cette fonction crée un nouvel objet de texte défilant avec les paramètres spécifiés.
 * 
 * @param text Le contenu textuel.
 * @param font La police utilisée pour rendre le texte.
 * @param color La couleur du texte.
 * @param x La position horizontale du texte.
 * @param y La position verticale du texte.
 * @param charDelay Le délai entre chaque caractère (en ms).
 * @return Un pointeur vers l'objet ScrollingText créé.
 */

ScrollingText* createScrollingText(char* text, TTF_Font* font, SDL_Color color, int charDelay, SDL_Rect backgroundPosition, const char* backgroundPath, SDL_Renderer* renderer);

/**
 * @fn void updateScrollingText(ScrollingText* text, SDL_Renderer* renderer)
 * @brief Met à jour un objet de texte défilant.
 * 
 * Cette fonction met à jour un objet de texte défilant avec les paramètres spécifiés.
 * 
 * @param text Un pointeur vers l'objet ScrollingText.
 * @param renderer Le renderer SDL.
 */
void updateScrollingText(ScrollingText* text, SDL_Renderer* renderer);

/**
 * @fn void renderScrollingText(ScrollingText* text, SDL_Renderer* renderer)
 * @brief Rend un objet de texte défilant.
 * 
 * Cette fonction rend un objet de texte défilant avec les paramètres spécifiés.
 * 
 * @param text Un pointeur vers l'objet ScrollingText.
 * @param renderer Le renderer SDL.
 */
void renderScrollingText(ScrollingText* text, SDL_Renderer* renderer);

/**
 * @fn void resetScrollingText(ScrollingText* text, const char* newText)
 * @brief Réinitialise un objet de texte défilant.
 * 
 * Cette fonction réinitialise un objet de texte défilant avec les paramètres spécifiés.
 * 
 * @param text Un pointeur vers l'objet ScrollingText.
 * @param newText Le nouveau texte à afficher.
 */
void resetScrollingText(ScrollingText* text, const char* newText);
/**
 * @fn void destroyScrollingText(ScrollingText* text)
 * @brief Détruit un objet de texte défilant.
 * 
 * Cette fonction détruit un objet de texte défilant avec les paramètres spécifiés.
 * 
 * @param text Un pointeur vers l'objet ScrollingText.
 */
void destroyScrollingText(ScrollingText* text);

/**
 * @fn void skipScrollingText(ScrollingText* text, SDL_Renderer* renderer)
 * @brief Passe à la suite un objet de texte défilant.
 * 
 * Cette fonction passe à la suite un objet de texte défilant avec les paramètres spécifiés.
 * 
 * @param text Un pointeur vers l'objet ScrollingText.
 * @param renderer Le renderer SDL.
 */
void skipScrollingText(ScrollingText* text, SDL_Renderer* renderer);

/**
 * @fn void cleanupScrollingText(ScrollingText** text)
 * @brief Nettoie un objet de texte défilant.
 * 
 * Cette fonction nettoie un objet de texte défilant avec les paramètres spécifiés.
 * 
 * @param text Un pointeur vers l'objet ScrollingText.
 */
void cleanupScrollingText(ScrollingText** text);

#endif /* TEXT_H */