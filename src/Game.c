/**
 * @file Game.c
 * @brief Initialisation du jeu, de ses interfaces et ressources.
 */

#include "../include/Game.h"

#define WINDOWS_W 1280
#define WINDOWS_H 720

/**
 * @brief Initialise le jeu et charge l'ensemble des ressources nécessaires.
 *
 * Cette fonction initialise la structure globale du jeu en définissant :
 * - Les éléments de l'interface utilisateur (UI) pour les différentes pages.
 * - L'état du jeu (GameState) et les gestionnaires d'états (StateHandler).
 * - Les paramètres de fréquence d'images (FPS) et la gestion des vitesses.
 * - Le chargement des fonds d'écran pour les différentes pages.
 * - L'initialisation de l'audio et le chargement de la musique.
 * - La création du joueur, la carte, la caméra et le texte.
 * - Le chargement et la définition des curseurs.
 *
 * @param win Pointeur sur la fenêtre principale.
 */
void initGame(Window *win) {
    game.win = win;
    
    /* Initialisation des éléments de l'interface utilisateur (UI) */
    game.nbMenu = 10;
    game.ui = malloc(game.nbMenu * sizeof(UI_Elements));
    game.ui[0] = (UI_Elements){ .buttons = malloc(sizeof(ButtonList)), .sliders = NULL, .background = malloc(sizeof(SDL_Texture*)) };  // Quit Page            = 0
    game.ui[1] = (UI_Elements){ .buttons = malloc(sizeof(ButtonList)), .sliders = malloc(sizeof(SliderList)), .background = malloc(sizeof(SDL_Texture*)) }; // Settings Page = 1
    game.ui[2] = (UI_Elements){ .buttons = malloc(sizeof(ButtonList)), .sliders = NULL, .background = malloc(sizeof(SDL_Texture*)) };  // Menu Page            = 2
    game.ui[3] = (UI_Elements){ .buttons = malloc(sizeof(ButtonList)), .sliders = NULL, .background = malloc(sizeof(SDL_Texture*)) };  // Game Page            = 3
    game.ui[4] = (UI_Elements){ .buttons = malloc(sizeof(ButtonList)), .sliders = NULL, .background = malloc(sizeof(SDL_Texture*)) };  // New Game Page        = 4
    game.ui[5] = (UI_Elements){ .buttons = malloc(sizeof(ButtonList)), .sliders = NULL, .background = malloc(sizeof(SDL_Texture*)) };  // Load Game Page       = 5
    game.ui[6] = (UI_Elements){ .buttons = malloc(sizeof(ButtonList)), .sliders = NULL, .background = malloc(sizeof(SDL_Texture*)) };  // ICMons Page          = 6
    game.ui[7] = (UI_Elements){ .buttons = malloc(sizeof(ButtonList)), .sliders = NULL, .background = malloc(sizeof(SDL_Texture*)) };  // Intermediate         = 7
    game.ui[8] = (UI_Elements){ .buttons = malloc(sizeof(ButtonList)), .sliders = NULL, .background = malloc(sizeof(SDL_Texture*)) };  // Pause Page           = 8
    game.ui[9] = (UI_Elements){ .buttons = malloc(sizeof(ButtonList)), .sliders = NULL, .background = malloc(sizeof(SDL_Texture*)) };  // Map Page             = 9

    for (int i = 0; i < game.nbMenu; i++) {
        game.ui[i].buttons->buttons = NULL;
        game.ui[i].buttons->size = 0;
    }
    /* Initialisation spécifique aux sliders de la page Settings (index 1) */
    game.ui[1].sliders->sliders = NULL;
    game.ui[1].sliders->size = 0;

    /* Initialisation de l'état du jeu */
    game.gameState = (GameState){ .music = NULL, .playerTurn = 1, .initialized = 0, .currentState = MENU };

    /* Initialisation des gestionnaires d'états */
    game.nbStates = 10;
    game.stateHandlers = malloc(game.nbStates * sizeof(StateHandler));
    game.stateHandlers[0] = (StateHandler){ QUIT, handleQuitEvent };
    game.stateHandlers[1] = (StateHandler){ SETTINGS, handleSettingsEvent };
    game.stateHandlers[2] = (StateHandler){ MENU, handleMenuEvent };
    game.stateHandlers[3] = (StateHandler){ GAME, handleGameEvent };
    game.stateHandlers[4] = (StateHandler){ NEWGAME, handleNewGameEvent };
    game.stateHandlers[5] = (StateHandler){ LOADGAME, handleLoadGameEvent };
    game.stateHandlers[6] = (StateHandler){ ICMONS, handleICMonsEvent };
    game.stateHandlers[7] = (StateHandler){ INTER, handleIntermediateEvent };
    game.stateHandlers[8] = (StateHandler){ PAUSE, handlePauseEvent };
    game.stateHandlers[9] = (StateHandler){ MAP, handlePlayerEvent };

    /* Configuration de la fréquence d'images (FPS) */
    game.FPS = 60;
    game.frameDelay = 1000 / game.FPS;
    
    game.currentButton = 0;
    game.saved = 0;

    /* Initialisation des vitesses (par exemple, pour l'animation ou la progression) */
    game.speeds = malloc(sizeof(float) * 3);
    game.speeds[0] = 0.5f;
    game.speeds[1] = 1.0f;
    game.speeds[2] = 1.5f;

    /* Chargement des fonds d'écran pour les différentes pages */
    loadBackground(&game.ui[2].background, win->renderer, "assets/Title Screen/BG.jpg");
    loadBackground(&game.ui[5].background, win->renderer, "assets/Title Screen/LoadGame.png");
    loadBackground(&game.ui[1].background, win->renderer, "assets/Battle Backgrounds/Other/zoonami_battle_party_background.png");
    loadBackground(&game.ui[4].background, win->renderer, "assets/Title Screen/GameStart.jpg");
    loadBackground(&game.ui[3].background, win->renderer, "assets/Title Screen/combat.png");
    loadBackground(&game.ui[6].background, win->renderer, "assets/Title Screen/BG.jpg");
    loadBackground(&game.ui[7].background, win->renderer, "assets/Title Screen/combat.png");
    loadBackground(&game.ui[8].background, win->renderer, "assets/Title Screen/BG.jpg");

    /* Initialisation de l'audio et chargement de la musique */
    initAudio();
    loadMusic(&game.gameState.music, "assets/audio/Battle.mp3");
    loadMusic(&game.gameState.music_inter, "assets/audio/Ciel.mp3");
    /* Création et configuration du joueur */
    game.gameData.player = createPlayer(win->renderer, "assets/Characters/Character 10.png");
    if (!game.gameData.player) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, 
                         "❌ Erreur : Impossible de créer le joueur");
        return;
    }
    game.gameData.player->position.x = MAP_WIDTH * TILE_SIZE_W_SCALE / 2;
    game.gameData.player->position.y = MAP_HEIGHT * TILE_SIZE_H_SCALE / 2;
    
    /* Initialisation de la carte et de la caméra */
    game.gameData.map = initMap(win->renderer, "assets/Tileset/Map/hall.png");
    game.gameData.camera = createCamera(WINDOWS_W, WINDOWS_H);
    
    /* Initialisation du système de texte */
    initText(win);

    /* Chargement et configuration des curseurs */
    game.cursor = IMG_Load("assets/pointer_a.png");
    if (!game.cursor) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, 
                         "❌ Erreur : Impossible de charger le curseur 'pointer_a.png'.");
        return;
    }
    SDL_SetCursor(SDL_CreateColorCursor(game.cursor, 0, 0));
    game.cursor_hover = IMG_Load("assets/hand_point.png");
    if (!game.cursor_hover) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, 
                         "❌ Erreur : Impossible de charger le curseur 'hand_point.png'.");
        return;
    }
}

