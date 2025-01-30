#include "../include/GameEngine.h"

// Global variables
Text title = {NULL,{0,0,0,0},{0,0,0,0}, {0,0,0,0}, NULL, NULL, NULL};


// Temporary variables
t_Team rouge;
t_Team bleu;

int initialize = 0;


// List of function pointers for rendering states
void (*stateHandlers[])(Window *) = {
    renderGame,
    renderSettings,
    renderMenu,
    renderQuit,
    renderNewGame,
    renderLoadGame
};

// List of function pointers for event handlers
void (*eventHandlers[])(Window *, SDL_Event *) = {
    handleGameEvent,
    handleSettingsEvent,
    handleMenuEvent,
    handleQuitEvent,
    handleNewGameEvent,
    handleLoadGameEvent
};

AppState states[] = {GAME, SETTINGS ,MENU, QUIT, NEWGAME, LOADGAME, PAUSE}; // 0 = GAME, 1 = SETTINGS, 2 = MENU, 3 = QUIT, 4 = NEWGAME, 5 = LOADGAME, 6 = PAUSE
double textSpeeds[] = {0.5, 1.0, 1.5};
// List of buttons for menu

ButtonList MenuButtons = {NULL, 0};

SDL_Texture *backgroundTexture = NULL;

// List of buttons for settings
SDL_Texture *backgroundTextureSettings = NULL;
ButtonList SettingsButtons = {NULL, 0};
SliderList SettingsSliders = {NULL, 0};

//List of buttons for load game

SDL_Texture *backgroundTextureLoadGame = NULL;
ButtonList LoadGameButtons = {NULL, 0};
SliderList LoadGameSliders = {NULL, 0};

// List of buttons for game

int playerTurn = 1; // 1 = au tour du joueur, 0 = IA
ButtonList GameButtons = {NULL, 0};
SDL_Texture * backgroundTextureGame = NULL;
int marginBottom = 200; // Marge en bas en pixels
int marginRight = 0;  // Marge à droite en pixels

// List of buttons for new game
Text NewGameText = {NULL,{0,0,0,0},{0,0,0,0}, {0,0,0,0}, NULL, NULL, NULL};
ButtonList NewGameButtons = {NULL, 0};
SDL_Texture * backgroundTextureNewGame = NULL;

//---------------------------------------------------------------------------------

// Functions for the menu

void renderMenu(Window *win) {
    // Set the background color for the menu
    if (backgroundTexture) SDL_RenderCopy(win->renderer, backgroundTexture, NULL, NULL);
    
    renderText(win, &title);
    renderButtonList(&MenuButtons);
}

void handleMenuEvent(Window *win, SDL_Event *event) {
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < MenuButtons.size; i++) {
            ButtonClicked(MenuButtons.buttons[i], x, y, win);
        }
    }
    handleEvent(win, event);
}
//---------------------------------------------------------------------------------

// Functions for the game

void renderGame(Window *win) {
    if (backgroundTextureGame) {
        SDL_Rect renderQuad = {0, 0, win->width, (int)(win->height * 0.722)};
        SDL_RenderCopy(win->renderer, backgroundTextureGame, NULL, &renderQuad);
    }

    

    renderButtonList(&GameButtons);
    
}

void handleGameEvent(Window *win, SDL_Event *event) 
{
    if (!playerTurn && isTeamAlive(&rouge) && isTeamAlive(&bleu)) {
        
        if (!isAlive(&(rouge.team[0]))) {swapActualAttacker(&rouge, rand() % 5 + 11);updateAttackButtons(win, &rouge);}
        if (!isAlive(&(bleu.team[0]))) swapActualAttacker(&bleu, rand() % 5 + 11);
        printf("pv rouge : %d\npv bleu : %d\n", rouge.team[0].current_pv, bleu.team[0].current_pv);
        playerTurn = 1;
    } else if (!isTeamAlive(&rouge) || !isTeamAlive(&bleu)) {
        printf("VICTOIRE DES %s!!!\n", isTeamAlive(&rouge) ? "ROUGES" : "BLEUS");
        win->state = isTeamAlive(&rouge) ? MENU : SETTINGS;
    }
    if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
        changeState(win, &states[2]);
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        // Handle game event
        for (int i = 0; i < GameButtons.size; i++)
            ButtonClicked(GameButtons.buttons[i], x, y, win);
    }
    handleEvent(win, event);
}

