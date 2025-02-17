#include "../include/window.h"
#include "../include/log.h"


void initWindow(Window *win, int width, int height, const char *FontPath) {
    InitLogFile();
    SDL_LogSetOutputFunction(LogToFile, NULL);
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "🔧 Initialisation de la fenêtre ..."); 
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || !(win->window = SDL_CreateWindow("ICPocket", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN)) || !(win->renderer = SDL_CreateRenderer(win->window, -1, SDL_RENDERER_SOFTWARE)) || (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) || TTF_Init() == -1 || !(win->LargeFont = TTF_OpenFont(FontPath, 70)) || !(win->MediumFont = TTF_OpenFont(FontPath, 56)) || !(win->SmallFont = TTF_OpenFont(FontPath, 24)) || !(win->font = TTF_OpenFont(FontPath, 18))) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "❌ SDL Error: %s", SDL_GetError());
        if (win->window) SDL_DestroyWindow(win->window);
        exit(EXIT_FAILURE);
    }
    

    win->width = win->InitialWidth = width;
    win->height = win->InitialHeight = height;
    win->quit = 0;
    win->state = MENU;
    win->textSpeed = 1;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ Initialisation de la fenêtre réussie");    
}

void destroyWindow(Window *win)
{
    // 1) Stop playing & free music
    if (game.gameState.music) {
        Mix_FreeMusic(game.gameState.music);
        game.gameState.music = NULL;
    }

    // 2) Destroy UI elements
    if (game.ui) {
        for (int i = 0; i < game.nbMenu; i++) {
            // Destroy button list
            if (game.ui[i].buttons) {
                destroyButtonList(game.ui[i].buttons);  
                free(game.ui[i].buttons);
                game.ui[i].buttons = NULL;
            }
            // Destroy slider list
            if (game.ui[i].sliders) {
                destroySliderList(game.ui[i].sliders);  
                free(game.ui[i].sliders);
                game.ui[i].sliders = NULL;
            }
            // Destroy background texture
            if (game.ui[i].background) {
                SDL_DestroyTexture(game.ui[i].background);
                game.ui[i].background = NULL;
            }
        }
        free(game.ui);
        game.ui = NULL;
    }

    // 3) Free state handlers
    if (game.stateHandlers) {
        free(game.stateHandlers);
        game.stateHandlers = NULL;
    }

    // 4) Free speeds array
    if (game.speeds) {
        free(game.speeds);
        game.speeds = NULL;
    }

    // 5) Close fonts
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

    // 6) Destroy renderer & window
    if (win->renderer) {
        SDL_DestroyRenderer(win->renderer);
        win->renderer = NULL;
    }
    if (win->window) {
        SDL_DestroyWindow(win->window);
        win->window = NULL;
    }

    // 7) Close audio
    Mix_CloseAudio();

    // 8) Quit TTF, IMG, SDL
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}


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

void renderText(Window * win, Text * text){
    SDL_RenderCopy(win->renderer, text->texture, NULL, &text->rect);
}

void handleQuitEvent(Window *win, SDL_Event *event) {
    win->quit = 1;
    handleEvent(win, event);
}