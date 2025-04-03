#ifndef RENDERICMONS_H
#define RENDERICMONS_H


#include "GameEngine.h"


/**
 * @file RenderICMons.h
 * @brief Définition et gestion du rendu des ICMons.
 * 
 * Ce fichier définit la structure IMG_ICMons et les fonctions associées pour le rendu des sprites ICMons dans une application SDL.
 * 
 * @author Alban Louis
 * @date 19/02/2025
 */

/**
 * @struct IMG_ICMons
 * @brief Représente un sprite ICMons dans l'application.
 * 
 * Cette structure contient des informations sur un sprite ICMons, y compris ses textures, sa position et les éléments de texte associés.
 */
typedef struct IMG_ICMons {
    SDL_Renderer *renderer;          /**< Pointeur vers le renderer SDL. */
    SDL_Texture *ICMonTexture;       /**< Texture pour le sprite ICMons. */
    Mix_Chunk *ICMonSound[4];        /**< Son pour le sprite ICMons. */
    SDL_Rect rect;                   /**< Rectangle définissant la position et la taille du sprite. */
    SDL_Rect initialRect;            /**< Rectangle initial pour les besoins de mise à l'échelle. */
    Text *PVText;                    /**< Objet Text pour afficher les PV (points de vie) de l'ICMons. */
    Text *LvlText;                   /**< Objet Text pour afficher le lvl de l'ICMons. */
    SDL_Texture *PVbarTexture;       /**< Texture pour la barre de PV. */
    SDL_Texture *PVbarTextureBack;   /**< Texture pour l'arrière-plan de la barre de PV. */
    float currentHPWidth;            /**< Pourcentage de vie de l'ICMons. */
    SDL_Rect PVRect;                 /**< Rectangle pour la barre de PV. */
    SDL_Rect PVInitialRect;          /**< Rectangle initial pour la barre de PV pour les besoins de mise à l'échelle. */
    SDL_Texture *nameTexture;        /**< Texture pour le nom de l'ICMons. */
    SDL_Rect nameRect;               /**< Rectangle pour l'affichage du nom. */
    SDL_Rect nameInitialRect;        /**< Rectangle initial pour l'affichage du nom pour les besoins de mise à l'échelle. */
    // New fields for entrance animation
    float entranceProgress;  // 0.0 to 1.0
    int isEntranceAnimating;
    int isFromRight;  // 1 if entering from right, 0 if from left
} IMG_ICMons;

/**
 * @fn IMG_ICMons *initICMonSprite(SDL_Renderer *renderer, SDL_Rect spriteRect, SDL_Rect nameRect, SDL_Rect pvRect, t_Poke *poke, TTF_Font *font, int team)
 * @brief Initialise le sprite d'un ICMon.
 *
 * Cette fonction crée et initialise le sprite associé à un poke, incluant le chargement
 * de l'image, le retournement horizontal si nécessaire, la création des textures, ainsi que
 * la configuration des textes et barres de points de vie.
 *
 * @param renderer Le renderer SDL.
 * @param spriteRect Le rectangle du sprite.
 * @param nameRect Le rectangle pour le nom.
 * @param pvRect Le rectangle pour la barre de points de vie.
 * @param poke Le pointeur vers la structure t_Poke contenant les informations du poke.
 * @param font La police TTF utilisée pour le texte.
 * @param team L'équipe (si 1, l'image est retournée horizontalement).
 * @return IMG_ICMons* Le pointeur vers le sprite initialisé, ou NULL en cas d'erreur.
 */
IMG_ICMons *initICMonSprite(SDL_Renderer *renderer, SDL_Rect spriteRect, SDL_Rect nameRect, SDL_Rect pvRect, t_Poke *poke, TTF_Font *font, int team);

/**
 * @fn void updateICMonsSprite(IMG_ICMons *icmons, float scaleX, float scaleY)
 * @brief Met à jour la position et la taille d'un sprite ICMons.
 * 
 * Cette fonction met à jour la position et la taille d'un sprite ICMons en fonction des facteurs de mise à l'échelle donnés.
 * 
 * @param poke Un pointeur vers la structure t_Poke représentant l'ICMons.
 * @param scaleX Le facteur de mise à l'échelle horizontal.
 * @param scaleY Le facteur de mise à l'échelle vertical.
 */
void updateICMonsSprite(t_Poke *poke, float scaleX, float scaleY);

/**
 * @fn void renderICMonsSprite(Window *win, t_Poke *poke)
 * @brief Rend un sprite ICMons.
 * 
 * Cette fonction rend le sprite ICMons spécifié à l'écran.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param poke Un pointeur vers la structure t_Poke représentant l'ICMons.
 */
void renderICMonsSprite(Window *win, t_Poke *poke);

/**
 * @fn void updateICMonText(t_Poke *poke)
 * @brief Met à jour le texte des ICMons.
 * 
 * Cette fonction met à jour le texte des ICMons en fonction des paramètres donnés.
 * 
 * @param poke Un pointeur vers la structure t_Poke représentant l'ICMons.
 */
void updateICMonText(t_Poke *poke);

/**
 * @fn void destroyICMonsSprite(t_Poke *poke)
 * @brief Détruit un sprite ICMons.
 * 
 * Cette fonction libère les ressources associées au sprite ICMons spécifié.
 * 
 * @param poke Un pointeur vers la structure t_Poke représentant l'ICMons.
 */
void destroyICMonsSprite(t_Poke *poke);

/**
 * @fn void startICMonEntranceAnimation(t_Poke *poke)
 * @brief Démarre l'animation d'entrée pour un ICMon.
 *
 * Cette fonction initialise les paramètres de l'animation d'entrée pour un ICMon.
 * L'animation fera glisser l'ICMon depuis le côté gauche ou droit.
 *
 * @param poke L'ICMon à animer.
 */
void startICMonEntranceAnimation(t_Poke *poke);

/**
 * @fn void updateICMonEntranceAnimation(t_Poke *poke)
 * @brief Met à jour l'animation d'entrée pour un ICMon.
 *
 * Cette fonction met à jour la position de l'ICMon pendant son animation d'entrée.
 * L'ICMon glissera depuis le côté gauche ou droit.
 *
 * @param poke L'ICMon en cours d'animation.
 */
void updateICMonEntranceAnimation(t_Poke *poke);

#endif