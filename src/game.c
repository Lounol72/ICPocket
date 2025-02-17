#include "../include/game.h"

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

        if(win->state == GAME){
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

// Functions for the window


void initGame(Window *win) {
    game.win = win;
    
    game.nbMenu = 8;
    game.ui = malloc(game.nbMenu * sizeof(UI_Elements));
    game.ui[0] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // Quit Page            = 0
    game.ui[1] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders=malloc(sizeof(SliderList)) ,.background=NULL};                        // Settings Page        = 1
    game.ui[2] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // Menu Page            = 2
    game.ui[3] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // Game Page            = 3
    game.ui[4] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // New Game Page        = 4
    game.ui[5] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // Load Game Page       = 5
    game.ui[6] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // ICMons Page          = 6
    game.ui[7] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // Intermediate         = 7

    for (int i = 0; i < game.nbMenu; i++) {
        game.ui[i].buttons->buttons = NULL;
        game.ui[i].buttons->size = 0;
    }
    game.ui[1].sliders->sliders = NULL;
    game.ui[1].sliders->size = 0;

    game.gameState = (GameState){.music = NULL, .playerTurn = 1, .initialized = 0, .currentState = MENU};

    game.nbStates = 8;
    game.stateHandlers = malloc(game.nbStates * sizeof(StateHandler));
    game.stateHandlers[0] = (StateHandler){QUIT, handleQuitEvent}; // Quit = 0
    game.stateHandlers[1] = (StateHandler){SETTINGS, handleSettingsEvent}; // Settings = 1
    game.stateHandlers[2] = (StateHandler){MENU, handleMenuEvent}; // Menu = 2
    game.stateHandlers[3] = (StateHandler){GAME, handleGameEvent}; // Game = 3
    game.stateHandlers[4] = (StateHandler){NEWGAME, handleNewGameEvent}; // New game = 4
    game.stateHandlers[5] = (StateHandler){LOADGAME, handleLoadGameEvent}; // Load game = 5
    game.stateHandlers[6] = (StateHandler){ICMONS, handleICMonsEvent}; // ICMons = 6
    game.stateHandlers[7] = (StateHandler){INTER, handleIntermediateEvent}; // Intermediate = 7


    game.FPS = 60;
    game.frameDelay = 1000 / game.FPS;
    game.newGameStartTime = 0;
    game.currentButton = 0;
    game.saved = 0;


    game.speeds = malloc(sizeof(float)*3);
    game.speeds[0] = 0.5f;
    game.speeds[1] = 1.0f;
    game.speeds[2] = 1.5f;
    loadBackground(&game.ui[2].background, win->renderer, "assets/Title Screen/BG.jpg");
    loadBackground(&game.ui[5].background, win->renderer, "assets/Title Screen/LoadGame.png");
    loadBackground(&game.ui[1].background, win->renderer, "assets/Battle Backgrounds/Other/zoonami_battle_party_background.png");
    loadBackground(&game.ui[4].background, win->renderer, "assets/Title Screen/GameStart.jpg");
    loadBackground(&game.ui[3].background, win->renderer, "assets/Title Screen/combat.png");
    loadBackground(&game.ui[6].background, win->renderer, "assets/Title Screen/BG.jpg");
    loadBackground(&game.ui[7].background, win->renderer, "assets/Title Screen/combat.png");

    initAudio();
    loadMusic(&game.gameState.music, "assets/audio/Battle.mp3");

    initText(win);

    
    
}

void updateCurrentButton() {
    if (game.ui[game.gameState.currentState].buttons->size > 0) {
        for (int i = 0; i < game.ui[game.gameState.currentState].buttons->size; i++) {
            if (i != game.currentButton) game.ui[game.gameState.currentState].buttons->buttons[i]->texture = game.ui[game.gameState.currentState].buttons->buttons[i]->initialTexture;
            else  game.ui[game.gameState.currentState].buttons->buttons[i]->texture = game.ui[game.gameState.currentState].buttons->buttons[i]->selectedTexture;
        }
    }
}

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
            playATurn(&game.battleState.rouge, moveIndex, &game.battleState.bleu, AI_move_choice(&game.battleState.ia.AI_t_Team,&game.battleState.rouge));

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
        playATurn(&game.battleState.rouge, idx, &game.battleState.bleu, AI_move_choice(&game.battleState.ia.AI_t_Team,&game.battleState.rouge));
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