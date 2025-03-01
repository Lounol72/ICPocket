#include "../include/Window.h"

/**
 * @brief Fonction utilitaire pour nettoyer et quitter en cas d'erreur.
 *
 * @param win Pointeur sur la structure Window.
 * @param errorMsg Message d'erreur à afficher.
 */
static void cleanupAndExit(Window *win, const char *errorMsg) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "❌ %s: %s", errorMsg, SDL_GetError());
    if (win && win->window) {
        SDL_DestroyWindow(win->window);
    }
    exit(EXIT_FAILURE);
}

void initWindow(Window *win, int width, int height, const char *FontPath) {
    InitLogFile();
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
    SDL_LogSetOutputFunction(LogToFile, NULL);
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "🔧 Initialisation de la fenêtre ...");

    /* Initialisation de SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cleanupAndExit(win, "SDL_Init failed");
    }

    /* Création de la fenêtre */
    win->window = SDL_CreateWindow("ICPocket", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (!win->window) {
        cleanupAndExit(win, "SDL_CreateWindow failed");
    }

    /* Création du renderer en mode logiciel */
    win->renderer = SDL_CreateRenderer(win->window, -1, SDL_RENDERER_SOFTWARE);
    if (!win->renderer) {
        SDL_DestroyWindow(win->window);
        cleanupAndExit(win, "SDL_CreateRenderer failed");
    }

    /* Initialisation de la gestion des images PNG */
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        cleanupAndExit(win, "IMG_Init failed");
    }

    /* Initialisation de TTF */
    if (TTF_Init() == -1) {
        cleanupAndExit(win, "TTF_Init failed");
    }

    /* Chargement des polices */
    win->LargeFont = TTF_OpenFont(FontPath, 24);
    win->MediumFont = TTF_OpenFont(FontPath, 18);
    win->SmallFont = TTF_OpenFont(FontPath, 14);
    win->font = TTF_OpenFont(FontPath, 12);
    if (!win->LargeFont || !win->MediumFont || !win->SmallFont || !win->font) {
        cleanupAndExit(win, "TTF_OpenFont failed");
    }

    /* Affichage des informations sur les flags du renderer */
    SDL_Log("Détails des flags pour le renderer :");
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ SDL Renderer Software: %d", SDL_RENDERER_SOFTWARE);
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ SDL Renderer Accelerated: %d", SDL_RENDERER_ACCELERATED);
#if defined(SDL_RENDERER_PRESENTVSYNC)
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ SDL Renderer V-Sync: Enabled");
#else
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ SDL Renderer V-Sync: Not available");
#endif
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ SDL Renderer Target Texture: %d", SDL_RENDERER_TARGETTEXTURE);
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ Utilisation des Flags: %d", renderer_flags);

    /* Configuration des dimensions et des états initiaux de la fenêtre */
    win->width = win->InitialWidth = width;
    win->height = win->InitialHeight = height;
    win->quit = 0;
    win->state = MENU;
    win->textSpeed = 1;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ Initialisation de la fenêtre réussie");    
}

void destroyWindow(Window *win) {
    /* 1) Libération de la musique */
    if (game.gameState.music) {
        Mix_FreeMusic(game.gameState.music);
        game.gameState.music = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ Musique libérée");
    }

    /* 2) Destruction des éléments de l'interface (UI) */
    if (game.ui) {
        for (int i = 0; i < game.nbMenu; i++) {
            if (game.ui[i].buttons) {
                destroyButtonList(game.ui[i].buttons);
                free(game.ui[i].buttons);
                game.ui[i].buttons = NULL;
            }
            if (game.ui[i].sliders) {
                destroySliderList(game.ui[i].sliders);
                free(game.ui[i].sliders);
                game.ui[i].sliders = NULL;
            }
            if (game.ui[i].background) {
                SDL_DestroyTexture(game.ui[i].background);
                game.ui[i].background = NULL;
            }
        }
        free(game.ui);
        game.ui = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ UI libérée");
    }

    /* 3) Destruction des ICMons pour l'équipe rouge */
    if (game.battleState.rouge.nb_poke != 0) {
        for (int i = 0; i < game.battleState.rouge.nb_poke; i++) {
            destroyICMonsSprite(&game.battleState.rouge.team[i]);
        }
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ ICMons rouges libérés");
    }
    
    /* 4) Destruction des ICMons pour l'équipe bleue */
    if (game.battleState.bleu.nb_poke != 0) {
        for (int i = 0; i < game.battleState.bleu.nb_poke; i++) {
            destroyICMonsSprite(&game.battleState.bleu.team[i]);
        }
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ ICMons bleus libérés");
    }

    /* 5) Libération des state handlers */
    if (game.stateHandlers) {
        free(game.stateHandlers);
        game.stateHandlers = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ State handlers libérés");
    }

    /* 6) Libération du tableau de vitesses */
    if (game.speeds) {
        free(game.speeds);
        game.speeds = NULL;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ Speeds libérés");
    }

    /* 7) Fermeture des polices */
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

    /* 8) Destruction du renderer et de la fenêtre */
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

    /* 9) Libération des surfaces du curseur */
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

    /* 10) Fermeture de l'audio */
    Mix_CloseAudio();
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ audio libéré");

    /* 11) Quitter les sous-systèmes SDL, IMG et TTF */
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "✅ SDL quitté");
}
