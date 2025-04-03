/**
 * @file Game.c
 * @brief Initialisation du jeu, de ses interfaces et ressources.
 */

#include "../include/Game.h"

#define WINDOWS_W 1280
#define WINDOWS_H 720

/**
 * @fn void initGame(Window *win);
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
    game.nbMenu = 14;
    game.ui = malloc(game.nbMenu * sizeof(UI_Elements));
    for(int i = 0; i < game.nbMenu; i++) {
        game.ui[i] = (UI_Elements){ .buttons = malloc(sizeof(ButtonList)), .sliders = NULL, .background = NULL };
    }
    game.ui[1].sliders = malloc(sizeof(SliderList)); 


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
    game.nbStates = 14;
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
    game.stateHandlers[10]= (StateHandler){ SWAP, handleSwapEvent };
    game.stateHandlers[11]= (StateHandler){ LEARNMOVE, handleLearningEvent };
    game.stateHandlers[12]= (StateHandler){ STARTERS, handleStartersEvent };
    game.stateHandlers[13]= (StateHandler){ RESUME, handleResumeEvent };
    game.touche = NULL;
    game.touche = malloc(3 * sizeof(Image*));
    game.touche[0] = initImage(win, "assets/User Interface/keyboard_e.png", (SDL_Rect){win->width * 0.85, win->height * 0.55, win->width * 0.075, win->width * 0.075}, "Interact",(SDL_Rect){win->width * 0.60, win->height * 0.58, win->width * 0.1725, win->width * 0.050} ,game.win->LargeFont);
    game.touche[1] = initImage(win, "assets/User Interface/keyboard_c.png", (SDL_Rect){win->width * 0.85, win->height * 0.70, win->width * 0.075, win->width * 0.075}, "Confirm", (SDL_Rect){win->width * 0.60, win->height * 0.730, win->width * 0.1725, win->width * 0.050}, game.win->LargeFont);
    game.touche[2] = initImage(win, "assets/User Interface/keyboard_arrows.png", (SDL_Rect){win->width * 0.85, win->height * 0.85, win->width * 0.075, win->width * 0.075}, "Movements", (SDL_Rect){win->width * 0.60, win->height * 0.880, win->width * 0.175, win->width * 0.050}, game.win->LargeFont);

    /* Configuration de la fréquence d'images (FPS) */
    game.FPS = 60;
    game.frameDelay = 1000 / game.FPS;
    
    game.currentButton = 0;
    game.saved = 0;

    /* Chargement des fonds d'écran pour les différentes pages */
    loadBackground(&game.ui[2].background, win->renderer, "assets/Title Screen/BG.jpg");
    loadBackground(&game.ui[5].background, win->renderer, "assets/Title Screen/LoadGame.png");
    loadBackground(&game.ui[1].background, win->renderer, "assets/Battle Backgrounds/Other/zoonami_battle_party_background.png");
    loadBackground(&game.ui[4].background, win->renderer, "assets/Title Screen/GameStart.jpg");
    loadBackground(&game.ui[3].background, win->renderer, "assets/Title Screen/combat.jpg");
    loadBackground(&game.ui[6].background, win->renderer, "assets/Title Screen/BG.jpg");
    loadBackground(&game.ui[7].background, win->renderer, "assets/Title Screen/combat.jpg");
    loadBackground(&game.ui[8].background, win->renderer, "assets/Title Screen/BG.jpg");
    loadBackground(&game.ui[10].background,win->renderer, "assets/Title Screen/BG.jpg");
    loadBackground(&game.ui[11].background,win->renderer, "assets/Battle Backgrounds/Other/zoonami_battle_backpack_background.png");
    loadBackground(&game.ui[12].background,win->renderer, "assets/Title Screen/starters.png");
    loadBackground(&game.ui[13].background,win->renderer, "assets/Title Screen/pokedex.png");
    game.scrollingTextIntro = NULL;

    /* Initialisation de l'audio et chargement de la musique */
    initAudio();
    loadMusic(&game.gameState.music, "assets/audio/Battle.mp3");
    game.gameState.music_inter = NULL;

    /* Initialisation de la carte et de la caméra */
    game.gameData.maps[0] = initMap(win->renderer, "assets/Tileset/Map/hall.png");
    game.gameData.maps[1] = initMap(win->renderer, "assets/Tileset/Map/2.png");
    game.gameData.maps[2] = initMap(win->renderer, "assets/Tileset/Map/3.png");
    game.gameData.camera = createCamera(WINDOWS_W, WINDOWS_H);

    /* Création et configuration du joueur */
    game.gameData.player = createPlayer(win->renderer, "assets/Characters/Character 10.png", game.gameData.maps[0], 0);
    if (!game.gameData.player) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, 
                         "❌ Erreur : Impossible de créer le joueur");
        return;
    }
    game.gameData.player->position.x = game.gameData.maps[0]->positions[0] * TILE_SIZE_W_SCALE;
    game.gameData.player->position.y = game.gameData.maps[0]->positions[1] * TILE_SIZE_H_SCALE;
        
    /* Initialisation du système de texte */
    initText(win);
    game.speed = 50;

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
 * @fn void loadBackground(SDL_Texture **Background, SDL_Renderer *renderer, const char *imagePath);
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

