#include "../include/GameEngine.h"
// Global variables
Game game;

Text title = {NULL,{0,0,0,0},{0,0,0,0}, {0,0,0,0}, NULL, NULL, NULL};
Text NewGameText = {NULL,{0,0,0,0},{0,0,0,0}, {0,0,0,0}, NULL, NULL, NULL};

int marginBottom = 200; // Marge en bas en pixels
int marginRight = 0;  // Marge à droite en pixels



//---------------------------------------------------------------------------------

// Functions for the menu

void render(Window *win) {
    if (game.gameState.currentState == GAME){
        SDL_Rect renderQuad = {0, 0, win->width, (int)(win->height * 0.722)};
        SDL_RenderCopy(win->renderer, game.ui[3].background, NULL, &renderQuad);
    }
    else if (game.ui[game.gameState.currentState].background) SDL_RenderCopy(win->renderer, game.ui[game.gameState.currentState].background, NULL, NULL);
    if (game.ui[game.gameState.currentState].buttons )renderButtonList(game.ui[game.gameState.currentState].buttons);
    if (game.ui[game.gameState.currentState].sliders)renderSliderList(game.ui[game.gameState.currentState].sliders);
    if(game.gameState.currentState == MENU) {
        renderText(win, &title);
    }else if(game.gameState.currentState == NEWGAME){
        renderText(win, &NewGameText);
    }
}

void handleMenuEvent(Window *win, SDL_Event *event) {
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[2].buttons->size; i++) {
            ButtonClicked(game.ui[2].buttons->buttons[i], x, y, win);
        }
    }
    handleEvent(win, event);
}
//---------------------------------------------------------------------------------

// Functions for the game

void handleGameEvent(Window *win, SDL_Event *event) 
{
    if (!game.gameState.playerTurn && isTeamAlive(&game.battleState.rouge) && isTeamAlive(&game.battleState.bleu)) {
        
        //printf("pv rouge : %d\npv bleu : %d\n", rouge.team[0].current_pv, bleu.team[0].current_pv);
        if (!isAlive(&(game.battleState.rouge.team[0]))){
            game.gameState.currentState = ICMONS;
            win->state = ICMONS;
            return;
        }
        game.gameState.playerTurn = 1;
    } else if (!isTeamAlive(&game.battleState.rouge) || !isTeamAlive(&game.battleState.bleu)) {
        printf("VICTOIRE DES %s!!!\n", isTeamAlive(&game.battleState.rouge) ? "ROUGES" : "BLEUS");
        sauvegarder("Save_1",&game.battleState.rouge,&game.battleState.bleu);
        win->state = isTeamAlive(&game.battleState.rouge) ? INTER : MENU;
        game.gameState.currentState = win->state;
        game.gameState.initialized = 0;
        return;
    }
    if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
        changeState(win, &game.stateHandlers[2].state);
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        // Handle game event
        for (int i = 0; i < game.ui[3].buttons->size; i++)
            ButtonClicked(game.ui[3].buttons->buttons[i], x, y, win);
    }
    handleEvent(win, event);
}

//---------------------------------------------------------------------------------

// Functions for the settings

void handleSettingsEvent(Window *win, SDL_Event *event) {
    if (!win || !event || !game.ui[1].sliders->sliders || game.ui[1].sliders->size <= 0) return;
    // Parcourt et gère les événements des sliders
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[1].sliders->size; i++) {
            if (handleSliderEvent(game.ui[1].sliders->sliders[i], x,y)) break;
        }
        for(int i = 0; i < game.ui[1].buttons->size; i++){
            ButtonClicked(game.ui[1].buttons->buttons[i], x, y, win);
        }
    }
    // Gérer les autres événements
    handleEvent(win, event);
}

//---------------------------------------------------------------------------------

// Functions for quitting

void handleQuitEvent(Window *win, SDL_Event *event) {
    win->quit = 1;
    handleEvent(win, event);
}

//---------------------------------------------------------------------------------

// Functions for new game

void handleNewGameEvent(Window * win, SDL_Event * event){
    handleEvent(win, event);
    if (!game.gameState.initialized) {
        initData();
        initTeam(&game.battleState.rouge, 3);
        initTeam(&game.battleState.bleu, 3);
        game.battleState.ia = (t_AI){10, damageOnly, &game.battleState.bleu};
        for(int i = 0; i < game.battleState.rouge.nb_poke; i++){
            printPoke(&(game.battleState.rouge.team[i]));
        }
        printf("pv rouge : %d\n\n",game.battleState.rouge.team[0].current_pv);
        printf("pv bleu : %d\n\n",game.battleState.bleu.team[0].current_pv);
        updateICButtons(win, &game.battleState.rouge);
        bleu = game.battleState.bleu;
        game.gameState.initialized = 1;
    }
}

