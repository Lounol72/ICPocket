#ifndef EVENTS_H
#define EVENTS_H

#include "GameEngine.h"

#include "Text.h"
#include "structPoke.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>

/**
 * @file Events.h
 * @author Louis Alban
 * @date 17/02/2025
 */

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

#endif /* HANDLEEVENTS_H */