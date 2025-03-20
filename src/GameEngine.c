/**
 * @file GameEngine.c
 * @brief Moteur principal du jeu : gestion de la boucle de jeu, des ressources et des interactions.
 */

#include "../include/GameEngine.h"

/* Variables globales */
Game game;

Text title = { NULL, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, NULL, NULL, NULL, 0 };
Text NewGameText = { NULL, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, NULL, NULL, NULL, 0 };

int marginBottom = 200;  /**< Marge en bas en pixels */
int marginRight = 0;     /**< Marge à droite en pixels */

/**
 * @brief Initialise la première manette détectée.
 *
 * Parcourt les joysticks connectés et ouvre la première manette reconnue.
 *
 * @return SDL_GameController* Pointeur vers la manette ouverte, ou NULL si aucune manette n'est trouvée.
 */
static SDL_GameController* initializeController(void) {
    int nbJoysticks = SDL_NumJoysticks();
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, 
                     "✅ Nombre de manettes connectées : %d", nbJoysticks);

    SDL_GameController *controller = NULL;
    for (int i = 0; i < nbJoysticks; i++) {
        if (SDL_IsGameController(i)) {
            controller = SDL_GameControllerOpen(i);
            if (controller) {
                SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, 
                               "✅ Manette %d : %s", i, SDL_GameControllerName(controller));
                break;
            }
        }
    }
    return controller;
}

/**
 * @brief Gère la synchronisation de la fréquence d'images.
 *
 * Attend si le rendu de la frame s'est terminé trop rapidement afin d'obtenir
 * un FPS cible (ici 60 FPS), calcule le deltaTime et affiche le résultat.
 *
 * @param frameStart Timestamp du début de la frame.
 */
void manageFrameRate(int frameStart) {
    static const int TARGET_FPS = 60;
    static const int FRAME_DELAY = 1000 / TARGET_FPS;

    int frameTime = SDL_GetTicks() - frameStart;
    
    if (frameTime < FRAME_DELAY) {
        SDL_Delay(FRAME_DELAY - frameTime);
        frameTime = FRAME_DELAY; // Pour un calcul précis du deltaTime
    }
    
    game.deltaTime = frameTime / 1000.0f;
    
    printf("\r⚡ Delta: %.2f | FPS: %.2f", game.deltaTime, 1 / game.deltaTime);
    fflush(stdout);
}

/**
 * @brief Libère les ressources allouées par le moteur.
 *
 * Détruit les textes, ferme la manette, et libère la carte, le joueur et la caméra.
 *
 * @param win Pointeur sur la fenêtre (non utilisé ici).
 * @param controller Pointeur sur la manette à fermer.
 */
static void cleanupResources(Window *win, SDL_GameController *controller) {
    (void)win;
    if (NewGameText.texture) {
        destroyText(&NewGameText);
    }
    if (title.texture) {
        destroyText(&title);
    }
    if (controller) {
        SDL_GameControllerClose(controller);
    }
    destroyMap(game.gameData.map);
    destroyPlayer(game.gameData.player);
    destroyCamera(game.gameData.camera);
}

/**
 * @brief Met à jour le texte des boutons ICMons.
 *
 * Pour chaque pokémon de l'équipe passée en paramètre, met à jour le texte du bouton
 * associé dans l'interface.
 *
 * @param win Pointeur sur la fenêtre.
 * @param team Pointeur sur l'équipe (t_Team) dont on met à jour l'affichage.
 */
static void updateICMonsButtonText(Window *win, t_Team *team, int endOfArray, AppState state) {
    for (int i = endOfArray - 6; i < endOfArray; i++) {
        if (team->team[i%6].name[0] != '\0')
            setButtonText(game.ui[state].buttons->buttons[i], team->team[i%6].name, win->renderer);
        else
            setButtonText(game.ui[state].buttons->buttons[i], "  ", win->renderer);
        
    }
}

