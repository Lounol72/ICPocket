#ifndef EVENTS_H
#define EVENTS_H

#include "GameEngine.h"


/**
 * @file Events.h
 * @author Louis Alban
 * @brief Gestion des Events SDL
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

/** @brief Main event handler that processes all SDL events
 *  @param win Pointer to the Window structure containing game state
 *  @param event Pointer to the SDL_Event to process
 */
void handleEvent(Window *win, SDL_Event *event);

/** @brief Handles window size change events
 *  @param win Pointer to the Window structure containing game state
 */
void handleWindowSizeChange(Window *win);

/** @brief Handles events in the intermediate game state
 *  @param win Pointer to the Window structure containing game state
 *  @param event Pointer to the SDL_Event to process
 */
void handleIntermediateEvent(Window *win, SDL_Event *event);

/** @brief Handles quit events to close the application
 *  @param win Pointer to the Window structure containing game state
 *  @param event Pointer to the SDL_Event to process
 */
void handleQuitEvent(Window *win, SDL_Event *event);

/** @brief Handles events in the settings menu
 *  @param win Pointer to the Window structure containing game state
 *  @param event Pointer to the SDL_Event to process
 */
void handleSettingsEvent(Window *win, SDL_Event *event);

/** @brief Handles events in the main menu
 *  @param win Pointer to the Window structure containing game state
 *  @param event Pointer to the SDL_Event to process
 */
void handleMenuEvent(Window *win, SDL_Event *event);

/** @brief Handles events during active gameplay
 *  @param win Pointer to the Window structure containing game state
 *  @param event Pointer to the SDL_Event to process
 */
void handleGameEvent(Window *win, SDL_Event *event);

/** @brief Handles events when starting a new game
 *  @param win Pointer to the Window structure containing game state
 *  @param event Pointer to the SDL_Event to process
 */
void handleNewGameEvent(Window *win, SDL_Event *event);

/** @brief Handles events when loading a saved game
 *  @param win Pointer to the Window structure containing game state
 *  @param event Pointer to the SDL_Event to process
 */
void handleLoadGameEvent(Window *win, SDL_Event *event);

/** @brief Handles events in the ICMons interface
 *  @param win Pointer to the Window structure containing game state
 *  @param event Pointer to the SDL_Event to process
 */
void handleICMonsEvent(Window *win, SDL_Event *event);

/** @brief Handles events when the game is paused
 *  @param win Pointer to the Window structure containing game state
 *  @param event Pointer to the SDL_Event to process
 */
void handlePauseEvent(Window *win, SDL_Event *event);

/** @brief Handles events when the player is in the map
 *  @param win Pointer to the Window structure containing game state
 *  @param event Pointer to the SDL_Event to process
 */
void handlePlayerEvent(Window *win, SDL_Event *event);

/** @brief Handles events when the player swap an ICmon with the AI
 *  @param win Pointer to the Window structure containing game state
 *  @param event Pointer to the SDL_Event to process
 */
 void handleSwapEvent(Window *win, SDL_Event *event);

 int initTeamSprites(Window *win, t_Team *teamSprite, float x_ratio, float y_ratio, int teamFlag);

 void handleLearningEvent(Window *win, SDL_Event *event);

#endif /* HANDLEEVENTS_H */