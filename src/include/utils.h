#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

/**
 * @brief Structure représentant une fenêtre et ses ressources associées. 
 */ 

typedef struct {
    SDL_Window* window;      /**< Fenêtre SDL */
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
 * @return int 0 en cas de succès, -1 en cas d'erreur.
 */
int initialize(Window* win);

/**
 * @brief Libère les ressources SDL et quitte le programme.
 * 
 * @param win Pointeur vers la structure Window à libérer.
 * @param codeError Code d'erreur à retourner lors de la sortie.
 */
void quitSDL(Window* win, int codeError);

#endif // UTILS_H