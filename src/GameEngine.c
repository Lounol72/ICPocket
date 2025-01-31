#include "../include/GameEngine.h"

// Global variables
Game game;

Text title = {NULL,{0,0,0,0},{0,0,0,0}, {0,0,0,0}, NULL, NULL, NULL};
Text NewGameText = {NULL,{0,0,0,0},{0,0,0,0}, {0,0,0,0}, NULL, NULL, NULL};

int marginBottom = 200; // Marge en bas en pixels
int marginRight = 0;  // Marge à droite en pixels

t_Team bleu ;

//---------------------------------------------------------------------------------

// Functions for the menu

void renderMenu(Window *win) {
    // Set the background color for the menu
    if (game.ui[2].background) SDL_RenderCopy(win->renderer, game.ui[2].background, NULL, NULL);
    
    renderText(win, &title);
    renderButtonList(game.ui[2].buttons);
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

void renderGame(Window *win) {
    if (game.ui[3].background) {
        SDL_Rect renderQuad = {0, 0, win->width, (int)(win->height * 0.722)};
        SDL_RenderCopy(win->renderer, game.ui[3].background, NULL, &renderQuad);
    }
    renderButtonList(game.ui[3].buttons);
    
}

void handleGameEvent(Window *win, SDL_Event *event) 
{
    if (!game.gameState.playerTurn && isTeamAlive(&game.battleState.rouge) && isTeamAlive(&game.battleState.bleu)) {
        
        while (!isAlive(&(game.battleState.rouge.team[0]))) {
            int swap=rand() % 5 + 11;
            if(testActionValidity(swap,&game.battleState.rouge)) {swapActualAttacker(&game.battleState.rouge, swap);updateAttackButtons(win, &game.battleState.rouge);}
        }
        while (!isAlive(&(game.battleState.bleu.team[0]))) {
            int swap=rand() % 5 + 11;
            if(testActionValidity(swap,&game.battleState.bleu)) swapActualAttacker(&game.battleState.bleu, swap);
        }
        //printf("pv rouge : %d\npv bleu : %d\n", rouge.team[0].current_pv, bleu.team[0].current_pv);
        game.gameState.playerTurn = 1;
    } else if (!isTeamAlive(&game.battleState.rouge) || !isTeamAlive(&game.battleState.bleu)) {
        printf("VICTOIRE DES %s!!!\n", isTeamAlive(&game.battleState.rouge) ? "ROUGES" : "BLEUS");
        win->state = isTeamAlive(&game.battleState.rouge) ? MENU : SETTINGS;
        game.gameState.initialized = 0;
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

void renderSettings(Window *win) {
    if (game.ui[1].background) 
        SDL_RenderCopy(win->renderer, game.ui[1].background, NULL, NULL);
    
    // Toujours rendre les sliders
    renderSliderList((game.ui[1].sliders));
    renderButtonList(game.ui[1].buttons);
}


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

void renderQuit(Window *win) {
    win->quit = 1;
}

void handleQuitEvent(Window *win, SDL_Event *event) {
    
    handleEvent(win, event);
}

//---------------------------------------------------------------------------------

// Functions for new game

void createFicGame() {
    FILE *file = fopen("savegame.txt", "w");
    if (file) {
        fprintf(file, "New game initialized.\n");
        fclose(file);
        SDL_Log("Fichier de sauvegarde créé avec succès.");
    } else {
        SDL_Log("Erreur : Impossible de créer le fichier de sauvegarde.");
    }
}

void renderNewGame(Window * win){
    if(game.ui[4].background) SDL_RenderCopy(win->renderer, game.ui[4].background, NULL, NULL);
    renderText(win, &NewGameText);
}

void handleNewGameEvent(Window * win, SDL_Event * event){
    handleEvent(win, event);
    if (!game.gameState.initialized) {
        initData();
        teamTest(&game.battleState.rouge, 3);
        teamTest(&game.battleState.bleu, 2);
        printPoke(&(game.battleState.rouge.team[0]));
        printPoke(&(game.battleState.bleu.team[0]));
        printf("pv rouge : %d\n\n",game.battleState.rouge.team[0].current_pv);
        printf("pv bleu : %d\n\n",game.battleState.bleu.team[0].current_pv);
        updateAttackButtons(win, &game.battleState.rouge);
        bleu = game.battleState.bleu;
        game.gameState.initialized = 1;
    }
}

//---------------------------------------------------------------------------------

// Functions for load game

void readFicGame(){
}

void renderLoadGame(Window *win) {
    if (game.ui[5].background) SDL_RenderCopy(win->renderer, game.ui[5].background, NULL, NULL);
    // Toujours rendre les sliders
    renderButtonList(game.ui[5].buttons);
}

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

// Functions for the ICMons selection

void renderICMons(Window *win) {
    if (game.ui[6].background) SDL_RenderCopy(win->renderer, game.ui[6].background, NULL, NULL);
    //renderButtonList(game.ui[6].buttons);
}

void handleICMonsEvent(Window *win, SDL_Event *event) {
    if (!win || !event) return;
    // Parcourt et gère les événements des boutons
    /*
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[6].buttons->size; i++) {
            ButtonClicked(game.ui[6].buttons->buttons[i], x, y, win);
        }
    }*/
    // Gérer les autres événements
    handleEvent(win, event);
}

//---------------------------------------------------------------------------------

// Functions for the main loop

void changeState(Window *win, void *data) {
    AppState newState = *(AppState *)data; // Convertit void* en AppState*
    win->state = newState;
    SDL_Log("Changement d'état : %d", newState);
}

void changeTextSpeed(Window *win, void *data) {
    double *speed = (double *)data;
    win->textSpeed = *speed;
}

void attqButtonClicked(Window *win, void *data) {
    (void)win; // Pour éviter le warning
    if (game.gameState.playerTurn && isTeamAlive(&game.battleState.rouge) && isTeamAlive(&game.battleState.bleu)) {
        playATurn(&game.battleState.rouge, (intptr_t)data, &game.battleState.bleu, AI_move_choice(&iaTest,&game.battleState.rouge));
        while (isTeamAlive(&game.battleState.rouge) && !isAlive(&(game.battleState.rouge.team[0]))){
            int swap=rand() % 5 + 11;
            if(testActionValidity(swap,&game.battleState.rouge)) swapActualAttacker(&game.battleState.rouge, swap);
        }
        while (isTeamAlive(&game.battleState.bleu) && !isAlive(&(game.battleState.bleu.team[0]))){
            int swap=rand() % 5 + 11;
            if(testActionValidity(swap,&game.battleState.bleu)) swapActualAttacker(&game.battleState.bleu, swap);
        }
        printf("pv rouge : %d\npv bleu : %d\n", game.battleState.rouge.team[0].current_pv, game.battleState.bleu.team[0].current_pv);
        game.gameState.playerTurn = 0;
    }
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
        game.stateHandlers[win->state].render(win);
        SDL_RenderPresent(win->renderer);

        if(win->state == GAME){
            if (!Mix_PlayingMusic()) {  // Vérifie si la musique est déjà en cours
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
        else if (win->state == NEWGAME && SDL_GetTicks() - game.newGameStartTime >= 1000) { // 1 seconds
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

void initWindow(Window *win, int width, int height, const char *FontPath) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || !(win->window = SDL_CreateWindow("ICPocket", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN)) || !(win->renderer = SDL_CreateRenderer(win->window, -1, SDL_RENDERER_SOFTWARE)) || (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) || TTF_Init() == -1 || !(win->LargeFont = TTF_OpenFont(FontPath, 70)) || !(win->MediumFont = TTF_OpenFont(FontPath, 56)) || !(win->SmallFont = TTF_OpenFont(FontPath, 24)) || !(win->font = TTF_OpenFont(FontPath, 18))) {
        SDL_Log("SDL Error: %s", SDL_GetError());
        if (win->window) SDL_DestroyWindow(win->window);
        exit(EXIT_FAILURE);
    }
    

    win->width = win->InitialWidth = width;
    win->height = win->InitialHeight = height;
    win->quit = 0;
    win->state = MENU;
    win->textSpeed = 1;
    SDL_Log("Initialisation de la fenêtre réussie.");    
}

void destroyWindow(Window *win) 
{

    // Free music
    if (game.gameState.music) {
        Mix_FreeMusic(game.gameState.music);
        game.gameState.music = NULL;
    }

    // Free UI elements
    SDL_Log("Freeing UI elements");
    if(game.ui){
        for (int i = 0; i < game.nbStates; i++) {
        
        // 1) Free the Buttons
        if (game.ui[i].buttons != NULL) {
            destroyButtonList(game.ui[i].buttons);
            free(game.ui[i].buttons);
            game.ui[i].buttons = NULL;
        }

        // 2) Free the Sliders
        if (game.ui[i].sliders != NULL) {
            destroySliderList(game.ui[i].sliders);
            free(game.ui[i].sliders);
            game.ui[i].sliders = NULL;
        }

        // 3) Free the Background
        if (game.ui[i].background != NULL) {
            SDL_DestroyTexture(game.ui[i].background);
            game.ui[i].background = NULL;
        }
    }

    free(game.ui);
    game.ui = NULL;
    }
    
    // Free state handlers
    SDL_Log("Freeing game state handlers");
    if (game.stateHandlers) {
        free(game.stateHandlers);
        game.stateHandlers = NULL;
    }

    // Free speeds array
    SDL_Log("Freeing speed array");
    if (game.speeds){    
        free(game.speeds);
        game.speeds = NULL;
    }

    // Close fonts
    if (win->LargeFont) {
        TTF_CloseFont(win->LargeFont);
        win->LargeFont = NULL;
    }
    if (win->MediumFont) {
        TTF_CloseFont(win->MediumFont);
        win->MediumFont = NULL;
    }
    if (win->SmallFont) {
        TTF_CloseFont(win->SmallFont);
        win->SmallFont = NULL;
    }
    if (win->font) {
        TTF_CloseFont(win->font);
        win->font = NULL;
    }
    
    // Destroy renderer and window
    SDL_DestroyRenderer(win->renderer);
    win->renderer = NULL;
    SDL_DestroyWindow(win->window);
    win->window = NULL;

    Mix_CloseAudio(); 
    TTF_Quit();       
    IMG_Quit();       
    SDL_Quit();
}

void handleEvent(Window *win, SDL_Event *event) 
{
    switch(event->type) {
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym) {
                case SDLK_DELETE:
                    win->quit = 1;
                    SDL_Log("Quit");
                    break;
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
                updateButtonPosition(game.ui[2].buttons, scaleX, scaleY);
                updateButtonPosition(game.ui[3].buttons, scaleX, scaleY);
                updateButtonPosition(game.ui[5].buttons, scaleX, scaleY);
                updateButtonPosition(game.ui[3].buttons, scaleX, scaleY);
                updateTextPosition(&NewGameText, scaleX, scaleY);
                updateTextPosition(&title, scaleX, scaleY);
            }
            break;
        default: break;
    }
}
void initGame(Window *win) {
    game.win = win;
    
    int nbMenu = 7;
    game.ui = malloc(nbMenu * sizeof(UI_Elements));
    game.ui[0] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // Quit Page            = 0
    game.ui[1] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders=malloc(sizeof(SliderList)) ,.background=NULL};                        // Settings Page        = 1
    game.ui[2] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // Menu Page            = 2
    game.ui[3] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // Game Page            = 3
    game.ui[4] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // New Game Page        = 4
    game.ui[5] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // Load Game Page       = 5
    game.ui[6] = (UI_Elements){.buttons=malloc(sizeof(ButtonList)),.sliders   = NULL, .background=NULL};                                          // ICMons Page          = 6

    for (int i = 0; i < nbMenu; i++) {
        game.ui[i].buttons->buttons = NULL;
        game.ui[i].buttons->size = 0;
    }
    game.ui[1].sliders->sliders = NULL;
    game.ui[1].sliders->size = 0;

    game.gameState = (GameState){.music = NULL, .playerTurn = 1, .initialized = 0, .currentState = MENU};

    game.nbStates = 7;
    game.stateHandlers = malloc(game.nbStates * sizeof(StateHandler));
    game.stateHandlers[0] = (StateHandler){QUIT, renderQuit, handleQuitEvent}; // Quit = 0
    game.stateHandlers[1] = (StateHandler){SETTINGS, renderSettings, handleSettingsEvent}; // Settings = 1
    game.stateHandlers[2] = (StateHandler){MENU, renderMenu, handleMenuEvent}; // Menu = 2
    game.stateHandlers[3] = (StateHandler){GAME, renderGame, handleGameEvent}; // Game = 3
    game.stateHandlers[4] = (StateHandler){NEWGAME, renderNewGame, handleNewGameEvent}; // New game = 4
    game.stateHandlers[5] = (StateHandler){LOADGAME, renderLoadGame, handleLoadGameEvent}; // Load game = 5
    game.stateHandlers[6] = (StateHandler){ICMONS, renderICMons, handleICMonsEvent}; // ICMons = 6


    game.FPS = 60;
    game.frameDelay = 1000 / game.FPS;
    game.newGameStartTime = 0;


    game.speeds = malloc(sizeof(float)*3);
    game.speeds[0] = 0.5f;
    game.speeds[1] = 1.0f;
    game.speeds[2] = 1.5f;
    loadBackground(&game.ui[2].background, win->renderer, "assets/Title Screen/BG.jpg");
    loadBackground(&game.ui[5].background, win->renderer, "assets/Title Screen/LoadGame.png");
    loadBackground(&game.ui[1].background, win->renderer, "assets/Battle Backgrounds/Other/zoonami_battle_party_background.png");
    loadBackground(&game.ui[4].background, win->renderer, "assets/Title Screen/GameStart.jpg");
    loadBackground(&game.ui[3].background, win->renderer, "assets/Battle Backgrounds/With Textboxes/zoonami_forest_background.png");
    loadBackground(&game.ui[6].background, win->renderer, "assets/Title Screen/BG.jpg");

    initAudio();
    loadMusic(&game.gameState.music, "assets/audio/Battle.mp3");

    initText(win);

}

void updateTextPosition(Text *text, float scaleX, float scaleY) {
    if (text && text->texture) {
        text->rect.x = text->initialRect.x * scaleX;
        text->rect.y = text->initialRect.y * scaleY;
        text->rect.w = text->initialRect.w * scaleX;
        text->rect.h = text->initialRect.h * scaleY;
    }
}

void loadBackground(SDL_Texture **Background, SDL_Renderer *renderer, const char *imagePath) {
    if (!renderer || !imagePath) {
        SDL_Log("Erreur : Le renderer ou le chemin de l'image est NULL.");
        return;
    }
    SDL_Surface *surface = IMG_Load(imagePath);
    if (!surface) {
        SDL_Log("Erreur : Impossible de charger l'image de fond '%s'. Message SDL_image : %s", imagePath, IMG_GetError());
        return;
    }
    *Background = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void renderText(Window * win, Text * text){
    SDL_RenderCopy(win->renderer, text->texture, NULL, &text->rect);
}

void destroyText(Text * text){
    if (text) {
        if (text->texture) {
            SDL_DestroyTexture(text->texture);
            text->texture = NULL;
        }
        if (text->surface) {
            SDL_FreeSurface(text->surface);
            text->surface = NULL;
        }
    }
}
void initAllButtons(Window * win)
{
    int nbButtonsLoad = 3;
    int nbButtonsMenu = 4;
    int nbSlidersSettings = 1;
    int nbButtonsParam = 4;
    int nbButtonsGame = 5;

    Button **buttonsMenu = malloc(nbButtonsMenu * sizeof(Button *));
    Button **buttonsParam = malloc(nbButtonsParam * sizeof(Button *));
    Button **buttonsLoadGame = malloc(nbButtonsLoad * sizeof(Button *));
    Button **buttonsGame = malloc(nbButtonsGame * sizeof(Button *));
    Slider **sliders = malloc(nbSlidersSettings * sizeof(Slider *));
    
    

    if (!buttonsMenu || !sliders || !buttonsParam || !buttonsLoadGame || !buttonsGame) {
        SDL_Log("Allocation de mémoire pour les boutons échouée !");
        return;
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

        for(int i = 0; i < 4; i++) {
            InitTextureButton(buttonsMenu[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
            InitTextureButton(buttonsParam[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
            if (i != 3) InitTextureButton(buttonsLoadGame[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
            InitTextureButton(buttonsGame[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
        }

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
    }
}

void initText(Window *win) {
    const char *texts[] = {"Lancement de la Nouvelle Partie...", "ICPocket"};
    Text *textObjects[] = {&NewGameText, &title};
    SDL_Rect rects[] = {
        {win->width / 2 - 250, win->height / 2 + 250, 500, 100},
        {win->width / 2 - 250, -25, 500, 170}
    };

    for (int i = 0; i < 2; i++) {
        *textObjects[i] = (Text){texts[i], rects[i], rects[i], {255, 255, 255, 255}, win->LargeFont, NULL, NULL};
        SDL_Surface *textSurface = TTF_RenderText_Solid(win->LargeFont, texts[i], textObjects[i]->color);
        if (!textSurface) {
            SDL_Log("Erreur de rendu du texte : %s", TTF_GetError());
            exit(EXIT_FAILURE);
        }
        textObjects[i]->texture = SDL_CreateTextureFromSurface(win->renderer, textSurface);
        if (!textObjects[i]->texture) {
            SDL_Log("Erreur de création de la texture : %s", SDL_GetError());
            SDL_FreeSurface(textSurface);
            exit(EXIT_FAILURE);
        }
        textObjects[i]->surface = textSurface;
    }
}

void updateAttackButtons(Window *win, t_Team *team) {
    if (!team || !team->team || !team->team[0].moveList) {
        SDL_Log("Erreur : team, team->team ou moveList est NULL\n");
        return;
    }
    for (int i = 0; i < 4 && game.ui[3].buttons->buttons && game.ui[3].buttons->buttons[i]; i++) {
        setButtonText(game.ui[3].buttons->buttons[i], team->team[0].moveList[i].name, win->renderer);
    }
}

//---------------------------------------------------------------------------------

// Functions for the music

void initAudio() {
    SDL_Log("🔧 Initialisation de SDL_mixer...");

    if (Mix_Init(MIX_INIT_MP3) == 0) {  
        SDL_Log("❌ Erreur Mix_Init: %s", Mix_GetError());
        exit(EXIT_FAILURE);
    } else {
        SDL_Log("✅ Mix_Init réussi.");
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 8192) < 0) {
        SDL_Log("❌ Erreur Mix_OpenAudio: %s", Mix_GetError());
        exit(EXIT_FAILURE);
    } else {
        SDL_Log("✅ Mix_OpenAudio réussi.");
    }
}

void loadMusic(Mix_Music **music, const char *musicPath) {
    *music = Mix_LoadMUS(musicPath);
    if (!*music) {
        SDL_Log("❌ Erreur Mix_LoadMUS: %s", Mix_GetError());
        exit(EXIT_FAILURE);
    } else {
        SDL_Log("✅ Musique chargée avec succès.");
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
}