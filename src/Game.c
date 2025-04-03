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
    int spawnX, spawnY;
    game.gameData.map = initMap(win->renderer, "assets/Tileset/Map/hall.png", MAP_WIDTH, MAP_HEIGHT, &spawnX, &spawnY);
    game.gameData.camera = createCamera(WINDOWS_W, WINDOWS_H);

    /* Création et configuration du joueur */
    game.gameData.player = createPlayer(win->renderer, "assets/Characters/Character 10.png", game.gameData.map, spawnX, spawnY);
    if (!game.gameData.player) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, 
                         "❌ Erreur : Impossible de créer le joueur");
        return;
    }
    game.gameData.player->position.x = spawnX * TILE_SIZE_W_SCALE;
    game.gameData.player->position.y = spawnY * TILE_SIZE_H_SCALE;
        
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
    game.isInDuel = false;
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
    if (game.scrollingTextIntro != NULL) {
        destroyScrollingText(game.scrollingTextIntro);
        game.scrollingTextIntro = NULL;
    }
    game.scrollingTextIntro = createScrollingText(
                    temp,
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

    if (game.battleState.bleu.team[0].img) {
        for (int i = 0; i < game.battleState.bleu.nb_poke; i++) {
            destroyICMonsSprite(&game.battleState.bleu.team[i]);
        }
    }
    if (game.battleState.rouge.team[0].img) {
        for (int i = 0; i < game.battleState.rouge.nb_poke; i++) {
            destroyICMonsSprite(&game.battleState.rouge.team[i]);
        }
        
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

    cleanupText();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ SDL quitté");
}

// ---------- Fonctions statiques pour les boutons ------------------

/**
 * @fn void makeWindowFullScreen(Window *win, void *data);
 * @brief Passe la fenêtre en mode plein écran.
 *
 * Change le mode d'affichage de la fenêtre en plein écran (mode desktop).
 *
 * @param win Pointeur sur la fenêtre.
 * @param data Paramètre non utilisé.
 */
static inline void makeWindowFullScreen(Window *win, void *data) {
    (void)data;
    SDL_SetWindowFullscreen(win->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_GetWindowSize(win->window, &win->width, &win->height);
}

/**
 * @fn void makeWindowWindowed(Window *win, void *data);
 * @brief Repasse la fenêtre en mode fenêtré.
 *
 * Si la fenêtre est actuellement en plein écran, la repasse en mode fenêtré et ajuste sa taille.
 *
 * @param win Pointeur sur la fenêtre.
 * @param data Paramètre non utilisé.
 */
static inline void makeWindowWindowed(Window *win, void *data) {
    (void)data;
    if (!(SDL_GetWindowFlags(win->window) & SDL_WINDOW_FULLSCREEN_DESKTOP))
        return;
    SDL_SetWindowFullscreen(win->window, 0);
    SDL_SetWindowSize(win->window, win->InitialWidth, win->InitialHeight);
    handleWindowSizeChange(win);
}

/**
 * @fn void changePokemon(Window *win, void *data);
 * @brief Change le pokémon actif lors d'un échange.
 *
 * Exécute un tour d'attaque ou échange l'attaquant actif selon la validité de l'action,
 * puis met à jour l'affichage des boutons.
 *
 * @param win Pointeur sur la fenêtre.
 * @param data Pointeur vers l'indice (int casté en void*) du pokémon à activer.
 */
static inline void changePokemon(Window *win, void *data) {
    (void)win;
    int idx = (int)(intptr_t)data;
    if (testActionValidity(idx, &game.battleState.rouge)) {
        changeState(win, &game.stateHandlers[3].state);
        SDL_RenderPresent(win->renderer);
        isAlive(&game.battleState.rouge.team[0]) ? startBattleTurn(idx, AI_move_choice(&game.battleState.ia, &game.battleState.rouge)) : swapActualAttacker(&game.battleState.rouge, idx);
    }
    updateICButtons(win, &game.battleState.rouge);
    updateICMonsButtonText(win, &game.battleState.rouge, 6 , 6);
}


/**
 * @brief Initialise tous les boutons et sliders de l'interface.
 *
 * Crée et configure les boutons pour les différents états (menu, paramètres, load game,
 * jeu, ICMons, et interactions) ainsi que les sliders de l'écran de réglages, puis les ajoute
 * aux listes correspondantes.
 *
 * @param win Pointeur sur la fenêtre.
 */
void initAllButtons(Window *win)
{
    int nbButtonsLoad = 3;
    int nbButtonsMenu = 4;
    int nbSlidersSettings = 1;
    int nbButtonsParam = 6;
    int nbButtonsGame = 5;
    int nbButtonsICMons = 7;
    int nbButtonsInter = 3;
    int nbButtonsSwap = 15;
    int nbButtonsLearn = 5;
    int nbButtonsStarters = 3;
    int nbButtonsResume = 5;

    Button *buttonsMenu[nbButtonsMenu];
    Button *buttonsParam[nbButtonsParam];
    Button *buttonsLoadGame[nbButtonsLoad];
    Button *buttonsGame[nbButtonsGame];
    Button *buttonsICMons[nbButtonsICMons];
    Button *buttonsInter[nbButtonsInter];
    Button *buttonsSwap[nbButtonsSwap];
    Button *buttonsLearn[nbButtonsLearn];
    Button *buttonsStarters[nbButtonsStarters];
    Button *buttonsResume[nbButtonsResume];
    Slider *sliders[nbSlidersSettings];

    /* Boutons du menu */
    buttonsMenu[0] = createButton(
        " PLAY ", win, (SDL_Rect){500, 150, 300, 100},
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        initStarters, &game.stateHandlers[NEWGAME].state, win->MediumFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsMenu[1] = createButton(
        " LOAD GAME ", win, (SDL_Rect){500, 300, 300, 100},
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &game.stateHandlers[5].state, win->MediumFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsMenu[2] = createButton(
        " SETTINGS ", win, (SDL_Rect){500, 450, 300, 100},
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &game.stateHandlers[1].state, win->MediumFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsMenu[3] = createButton(
        "QUIT", win, (SDL_Rect){500, 600, 300, 100},
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &game.stateHandlers[0].state, win->MediumFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );

    /* Boutons des paramètres */
    buttonsParam[0] = createButton(
        "  0.5  ", win, (SDL_Rect){100, 200, 200, 50},
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeTextSpeed, (void*)(intptr_t)100, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsParam[1] = createButton(
        "  1  ", win, (SDL_Rect){400, 200, 200, 50},
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeTextSpeed, (void*)(intptr_t)50, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsParam[2] = createButton(
        "  1.5  ", win, (SDL_Rect){700, 200, 200, 50},
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeTextSpeed, (void*)(intptr_t)25, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsParam[3] = createButton(
        " Fullscreen ", win, (SDL_Rect){100, 300, 220, 75},
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        makeWindowFullScreen, NULL, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsParam[4] = createButton(
        " Windowed ", win, (SDL_Rect){400, 300, 220, 75},
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        makeWindowWindowed, NULL, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsParam[5] = createButton(
        "  Back  ", win, (SDL_Rect){100, 600, 300, 100},
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &game.stateHandlers[2].state, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );

    /* Boutons de Load Game */
    buttonsLoadGame[0] = createButton(
        " Save 1 ", win, (SDL_Rect){500, 104, 300, 100},
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        loadFile, (void*)(char*)"1", win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsLoadGame[1] = createButton(
        " Save 2 ", win, (SDL_Rect){500, 258, 300, 100},
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        loadFile, (void*)(char*)"2" , win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsLoadGame[2] = createButton(
        "  Back  ", win, (SDL_Rect){100, 600, 300, 100},
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &game.stateHandlers[2].state, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );

    /* Boutons du jeu */
    int buttonWidth = 430;
    int buttonHeight = 88;
    int spacingX = 5;
    int spacingY = 7;
    int startX = 20;
    int startY = 532;
    buttonsGame[0] = createButton(
        "Attack 1", win, (SDL_Rect){startX, startY, buttonWidth, buttonHeight},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        attqButtonClicked, (void*)(intptr_t)0, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsGame[1] = createButton(
        "Attack 2", win, (SDL_Rect){startX, startY + buttonHeight + spacingY, buttonWidth, buttonHeight},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        attqButtonClicked, (void*)(intptr_t)1, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsGame[2] = createButton(
        "Attack 3", win, (SDL_Rect){startX + buttonWidth + spacingX, startY, buttonWidth, buttonHeight},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        attqButtonClicked, (void*)(intptr_t)2, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsGame[3] = createButton(
        "Attack 4", win, (SDL_Rect){startX + buttonWidth + spacingX, startY + buttonHeight + spacingY, buttonWidth, buttonHeight},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        attqButtonClicked, (void*)(intptr_t)3, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsGame[4] = createButton(
        " ICMons ", win, (SDL_Rect){950, startY, 300, 180},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changeState, &game.stateHandlers[6].state, win->LargeFont,
        "assets/User Interface/Blue/button_rectangle_depth_gloss.png"
    );

    /* Boutons ICMons */
    buttonsICMons[0] = createButton(
        "ICMon1", win, (SDL_Rect){20, 20, 160, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        NULL, (void*)(intptr_t)0, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsICMons[1] = createButton(
        "ICMon2", win, (SDL_Rect){240, 20, 160, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changePokemon, (void*)(intptr_t)11, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsICMons[2] = createButton(
        "ICMon3", win, (SDL_Rect){460, 20, 160, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changePokemon, (void*)(intptr_t)12, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsICMons[3] = createButton(
        "ICMon4", win, (SDL_Rect){680, 20, 160, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changePokemon, (void*)(intptr_t)13, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsICMons[4] = createButton(
        "ICMon5", win, (SDL_Rect){900, 20, 160, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changePokemon, (void*)(intptr_t)14, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsICMons[5] = createButton(
        "ICMon6", win, (SDL_Rect){1120, 20, 160, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changePokemon, (void*)(intptr_t)15, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsICMons[6] = createButton(
        "Back", win, (SDL_Rect){100, 600, 300, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changeState, &game.stateHandlers[3].state, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );

    /* Boutons d'interaction */
    buttonsInter[0] = createButton(
        "Next Duel", win, (SDL_Rect){500, 200, 300, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        nextDuel, NULL, win->LargeFont,
        //changeState, &game.stateHandlers[MAP].state, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsInter[1] = createButton(
        "Back", win, (SDL_Rect){500, 350, 300, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changeState, &game.stateHandlers[2].state, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsInter[2] = createButton(
        "Swap an ICmon", win, (SDL_Rect){500, 500, 300, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        initSwapTeam, &game.stateHandlers[10].state, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );


    /* Boutons de l'écran d'échange*/
    buttonsSwap[0] = createButton(
        "AdvICMon1", win, (SDL_Rect){20, 20, 160, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changeIndexSwap, (void*)(intptr_t)0, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsSwap[1] = createButton(
        "AdvICMon2", win, (SDL_Rect){240, 20, 160, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changeIndexSwap, (void*)(intptr_t)1, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsSwap[2] = createButton(
        "AdvICMon3", win, (SDL_Rect){460, 20, 160, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changeIndexSwap, (void*)(intptr_t)2, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsSwap[3] = createButton(
        "AdvICMon4", win, (SDL_Rect){680, 20, 160, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changeIndexSwap, (void*)(intptr_t)3, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsSwap[4] = createButton(
        "AdvICMon5", win, (SDL_Rect){900, 20, 160, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changeIndexSwap, (void*)(intptr_t)4, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsSwap[5] = createButton(
        "AdvICMon6", win, (SDL_Rect){1120, 20, 160, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changeIndexSwap, (void*)(intptr_t)5, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsSwap[6] = createButton(
        "PlayerICMon1", win, (SDL_Rect){20, 300, 160, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changeIndexSwap, (void*)(intptr_t)6, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsSwap[7] = createButton(
        "PlayerICMon2", win, (SDL_Rect){240, 300, 160, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changeIndexSwap, (void*)(intptr_t)7, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsSwap[8] = createButton(
        "PlayerICMon3", win, (SDL_Rect){460, 300, 160, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changeIndexSwap, (void*)(intptr_t)8, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsSwap[9] = createButton(
        "PlayerICMon4", win, (SDL_Rect){680, 300, 160, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changeIndexSwap, (void*)(intptr_t)9, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsSwap[10] = createButton(
        "PlayerICMon5", win, (SDL_Rect){900, 300, 160, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changeIndexSwap, (void*)(intptr_t)10, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsSwap[11] = createButton(
        "PlayerICMon6", win, (SDL_Rect){1120, 300, 160, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changeIndexSwap, (void*)(intptr_t)11, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsSwap[12] = createButton(
        "  Back  ", win, (SDL_Rect){180, 600, 300, 100},
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &game.stateHandlers[7].state, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsSwap[13] = createButton(
        "  Validate  ", win, (SDL_Rect){840, 600, 300, 100},
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        validateSwap, &game.stateHandlers[7].state, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsSwap[14] = createButton(
        "  Resume  ", win, (SDL_Rect){510, 600, 300, 100},
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        initResume, &game.stateHandlers[7].state, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );

    /* Boutons Learning Move */
    
    buttonsLearn[0] = createButton(
        "Attack 1", win, (SDL_Rect){startX, startY, buttonWidth, buttonHeight},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        learningMoveChoice, (void*)(intptr_t)0, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsLearn[1] = createButton(
        "Attack 2", win, (SDL_Rect){startX, startY + buttonHeight + spacingY, buttonWidth, buttonHeight},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        learningMoveChoice, (void*)(intptr_t)1, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsLearn[2] = createButton(
        "Attack 3", win, (SDL_Rect){startX + buttonWidth + spacingX, startY, buttonWidth, buttonHeight},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        learningMoveChoice, (void*)(intptr_t)2, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsLearn[3] = createButton(
        "Attack 4", win, (SDL_Rect){startX + buttonWidth + spacingX, startY + buttonHeight + spacingY, buttonWidth, buttonHeight},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        learningMoveChoice, (void*)(intptr_t)3, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsLearn[4] = createButton(
        "Attack 5", win, (SDL_Rect){950, startY, 300, 180},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        learningMoveChoice, (void*)(intptr_t)4, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );

    /*Boutons Starters screen*/

    buttonsStarters[0] = createButton(
        " ", win, (SDL_Rect){280, 600, 200, 100},
        (SDL_Color){0, 0, 0, 0}, (SDL_Color){0, 0, 0, 255},
        selectOtherStarter, (void*)(intptr_t)-1, win->LargeFont,
        "assets/User Interface/Grey/arrow_basic_w.png"
    );
    
    buttonsStarters[1] = createButton(
        "Choisir", win, (SDL_Rect){510, 600, 300, 100},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 0},
        validateStarterChoice, (void*)(intptr_t)0, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsStarters[2] = createButton(
        " ", win, (SDL_Rect){840, 600, 200, 100},
        (SDL_Color){0, 0, 0, 0}, (SDL_Color){0, 0, 0, 0},
        selectOtherStarter, (void*)(intptr_t)1, win->LargeFont,
        "assets/User Interface/Grey/arrow_basic_e.png"
    );

    /*Boutons Resume screen*/
    startX+=60;
    startY-=45;
    buttonsResume[0] = createButton(
        "Attack 1", win, (SDL_Rect){startX, startY, buttonWidth, buttonHeight},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        NULL, (void*)(intptr_t)0, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsResume[1] = createButton(
        "Attack 2", win, (SDL_Rect){startX, startY + buttonHeight + spacingY, buttonWidth, buttonHeight},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        NULL, (void*)(intptr_t)0, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsResume[2] = createButton(
        "Attack 3", win, (SDL_Rect){startX + buttonWidth + spacingX, startY, buttonWidth, buttonHeight},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        NULL, (void*)(intptr_t)0, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsResume[3] = createButton(
        "Attack 4", win, (SDL_Rect){startX + buttonWidth + spacingX, startY + buttonHeight + spacingY, buttonWidth, buttonHeight},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        NULL, (void*)(intptr_t)0, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );

    buttonsResume[4] = createButton(
        "Retour", win, (SDL_Rect){950, startY, 250, 180},
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        destroyResume, (void*)(intptr_t)0, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );

    /* Création du slider */
    sliders[0] = createSlider(win->renderer, 100, 100, 200, 25,
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){255, 0, 0, 255});

    /* Ajout des listes d'éléments */
    addListSlider(game.ui[1].sliders, sliders, nbSlidersSettings);
    addListButton(game.ui[2].buttons, buttonsMenu, nbButtonsMenu);
    addListButton(game.ui[1].buttons, buttonsParam, nbButtonsParam);
    addListButton(game.ui[5].buttons, buttonsLoadGame, nbButtonsLoad);
    addListButton(game.ui[3].buttons, buttonsGame, nbButtonsGame);
    addListButton(game.ui[6].buttons, buttonsICMons, nbButtonsICMons);
    addListButton(game.ui[7].buttons, buttonsInter, nbButtonsInter);
    addListButton(game.ui[10].buttons, buttonsSwap, nbButtonsSwap);
    addListButton(game.ui[11].buttons, buttonsLearn, nbButtonsLearn);
    addListButton(game.ui[12].buttons, buttonsStarters, nbButtonsStarters);
    addListButton(game.ui[13].buttons, buttonsResume, nbButtonsResume);
}