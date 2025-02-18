#include "../include/Game.h"

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