//---------------------------------------------------------------------------------
/* Fonctions de rendu et de gestion des états visuels */

/**
 * @brief Affiche le contenu de l'écran en fonction de l'état actuel du jeu.
 *
 * Affiche le fond, les boutons, les sliders et les textes associés en fonction
 * de l'état du jeu (GAME, MENU, NEWGAME, etc.).
 *
 * @param win Pointeur sur la fenêtre.
 */
void render(Window *win) {
    if (game.gameState.currentState == GAME) {
        SDL_Rect rect = {0, 0, win->width, win->height};
        SDL_SetRenderDrawColor(win->renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(win->renderer, &rect);
        
        SDL_Rect renderQuad = {0, 0, win->width, (int)(win->height * 0.722)};
        SDL_RenderCopy(win->renderer, game.ui[3].background, NULL, &renderQuad);
        if (strlen(game.battleState.rouge.team[0].name) > 0)
            renderICMonsSprite(win, &(game.battleState.rouge.team[0]));
        if (strlen(game.battleState.bleu.team[0].name) > 0)
            renderICMonsSprite(win, &(game.battleState.bleu.team[0]));
    }
    else if (game.ui[game.gameState.currentState].background) {
        SDL_RenderCopy(win->renderer, game.ui[game.gameState.currentState].background, NULL, NULL);
    }

    if (game.ui[game.gameState.currentState].buttons)
        renderButtonList(game.ui[game.gameState.currentState].buttons);
    if (game.ui[game.gameState.currentState].sliders)
        renderSliderList(game.ui[game.gameState.currentState].sliders);
    if (game.gameState.currentState == MENU)
        renderText(win, &title);
    else if (game.gameState.currentState == NEWGAME)
        renderText(win, &NewGameText);
    else if (game.gameState.currentState == SWAP){
        renderICMonsSprite(win, &(game.battleState.bleu.team[game.swappingIndex[0]]));
        renderICMonsSprite(win, &(game.battleState.rouge.team[game.swappingIndex[1]]));
    }
    else if (game.gameState.currentState == LEARNMOVE){
        renderICMonsSprite(win, lvl_up_buffer[lvl_up_buffer_size-1].target);
    }
}

/**
 * @brief Affiche la carte et le joueur à l'aide de la caméra.
 *
 * La fonction verrouille le mutex de la physique, nettoie le rendu, affiche la carte et le joueur,
 * puis présente le rendu à l'écran.
 *
 * @param win Pointeur sur la fenêtre.
 */
static void renderMap(Window *win) {
    pthread_mutex_lock(&game.threadManager.physicsMutex);
    SDL_RenderClear(win->renderer);
    renderMapWithCamera(game.gameData.map, win->renderer, game.gameData.camera);
    renderPlayerWithCamera(game.gameData.player, win->renderer, game.gameData.camera);
    pthread_mutex_unlock(&game.threadManager.physicsMutex);
    SDL_RenderPresent(game.win->renderer);
}

//---------------------------------------------------------------------------------
/* Fonctions de gestion des boutons et des états */

/**
 * @brief Change l'état actuel du jeu.
 *
 * Met à jour l'état de la fenêtre et de la structure globale du jeu.
 *
 * @param win Pointeur sur la fenêtre.
 * @param data Pointeur vers la nouvelle valeur d'état (AppState).
 */
void changeState(Window *win, void *data) {
    AppState newState = *(AppState *)data;
    win->state = newState;
    game.gameState.currentState = newState;
    game.currentButton = 0;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Changement d'état : %d", newState);
}

/**
 * @brief Passe la fenêtre en mode plein écran.
 *
 * Change le mode d'affichage de la fenêtre en plein écran (mode desktop).
 *
 * @param win Pointeur sur la fenêtre.
 * @param data Paramètre non utilisé.
 */
void makeWindowFullScreen(Window *win, void *data) {
    (void)data;
    SDL_SetWindowFullscreen(win->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_GetWindowSize(win->window, &win->width, &win->height);
}

/**
 * @brief Repasse la fenêtre en mode fenêtré.
 *
 * Si la fenêtre est actuellement en plein écran, la repasse en mode fenêtré et ajuste sa taille.
 *
 * @param win Pointeur sur la fenêtre.
 * @param data Paramètre non utilisé.
 */
void makeWindowWindowed(Window *win, void *data) {
    (void)data;
    if (!(SDL_GetWindowFlags(win->window) & SDL_WINDOW_FULLSCREEN_DESKTOP))
        return;
    SDL_SetWindowFullscreen(win->window, 0);
    SDL_SetWindowSize(win->window, win->InitialWidth, win->InitialHeight);
    handleWindowSizeChange(win);
}

/**
 * @brief Gère l'appui sur un bouton d'attaque.
 *
 * Valide l'indice du mouvement et, si possible, exécute le tour du joueur ou effectue un échange
 * dans l'équipe adverse.
 *
 * @param win Pointeur sur la fenêtre.
 * @param data Pointeur vers l'indice du mouvement (int casté en void*).
 */
void attqButtonClicked(Window *win, void *data) {
    (void)win; // Pour éviter le warning
    
    if (game.gameState.playerTurn && isTeamAlive(&game.battleState.rouge) && isTeamAlive(&game.battleState.bleu)) {
        int moveIndex = (int)(intptr_t)data;
        if (moveIndex < 0 || moveIndex >= game.battleState.rouge.team[0].nb_move) {
            return;
        }
        if (isAlive(&(game.battleState.rouge.team[0])) && game.battleState.turnState == TURN_NONE) {
            Mix_PlayChannel(2, game.battleState.rouge.team[0].img->ICMonSound, 0);
            startBattleTurn(moveIndex, AI_move_choice(&game.battleState.ia, &game.battleState.rouge));
        }
        game.gameState.playerTurn = 0;
        

    }
}

void changeTextSpeed(Window *win, void *data) {
    (void)win;
    game.speed = (int)(intptr_t)data;
}

/**
 * @brief Change le pokémon actif lors d'un échange.
 *
 * Exécute un tour d'attaque ou échange l'attaquant actif selon la validité de l'action,
 * puis met à jour l'affichage des boutons.
 *
 * @param win Pointeur sur la fenêtre.
 * @param data Pointeur vers l'indice (int casté en void*) du pokémon à activer.
 */
void changePokemon(Window *win, void *data) {
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

void initSwapTeam(Window *win, void *data) {
    game.swappingIndex[0]=0;
    game.swappingIndex[1]=game.battleState.rouge.nb_poke%6;
    updateICMonsButtonText(win, &game.battleState.bleu, 6 ,10);
    updateICMonsButtonText(win, &game.battleState.rouge, 12 ,10);

    changeState(win,data);
}

void changeIndexSwap(Window *win, void *data) {
    (void)win;
    int value=(int)(intptr_t)data;
    int teamIndex=value>=6?1:0;
    if(value-6>game.battleState.rouge.nb_poke) value=game.battleState.rouge.nb_poke;
    game.swappingIndex[teamIndex]=value%6;
}

void validateSwap(Window *win, void *data) {
    if(game.swappingIndex[0]<game.battleState.bleu.nb_poke){
        if(game.swappingIndex[1]<game.battleState.rouge.nb_poke){
            destroyICMonsSprite(&game.battleState.rouge.team[game.swappingIndex[1]]);
        }
        destroyICMonsSprite(&game.battleState.bleu.team[game.swappingIndex[0]]);
        getPokeFromTeam(&game.battleState.rouge,game.swappingIndex[1],&game.battleState.bleu,game.swappingIndex[0]);
        initTeamSprites(win, &game.battleState.rouge,RED_SPRITE_X_RATIO, RED_SPRITE_Y_RATIO, 0);
        updateICButtons(win, &game.battleState.rouge);
        updateICMonsButtonText(win, &game.battleState.rouge, 6, 6);
        changeState(win,data);
    }
}

void learningMoveChoice(Window *win, void *data){
    int value=(int)(intptr_t)data;
    if(value!=4){
        lvl_up_buffer[lvl_up_buffer_size-1].target->moveList[value]=generateMove(lvl_up_buffer[lvl_up_buffer_size-1].moveId);
    }
    lvl_up_buffer_size--;
    changeState(win,&game.stateHandlers[3].state);
}

/**
 * @brief Passe au duel suivant.
 *
 * Sauvegarde l'état de la partie, soigne l'équipe du joueur, réinitialise l'équipe adverse,
 * met à jour l'affichage et change l'état du jeu.
 *
 * @param win Pointeur sur la fenêtre.
 * @param data Paramètre non utilisé.
 */
void nextDuel(Window *win, void *data) {
    (void)win;
    (void)data;

    // Arrêter tous les sons avant de sauvegarder
    Mix_HaltChannel(-1);
    Mix_HaltMusic();

    // Sauvegarder l'état
    sauvegarder("Save_1", &game.battleState.rouge, &game.battleState.bleu);
    
    // Soigner et réinitialiser
    healTeam(&game.battleState.rouge);
    game.battleState.rouge.nb_enemiBeat++;
    initBlueTeam(&game.battleState.bleu, &game.battleState.rouge);
    
    // Réinitialiser l'IA
    game.battleState.ia = (t_AI){10, damageOnly, &game.battleState.bleu};
    
    // Initialiser les sprites avec vérification d'erreur
    if (initTeamSprites(win, &game.battleState.bleu, BLUE_SPRITE_X_RATIO, BLUE_SPRITE_Y_RATIO, 1) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur lors de l'initialisation des sprites");
        return;
    }
    
    // Mettre à jour l'interface
    updateICButtons(win, &game.battleState.rouge);
    
    // Changer d'état
    changeState(win, &game.stateHandlers[3].state);
}

/**
 * @brief Boucle principale du jeu.
 *
 * Initialise le jeu, les boutons, le contrôleur, le gestionnaire de threads,
 * puis exécute la boucle principale gérant les événements, le rendu et le frame rate.
 * En fin de boucle, libère les ressources.
 *
 * @param win Pointeur sur la fenêtre.
 */
void mainLoop(Window *win) {
    initGame(win);
    initAllButtons(win);
    
    SDL_GameController *controller = initializeController();
    
    initThreadManager(&game);
    
    SDL_Event event;
    Uint32 frameStart;
    
    while (!win->quit) {
        frameStart = SDL_GetTicks();
        
        while (SDL_PollEvent(&event)) {
            game.stateHandlers[win->state].handleEvent(win, &event);
        }
        
        if (game.gameState.currentState == MAP) {
            
            renderMap(win);
            
        } else {
            SDL_RenderClear(win->renderer);
            render(win);
            
            updateCurrentButton();
            if (game.battleState.turnState != TURN_NONE) updateBattleTurn();
            SDL_RenderPresent(win->renderer);
        }
        game.deltaTime = (SDL_GetTicks() - frameStart) / 1000.0f;
        
        manageFrameRate(frameStart);
    }
    
    cleanupThreads(&game);
    cleanupResources(win, controller);
}

//---------------------------------------------------------------------------------
/* Fonctions de gestion et d'initialisation des boutons et sliders */

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
    int nbButtonsSwap = 14;
    int nbButtonsLearn = 5;

    Button *buttonsMenu[nbButtonsMenu];
    Button *buttonsParam[nbButtonsParam];
    Button *buttonsLoadGame[nbButtonsLoad];
    Button *buttonsGame[nbButtonsGame];
    Button *buttonsICMons[nbButtonsICMons];
    Button *buttonsInter[nbButtonsInter];
    Button *buttonsSwap[nbButtonsSwap];
    Button *buttonsLearn[nbButtonsLearn];
    Slider *sliders[nbSlidersSettings];

    /* Boutons du menu */
    buttonsMenu[0] = createButton(
        " PLAY ", win, (SDL_Rect){500, 150, 300, 100},
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &game.stateHandlers[NEWGAME].state, win->MediumFont,
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
        changeTextSpeed, (void*)(intptr_t)25, win->LargeFont,
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
        changeTextSpeed, (void*)(intptr_t)100, win->LargeFont,
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
        changeState, &game.stateHandlers[4].state, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsLoadGame[1] = createButton(
        " Save 2 ", win, (SDL_Rect){500, 258, 300, 100},
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &game.stateHandlers[4].state, win->LargeFont,
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
        changeState, &game.stateHandlers[MAP].state, win->LargeFont,
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
        "  Back  ", win, (SDL_Rect){100, 600, 300, 100},
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &game.stateHandlers[7].state, win->LargeFont,
        "assets/User Interface/Grey/button_rectangle_depth_gloss.png"
    );
    buttonsSwap[13] = createButton(
        "  Validate  ", win, (SDL_Rect){1000, 600, 300, 100},
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        validateSwap, &game.stateHandlers[7].state, win->LargeFont,
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
}

/**
 * @brief Met à jour les textes des boutons d'attaque et des ICMons.
 *
 * Pour les quatre premiers boutons du jeu, met à jour le texte affiché en fonction des mouvements
 * disponibles du premier pokémon de l'équipe rouge. Si l'état actuel est NEWGAME ou LOADGAME,
 * la fonction met également à jour les textes des boutons ICMons.
 *
 * @param win Pointeur sur la fenêtre.
 * @param team Pointeur sur l'équipe (t_Team) à mettre à jour.
 */
void updateICButtons(Window *win, t_Team *team) {
    char temp[100];
    for (int i = 0; i < 4; i++) {
        if (team->team[0].nb_move > i) {
            snprintf(temp, sizeof(temp), "%s  %d/%d", 
                     team->team[0].moveList[i].name,
                     team->team[0].moveList[i].current_pp,
                     team->team[0].moveList[i].max_pp);
            setButtonText(game.ui[3].buttons->buttons[i], temp, win->renderer);
        } else {
            setButtonText(game.ui[3].buttons->buttons[i], "  ", win->renderer);
        }
    }
    if (game.gameState.currentState == NEWGAME || game.gameState.currentState == LOADGAME || game.gameState.currentState == GAME)
        updateICMonsButtonText(win, &game.battleState.rouge, 6, 6);
}

/**
 * @brief Met à jour l'apparence du bouton actuellement sélectionné.
 *
 * Parcourt la liste des boutons de l'état courant et met à jour leur texture pour
 * indiquer lequel est sélectionné.
 */
void updateCurrentButton() {
    if (game.ui[game.gameState.currentState].buttons->size > 0) {
        for (int i = 0; i < game.ui[game.gameState.currentState].buttons->size; i++) {
            if (i != game.currentButton)
                game.ui[game.gameState.currentState].buttons->buttons[i]->texture =
                    game.ui[game.gameState.currentState].buttons->buttons[i]->initialTexture;
            else
                game.ui[game.gameState.currentState].buttons->buttons[i]->texture =
                    game.ui[game.gameState.currentState].buttons->buttons[i]->selectedTexture;
        }
    }
}

/**
 * @brief Met à jour la musique en fonction de l'état du jeu.
 *
 * Si l'état du jeu est GAME ou ICMONS et qu'aucune musique n'est en lecture, démarre la musique.
 * Dans le cas contraire, arrête la lecture.
 */


Mix_Music *currentMusic = NULL;

void updateMusic() {
    Mix_Music *desiredMusic = NULL;

    // Déterminer la musique souhaitée selon l'état du jeu.
    switch (game.gameState.currentState) {
        case GAME:
        case ICMONS:
            desiredMusic = game.gameState.music;
            break;
        case MAP:
        case INTER:
            desiredMusic = game.gameState.music_inter;
            break;
        default:
            desiredMusic = NULL;
            break;
    }

    // Si aucune musique n'est souhaitée, arrêter la lecture.
    if (!desiredMusic) {
        Mix_HaltMusic();
        currentMusic = NULL;
        return;
    }

    // Si aucune musique n'est en cours ou si la musique en cours diffère de celle souhaitée, mettre à jour.
    if (!Mix_PlayingMusic() || currentMusic != desiredMusic) {
        Mix_HaltMusic();
        currentMusic = desiredMusic;
        Mix_PlayMusic(desiredMusic, -1);
    }
}

void startBattleTurn(int moveRouge, int moveBleu) {
    game.battleState.moveRouge = moveRouge;
    game.battleState.moveBleu  = moveBleu;
    
    if(!hasMoveLeft(&(game.battleState.rouge.team[0])) && isAttacking(moveRouge)) moveRouge=STRUGGLE;
    if(!hasMoveLeft(&(game.battleState.bleu.team[0])) && isAttacking(moveBleu)) moveBleu=STRUGGLE;

    game.battleState.first = PriorityForFirstPoke(&game.battleState.rouge, &game.battleState.bleu, moveRouge==STRUGGLE?&struggle:&(game.battleState.rouge.team[0].moveList[moveRouge]), moveBleu==STRUGGLE?&struggle:&(game.battleState.bleu.team[0].moveList[moveBleu]), moveRouge, moveBleu);
    // Si un texte défilant existait déjà, on le détruit pour repartir de zéro.
    if (game.battleState.text) {
        cleanupScrollingText(&game.battleState.text);
    }
    game.battleState.turnState = TURN_INIT;
}

void updateBattleTurn() {
    if(lvl_up_buffer_size!=0) {
        char temp[100];
        for(int i=0; i<4; i++){
            if (lvl_up_buffer[lvl_up_buffer_size-1].target->nb_move > i) {
                snprintf(temp, sizeof(temp), "%s  %d/%d",
                    lvl_up_buffer[lvl_up_buffer_size-1].target->moveList[i].name,
                    lvl_up_buffer[lvl_up_buffer_size-1].target->moveList[i].current_pp,
                    lvl_up_buffer[lvl_up_buffer_size-1].target->moveList[i].max_pp);
                setButtonText(game.ui[LEARNMOVE].buttons->buttons[i], temp, game.win->renderer);
            }
            else setButtonText(game.ui[LEARNMOVE].buttons->buttons[i], "  ", game.win->renderer);
        }
        t_Move tempMove=generateMove(lvl_up_buffer[lvl_up_buffer_size-1].moveId);
        snprintf(temp, sizeof(temp), "%s  %d/%d",
            tempMove.name,
            tempMove.current_pp,
            tempMove.max_pp);
        setButtonText(game.ui[LEARNMOVE].buttons->buttons[4], temp, game.win->renderer);
        printf("buffer : %d\n",lvl_up_buffer_size);
        changeState(game.win,&game.stateHandlers[11].state);
        game.battleState.turnState=TURN_NONE;
        return;
    }
    switch (game.battleState.turnState) {
        case TURN_INIT: {
            // Création et affichage d'un texte annonçant le début du tour
            if (!game.battleState.text) {
                char *msg ="Début du tour de combat";
                if (!msg) return;
                game.battleState.text = createScrollingText(
                    msg,
                    game.win->LargeFont,
                    (SDL_Color){255, 255, 255, 255},
                    game.speed,     // Délai entre les caractères en ms
                    (SDL_Rect){game.win->width * 0.4531, game.win->height *0.5
                    , game.win->width * 0.5, game.win->height * 0.2}, // Fond du texte
                    "assets/User Interface/Grey/button_rectangle_depth_flat.png", // Chemin de l'image de fond
                    game.win->renderer
                );
            }
            if (game.battleState.text->isComplete) {
                // Une fois le texte affiché, passez à l'action du premier pokémon
                game.battleState.text->isComplete = false;
                game.battleState.turnState = TURN_ACTION1;
                game.battleState.hasAttacked = false;
            }
            
            break;
        }
        case TURN_ACTION1: {
            // Exécuter l'action du pokémon rouge
            if (game.battleState.hasAttacked == false){
                // La fonction executeAction reprend la logique de playATurn pour une action
                if(game.battleState.first) {
                    if (isTeamAlive(&game.battleState.rouge) && isAlive(&(game.battleState.rouge.team[0])))
                        executeAction(&game.battleState.rouge, &game.battleState.bleu, game.battleState.moveRouge);
                }
                else {
                    if (isTeamAlive(&game.battleState.bleu) && !isAlive(&(game.battleState.bleu.team[0]))) {
                        int nb_valide = 0;
                        int liste_valide[game.battleState.bleu.nb_poke];
                        for (int i = 0; i < game.battleState.bleu.nb_poke; i++) {
                            if (isAlive(&game.battleState.bleu.team[i]))
                                liste_valide[nb_valide++] = i + 10;
                        }
                        int x = rand() % nb_valide;
                        swapActualAttacker(&game.battleState.bleu, liste_valide[x]);
                    } else
                        executeAction(&game.battleState.bleu, &game.battleState.rouge, game.battleState.moveBleu);
                }
                game.battleState.hasAttacked = true;
            }
            if (game.battleState.text->isComplete) {
                game.battleState.text->isComplete = false;
                game.battleState.turnState = TURN_ACTION2;
                game.battleState.hasAttacked = false;
            }
            break;
        }
        case TURN_ACTION2: {
            // Si le pokémon bleu est toujours vivant, exécute son action
            if (game.battleState.hasAttacked == false) {
                if(game.battleState.first) {
                    if (isTeamAlive(&game.battleState.bleu) && !isAlive(&(game.battleState.bleu.team[0]))) {
                        int nb_valide = 0;
                        int liste_valide[game.battleState.bleu.nb_poke];
                        for (int i = 0; i < game.battleState.bleu.nb_poke; i++) {
                            if (isAlive(&game.battleState.bleu.team[i]))
                                liste_valide[nb_valide++] = i + 10;
                        }
                        int x = rand() % nb_valide;
                        swapActualAttacker(&game.battleState.bleu, liste_valide[x]);
                    }
                    else
                        executeAction(&game.battleState.bleu, &game.battleState.rouge, game.battleState.moveBleu);
                }
                else {
                    if (isTeamAlive(&game.battleState.rouge) && isAlive(&(game.battleState.rouge.team[0])))
                        executeAction(&game.battleState.rouge, &game.battleState.bleu, game.battleState.moveRouge);
                }
                game.battleState.hasAttacked = true;
            }
            if (game.battleState.text->isComplete) {
                game.battleState.text->isComplete = false;
                game.battleState.turnState = TURN_FINISHED;
            }
            break;
        }
        case TURN_FINISHED: {
            // Le tour est terminé, vous pouvez ici déclencher des mises à jour d'interface ou des vérifications de fin de combat.
            updateICButtons(game.win, &game.battleState.rouge);
            if(!isAlive(&game.battleState.rouge.team[0]) && isTeamAlive(&game.battleState.rouge)) changeState(game.win, &game.stateHandlers[6].state);
            if (!isAlive(&(game.battleState.bleu.team[0]))) gainExp(&game.battleState.rouge, &game.battleState.bleu.team[0]);
            if (isTeamAlive(&game.battleState.bleu) && !isAlive(&(game.battleState.bleu.team[0]))) {
                int nb_valide = 0;
                int liste_valide[game.battleState.bleu.nb_poke];
                for (int i = 0; i < game.battleState.bleu.nb_poke; i++) {
                    if (isAlive(&game.battleState.bleu.team[i]))
                        liste_valide[nb_valide++] = i + 10;
                }
                int x = rand() % nb_valide;
                swapActualAttacker(&game.battleState.bleu, liste_valide[x]);
            }
            if(!isAlive(&game.battleState.bleu.team[0]) &&  !isTeamAlive(&game.battleState.bleu)) changeState(game.win, &game.stateHandlers[7].state);
            destroyScrollingText(game.battleState.text);
            game.battleState.text = NULL;
            game.battleState.turnState = TURN_NONE;
            
            break;
        }
        case TURN_NONE:
        default:
            // Aucun tour en cours
            break;
    }
    updateScrollingText(game.battleState.text, game.win->renderer);
    if (game.gameState.currentState == GAME) {
        renderScrollingText(game.battleState.text, game.win->renderer);
    }
}

void executeAction(t_Team *attacker, t_Team *defender, int move) {
    char msg[50] = "";
    // Si le Pokémon n'a plus de PP et qu'il attaque, forcer l'utilisation de Lutte.
    if (!hasMoveLeft(&(attacker->team[0])) && isAttacking(move)) {
        printf("Lutte\n");
        move = STRUGGLE;
        sprintf(msg, "%s utilise Lutte !", attacker->team[0].name);
        resetScrollingText(game.battleState.text, msg);
        // Vous pouvez appliquer les dégâts de Lutte ici ou dans une étape ultérieure
        affectDamage(attacker, defender, move);
        return;
    }

    // Vérification de la validité de l'action
    if (!testActionValidity(move, attacker)) {
        sprintf(msg, "Action invalide !");
        resetScrollingText(game.battleState.text, msg);
        game.battleState.turnState = TURN_NONE;
        cleanupScrollingText(&game.battleState.text);
        return;
    }

    // Si l'action est une attaque
    if (isAttacking(move)) {
        // Gestion de la confusion
        if (attacker->effect == confusion) {
            // Si le Pokémon est confus et utilise Lutte, ne pas afficher "Lutte" mais "se blesse dans sa confusion"
            if (move == STRUGGLE) {
                sprintf(msg, "Il se blesse dans sa confusion !");
                resetScrollingText(game.battleState.text, msg);
                affectDamage(attacker, attacker, CONFUSED_MOVE);
                return;
            }
            // Sinon, possibilité de sortir de la confusion ou de se blesser
            if (rand() % 100 < 20) {
                sprintf(msg, "%s se remet de sa confusion !", attacker->team[0].name);
                resetScrollingText(game.battleState.text, msg);
                attacker->effect = noEffect;
            }
            else if (rand() % 100 < 33) {
                sprintf(msg, "Il se blesse dans sa confusion !");
                resetScrollingText(game.battleState.text, msg);
                affectDamage(attacker, attacker, CONFUSED_MOVE);
                return;
            }
        }
        // Afficher le message d'attaque utilisé
        if (move == STRUGGLE) {
            sprintf(msg, "%s utilise Lutte !", attacker->team[0].name);
        } else {
            sprintf(msg, "%s utilise %s !", attacker->team[0].name, attacker->team[0].moveList[move].name);
        }
        // Appliquer l'action (dégâts ou autre effet)
        if (!affectDamage(attacker, defender, move)) {
            sprintf(msg, "%s rate son attaque !", attacker->team[0].name);   
        }
        resetScrollingText(game.battleState.text, msg);
    }
    // Si l'action est un échange
    else if (isSwitching(move)) {
        swapActualAttacker(attacker, move);
        sprintf(msg, "%s change de pokémon !", attacker->team[0].name);
        resetScrollingText(game.battleState.text, msg);
    }
}
