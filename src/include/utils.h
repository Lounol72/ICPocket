#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

/**
 * @brief Structure représentant une fenêtre et ses ressources associées.
 */ 
typedef struct {
    SDL_Window* window;      /**< Fenêtre SDL */
    int w;                 /**< Largeur de la fenêtre */
    int h;                 /**< Hauteur de la fenêtre */
    int initialW;
    int initialH;
    SDL_Surface* image;      /**< Surface de l'image affichée */
    SDL_Surface* icon;       /**< Icône de la fenêtre */
    Mix_Music* music;        /**< Musique de fond */
    int musicVolume;         /**< Volume de la musique */
    int musicPlaying;        /**< Indicateur si la musique est en cours de lecture */
} Window;

/**
 * @brief Initialise les ressources SDL et crée une fenêtre.
 * 
 * @param win Pointeur vers la structure Window à initialiser.
 * @param width Largeur de la fenêtre.
 * @param height Hauteur de la fenêtre.
 * @return int 0 en cas de succès, -1 en cas d'erreur.
 */
int initialize(Window* win, int w, int h);

/**
 * @brief Libère les ressources SDL et quitte le programme.
 * 
 * @param win Pointeur vers la structure Window à libérer.
 * @param codeError Code d'erreur à retourner lors de la sortie.
 */
void quitSDL(Window* win, int codeError);

/**
 * @brief Initialise une police de caractères avec le chemin et la taille donnés.
 * 
 * @param fontPath Le chemin du fichier de police de caractères.
 * @param fontSize La taille de la police de caractères.
 * @return TTF_Font* La police de caractères initialisée.
 */
TTF_Font* initializeFont(const char* fontPath, int fontSize);

#endif // UTILS_H