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
    
    SDL_SetRenderDrawColor(win->renderer, 0, 255, 0, 255); // Green background
    SDL_RenderClear(win->renderer);
    

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
    renderSliderList(&SettingsSliders);
}

void handleSettingsEvent(Window *win, SDL_Event *event) 
{
    if (!win || !event) return; // Vérifie que les pointeurs ne sont pas nuls

    // Parcourt les sliders et gère les événements pour chacun
    if (SettingsSliders.sliders && SettingsSliders.size > 0) {
        for (int i = 0; i < SettingsSliders.size; i++) {
            handleSliderEvent(SettingsSliders.sliders[i], event);
        }
    }
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


void mainLoop(Window *win) {
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    int frameStart;
    SDL_Event event;
    MenuButtons.buttons = NULL;
    MenuButtons.size = 0;

    // Création des boutons
    SDL_Log("Création des boutons...");
    AppState states[] = {GAME, SETTINGS, QUIT, MENU};
    int nbButtonsMenu = 2;
    int nbSlidersSettings = 1;
    Button **buttonsMenu = malloc(nbButtonsMenu * sizeof(Button *));
    Slider **sliders = malloc(nbSlidersSettings * sizeof(Slider *));
    if (!buttonsMenu || !sliders) {
        SDL_Log("Allocation de mémoire pour les boutons échouée !");
        return;
    }

    buttonsMenu[0] = createButton("PLAY", win, 500, 104, 300, 100, (SDL_Color){0, 255, 255, 255}, (SDL_Color){128,128,128, 255}, changeState, &states[0]);
    buttonsMenu[1] = createButton("SETTINGS", win, 500, 258, 300, 100, (SDL_Color){0, 255, 255, 255}, (SDL_Color){128,128,128, 255}, changeState, &states[1]);


    InitTextureButton(buttonsMenu[0], win->renderer, "assets/User Interface/zoonami_menu_button6.png");
    InitTextureButton(buttonsMenu[1], win->renderer, "assets/User Interface/zoonami_menu_button6.png");
    addListButton(&MenuButtons, buttonsMenu, nbButtonsMenu);
    free(buttonsMenu); // Libération du tableau temporaire

    // Création d'un slider
    sliders[0] = createSlider(win->renderer, 100, 100, 200, 25, (SDL_Color){128,128,128, 255}, (SDL_Color){255, 0, 0, 255});
    addListSlider(&SettingsSliders, sliders, nbSlidersSettings);
    free(sliders); // Libération du tableau temporaire

    

    // Boucle principale
    while (!win->quit) {
        frameStart = SDL_GetTicks();

        // Rendu de l'état actuel
        stateHandlers[win->state](win);

        SDL_RenderPresent(win->renderer);

        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            eventHandlers[win->state](win, &event);
        }

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
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || !(win->window = SDL_CreateWindow("ICPocket", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN)) || !(win->renderer = SDL_CreateRenderer(win->window, -1, SDL_RENDERER_ACCELERATED)) || (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) || TTF_Init() == -1 || !(win->font = TTF_OpenFont(FontPath, 56))) {
        SDL_Log("SDL Error: %s", SDL_GetError());
        if (win->window) SDL_DestroyWindow(win->window);
        exit(EXIT_FAILURE);
    }
    win->width = win->InitialWidth = width;
    win->height = win->InitialHeight = height;
    win->quit = 0;
    win->state = MENU;

    loadBackground(&backgroundTexture, win->renderer, "assets/Title Screen/Title Screen Background.png");
    loadBackground(&backgroundTextureSettings, win->renderer, "assets/Battle Backgrounds/Other/zoonami_battle_party_background.png");
}

void destroyWindow(Window *win) 
{
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = NULL;
    }
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