//---------------------------------------------------------------------------------

// Functions for the settings

void renderSettings(Window *win) {
    if (backgroundTextureSettings) 
        SDL_RenderCopy(win->renderer, backgroundTextureSettings, NULL, NULL);
    
    // Toujours rendre les sliders
    renderSliderList(&SettingsSliders);
    renderButtonList(&SettingsButtons);
}


void handleSettingsEvent(Window *win, SDL_Event *event) {
    if (!win || !event || !SettingsSliders.sliders || SettingsSliders.size <= 0) return;
    // Parcourt et gère les événements des sliders
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < SettingsSliders.size; i++) {
            if (handleSliderEvent(SettingsSliders.sliders[i], x,y)) break;
        }
        for(int i = 0; i < SettingsButtons.size; i++){
            ButtonClicked(SettingsButtons.buttons[i], x, y, win);
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
    
    if(backgroundTextureGame) SDL_RenderCopy(win->renderer, backgroundTextureNewGame, NULL, NULL);
    
    renderText(win, &NewGameText);
    renderButtonList(&NewGameButtons);

    if (!initialize) {
        initData();
        teamTest(&rouge, 3);
        teamTest(&bleu, 2);
        printPoke(&(rouge.team[0]));
        printPoke(&(bleu.team[0]));
        printf("pv rouge : %d\n\n",rouge.team[0].current_pv);
        printf("pv bleu : %d\n\n",bleu.team[0].current_pv);
        updateAttackButtons(win, &rouge);
        initialize = 1;
    }
}

void handleNewGameEvent(Window * win, SDL_Event * event){
    handleEvent(win, event);
}

//---------------------------------------------------------------------------------

// Functions for load game

void readFicGame(){

}

void renderLoadGame(Window *win) {
    if (backgroundTextureSettings) SDL_RenderCopy(win->renderer, backgroundTextureLoadGame, NULL, NULL);
    // Toujours rendre les sliders
    renderButtonList(&LoadGameButtons);
}

void handleLoadGameEvent(Window *win, SDL_Event *event) {
    if (!win || !event) return;
    // Parcourt et gère les événements des sliders
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < LoadGameButtons.size; i++) {
            ButtonClicked(LoadGameButtons.buttons[i], x, y, win);
        }
    }
    // Gérer les autres événements
    handleEvent(win, event);
}

//---------------------------------------------------------------------------------

// Functions for the main loop

void changeState(Window *win, void *data) {
    AppState newState = *(AppState *)data; // Convertit void* en AppState*
    win->state = newState;
}

void changeTextSpeed(Window *win, void *data) {
    double *speed = (double *)data;
    win->textSpeed = *speed;
}

void attqButtonClicked(Window *win, void *data) {
    (void)win; // Pour éviter le warning
    if (playerTurn && isTeamAlive(&rouge) && isTeamAlive(&bleu)) {
        playATurn(&rouge, (int)data, &bleu, AI_move_choice(&iaTest,&rouge));
        if (!isAlive(&(rouge.team[0]))) swapActualAttacker(&rouge, rand() % 5 + 11);
        if (!isAlive(&(bleu.team[0]))) swapActualAttacker(&bleu, rand() % 5 + 11);
        printf("pv rouge : %d\npv bleu : %d\n", rouge.team[0].current_pv, bleu.team[0].current_pv);
        playerTurn = 0;
    }
}



