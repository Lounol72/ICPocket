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
    Mix_Chunk *ICMonSound;           /**< Son pour le sprite ICMons. */
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
} IMG_ICMons;

/**
 * @brief Initialise un sprite ICMons.
 * 
 * Cette fonction initialise un sprite ICMons avec les paramètres spécifiés.
 * 
 * @param renderer Le renderer SDL.
 * @param spriteRect Le rectangle définissant la position et la taille du sprite.
 * @param nameRect Le rectangle pour l'affichage du nom.
 * @param pvRect Le rectangle pour la barre de PV.
 * @param poke Un pointeur vers la structure t_Poke représentant l'ICMons.
 * @param font La police utilisée pour le rendu du texte.
 * @param team L'identifiant de l'équipe pour l'ICMons.
 * @return Un pointeur vers la structure IMG_ICMons initialisée.
 */
IMG_ICMons *initICMonSprite(SDL_Renderer *renderer, SDL_Rect spriteRect, SDL_Rect nameRect, SDL_Rect pvRect, t_Poke *poke, TTF_Font *font, int team);

/**
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
 * @brief Rend un sprite ICMons.
 * 
 * Cette fonction rend le sprite ICMons spécifié à l'écran.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param poke Un pointeur vers la structure t_Poke représentant l'ICMons.
 */
void renderICMonsSprite(Window *win, t_Poke *poke);

/**
 * @brief Détruit un sprite ICMons.
 * 
 * Cette fonction libère les ressources associées au sprite ICMons spécifié.
 * 
 * @param poke Un pointeur vers la structure t_Poke représentant l'ICMons.
 */
void destroyICMonsSprite(t_Poke *poke);

#endif