//---------------------------------------------------------------------------------

// Functions for load game


void handleLoadGameEvent(Window *win, SDL_Event *event) {
    if (!win || !event) return;
    // Parcourt et gère les événements des boutons
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[5].buttons->size; i++) {
            ButtonClicked(game.ui[5].buttons->buttons[i], x, y, win);
        }
    }
    // Gérer les autres événements
    handleEvent(win, event);
}

//---------------------------------------------------------------------------------
// Functions for the ICmons Sprite
void uptadeICMonsSprite(t_Poke *poke, float scaleX, float scaleY) {
    if (poke) {
        poke->rect.x = poke->initialRect.x * scaleX;
        poke->rect.y = poke->initialRect.y * scaleY;
        poke->rect.w = poke->initialRect.w * scaleX;
        poke->rect.h = poke->initialRect.h * scaleY;
    }
}
void initICMonsSprite(SDL_Renderer *renderer, const char *imagePath, t_Poke *poke, int x, int y, int w, int h) {
    //temporaire
    (void)imagePath;
    SDL_Surface *surface = IMG_Load("assets/Monsters/New Versions/calamd.png");
    if (!surface) {
        SDL_Log("❌ Erreur lors du chargement de l'image : %s", IMG_GetError());
        return ;
    }
    poke->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = NULL;
    if(!poke->texture){
        SDL_Log("❌ Erreur lors de la création de la texture : %s", SDL_GetError());
        return ;
    }
    poke->rect = (SDL_Rect){x, y, w, h};
    poke->initialRect = poke->rect;
}
void renderICMonsSprite(Window *win, t_Poke *poke) {
    if (poke && poke->texture) {
        SDL_RenderCopy(win->renderer, poke->texture, NULL, &poke->rect);
    }
}
void destroyICMonsSprite(Window *win, t_Poke *poke) {
    (void)win;
    if ( poke->texture) {
        SDL_DestroyTexture(poke->texture);
        poke->texture = NULL;
    }
    free(poke);
    poke = NULL;
}

// Functions for the ICMons selection

void handleICMonsEvent(Window *win, SDL_Event *event) {
    if (!win || !event) return;
    if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[6].buttons->size; i++) {
            ButtonClicked(game.ui[6].buttons->buttons[i], x, y, win);
        }
    }
    handleEvent(win, event);
}

//---------------------------------------------------------------------------------

// Functions for intermediate

void handleIntermediateEvent(Window *win, SDL_Event *event) {
    if (!win || !event) return;
    if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[7].buttons->size; i++) {
            ButtonClicked(game.ui[7].buttons->buttons[i], x, y, win);
        }
    }
    if(!game.saved){

    }
    handleEvent(win, event);
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
    SDL_SetWindowFullscreen(win->window, 0);
    SDL_SetWindowSize(win->window, win->InitialWidth, win->InitialHeight);
    SDL_GetWindowSize(win->window, &win->width, &win->height);
}

void attqButtonClicked(Window *win, void *data) {
    (void)win; // Pour éviter le warning
    if (game.gameState.playerTurn && isTeamAlive(&game.battleState.rouge) && isTeamAlive(&game.battleState.bleu)) {
        int moveIndex = (int)(intptr_t)data;
        if (moveIndex < 0 || moveIndex >= game.battleState.rouge.team[0].nb_move) {
            SDL_Log("Indice de mouvement invalide : %d", moveIndex);
            return;
        }
        //debug purposes
        printf("\t\t\tPP avant : %d\n", game.battleState.rouge.team[0].moveList[(intptr_t)data].current_pp);
        printf("\t\t\tPV avant : %d\n", game.battleState.rouge.team[0].current_pv);
        if (isAlive(&(game.battleState.rouge.team[0]))) 
            playATurn(&game.battleState.rouge, moveIndex, &game.battleState.bleu, AI_move_choice(&game.battleState.ia, &game.battleState.rouge));

        while (isTeamAlive(&game.battleState.bleu) && !isAlive(&(game.battleState.bleu.team[0]))){
            int swap=rand() % 5 + 11;
            if(testActionValidity(swap,&game.battleState.bleu)) swapActualAttacker(&game.battleState.bleu, swap);
        }
        printf("pv rouge : %d\npv bleu : %d\n", game.battleState.rouge.team[0].current_pv, game.battleState.bleu.team[0].current_pv);
        game.gameState.playerTurn = 0;
    }
}

