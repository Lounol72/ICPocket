#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "Utils.h"
// Added explicit include to ensure AppState is defined:

#include "interDuel.h"


#include "Images.h"

#include "Player.h"

#include "ThreadManager.h"

#include <sys/time.h>

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
    int moveRouge;              /**< Mouvement rouge. */
    int moveBleu;               /**< Mouvement bleu. */
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
    struct timeval time_in_state;  /**< Temps anti double clic entre deux gameStates */
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
    Map *maps[3];
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
    int speed;              /**< Vitesse de l'animation. */
    int FPS;                   /**< Images par seconde. */
    int frameDelay;            /**< Délai entre les images. */
    int frameStart;            /**< Début de la trame. */
    int currentButton;         /**< Bouton actuellement sélectionné. */
    int saved;                 /**< Indicateur de sauvegarde. */
    float deltaTime;           /**< Temps écoulé entre les trames. */
    ThreadManager threadManager; /**< Gestionnaire de threads. */
    SDL_Surface *cursor;       /**< Surface du curseur. */
    SDL_Surface *cursor_hover; /**< Surface du curseur au survol. */
    int swappingIndex[2];      /**< Index d'échange. */
    int hasExchanged;           /**< Booleen "si echange disponible" */
    Text * windowText;         /**< Texte de la fenêtre. */
    t_Poke starters[4];        /**< Liste des starters. */
    int startersIndex;        /**< Index des starters. */
    Image **touche;         /**< Images des touches. */
    ScrollingText *scrollingTextIntro;  /**< Texte défilant d'introduction. */
    bool isInDuel;
} Game;

#include "Game.h"
#include "Events.h"
#include "RenderICMons.h"

/* ------------- Function Prototypes ------------- */

/**
 * @fn void updateICMonsButtonText(Window *win, t_Team *team, int endOfArray, AppState state);
 * @brief Met à jour le texte des boutons ICMons.
 *
 * Pour chaque pokémon de l'équipe passée en paramètre, met à jour le texte du bouton
 * associé dans l'interface.
 *
 * @param win Pointeur sur la fenêtre.
 * @param team Pointeur sur l'équipe (t_Team) dont on met à jour l'affichage.
 */
void updateICMonsButtonText(Window *win, t_Team *team, int endOfArray, AppState state);

/* Render */
/**
 * @fn void render(Window *win);
 * @brief Rend le contenu de la fenêtre.
 * 
 * Cette fonction gère le rendu de tous les éléments de la fenêtre.
 * 
 * @param win Un pointeur vers la structure Window.
 */
void render(Window *win);

/* Main loop */
/**
 * @fn void mainLoop(Window *win);
 * @brief Boucle principale du jeu.
 * 
 * Cette fonction gère la boucle principale du jeu, y compris la gestion des événements et le rendu.
 * 
 * @param win Un pointeur vers la structure Window.
 */
void mainLoop(Window *win);


/* Attack button */
/**
 * @fn void attqButtonClicked(Window *win, void *data);
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
 * @fn changeState(Window *win, void *data);
 * @brief Callback générique pour le changement d'état.
 * 
 * Cette fonction change l'état actuel de l'application.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param data Données supplémentaires pour le changement d'état.
 */
void changeState(Window *win, void *data);

/**
 * @fn void initLearningMove(void);
 * @brief Initialise l'apprentissage d'une nouvelle capacité.
 * 
 * Cette fonction prépare l'état pour l'apprentissage d'une nouvelle capacité par un ICmon.
 */
void initLearningMove(void);

/**
 * @fn initSwapTeam(Window *win, void *data);
 * @brief Initialise l'échange d'équipe.
 * 
 * Cette fonction configure l'état pour permettre l'échange de membres de l'équipe.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param data Données supplémentaires pour l'échange.
 */
void initSwapTeam(Window *win, void *data);

/**
 * @fn changeIndexSwap(Window *win, void *data);
 * @brief Change l'index de l'échange.
 * 
 * Cette fonction met à jour l'index des membres de l'équipe à échanger.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param data Données supplémentaires pour l'échange.
 */
void changeIndexSwap(Window *win, void *data);

/**
 * @fn validateSwap(Window *win, void *data);
 * @brief Valide l'échange d'équipe.
 * 
 * Cette fonction applique les changements d'équipe après validation.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param data Données supplémentaires pour l'échange.
 */
void validateSwap(Window *win, void *data);

