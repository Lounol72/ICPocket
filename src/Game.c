#include "../include/Game.h"

#define WINDOWS_W 1280
#define WINDOWS_H 720

void initGame(Window *win) {
    game.win = win;
    
    game.nbMenu = 10;
    game.ui = malloc(game.nbMenu * sizeof(UI_Elements));
    game.ui[0] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // Quit Page            = 0
    game.ui[1] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders=malloc(sizeof(SliderList)) ,.background=NULL};                        // Settings Page        = 1
    game.ui[2] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // Menu Page            = 2
    game.ui[3] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // Game Page            = 3
    game.ui[4] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // New Game Page        = 4
    game.ui[5] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // Load Game Page       = 5
    game.ui[6] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // ICMons Page          = 6
    game.ui[7] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // Intermediate         = 7
    game.ui[8] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // Pause Page           = 8
    game.ui[9] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // Map Page             = 9

    for (int i = 0; i < game.nbMenu; i++) {
        game.ui[i].buttons->buttons = NULL;
        game.ui[i].buttons->size = 0;
    }
    game.ui[1].sliders->sliders = NULL;
    game.ui[1].sliders->size = 0;

    game.gameState = (GameState){.music = NULL, .playerTurn = 1, .initialized = 0, .currentState = MENU};

    game.nbStates =10;
    game.stateHandlers = malloc(game.nbStates * sizeof(StateHandler));
    game.stateHandlers[0] = (StateHandler){QUIT, handleQuitEvent};
    game.stateHandlers[1] = (StateHandler){SETTINGS, handleSettingsEvent};
    game.stateHandlers[2] = (StateHandler){MENU, handleMenuEvent};
    game.stateHandlers[3] = (StateHandler){GAME, handleGameEvent};
    game.stateHandlers[4] = (StateHandler){NEWGAME, handleNewGameEvent};
    game.stateHandlers[5] = (StateHandler){LOADGAME, handleLoadGameEvent};
    game.stateHandlers[6] = (StateHandler){ICMONS, handleICMonsEvent};
    game.stateHandlers[7] = (StateHandler){INTER, handleIntermediateEvent};
    game.stateHandlers[8] = (StateHandler){PAUSE, handlePauseEvent};
    game.stateHandlers[9] = (StateHandler){MAP, handlePlayerEvent};

    game.FPS = 60;
    game.frameDelay = 1000 / game.FPS;
    
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
    loadBackground(&game.ui[8].background, win->renderer, "assets/Title Screen/BG.jpg");

    initAudio();
    loadMusic(&game.gameState.music, "assets/audio/Battle.mp3");

    game.gameData.player = createPlayer(win->renderer, "assets/Characters/Character 10.png");
    if (!game.gameData.player) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, 
                      "❌ Erreur : Impossible de créer le joueur");
        return;
    }
    
    game.gameData.player->position.x = WINDOWS_W / 2;
    game.gameData.player->position.y = WINDOWS_H / 2;
    
    game.gameData.map = initMap(win->renderer, "assets/Tileset/Map/MapFloor.png");
    game.gameData.camera = createCamera(WINDOWS_W, WINDOWS_H);
    
    initText(win);

    game.cursor = IMG_Load("assets/Characters/Lou.png");
    if (!game.cursor) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "❌ Erreur : Impossible de charger le curseur 'Lou.png'.");
        return;
    }
    SDL_SetCursor(SDL_CreateColorCursor(game.cursor, 0, 0));
    game.cursor_hover = IMG_Load("assets/pointer_d.png");
    if (!game.cursor_hover) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "❌ Erreur : Impossible de charger le curseur 'Lou_hover.png'.");
        return;
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