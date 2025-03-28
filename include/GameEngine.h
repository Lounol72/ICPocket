#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "Utils.h"
// Added explicit include to ensure AppState is defined:

#include "interDuel.h"


#include "Text.h"

#include "Player.h"

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

typedef enum {
    TURN_NONE,
    TURN_INIT,
    TURN_WAIT_TEXT,
    TURN_ACTION1,
    TURN_ACTION2,
    TURN_FINISHED
}BattleTurnState;

/* Battle data (assuming t_Team is defined in structPoke.h or similar) */
/**
 * @struct BattleState
 * @brief Données de combat.
 * 
 * Cette structure contient les équipes de combat et l'intelligence artificielle du dresseur.
 */
typedef struct {
    t_Team rouge;               /**< Équipe rouge. */
    t_Team bleu;                /**< Équipe bleue. */
    t_AI ia;                    /**< Intelligence artificielle du dresseur. */
    
    BattleTurnState turnState;  /**< État du tour de combat. */
    int moveRouge;              /**< Mouvement rouge. */;
    int moveBleu;               /**< Mouvement bleu. */;
    ScrollingText *text;        /**< Texte en cours de lecture. */
    int first;                  /**< Indicateur de priorité. */
    int hasAttacked;            /**< Indicateur d'attaque. */
    
} BattleState;

/* General game state info */
/**
 * @struct GameState
 * @brief Informations générales sur l'état du jeu.
 * 
 * Cette structure contient des informations sur la musique, le tour du joueur, et l'état actuel du jeu.
 */
typedef struct {
    Mix_Music *music;              /**< Musique de fond. */
    Mix_Music *music_inter;        /**< Musique de fond. */
    int playerTurn;                /**< Indicateur du tour du joueur. */
    int initialized;               /**< Indicateur d'initialisation du jeu. */
    AppState currentState;         /**< État actuel de l'application. */
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
    int speed;
    int FPS;                   /**< Images par seconde. */
    int frameDelay;            /**< Délai entre les images. */
    int frameStart;            /**< Début de la trame. */
    int currentButton;         /**< Bouton actuellement sélectionné. */
    int saved;                 /**< Indicateur de sauvegarde. */
    float deltaTime;           /**< Temps écoulé entre les trames. */
    ThreadManager threadManager; /**< Gestionnaire de threads. */
    SDL_Surface *cursor;       /**< Surface du curseur. */
    SDL_Surface *cursor_hover; /**< Surface du curseur au survol. */
    int swappingIndex[2];
    Text * windowText;
    t_Poke starters[4];
    int startersIndex;
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

/* Initialize basics for a Team Swap*/
/**
 * @brief Callback pour le gameState SWAP.
 * 
 * Cette fonction prépare le changement de state pour l'échange de ICmons.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param data Données supplémentaires pour le changement d'état.
 */

 void initLearningMove(void);

 void initSwapTeam(Window *win, void *data);

 void changeIndexSwap(Window *win, void *data);

 void validateSwap(Window *win, void *data);

 void learningMoveChoice(Window *win, void *data);

 void selectOtherStarter(Window *win, void *data);

 void validateStarterChoice(Window *win, void *data);

 void initStarters(Window *win, void * data);

 void initResume(Window *win, void *data);

 void destroyResume(Window *win, void *data);

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

/**
 * @brief Démarre le tour de combat.
 * 
 * Cette fonction démarre le tour de combat.
 */
void startBattleTurn(int moveRouge, int moveBleu);

/**
 * @brief Exécute une action de combat.
 * 
 * Cette fonction exécute une action de combat pour l'équipe spécifiée.
 */
void executeAction(t_Team *attacker, t_Team *defender, int move);

/**
 * @brief Met à jour le tour de combat.
 * 
 * Cette fonction met à jour le tour de combat.
 */
void updateBattleTurn();

void finishApplyEffectDamage();


#endif /* GAMEENGINE_H */