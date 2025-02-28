#include "../include/GameEngine.h"
// Global variables
Game game;

Text title = {NULL,{0,0,0,0},{0,0,0,0}, {0,0,0,0}, NULL, NULL, NULL,0};
Text NewGameText = {NULL,{0,0,0,0},{0,0,0,0}, {0,0,0,0}, NULL, NULL, NULL,0};

int marginBottom = 200; // Marge en bas en pixels
int marginRight = 0;  // Marge à droite en pixels

// Helper function to initialize controller
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
            }
        }
    }
    return controller;
}

// Helper function to manage frame rate
void manageFrameRate(int frameStart) {
    // Définir le FPS cible (60 FPS)
    static const int TARGET_FPS = 60;
    static const int FRAME_DELAY = 1000 / TARGET_FPS;

    int frameTime = SDL_GetTicks() - frameStart;
    
    // Si le frame a été rendu plus rapidement que nécessaire, on attend
    if (frameTime < FRAME_DELAY) {
        SDL_Delay(FRAME_DELAY - frameTime);
        frameTime = FRAME_DELAY; // Pour un calcul précis du deltaTime
    }
    
    game.deltaTime = frameTime / 1000.0f;
    
    printf("\r⚡ Delta: %.2f | FPS: %.2f", game.deltaTime, 1/game.deltaTime);
    fflush(stdout);
}


// Helper function to cleanup resources
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

// Helper function to update the ICMons button text
static void updateICMonsButtonText(Window *win, t_Team *team) {
    for (int i  = 0; i < 6; i++) {
        if (team->team[i].name[0] != '\0') setButtonText(game.ui[6].buttons->buttons[i], team->team[i].name, win->renderer);
        else setButtonText(game.ui[6].buttons->buttons[i], "  ", win->renderer);
    }
}

//---------------------------------------------------------------------------------

// Functions for the menu

