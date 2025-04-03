#ifndef EVENTS_H
#define EVENTS_H

#include "GameEngine.h"

/**
 * @file Events.h
 * @author Louis Alban
 * @brief Gestion des événements SDL pour le jeu.
 * @date 17/02/2025
 */

/* Constantes pour le positionnement et la taille des sprites */
#define RED_SPRITE_X_RATIO 0.23f
#define RED_SPRITE_Y_RATIO 0.4f
#define BLUE_SPRITE_X_RATIO 0.60f 
#define BLUE_SPRITE_Y_RATIO 0.275f
#define SPRITE_WIDTH_RATIO 0.2f
#define SPRITE_HEIGHT_RATIO 0.25f
#define NAME_Y_OFFSET 25
#define PV_Y_OFFSET 25
#define NAME_HEIGHT 20
#define PV_BAR_HEIGHT 15

/** 
 * @fn int initTeamSprites(Window *win, t_Team *teamSprite, float x_ratio, float y_ratio, int teamFlag) 
 * @brief Initialise les sprites des équipes.
 * 
 * Cette fonction configure les sprites des équipes en fonction des ratios et des drapeaux spécifiés.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param teamSprite Pointeur vers la structure des sprites de l'équipe.
 * @param x_ratio Ratio X pour le positionnement du sprite.
 * @param y_ratio Ratio Y pour le positionnement du sprite.
 * @param teamFlag Drapeau de l'équipe : 0 = rouge, 1 = bleu.
 * @return Retourne 0 en cas de succès, une valeur différente en cas d'erreur.
 */
int initTeamSprites(Window *win, t_Team *teamSprite, float x_ratio, float y_ratio, int teamFlag);

/**
 * @fn void handleEvent(Window *win, SDL_Event *event) 
 * @brief Gère les événements SDL principaux.
 * 
 * Cette fonction traite tous les événements SDL et les redirige vers les gestionnaires appropriés.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void handleEvent(Window *win, SDL_Event *event);

/** 
 * @fn void handleWindowSizeChange(Window *win)
 * @brief Gère les événements de changement de taille de la fenêtre.
 * 
 * Cette fonction ajuste les éléments du jeu en fonction de la nouvelle taille de la fenêtre.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 */
void handleWindowSizeChange(Window *win);

/** 
 * @fn void handleIntermediateEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements dans l'état intermédiaire du jeu.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void handleIntermediateEvent(Window *win, SDL_Event *event);

/** 
 * @fn void handleQuitEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements de fermeture de l'application.
 * 
 * Cette fonction traite les interactions pour quitter le jeu.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void handleQuitEvent(Window *win, SDL_Event *event);

/** 
 * @fn void handleSettingsEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements dans le menu des paramètres.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void handleSettingsEvent(Window *win, SDL_Event *event);

/** 
 * @fn void handleMenuEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements dans le menu principal.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void handleMenuEvent(Window *win, SDL_Event *event);

/** 
 * @fn void handleGameEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements pendant le jeu actif.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void handleGameEvent(Window *win, SDL_Event *event);

/** 
 * @fn void handleNewGameEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements lors du démarrage d'une nouvelle partie.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void handleNewGameEvent(Window *win, SDL_Event *event);

/** 
 * @fn void loadFile(Window *win, void *event)
 * @brief Gère les événements liés au chargement d'une partie sauvegardée.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void loadFile(Window *win, void *event);

/** 
 * @fn void handleLoadGameEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements de chargement d'une partie.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void handleLoadGameEvent(Window *win, SDL_Event *event);

/** 
 * @fn void handleICMonsEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements dans l'interface des ICMons.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void handleICMonsEvent(Window *win, SDL_Event *event);

/** 
 * @fn void handlePauseEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements lorsque le jeu est en pause.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void handlePauseEvent(Window *win, SDL_Event *event);

/** 
 * @fn void handlePlayerEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements lorsque le joueur est sur la carte.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void handlePlayerEvent(Window *win, SDL_Event *event);

/** 
 * @fn void handleSwapEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements d'échange entre un ICmon du joueur et de l'IA.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void handleSwapEvent(Window *win, SDL_Event *event);

/** 
 * @fn void handleCombatEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements lorsque le joueur est en combat.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void handleCombatEvent(Window *win, SDL_Event *event);

/** 
 * @fn void handleLearningEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements lorsque le joueur apprend une nouvelle capacité.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void handleLearningEvent(Window *win, SDL_Event *event);

/**
 * @fn void handleStartersEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements liés au choix des starters.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void handleStartersEvent(Window *win, SDL_Event *event);

/**
 * @fn void handleResumeEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements liés à la reprise d'une partie.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void handleResumeEvent(Window *win, SDL_Event *event);

#endif /* HANDLEEVENTS_H */