/**
 * @fn learningMoveChoice(Window *win, void *data);
 * @brief Gère le choix d'une nouvelle capacité.
 * 
 * Cette fonction permet au joueur de choisir une nouvelle capacité à apprendre.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param data Données supplémentaires pour le choix.
 */
void learningMoveChoice(Window *win, void *data);

/**
 * @fn void selectOtherStarter(Window *win, void *data);
 * @brief Sélectionne un autre starter.
 * 
 * Cette fonction permet au joueur de sélectionner un autre ICmon starter.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param data Données supplémentaires pour la sélection.
 */
void selectOtherStarter(Window *win, void *data);

/**
 * @fn void validateStarterChoice(Window *win, void *data);
 * @brief Valide le choix du starter.
 * 
 * Cette fonction confirme le choix du starter par le joueur.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param data Données supplémentaires pour la validation.
 */
void validateStarterChoice(Window *win, void *data);

/**
 * @fn void initStarters(Window *win, void *data);
 * @brief Initialise les starters.
 * 
 * Cette fonction configure l'état pour permettre au joueur de choisir un starter.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param data Données supplémentaires pour l'initialisation.
 */
void initStarters(Window *win, void *data);

/**
 * @fn void initResume(Window *win, void *data);
 * @brief Initialise l'état de reprise.
 * 
 * Cette fonction configure l'état pour reprendre une partie sauvegardée.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param data Données supplémentaires pour l'initialisation.
 */
void initResume(Window *win, void *data);

/**
 * @fn void destroyResume(Window *win, void *data);
 * @brief Détruit l'état de reprise.
 * 
 * Cette fonction nettoie les ressources utilisées pour l'état de reprise.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param data Données supplémentaires pour la destruction.
 */
void destroyResume(Window *win, void *data);

/* Text-related updates for attacks */
/**
 * @fn void updateICButtons(Window *win, t_Team *team);
 * @brief Met à jour les boutons d'attaque.
 * 
 * Cette fonction met à jour les boutons d'attaque pour l'équipe spécifiée.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param team Un pointeur vers l'équipe t_Team.
 */
void updateICButtons(Window *win, t_Team *team);

/**
 * @fn void nextDuel(Window *win, void *data);
 * @brief Passe au duel suivant.
 * 
 * Cette fonction gère la transition vers le duel suivant.
 * 
 * @param win Un pointeur vers la structure Window.
 * @param data Données supplémentaires pour la transition.
 */
void nextDuel(Window* win, void *data);

/**
 * @fn void updateCurrentButton()
 * @brief Met à jour le bouton actuel.
 * 
 * Cette fonction met à jour le bouton actuellement sélectionné.
 */
void updateCurrentButton();

/**
 * @fn void updateMusic();
 * @brief Met à jour la musique.
 * 
 * Cette fonction gère la mise à jour de la musique de fond.
 */
void updateMusic();

/**
 * @fn void manageFrameRate(int frameStart);
 * @brief Gère le taux de rafraîchissement.
 * 
 * Cette fonction gère le taux de rafraîchissement des images pour maintenir une performance stable.
 * 
 * @param frameStart Le temps de début de la trame.
 */
void manageFrameRate(int frameStart);

/**
 * @fn void startBattleTurn(int moveRouge, int moveBleu); 
 * @brief Démarre le tour de combat.
 * 
 * Cette fonction démarre le tour de combat.
 * 
 * @param moveRouge Le mouvement de l'équipe rouge.
 * @param moveBleu Le mouvement de l'équipe bleue.
 */
void startBattleTurn(int moveRouge, int moveBleu);

/**
 * @fn void executeAction(t_Team *attacker, t_Team *defender, int move);
 * @brief Exécute une action de combat.
 * 
 * Cette fonction exécute une action de combat pour l'équipe spécifiée.
 * 
 * @param attacker l'équipe attaquante.
 * @param defender l'équipe défensive.
 * @param move le mouvement à exécuter.
 */
void executeAction(t_Team *attacker, t_Team *defender, int move);

/**
 * @fn void updateBattleTurn();
 * @brief Met à jour le tour de combat.
 * 
 * Cette fonction met à jour le tour de combat.
 */
void updateBattleTurn();

/**
 * @fn void finishApplyEffectDamage();
 * @brief appllique les effets brulûre et poison
 * 
 * cette fonction permet d'appliquer les effets jusqu'à fin d'application de ce dernier
 */
void finishApplyEffectDamage();


#endif /* GAMEENGINE_H */