void render(Window *win) {
    if (game.gameState.currentState == GAME) {
        SDL_Rect rect = {0, 0, win->width, win->height};
        SDL_SetRenderDrawColor(win->renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(win->renderer, &rect);
        
        SDL_Rect renderQuad = {0, 0, win->width, (int)(win->height * 0.722)};
        SDL_RenderCopy(win->renderer, game.ui[3].background, NULL, &renderQuad);
        if (game.battleState.rouge.team[0].name[0] != ' ') renderICMonsSprite(win, &(game.battleState.rouge.team[0]));
        if (game.battleState.bleu.team[0].name[0] != ' ') renderICMonsSprite(win, &(game.battleState.bleu.team[0]));
    }
    else if (game.ui[game.gameState.currentState].background) {
        SDL_RenderCopy(win->renderer, game.ui[game.gameState.currentState].background, NULL, NULL);
    }

    if (game.ui[game.gameState.currentState].buttons) renderButtonList(game.ui[game.gameState.currentState].buttons);
    if (game.ui[game.gameState.currentState].sliders) renderSliderList(game.ui[game.gameState.currentState].sliders);
    if (game.gameState.currentState == MENU) {
        renderText(win, &title);
    } else if (game.gameState.currentState == NEWGAME) {
        renderText(win, &NewGameText);
    }
}

static void renderMap(Window *win) {
    pthread_mutex_lock(&game.threadManager.physicsMutex);
    SDL_RenderClear(win->renderer);
    renderMapWithCamera(game.gameData.map, win->renderer, game.gameData.camera);
    renderPlayerWithCamera(game.gameData.player, win->renderer, game.gameData.camera);
    pthread_mutex_unlock(&game.threadManager.physicsMutex);
    SDL_RenderPresent(game.win->renderer);
}

//---------------------------------------------------------------------------------

// Functions to handles buttons 

void changeState(Window *win, void *data) {
    AppState newState = *(AppState *)data;
    win->state = newState;
    game.gameState.currentState = newState;
    game.currentButton = 0;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Changement d'état : %d", newState);
}

void makeWindowFullScreen(Window *win, void *data) {
    (void)data;
    SDL_SetWindowFullscreen(win->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_GetWindowSize(win->window, &win->width, &win->height);
}
void makeWindowWindowed(Window *win, void *data) {
    (void)data;
    if (!(SDL_GetWindowFlags(win->window) & SDL_WINDOW_FULLSCREEN_DESKTOP)) {
        return;
    }
    SDL_SetWindowFullscreen(win->window, 0);
    SDL_SetWindowSize(win->window, win->InitialWidth, win->InitialHeight);
    handleWindowSizeChange(win);
}

void attqButtonClicked(Window *win, void *data) {
    (void)win; // Pour éviter le warning
    if (game.gameState.playerTurn && isTeamAlive(&game.battleState.rouge) && isTeamAlive(&game.battleState.bleu)) {
        int moveIndex = (int)(intptr_t)data;
        if (moveIndex < 0 || moveIndex >= game.battleState.rouge.team[0].nb_move) {
            SDL_Log("Indice de mouvement invalide : %d", moveIndex);
            return;
        }
        
        if (isAlive(&(game.battleState.rouge.team[0]))) {
            playATurn(&game.battleState.rouge, moveIndex, &game.battleState.bleu, AI_move_choice(&game.battleState.ia, &game.battleState.rouge));
        }
        if (!isAlive(&(game.battleState.bleu.team[0]))) {
            gainExp(&(game.battleState.rouge), &(game.battleState.bleu.team[0]));
        }
        if (isTeamAlive(&game.battleState.bleu) && !isAlive(&(game.battleState.bleu.team[0]))){
            int nb_valide=0;
            int liste_valide[game.battleState.bleu.nb_poke];
            for(int i=0;i<game.battleState.bleu.nb_poke;i++) if(isAlive(&(game.battleState.bleu.team[i]))) liste_valide[nb_valide++]=i+10;
            for(int i=0;i<nb_valide;i++) printf("%d ",liste_valide[i]);
            printf("\n");
            int x = rand()%nb_valide;
            printf("x=%d\n",x);
            swapActualAttacker(&game.battleState.bleu, liste_valide[x]);
            printPoke(&game.battleState.bleu.team[0]);
        }
        updateICButtons(win, &game.battleState.rouge);
        game.gameState.playerTurn = 0;
    }
}

void changePokemon(Window *win, void *data) {
    (void)win; 
    int idx = (int)(intptr_t)data; 
    if (testActionValidity(idx, &game.battleState.rouge)) {
        if (isAlive(&(game.battleState.rouge.team[idx-10]))) {
            playATurn(&game.battleState.rouge, idx, &game.battleState.bleu, AI_move_choice(&(game.battleState.ia), &game.battleState.rouge));
        } else {
            swapActualAttacker(&game.battleState.rouge, idx);
        }
        changeState(win, &game.stateHandlers[4].state);
    } else {
        SDL_Log("Action invalide : %d", idx);
    }
    updateICButtons(win, &game.battleState.rouge);
    updateICMonsButtonText(win, &game.battleState.rouge);
}

void nextDuel(Window *win, void *data) {
    (void)win; 
    (void)data;
    // Sauvegarde de la partie TODO
    sauvegarder("Save_1",&game.battleState.rouge,&game.battleState.bleu);
    healTeam(&game.battleState.rouge);
    initTeam(&game.battleState.bleu, 3);
    updateICButtons(win, &game.battleState.rouge);
    changeState(win, &game.stateHandlers[3].state);
}

void mainLoop(Window *win) {
    // Initialize game components
    initGame(win);
    initAllButtons(win);
    
    // Controller setup
    SDL_GameController *controller = initializeController();
    
    // Initialize thread manager AFTER all resources are loaded
    initThreadManager(&game);
    
    // Main game variables
    SDL_Event event;
    Uint32 frameStart;
    
    // Main game loop
    while (!win->quit) {
        frameStart = SDL_GetTicks();
        
        // Handle events
        while (SDL_PollEvent(&event)) {
            game.stateHandlers[win->state].handleEvent(win, &event);
        }
        if (game.gameState.currentState == MAP) {
            renderMap(win);
        }else{
            SDL_RenderClear(win->renderer);
            render(win);
            updateCurrentButton();
            SDL_RenderPresent(win->renderer);
        }
        
        // Update deltaTime
        game.deltaTime = (SDL_GetTicks() - frameStart) / 1000.0f;
        
        manageFrameRate(frameStart);
    }
    
    // Cleanup
    cleanupThreads(&game);
    cleanupResources(win, controller);
}

//---------------------------------------------------------------------------------

// Functions for the buttons

void initAllButtons(Window *win)
{
    int nbButtonsLoad = 3;
    int nbButtonsMenu = 4;
    int nbSlidersSettings = 1;
    int nbButtonsParam = 6;
    int nbButtonsGame = 5;
    int nbButtonsICMons = 7;
    int nbButtonsInter = 2;

    Button *buttonsMenu[4];
    Button *buttonsParam[6];
    Button *buttonsLoadGame[3];
    Button *buttonsGame[5];
    Button *buttonsICMons[7];
    Button *buttonsInter[2];
    Slider *sliders[1];

    // Initialisation des boutons du menu
    buttonsMenu[0] = createButton(
        "PLAY", win, 500, 150, 300, 100,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &game.stateHandlers[4].state, win->MediumFont
    );
    buttonsMenu[1] = createButton(
        "LOAD GAME", win, 500, 300, 300, 100,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &game.stateHandlers[5].state, win->MediumFont
    );
    buttonsMenu[2] = createButton(
        "SETTINGS", win, 500, 450, 300, 100,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &game.stateHandlers[1].state, win->MediumFont
    );
    buttonsMenu[3] = createButton(
        "QUIT", win, 500, 600, 300, 100,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &game.stateHandlers[0].state, win->MediumFont
    );

    // Initialisation des boutons de paramètres
    buttonsParam[0] = createButton(
        "0.5", win, 100, 200, 200, 50,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeTextSpeed, &game.speeds[0], win->LargeFont
    );
    buttonsParam[1] = createButton(
        "1", win, 400, 200, 200, 50,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeTextSpeed, &game.speeds[1], win->LargeFont
    );
    buttonsParam[2] = createButton(
        "1.5", win, 700, 200, 200, 50,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeTextSpeed, &game.speeds[2], win->LargeFont
    );
    buttonsParam[3] = createButton(
        "Fullscreen", win, 100, 300, 220, 75,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        makeWindowFullScreen, NULL, win->LargeFont
    );
    buttonsParam[4] = createButton(
        "Windowed", win, 400, 300, 220, 75,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        makeWindowWindowed, NULL, win->LargeFont
    );
    buttonsParam[5] = createButton(
        "Back", win, 100, 600, 300, 100,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &game.stateHandlers[2].state, win->LargeFont
    );

    // Initialisation des boutons de Load Game
    buttonsLoadGame[0] = createButton(
        "Save 1", win, 500, 104, 300, 100,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &game.stateHandlers[4].state, win->LargeFont
    );
    buttonsLoadGame[1] = createButton(
        "Save 2", win, 500, 258, 300, 100,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &game.stateHandlers[4].state, win->LargeFont
    );
    buttonsLoadGame[2] = createButton(
        "Back", win, 100, 600, 300, 100,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &game.stateHandlers[2].state, win->LargeFont
    );

    // Initialisation des boutons de jeu
    int buttonWidth = 430;
    int buttonHeight = 88;
    int spacingX = 5;
    int spacingY = 7;
    int startX = 20;
    int startY = 532;
    buttonsGame[0] = createButton(
        "Attack 1", win, startX, startY, buttonWidth, buttonHeight,
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        attqButtonClicked, (void*)(intptr_t)0, win->LargeFont
    );
    buttonsGame[1] = createButton(
        "Attack 2", win, startX, startY + buttonHeight + spacingY, buttonWidth, buttonHeight,
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        attqButtonClicked, (void*)(intptr_t)1, win->LargeFont
    );
    buttonsGame[2] = createButton(
        "Attack 3", win, startX + buttonWidth + spacingX, startY, buttonWidth, buttonHeight,
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        attqButtonClicked, (void*)(intptr_t)2, win->LargeFont
    );
    buttonsGame[3] = createButton(
        "Attack 4", win, startX + buttonWidth + spacingX, startY + buttonHeight + spacingY, buttonWidth, buttonHeight,
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        attqButtonClicked, (void*)(intptr_t)3, win->LargeFont
    );
    buttonsGame[4] = createButton(
        "ICMons", win, 950, startY, 300, 180,
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changeState, &game.stateHandlers[6].state, win->LargeFont
    );

    // Initialisation des boutons ICMons
    buttonsICMons[0] = createButton(
        "ICMon1", win, 20, 20, 160, 100,
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        NULL, (void*)(intptr_t)0, win->LargeFont
    );
    buttonsICMons[1] = createButton(
        "ICMon2", win, 240, 20, 160, 100,
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changePokemon, (void*)(intptr_t)11, win->LargeFont
    );
    buttonsICMons[2] = createButton(
        "ICMon3", win, 460, 20, 160, 100,
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changePokemon, (void*)(intptr_t)12, win->LargeFont
    );
    buttonsICMons[3] = createButton(
        "ICMon4", win, 680, 20, 160, 100,
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changePokemon, (void*)(intptr_t)13, win->LargeFont
    );
    buttonsICMons[4] = createButton(
        "ICMon5", win, 900, 20, 160, 100,
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changePokemon, (void*)(intptr_t)14, win->LargeFont
    );
    buttonsICMons[5] = createButton(
        "ICMon6", win, 1120, 20, 160, 100,
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changePokemon, (void*)(intptr_t)15, win->LargeFont
    );
    buttonsICMons[6] = createButton(
        "Back", win, 100, 600, 300, 100,
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changeState, &game.stateHandlers[3].state, win->LargeFont
    );

    // Initialisation des boutons d'interaction
    buttonsInter[0] = createButton(
        "Next Duel", win, 500, 200, 300, 100,
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        nextDuel, (void*)(intptr_t)0, win->LargeFont
    );
    buttonsInter[1] = createButton(
        "Back", win, 500, 350, 300, 100,
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){0, 0, 0, 255},
        changeState, &game.stateHandlers[2].state, win->LargeFont
    );

    // Application des textures (boutons menu, paramètres, etc.)
    for (int i = 0; i < 4; i++) {
        InitTextureButton(buttonsMenu[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
        InitTextureButton(buttonsParam[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
        if (i != 3)
            InitTextureButton(buttonsLoadGame[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
        InitTextureButton(buttonsGame[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
        InitTextureButton(buttonsICMons[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
        if (i < 2)
            InitTextureButton(buttonsInter[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
    }
    InitTextureButton(buttonsParam[4], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
    InitTextureButton(buttonsParam[5], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
    InitTextureButton(buttonsICMons[4], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
    InitTextureButton(buttonsICMons[5], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
    InitTextureButton(buttonsICMons[6], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
    InitTextureButton(buttonsGame[4], win->renderer, "assets/User Interface/Blue/button_square_depth_gloss.png");

    // Création du slider
    sliders[0] = createSlider(win->renderer, 100, 100, 200, 25,
        (SDL_Color){128, 128, 128, 255}, (SDL_Color){255, 0, 0, 255});

    // Ajout des tableaux dans les listes correspondantes
    addListSlider(game.ui[1].sliders, sliders, nbSlidersSettings);
    addListButton(game.ui[2].buttons, buttonsMenu, nbButtonsMenu);
    addListButton(game.ui[1].buttons, buttonsParam, nbButtonsParam);
    addListButton(game.ui[5].buttons, buttonsLoadGame, nbButtonsLoad);
    addListButton(game.ui[3].buttons, buttonsGame, nbButtonsGame);
    addListButton(game.ui[6].buttons, buttonsICMons, nbButtonsICMons);
    addListButton(game.ui[7].buttons, buttonsInter, nbButtonsInter);
}

void updateICButtons(Window *win, t_Team *team) {
    char temp[100];
    for (int i = 0; i < 4; i++) {
        if(team->team[0].nb_move > i) {
            snprintf(temp, sizeof(temp), "%s  %d/%d", 
                team->team[0].moveList[i].name,
                team->team[0].moveList[i].current_pp,
                team->team[0].moveList[i].max_pp);
            setButtonText(game.ui[3].buttons->buttons[i], temp, win->renderer);
        }
        else {
            setButtonText(game.ui[3].buttons->buttons[i], "  ", win->renderer);
        }
    }
    if (game.gameState.currentState == NEWGAME || game.gameState.currentState == LOADGAME) updateICMonsButtonText(win, team);
}


void updateCurrentButton() {
    if (game.ui[game.gameState.currentState].buttons->size > 0) {
        for (int i = 0; i < game.ui[game.gameState.currentState].buttons->size; i++) {
            if (i != game.currentButton) game.ui[game.gameState.currentState].buttons->buttons[i]->texture = game.ui[game.gameState.currentState].buttons->buttons[i]->initialTexture;
            else  game.ui[game.gameState.currentState].buttons->buttons[i]->texture = game.ui[game.gameState.currentState].buttons->buttons[i]->selectedTexture;
        }
    }
}
void updateMusic() {
    if(game.gameState.currentState==GAME||game.gameState.currentState==ICMONS){if(!Mix_PlayingMusic())Mix_PlayMusic(game.gameState.music,-1);}else Mix_HaltMusic();
}
