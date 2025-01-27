#include "include/GameEngine.h"

// Global variables

// List of function pointers for rendering states
void (*stateHandlers[])(Window *) = {
    renderMenu,
    renderGame,
    renderSettings,
    renderQuit
};

// List of function pointers for event handlers
void (*eventHandlers[])(Window *, SDL_Event *) = {
    handleMenuEvent,
    handleGameEvent,
    handleSettingsEvent,
    handleQuitEvent
};

// List of buttons for menu

ButtonList MenuButtons = {NULL, 0};

SDL_Texture *backgroundTexture = NULL;

// List of buttons for settings
SDL_Texture *backgroundTextureSettings = NULL;
ButtonList SettingsButtons = {NULL, 0};
SliderList SettingsSliders = {NULL, 0};

// List of buttons for game
ButtonList GameButtons = {NULL, 0};
SDL_Texture * backgroundTextureGame = NULL;

//---------------------------------------------------------------------------------

// Functions for the menu

void renderMenu(Window *win) {
    // Set the background color for the menu
    if (backgroundTexture) {
        SDL_RenderCopy(win->renderer, backgroundTexture, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(win->renderer, 255, 0, 0, 255); // Red background
        SDL_RenderClear(win->renderer);
    }
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
    if(backgroundTextureGame){
        SDL_RenderCopy(win->renderer, backgroundTextureGame, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(win->renderer, 0, 255, 0, 255); // Green background
        SDL_RenderClear(win->renderer);
    }
    

}

void handleGameEvent(Window *win, SDL_Event *event) 
{
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        // Handle game event
    }
    handleEvent(win, event);
}

//---------------------------------------------------------------------------------

// Functions for the settings

void renderSettings(Window *win) {
    if (backgroundTextureSettings) {
        SDL_RenderCopy(win->renderer, backgroundTextureSettings, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(win->renderer, 0, 0, 255, 255); // Blue background
        SDL_RenderClear(win->renderer);
    }
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

void renderQuit(Window *win) 
{
    win->quit = 1;
}

void handleQuitEvent(Window *win, SDL_Event *event) 
{
    
    handleEvent(win, event);
}

//---------------------------------------------------------------------------------

// Functions for the main loop

void changeState(Window *win, void *data) {
    AppState newState = *(AppState *)data; // Convertit void* en AppState*
    SDL_Log("Changement d'état vers %d", newState);
    win->state = newState;
}

void changeTextSpeed(Window *win, void *data) {
    double *speed = (double *)data;
    win->textSpeed = *speed;
    SDL_Log("Vitesse du texte changée à %.1f", win->textSpeed);
}

void mainLoop(Window *win) {
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    int frameStart;
    SDL_Event event;

    MenuButtons.buttons = NULL;
    MenuButtons.size = 0;
    SettingsButtons.buttons = NULL;
    SettingsButtons.size = 0;
    SettingsSliders.sliders = NULL;
    SettingsSliders.size = 0;
    SDL_Log("Création des boutons...");
    AppState states[] = {GAME, SETTINGS, QUIT, MENU};
    double textSpeeds[] = {0.5, 1.0, 1.5};
    int nbButtonsMenu = 4;
    int nbSlidersSettings = 1;
    int nbButtonsParam = 4;
    Button **buttonsMenu = malloc(nbButtonsMenu * sizeof(Button *));
    Button **buttonsParam = malloc(nbButtonsParam * sizeof(Button *));
    Slider **sliders = malloc(nbSlidersSettings * sizeof(Slider *));
    if (!buttonsMenu || !sliders || !buttonsParam) {
        SDL_Log("Allocation de mémoire pour les boutons échouée !");
        return;
    }

    buttonsMenu[0] = createButton(
        "PLAY", win, 500, 104, 300, 100,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &states[0], win->LargeFont
    );

    buttonsMenu[1] = createButton(
        "LOAD GAME", win, 500, 258, 300, 100,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &states[2], win->LargeFont
    );
    buttonsMenu[2] = createButton(
        "SETTINGS", win, 500, 412, 300, 100,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &states[1], win->LargeFont 
    );

    buttonsMenu[3] = createButton(
        "QUIT", win, 500, 566, 300, 100,
        (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
        changeState, &states[2], win->LargeFont
    );

    InitTextureButton(buttonsMenu[0], win->renderer, "assets/User Interface/zoonami_menu_button6.png");
    InitTextureButton(buttonsMenu[1], win->renderer, "assets/User Interface/zoonami_menu_button6.png");
    InitTextureButton(buttonsMenu[2], win->renderer, "assets/User Interface/zoonami_menu_button6.png");
    InitTextureButton(buttonsMenu[3], win->renderer, "assets/User Interface/zoonami_menu_button6.png");
    addListButton(&MenuButtons, buttonsMenu, nbButtonsMenu);
    free(buttonsMenu); // Libération du tableau temporaire

    // Création d'un slider
    sliders[0] = createSlider(win->renderer, 100, 100, 200, 25, (SDL_Color){128,128,128, 255}, (SDL_Color){255, 0, 0, 255});
    addListSlider(&SettingsSliders, sliders, nbSlidersSettings);
    free(sliders); // Libération du tableau temporaire

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
        changeState, &states[3], win->LargeFont
    );
    InitTextureButton(buttonsParam[0], win->renderer, "assets/User Interface/zoonami_menu_button6.png");
    InitTextureButton(buttonsParam[1], win->renderer, "assets/User Interface/zoonami_menu_button6.png");
    InitTextureButton(buttonsParam[2], win->renderer, "assets/User Interface/zoonami_menu_button6.png");
    InitTextureButton(buttonsParam[3], win->renderer, "assets/User Interface/zoonami_menu_button7.png");
    addListButton(&SettingsButtons, buttonsParam, nbButtonsParam);
    free(buttonsParam); // Libération du tableau temporaire
    

    // Boucle principale
    while (!win->quit) {
        frameStart = SDL_GetTicks();

        // Gestion des événements en premier
        while (SDL_PollEvent(&event)) {
            eventHandlers[win->state](win, &event);
        }

        // Rendu après les événements
        stateHandlers[win->state](win);
        SDL_RenderPresent(win->renderer);

        // Gestion du temps pour maintenir le FPS
        int frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // Libération des ressources
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
}

//---------------------------------------------------------------------------------

// Functions for the window

void initWindow(Window *win, int width, int height, const char *FontPath) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || !(win->window = SDL_CreateWindow("ICPocket", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN)) || !(win->renderer = SDL_CreateRenderer(win->window, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC)) || (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) || TTF_Init() == -1 || !(win->LargeFont = TTF_OpenFont(FontPath, 56)) || !(win->MediumFont = TTF_OpenFont(FontPath, 36)) || !(win->SmallFont = TTF_OpenFont(FontPath, 24)) || !(win->font = TTF_OpenFont(FontPath, 18))) {
        SDL_Log("SDL Error: %s", SDL_GetError());
        if (win->window) SDL_DestroyWindow(win->window);
        exit(EXIT_FAILURE);
    }
    win->width = win->InitialWidth = width;
    win->height = win->InitialHeight = height;
    win->quit = 0;
    win->state = MENU;
    win->textSpeed = 1;

    loadBackground(&backgroundTexture, win->renderer, "assets/Title Screen/Title Screen Background.png");
    loadBackground(&backgroundTextureSettings, win->renderer, "assets/Battle Backgrounds/Other/zoonami_battle_party_background.png");
    loadBackground(&backgroundTextureGame, win->renderer, "assets/Title Screen/Start.jpg");

    
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
                case SDLK_ESCAPE:
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
            }
            break;
        default: break;
    }
}

void loadBackground(SDL_Texture **Background, SDL_Renderer *renderer, const char *imagePath) {
    SDL_Surface *surface = IMG_Load(imagePath); // Utiliser SDL_image pour charger l'image
    if (!surface) {
        SDL_Log("Erreur : Impossible de charger l'image de fond '%s'. Message SDL_image : %s", imagePath, IMG_GetError());
        return;
    }

    *Background = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // Libérer la surface après la conversion

    if (!*Background) {
        SDL_Log("Erreur : Impossible de créer une texture pour l'image de fond. Message SDL : %s", SDL_GetError());
    } else {
        SDL_Log("Image de fond chargée avec succès.");
    }
}

void renderText(Window * win, const char * text,SDL_Rect  * rect, SDL_Color color, TTF_Font *font)
{
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(win->renderer, textSurface);
    SDL_RenderCopy(win->renderer, textTexture, NULL, rect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void initAllButtons(Window * win)
{
    
}