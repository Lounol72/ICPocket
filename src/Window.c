#include "../include/Window.h"

void initWindow(Window *win, int width, int height, const char *FontPath) {
    InitLogFile();
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
    SDL_LogSetOutputFunction(LogToFile, NULL);
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "🔧 Initialisation de la fenêtre ..."); 

    if (SDL_Init(SDL_INIT_VIDEO) < 0 || 
        !(win->window = SDL_CreateWindow("ICPocket", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN)) || 
        !(win->renderer = SDL_CreateRenderer(win->window, -1, SDL_RENDERER_SOFTWARE)) || 
        (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) || 
        TTF_Init() == -1 || 
        !(win->LargeFont = TTF_OpenFont(FontPath,   24)) || 
        !(win->MediumFont = TTF_OpenFont(FontPath, 18)) || 
        !(win->SmallFont = TTF_OpenFont(FontPath, 14)) || 
        !(win->font = TTF_OpenFont(FontPath, 12))) {
        
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "❌ SDL Error: %s", SDL_GetError());
        if (win->window) SDL_DestroyWindow(win->window);
        exit(EXIT_FAILURE);
    }

    SDL_Log("Détails des flags pour le renderer :");
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ SDL Renderer Software: %d", SDL_RENDERER_SOFTWARE);
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ SDL Renderer Accelerated: %d", SDL_RENDERER_ACCELERATED);
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ SDL Renderer V-Sync: %s", 
        #if defined(SDL_RENDERER_PRESENTVSYNC)
            "Enabled"
        #else
            "Not available"
        #endif
    );
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ SDL Renderer Target Texture: %d", SDL_RENDERER_TARGETTEXTURE);
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ Utilisation des Flags: %d", renderer_flags);
    
    win->width = win->InitialWidth = width;
    win->height = win->InitialHeight = height;
    win->quit = 0;
    win->state = MENU;
    win->textSpeed = 1;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ Initialisation de la fenêtre réussie");    
}

void destroyWindow(Window *win)
{
    if (game.gameState.music) {
        Mix_FreeMusic(game.gameState.music);
        game.gameState.music = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ Musique libérée");
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
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ UI libérée");
    }

    if(game.battleState.rouge.nb_poke != 0) {
        for (int i = 0; i < game.battleState.rouge.nb_poke; i++) {
            destroyICMonsSprite(&game.battleState.rouge.team[i]);
            free(game.battleState.rouge.team[i].img);
        }
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ ICMons rougeslibérés");
        
    }
    if(game.battleState.bleu.nb_poke != 0) {
        for (int i = 0; i < game.battleState.bleu.nb_poke; i++) {
            destroyICMonsSprite(&game.battleState.bleu.team[i]);
            free(game.battleState.bleu.team[i].img);
        }
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ ICMons bleus libérés");
    }

    // 3) Free state handlers
    if (game.stateHandlers) {
        free(game.stateHandlers);
        game.stateHandlers = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ State handlers libérés");
    }

    // 4) Free speeds array
    if (game.speeds) {
        free(game.speeds);
        game.speeds = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ Speeds libérés");
    }

    // 5) Close fonts
    if (win->LargeFont) {
        TTF_CloseFont(win->LargeFont);
        win->LargeFont = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ LargeFont libéré");
    }
    if (win->MediumFont) {
        TTF_CloseFont(win->MediumFont);
        win->MediumFont = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ MediumFont libéré");
    }
    if (win->SmallFont) {
        TTF_CloseFont(win->SmallFont);
        win->SmallFont = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ SmallFont libéré");
    }
    if (win->font) {
        TTF_CloseFont(win->font);
        win->font = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ font libéré");
    }

    // 6) Destroy renderer & window
    if (win->renderer) {
        SDL_DestroyRenderer(win->renderer);
        win->renderer = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ renderer libéré");
        }
    if (win->window) {
        SDL_DestroyWindow(win->window);
        win->window = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ window libéré");
    }

    if (game.cursor) {
        SDL_FreeSurface(game.cursor);
        game.cursor = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ cursor libéré");
    }

    if (game.cursor_hover) {
        SDL_FreeSurface(game.cursor_hover);
        game.cursor_hover = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ cursor_hover libéré");
    }

    // 7) Close audio
    Mix_CloseAudio();
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ audio libéré");
    // 8) Quit TTF, IMG, SDL
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ SDL quitté");
}