void mainLoop(Window *win) {
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    int frameStart;
    SDL_Event event;
    int newGameStartTime = 0;

    MenuButtons.buttons = NULL;
    MenuButtons.size = 0;
    SettingsButtons.buttons = NULL;
    SettingsButtons.size = 0;
    SettingsSliders.sliders = NULL;
    SettingsSliders.size = 0;
    
    initAllButtons(win);
    

    // Boucle principale
    while (!win->quit) {
        frameStart = SDL_GetTicks();

        // Manage events
        while (SDL_PollEvent(&event)) {
            eventHandlers[win->state](win, &event);
        }

        // Render the window
        SDL_RenderClear(win->renderer);
        stateHandlers[win->state](win);
        SDL_RenderPresent(win->renderer);

        // Manage frame rate
        int frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
        // If the state is NEWGAME and 5 seconds have passed, go to game state
        if (win->state == NEWGAME && newGameStartTime == 0) newGameStartTime = SDL_GetTicks();
        else if (win->state == NEWGAME && SDL_GetTicks() - newGameStartTime >= 1000) { // 5 seconds
            changeState(win, &states[0]); // Change state to GAME
            newGameStartTime = 0; // reinitialize the start time
        }
    }

    // Free memory
    destroyButtonList(&MenuButtons);
    destroyButtonList(&SettingsButtons);
    destroySliderList(&SettingsSliders);
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = NULL;
    }
    if (backgroundTextureSettings) {
        SDL_DestroyTexture(backgroundTextureSettings);
        backgroundTextureSettings = NULL;
    }
    if (backgroundTextureGame) {
        SDL_DestroyTexture(backgroundTextureGame);
        backgroundTextureGame = NULL;
    }
    if (NewGameText.texture) {
        destroyText(&NewGameText);
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

    loadBackground(&backgroundTexture, win->renderer, "assets/Title Screen/BG.jpg");
    loadBackground(&backgroundTextureLoadGame, win->renderer, "assets/Title Screen/LoadGame.png");
    loadBackground(&backgroundTextureSettings, win->renderer, "assets/Battle Backgrounds/Other/zoonami_battle_party_background.png");
    loadBackground(&backgroundTextureNewGame, win->renderer, "assets/Title Screen/GameStart.jpg");
    loadBackground(&backgroundTextureGame, win->renderer, "assets/Battle Backgrounds/With Textboxes/zoonami_forest_background.png");

    initText(win);

}

void destroyWindow(Window *win) 
{
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = NULL;
    }
    if (backgroundTextureSettings) {
        SDL_DestroyTexture(backgroundTextureSettings);
        backgroundTextureSettings = NULL;
    }
    if(win->font)
        TTF_CloseFont(win->font);
    win->font = NULL;
    if(win->LargeFont)
        TTF_CloseFont(win->LargeFont);
    win->LargeFont = NULL;
    if(win->MediumFont)
        TTF_CloseFont(win->MediumFont);
    win->MediumFont = NULL;
    if(win->SmallFont)
    TTF_CloseFont(win->SmallFont);
    win->SmallFont = NULL;
    SDL_DestroyRenderer(win->renderer);
    SDL_DestroyWindow(win->window);
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
                updateButtonPosition(&MenuButtons, scaleX, scaleY);
                updateButtonPosition(&SettingsButtons, scaleX, scaleY);
                updateButtonPosition(&LoadGameButtons, scaleX, scaleY);
                updateButtonPosition(&GameButtons, scaleX, scaleY);
                updateTextPosition(&NewGameText, scaleX, scaleY);
                updateTextPosition(&title, scaleX, scaleY);
            }
            break;
        default: break;
    }
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
    SDL_Surface *surface = IMG_Load(imagePath);
    if (!surface) {
        SDL_Log("Erreur : Impossible de charger l'image de fond '%s'. Message SDL_image : %s", imagePath, IMG_GetError());
        return;
    }
    *Background = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!*Background) SDL_Log("Erreur : Impossible de créer une texture pour l'image de fond. Message SDL : %s", SDL_GetError());
}

void renderText(Window * win, Text * text){
    SDL_RenderCopy(win->renderer, text->texture, NULL, &text->rect);
}

void destroyText(Text * text){
    if(text->texture){
        SDL_DestroyTexture(text->texture);
        text->texture = NULL;
    }
    if(text->surface){
        SDL_FreeSurface(text->surface);
        text->surface = NULL;
    }
}