void changePokemon(Window *win, void *data) {
    (void)win; 
    int idx = (int)(intptr_t)data; 
    if (testActionValidity(idx, &game.battleState.rouge)) {
        playATurn(&game.battleState.rouge, idx, &game.battleState.bleu, AI_move_choice(&(game.battleState.ia),&game.battleState.rouge));
        updateICButtons(win, &game.battleState.rouge);
        win->state = GAME;
        game.gameState.currentState = GAME;
    }
}

void nextDuel(Window *win, void *data) {
    (void)win; 
    (void)data;
    // Sauvegarde de la partie TODO 
    healTeam(&game.battleState.rouge);
    initTeam(&game.battleState.bleu, 3);
    changeState(win, &game.stateHandlers[3].state);
}

void mainLoop(Window *win) {
    
    initGame(win);
    int frameStart;
    SDL_Event event;
    
    initAllButtons(win);
    
    // Boucle principale
    while (!win->quit) {
        frameStart = SDL_GetTicks();

        // Manage events
        while (SDL_PollEvent(&event)) {
            game.stateHandlers[win->state].handleEvent(win, &event);
        }
        // Render the window
        SDL_RenderClear(win->renderer);
        render(win);
        updateCurrentButton();
        SDL_RenderPresent(win->renderer);

        if(win->state == GAME || win->state == ICMONS){
            if (!Mix_PlayingMusic()) {  // Verify if the music is already playing
                Mix_PlayMusic(game.gameState.music, -1);
            }
        } else {
            Mix_HaltMusic();
        }

        // Manage frame rate
        int frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < game.frameDelay) {
            SDL_Delay(game.frameDelay - frameTime);
        }
        // If the state is NEWGAME and 5 seconds have passed, go to game state
        if (win->state == NEWGAME && game.newGameStartTime == 0) game.newGameStartTime = SDL_GetTicks();
        else if (win->state == NEWGAME && SDL_GetTicks() - game.newGameStartTime >= 1000) { // 1 second
            changeState(win, &game.stateHandlers[3].state); // Change state to GAME
            game.newGameStartTime = 0; // reinitialize the start time
        }
    }
    // Destroy text objects
    if (NewGameText.texture) {
        destroyText(&NewGameText);
    }
    if (title.texture) {
        destroyText(&title);
    }
}

//---------------------------------------------------------------------------------

// Functions for the buttons


void handleEvent(Window *win, SDL_Event *event) {
    switch(event->type) {
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym) {
                case SDLK_DELETE:
                    win->quit = 1;
                    SDL_LogMessage(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_INFO, "Quit");
                    break;
            }
            if (game.ui[game.gameState.currentState].buttons) {
                int max = game.ui[game.gameState.currentState].buttons->size - 1;
                switch (event->key.keysym.sym) {
                    case SDLK_UP: game.currentButton = (game.currentButton > 0) ? game.currentButton - 1 : game.currentButton; break;
                    case SDLK_DOWN: game.currentButton = (game.currentButton < max) ? game.currentButton + 1 : game.currentButton; break;
                    case SDLK_LEFT: game.currentButton = (game.currentButton >= max / 2) ? game.currentButton - max / 2 : game.currentButton; break;
                    case SDLK_RIGHT: game.currentButton = (game.currentButton + max / 2 <= max) ? game.currentButton + max / 2 : game.currentButton; break;
                    case SDLK_c: ButtonClicked(game.ui[game.gameState.currentState].buttons->buttons[game.currentButton], -1, -1, win); break;
                    default: break;
                }
            }
            break;
        case SDL_QUIT:
            win->quit = 1;
            break;
        case SDL_WINDOWEVENT:
            if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
                win->width = event->window.data1;
                win->height = event->window.data2;
                float scaleX = (float)win->width / win->InitialWidth;
                float scaleY = (float)win->height / win->InitialHeight;
                for (int i = 1; i < game.nbMenu; i++) {
                    if (game.ui[i].buttons) updateButtonPosition(game.ui[i].buttons, scaleX, scaleY);
                    if (game.ui[i].sliders) updateSliderPosition(game.ui[i].sliders, scaleX, scaleY);
                }
                
                updateTextPosition(&NewGameText, scaleX, scaleY);
                updateTextPosition(&title, scaleX, scaleY);
            }
            break;
        
        default: break;
        
    }
}

