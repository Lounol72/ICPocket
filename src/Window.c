#include "../include/Window.h"

/**
 * @fn void cleanupAndExit(Window *win, const char *errorMsg)
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

/**
 * @fn void initWindow(Window *win, int width, int height, const char *FontPath)
 * @brief Initialise la fenêtre principale de l'application.
 * 
 * Cette fonction initialise la fenêtre principale de l'application avec les dimensions et le chemin de police spécifiés.
 * 
 * @param win Un pointeur vers la structure Window à initialiser.
 * @param width La largeur initiale de la fenêtre.
 * @param height La hauteur initiale de la fenêtre.
 * @param FontPath Le chemin du fichier de police à utiliser.
 */
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