void initAllButtons(Window * win)
{
    SDL_Log("Création des boutons...");
    
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
    }

    buttonsMenu[0] = createButton(
        "PLAY", win, 500, 150, 300, 100,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &states[4], win->MediumFont
    );

    buttonsMenu[1] = createButton(
        "LOAD GAME", win, 500, 300, 300, 100,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &states[5], win->MediumFont
    );
    buttonsMenu[2] = createButton(
        "SETTINGS", win, 500, 450, 300, 100,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &states[1], win->MediumFont
    );

    buttonsMenu[3] = createButton(
        "QUIT", win, 500, 600, 300, 100,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &states[3], win->MediumFont
    );

    buttonsParam[0] =createButton(
        "0.5", win, 100, 200, 200, 50,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeTextSpeed, &textSpeeds[0], win->LargeFont
    );

    buttonsParam[1] =createButton(
        "1", win, 400, 200, 200, 50,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeTextSpeed, &textSpeeds[1], win->LargeFont
    );

    buttonsParam[2] =createButton(
        "1.5", win, 700, 200, 200, 50,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeTextSpeed, &textSpeeds[2], win->LargeFont
    );
    buttonsParam[3] =createButton(
        "Back", win, 100, 600, 300, 100,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &states[2], win->LargeFont
    );

    buttonsLoadGame[0] = createButton(
        "Save 1", win, 500, 104, 300, 100,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &states[4], win->LargeFont
    );

    buttonsLoadGame[1] = createButton(
        "Save 2", win, 500, 258, 300, 100,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &states[5], win->LargeFont
    );

    buttonsLoadGame[2]  =createButton(
        "Back", win, 100, 600, 300, 100,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &states[2], win->LargeFont
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
        changeState, &states[2], win->LargeFont
    );

    InitTextureButton(buttonsMenu[0], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
    InitTextureButton(buttonsMenu[1], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
    InitTextureButton(buttonsMenu[2], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
    InitTextureButton(buttonsMenu[3], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");

    InitTextureButton(buttonsParam[0], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
    InitTextureButton(buttonsParam[1], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
    InitTextureButton(buttonsParam[2], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
    InitTextureButton(buttonsParam[3], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");

    InitTextureButton(buttonsLoadGame[0], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
    InitTextureButton(buttonsLoadGame[1], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
    InitTextureButton(buttonsLoadGame[2], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");

    InitTextureButton(buttonsGame[0], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
    InitTextureButton(buttonsGame[1], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
    InitTextureButton(buttonsGame[2], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
    InitTextureButton(buttonsGame[3], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
    InitTextureButton(buttonsGame[4], win->renderer, "assets/User Interface/Blue/button_square_depth_gloss.png");

    sliders[0] = createSlider(win->renderer, 100, 100, 200, 25, (SDL_Color){128,128,128, 255}, (SDL_Color){255, 0, 0, 255});

    addListSlider(&SettingsSliders, sliders, nbSlidersSettings);
    free(sliders); // Libération du tableau temporaire
    addListButton(&MenuButtons, buttonsMenu, nbButtonsMenu);
    free(buttonsMenu); // Libération du tableau temporaire
    addListButton(&SettingsButtons, buttonsParam, nbButtonsParam);
    free(buttonsParam); // Libération du tableau temporaire
    addListButton(&LoadGameButtons, buttonsLoadGame, nbButtonsLoad);
    free(buttonsLoadGame);
    addListButton(&GameButtons, buttonsGame, nbButtonsGame);
    free(buttonsGame);
    
    sliders = NULL;
    buttonsMenu = NULL;
    buttonsParam = NULL;
    buttonsLoadGame = NULL;
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
    SDL_FreeSurface(textObjects[1]->surface);
}

void updateAttackButtons(Window *win, t_Team *team) {
    if (!team || !team->team || !team->team[0].moveList) {
        SDL_Log("Erreur : team, team->team ou moveList est NULL\n");
        return;
    }
    for (int i = 0; i < 4 && GameButtons.buttons && GameButtons.buttons[i]; i++) {
        setButtonText(GameButtons.buttons[i], team->team[0].moveList[i].name, win->renderer);
    }
}

