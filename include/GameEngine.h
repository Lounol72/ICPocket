#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_gamecontroller.h>
#include <stdio.h>
#include <stdlib.h>
#include "Log.h"
#include "Audio.h"
#include "Buttons.h"
#include "structPoke.h"
#include "duel.h"
#include "trainerAI.h"
#include "save.h"
#include "interDuel.h"
#include "Utils.h"
#include "Text.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "Camera.h"
#include "ThreadManager.h"
/**
 * @file GameEngine.h
 * @brief Définition et gestion du moteur de jeu.
 * 
 * Ce fichier définit les structures et fonctions principales pour gérer le moteur de jeu, y compris les états du jeu, les éléments de l'interface utilisateur, et les mécanismes de rendu.
 * 
 * @author Louis
 * @date 24/01/2025
 */



/* ------------- Structs ------------- */
/* UI element container, referencing the ButtonList & SliderList defined in Buttons.h */
/**
 * @struct UI_Elements
 * @brief Conteneur pour les éléments de l'interface utilisateur.
 * 
 * Cette structure référence les listes de boutons et de curseurs définies dans Buttons.h, ainsi que la texture de fond.
 */
typedef struct {
    ButtonList *buttons;     /**< Liste des boutons de l'interface utilisateur. */
    SliderList *sliders;     /**< Liste des curseurs de l'interface utilisateur. */
    SDL_Texture *background; /**< Texture de fond de l'interface utilisateur. */
} UI_Elements;

/* Battle data (assuming t_Team is defined in structPoke.h or similar) */
/**
 * @struct BattleState
 * @brief Données de combat.
 * 
 * Cette structure contient les équipes de combat et l'intelligence artificielle du dresseur.
 */
typedef struct {
    t_Team rouge;     /**< Équipe rouge. */
    t_Team bleu;      /**< Équipe bleue. */
    t_AI ia;          /**< Intelligence artificielle du dresseur. */
} BattleState;

/* General game state info */
/**
 * @struct GameState
 * @brief Informations générales sur l'état du jeu.
 * 
 * Cette structure contient des informations sur la musique, le tour du joueur, et l'état actuel du jeu.
 */
typedef struct {
    Mix_Music *music;       /**< Musique de fond. */
    int playerTurn;         /**< Indicateur du tour du joueur. */
    int initialized;        /**< Indicateur d'initialisation du jeu. */
    AppState currentState;   /**< État actuel de l'application. */
} GameState;

/* Each state has a render function and an event handler. */
/**
 * @struct StateHandler
 * @brief Gestionnaire d'état.
 * 
 * Chaque état a une fonction de rendu et un gestionnaire d'événements.
 */
typedef struct StateHandler {
    AppState state;          /**< État de l'application. */
    void (*handleEvent)(Window *, SDL_Event *); /**< Pointeur vers la fonction de gestion des événements. */
} StateHandler;

typedef struct {
    Player *player;
    Map *map;
    Camera *camera;
} GameData;

/* Main Game struct that ties everything together */
/**
 * @struct Game
 * @brief Structure principale du jeu.
 * 
 * Cette structure regroupe tous les éléments du jeu, y compris l'interface utilisateur, les états du jeu, et les données de combat.
 */
typedef struct Game {
    UI_Elements *ui;          /**< Éléments de l'interface utilisateur. */
    int nbMenu;               /**< Nombre de menus. */
    GameData gameData;        /**< Données du jeu. */
    BattleState battleState;   /**< État du combat. */
    GameState gameState;       /**< État général du jeu. */
    StateHandler *stateHandlers; /**< Gestionnaires d'état. */
    int nbStates;             /**< Nombre d'états. */
    Window *win;               /**< Pointeur vers la fenêtre du jeu. */
    float *speeds;             /**< Tableau des vitesses. */
    int FPS;                   /**< Images par seconde. */
    int frameDelay;            /**< Délai entre les images. */
    int frameStart;            /**< Début de la trame. */
    int currentButton;         /**< Bouton actuellement sélectionné. */
    int saved;                 /**< Indicateur de sauvegarde. */
    float deltaTime;           /**< Temps écoulé entre les trames. */
    ThreadManager threadManager; /**< Gestionnaire de threads. */
    SDL_Surface *cursor;       /**< Surface du curseur. */
    SDL_Surface *cursor_hover; /**< Surface du curseur au survol. */
} Game;

#include "Game.h"
#include "Events.h"
#include "RenderICMons.h"

/* ------------- Function Prototypes ------------- */

/**
 * @brief Initialise tous les boutons.
 * 
 * Cette fonction initialise tous les boutons de l'interface utilisateur.
 * 
 * @param win Un pointeur vers la structure Window.
 */
void initAllButtons(Window *win);

/* Render */
/**
 * @brief Rend le contenu de la fenêtre.
 * 
 * Cette fonction gère le rendu de tous les éléments de la fenêtre.
 * 
 * @param win Un pointeur vers la structure Window.
 */
void render(Window *win);

/* Main loop */
/**
 * @brief Boucle principale du jeu.
 * 
 * Cette fonction gère la boucle principale du jeu, y compris la gestion des événements et le rendu.
 * 
 * @param win Un pointeur vers la structure Window.
 */
void mainLoop(Window *win);


/* Attack button */
/**
 * @brief Gestionnaire de clic pour le bouton d'attaque.
 * 
 * Cette fonction est appelée lorsque le bouton d'attaque est cliqué.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param data Données supplémentaires pour le gestionnaire de clic.
 */
void attqButtonClicked(Window *win, void *data);

/* Generic state change button callback */
/**
 * @brief Callback générique pour le changement d'état.
 * 
 * Cette fonction change l'état actuel de l'application.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param data Données supplémentaires pour le changement d'état.
 */
void changeState(Window *win, void *data);

/* Text-related updates for attacks */
/**
 * @brief Met à jour les boutons d'attaque.
 * 
 * Cette fonction met à jour les boutons d'attaque pour l'équipe spécifiée.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param team Un pointeur vers l'équipe t_Team.
 */
void updateICButtons(Window *win, t_Team *team);

/**
 * @brief Passe au duel suivant.
 * 
 * Cette fonction gère la transition vers le duel suivant.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param data Données supplémentaires pour la transition.
 */
void nextDuel(Window* win, void *data);

/**
 * @brief Met à jour le bouton actuel.
 * 
 * Cette fonction met à jour le bouton actuellement sélectionné.
 */
void updateCurrentButton();

/**
 * @brief Met à jour la musique.
 * 
 * Cette fonction gère la mise à jour de la musique de fond.
 */
void updateMusic();

/**
 * @brief Gère le taux de rafraîchissement.
 * 
 * Cette fonction gère le taux de rafraîchissement des images pour maintenir une performance stable.
 * 
 * @param frameStart Le temps de début de la trame.
 */
void manageFrameRate(int frameStart);

#endif /* GAMEENGINE_H */