/**
 * @brief Charge une image de fond et crée une texture associée.
 *
 * Cette fonction charge une image depuis le chemin spécifié, crée une texture à partir de cette image
 * et la stocke dans le pointeur passé en paramètre.
 *
 * @param Background Pointeur vers la texture qui recevra le fond.
 * @param renderer Renderer SDL utilisé pour créer la texture.
 * @param imagePath Chemin vers l'image de fond.
 */
void loadBackground(SDL_Texture **Background, SDL_Renderer *renderer, const char *imagePath) {
    if (!renderer || !imagePath) {
        SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_ERROR, 
                         "❌ Erreur : Le renderer ou le chemin de l'image est NULL.");
        return;
    }
    SDL_Surface *surface = IMG_Load(imagePath);
    if (!surface) {
        SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_ERROR, 
                         "❌ Erreur : Impossible de charger l'image de fond '%s'. Message SDL_image : %s", imagePath, IMG_GetError());
        return;
    }
    *Background = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void destroyGame() {
    /* 1) Libération de la musique */
    if (game.gameState.music) {
        Mix_FreeMusic(game.gameState.music);
        game.gameState.music = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ Musique libérée");
    }
    if (game.gameState.music_inter) {
        Mix_FreeMusic(game.gameState.music_inter);
        game.gameState.music_inter = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ Musique inter libérée");
    }

    /* 2) Destruction des éléments de l'interface (UI) */
    if (game.ui) {
        for (int i = 0; i < game.nbMenu; i++) {
            if (game.ui[i].buttons) {
                destroyButtonList(game.ui[i].buttons);
                free(game.ui[i].buttons);
                game.ui[i].buttons = NULL;
            }
            if (game.ui[i].sliders) {
                destroySliderList(game.ui[i].sliders);
                free(game.ui[i].sliders);
                game.ui[i].sliders = NULL;
            }
            if (game.ui[i].background) {
                SDL_DestroyTexture(game.ui[i].background);
                free(game.ui[i].background);
                game.ui[i].background = NULL;
            }
        }
        free(game.ui);
        game.ui = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ UI libérée");
    }

    /* 3) Destruction des ICMons pour l'équipe rouge */
    if (game.battleState.rouge.nb_poke != 0) {
        for (int i = 0; i < game.battleState.rouge.nb_poke; i++) {
            destroyICMonsSprite(&game.battleState.rouge.team[i]);
        }
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ ICMons rouges libérés");
    }
    
    /* 4) Destruction des ICMons pour l'équipe bleue */
    if (game.battleState.bleu.nb_poke != 0) {
        for (int i = 0; i < game.battleState.bleu.nb_poke; i++) {
            destroyICMonsSprite(&game.battleState.bleu.team[i]);
        }
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ ICMons bleus libérés");
    }

    /* 5) Libération des state handlers */
    if (game.stateHandlers) {
        free(game.stateHandlers);
        game.stateHandlers = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ State handlers libérés");
    }

    /* 6) Libération du tableau de vitesses */
    if (game.speeds) {
        free(game.speeds);
        game.speeds = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ Speeds libérés");
    }

    /* 7) Fermeture des polices */
    if (game.win->LargeFont) {
        TTF_CloseFont(game.win->LargeFont);
        game.win->LargeFont = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ LargeFont libéré");
    }
    if (game.win->MediumFont) {
        TTF_CloseFont(game.win->MediumFont);
        game.win->MediumFont = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ MediumFont libéré");
    }
    if (game.win->SmallFont) {
        TTF_CloseFont(game.win->SmallFont);
        game.win->SmallFont = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ SmallFont libéré");
    }
    if (game.win->font) {
        TTF_CloseFont(game.win->font);
        game.win->font = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ font libéré");
    }

    /* 8) Destruction du renderer et de la fenêtre */
    if (game.win->renderer) {
        SDL_DestroyRenderer(game.win->renderer);
        game.win->renderer = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ renderer libéré");
    }
    if (game.win->window) {
        SDL_DestroyWindow(game.win->window);
        game.win->window = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ window libéré");
    }

    /* 9) Libération des surfaces du curseur */
    if (game.cursor) {
        SDL_FreeSurface(game.cursor);
        game.cursor = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ cursor libéré");
    }
    if (game.cursor_hover) {
        SDL_FreeSurface(game.cursor_hover);
        game.cursor_hover = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ cursor_hover libéré");
    }

    /* 10) Fermeture de l'audio */
    Mix_CloseAudio();
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ audio libéré");

    /* 11) Quitter les sous-systèmes SDL, IMG et TTF */
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ SDL quitté");
}