void loadBackground(SDL_Texture **Background, SDL_Renderer *renderer, const char *imagePath) {
    if (!renderer || !imagePath) {
        SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_ERROR, "❌ Erreur : Le renderer ou le chemin de l'image est NULL.");
        return;
    }
    SDL_Surface *surface = IMG_Load(imagePath);
    if (!surface) {
        SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_ERROR, "❌ Erreur : Impossible de charger l'image de fond '%s'. Message SDL_image : %s", imagePath, IMG_GetError());
        return;
    }
    *Background = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void initAllButtons(Window * win)
{
    int nbButtonsLoad = 3;
    int nbButtonsMenu = 4;
    int nbSlidersSettings = 1;
    int nbButtonsParam = 6;
    int nbButtonsGame = 5;
    int nbButtonsICMons = 7;
    int nbButtonsInter = 2;

    Button **buttonsMenu = malloc(nbButtonsMenu * sizeof(Button *));
    Button **buttonsParam = malloc(nbButtonsParam * sizeof(Button *));
    Button **buttonsLoadGame = malloc(nbButtonsLoad * sizeof(Button *));
    Button **buttonsGame = malloc(nbButtonsGame * sizeof(Button *));
    Button **buttonsICMons = malloc(nbButtonsICMons * sizeof(Button *));
    Button **buttonsInter = malloc(nbButtonsInter * sizeof(Button *));

    Slider **sliders = malloc(nbSlidersSettings * sizeof(Slider *));
    
    

    if (!buttonsMenu || !sliders || !buttonsParam || !buttonsLoadGame || !buttonsGame ||!buttonsICMons  || !buttonsInter) {
        SDL_LogMessage(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_ERROR,"❌ Allocation de mémoire pour les boutons échouée !");
        exit(EXIT_FAILURE);
    }else{
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

        buttonsParam[0] =createButton(
            "0.5", win, 100, 200, 200, 50,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            changeTextSpeed, &game.speeds[0], win->LargeFont
        );

        buttonsParam[1] =createButton(
            "1", win, 400, 200, 200, 50,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            changeTextSpeed, &game.speeds[1], win->LargeFont
        );

        buttonsParam[2] =createButton(
            "1.5", win, 700, 200, 200, 50,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            changeTextSpeed, &game.speeds[2], win->LargeFont
        );

        buttonsParam[3] =createButton(
            "Fullscreen", win, 100, 300, 220, 75,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            makeWindowFullScreen, NULL, win->LargeFont
        );

        buttonsParam[4] =createButton(
            "Windowed", win, 400, 300, 220, 75,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            makeWindowWindowed, NULL, win->LargeFont
        );

        buttonsParam[5] =createButton(
            "Back", win, 100, 600, 300, 100,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            changeState, &game.stateHandlers[2].state, win->LargeFont
        );


        buttonsLoadGame[0] = createButton(
            "Save 1", win, 500, 104, 300, 100,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            changeState, &game.stateHandlers[5].state, win->LargeFont
        );

        buttonsLoadGame[1] = createButton(
            "Save 2", win, 500, 258, 300, 100,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            changeState, &game.stateHandlers[5].state, win->LargeFont
        );

        buttonsLoadGame[2]  =createButton(
            "Back", win, 100, 600, 300, 100,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            changeState, &game.stateHandlers[2].state, win->LargeFont
        );

        int buttonWidth = 430;
        int buttonHeight = 88;
        int spacingX = 5;
        int spacingY = 7;
        int startX = 20;
        int startY = 532;

        buttonsGame[0] = createButton(
            "Attack 1", win, startX, startY, buttonWidth, buttonHeight,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            attqButtonClicked, 0, win->LargeFont
        );
        buttonsGame[1] = createButton(
            "Attack 2", win, startX , startY + buttonHeight + spacingY, buttonWidth, buttonHeight,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            attqButtonClicked, 1, win->LargeFont
        );
        
        buttonsGame[2] = createButton(
            "Attack 3", win, startX + buttonWidth + spacingX, startY , buttonWidth, buttonHeight,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            attqButtonClicked, 2, win->LargeFont
        );
        buttonsGame[3] = createButton(
            "Attack 4", win, startX + buttonWidth + spacingX, startY + buttonHeight + spacingY, buttonWidth, buttonHeight,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            attqButtonClicked, 3, win->LargeFont
        );
        buttonsGame[4] = createButton(
            "ICMons", win, 950, startY, 300, 180,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            changeState, &game.stateHandlers[6].state, win->LargeFont
        );

        // ICMons buttons

        buttonsICMons[0] = createButton(
            "ICMon1", win, 20, 20, 160, 100,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            NULL, 0, win->LargeFont
        );
        buttonsICMons[1] = createButton(
            "ICMon2", win, 240, 20, 160, 100,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            changePokemon, 11, win->LargeFont
        );
        buttonsICMons[2] = createButton(
            "ICMon3", win, 460, 20, 160, 100,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            changePokemon, 12, win->LargeFont
        );
        buttonsICMons[3] = createButton(
            "ICMon4", win, 680, 20, 160, 100,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            changePokemon, 13, win->LargeFont
        );
        buttonsICMons[4] = createButton(
            "ICMon5", win, 900, 20, 160, 100,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            changePokemon, 14, win->LargeFont
        );
        buttonsICMons[5] = createButton(
            "ICMon6", win, 1120, 20, 160, 100,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            changePokemon, 15, win->LargeFont
        );
        buttonsICMons[6] = createButton(
            "Back", win, 100, 600, 300, 100,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            changeState, &game.stateHandlers[3].state, win->LargeFont
        );

        buttonsInter[0] = createButton(
            "Next Duel", win, 500, 200, 300, 100,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            nextDuel, 0, win->LargeFont
        );

        buttonsInter[1] = createButton(
            "Back", win, 500, 350, 300, 100,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            changeState, &game.stateHandlers[2].state, win->LargeFont
        );


        for(int i = 0; i < 4; i++) {
            InitTextureButton(buttonsMenu[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
            InitTextureButton(buttonsParam[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
            if (i != 3) InitTextureButton(buttonsLoadGame[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
            InitTextureButton(buttonsGame[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
            InitTextureButton(buttonsICMons[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
            if (i < 2) InitTextureButton(buttonsInter[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
        }
        InitTextureButton(buttonsParam[4], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
        InitTextureButton(buttonsParam[5], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
        InitTextureButton(buttonsICMons[4], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
        InitTextureButton(buttonsICMons[5], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
        InitTextureButton(buttonsICMons[6], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");

        InitTextureButton(buttonsGame[4], win->renderer, "assets/User Interface/Blue/button_square_depth_gloss.png");

        sliders[0] = createSlider(win->renderer, 100, 100, 200, 25, (SDL_Color){128,128,128, 255}, (SDL_Color){255, 0, 0, 255});

        addListSlider((game.ui[1].sliders), sliders, nbSlidersSettings);
        free(sliders);
        sliders = NULL;
        addListButton(game.ui[2].buttons, buttonsMenu, nbButtonsMenu);
        
        free(buttonsMenu);
        buttonsMenu = NULL;
        addListButton(game.ui[1].buttons, buttonsParam, nbButtonsParam);
        
        free(buttonsParam);
        buttonsParam = NULL;
        addListButton(game.ui[5].buttons, buttonsLoadGame, nbButtonsLoad);
        
        free(buttonsLoadGame);
        buttonsLoadGame = NULL;
        addListButton(game.ui[3].buttons, buttonsGame, nbButtonsGame);
        
        free(buttonsGame);
        buttonsGame = NULL;
        addListButton(game.ui[6].buttons, buttonsICMons, nbButtonsICMons);
        
        free(buttonsICMons);
        buttonsICMons = NULL;

        addListButton(game.ui[7].buttons, buttonsInter, nbButtonsInter);
        free(buttonsInter);
        buttonsInter = NULL;
    }
}

void updateICButtons(Window *win, t_Team *team) {
    for (int i = 0; i < 4; i++) {
        if(team->team[0].nb_move > i) setButtonText(game.ui[3].buttons->buttons[i], team->team[0].moveList[i].name, win->renderer);
        else setButtonText(game.ui[3].buttons->buttons[i], "  ", win->renderer);
    }
    for (int i  = 0; i < 6; i++) {
        if (team->team[i].name[0] != '\0') setButtonText(game.ui[6].buttons->buttons[i], team->team[i].name, win->renderer);
        else setButtonText(game.ui[6].buttons->buttons[i], "  ", win->renderer);
    }
}

void updateCurrentButton() {
    if (game.ui[game.gameState.currentState].buttons->size > 0) {
        for (int i = 0; i < game.ui[game.gameState.currentState].buttons->size; i++) {
            if (i != game.currentButton) game.ui[game.gameState.currentState].buttons->buttons[i]->texture = game.ui[game.gameState.currentState].buttons->buttons[i]->initialTexture;
            else  game.ui[game.gameState.currentState].buttons->buttons[i]->texture = game.ui[game.gameState.currentState].buttons->buttons[i]->selectedTexture;
        }
    }
}