void loadPhrase(void){
    FILE *file = fopen("data/dataText.csv", "r");
    if (file == NULL) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Failed to open data/dataText.csv");
        return;
    }
    char buffer[256];
    char temp[256];
    for (int i = 1; i < game.battleState.bleu.id; i++){
        if (fgets(buffer, sizeof(buffer), file) == NULL){
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Failed to read line %d from data/dataText.csv",i);
            fclose(file);
            return;
        }
    }
    
    fscanf(file, "%d,%[^\n]\n",&(game.battleState.bleu.id), temp);
    fclose(file);
    char phrase[256];
    snprintf(phrase, sizeof(phrase), "%s veut se battre ! %s", game.battleState.bleu.trainerName, temp);
    if (game.scrollingTextIntro != NULL) {
        destroyScrollingText(game.scrollingTextIntro);
        game.scrollingTextIntro = NULL;
    }
    game.scrollingTextIntro = createScrollingText(
                    phrase,
                    game.win->LargeFont,
                    (SDL_Color){255, 255, 255, 255},
                    game.speed,     // Délai entre les caractères en ms
                    (SDL_Rect){game.win->width * 0.014, game.win->height *0.736, game.win->width * 0.7, game.win->height * 0.27}, // Fond du texte
                    "assets/User Interface/Grey/button_rectangle_depth_flat.png", // Chemin de l'image de fond
                    game.win->renderer);

}

/**
 * @fn void destroyGame();
 * @brief detruit le jeu.
 * 
 */
void destroyGame() {
    /* 1) Libération de la musique */
    if (game.gameState.music) {
        Mix_FreeMusic(game.gameState.music);
        game.gameState.music = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ Musique libérée");
    }
    if (game.touche != NULL){
        for (int i = 0; i < 3; i++) {
            destroyImage(game.touche[i]);
        }
        free(game.touche);
        game.touche = NULL;
        
    }

    if (game.battleState.text) {
        cleanupScrollingText(&game.battleState.text);
        game.battleState.text = NULL;
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
                game.ui[i].background = NULL;
            }
        }
        free(game.ui);
        game.ui = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ UI libérée");
    }


    /* 3) Libération des state handlers */
    if (game.stateHandlers) {
        free(game.stateHandlers);
        game.stateHandlers = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ State handlers libérés");
    }

    /* 6) Fermeture des polices */
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

    /* 7) Destruction du renderer et de la fenêtre */
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

    /* 8) Libération des surfaces du curseur */
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

    /* 9) Fermeture de l'audio */
    Mix_CloseAudio();
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ audio libéré");

    /* 10) Quitter les sous-systèmes SDL, IMG et TTF */
    if (game.battleState.text!= NULL) {
        destroyScrollingText(game.battleState.text);
        game.battleState.text = NULL;
    }
    if (game.scrollingTextIntro != NULL){
        destroyScrollingText(game.scrollingTextIntro);
        game.scrollingTextIntro = NULL;
    }

    if (game.battleState.rouge.nb_poke != 0) {
        for (int i = 0; i < game.battleState.rouge.nb_poke; i++) {
            if (game.battleState.rouge.team[i].img) {
                destroyICMonsSprite(&game.battleState.rouge.team[i]);
            }
        }
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ ICMons rouges libérés");
    }
    
    /* 4) Destruction des ICMons pour l'équipe bleue */
    if (game.battleState.bleu.nb_poke != 0) {
        for (int i = 0; i < game.battleState.bleu.nb_poke; i++) {
            if (game.battleState.bleu.team[i].img) {
                destroyICMonsSprite(&game.battleState.bleu.team[i]);
            }
        }
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ ICMons bleus libérés");
    }
    
    cleanupText();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ SDL quitté");
}
