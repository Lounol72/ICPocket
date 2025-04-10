/**
 * @file Events.c
 * @brief Gestion des événements et interactions utilisateur pour l'application.
 */

#include "../include/Events.h"


/**
 * @fn int initTeamSprites(Window *win, t_Team *teamSprite, float x_ratio, float y_ratio, int teamFlag)
 * @brief Initialise les sprites pour tous les pokémons d'une équipe.
 *
 * Cette fonction initialise le sprite de chaque pokémon dans une équipe en définissant les rectangles
 * de positionnement pour le sprite, le nom et la barre de points de vie.
 *
 * @note Le type @c Team doit être défini dans vos en-têtes (ex. via un typedef avec les champs @c nb_poke et @c team).
 *
 * @param win Pointeur sur la fenêtre.
 * @param teamSprite Pointeur sur la structure représentant l'équipe.
 * @param x_ratio Ratio de positionnement horizontal du sprite.
 * @param y_ratio Ratio de positionnement vertical du sprite.
 * @param teamFlag Indicateur d'équipe (0 pour l'équipe rouge, 1 pour l'équipe bleue).
 * @return int Retourne 0 en cas de succès, -1 en cas d'erreur.
 */
int initTeamSprites(Window *win, t_Team *teamSprite, float x_ratio, float y_ratio, int teamFlag) {
    for (int i = 0; i < teamSprite->nb_poke; i++) {
        t_Poke *poke = &(teamSprite->team[i]);
        SDL_Rect spriteRect = {
            .x = (int)(win->width * x_ratio),
            .y = (int)(win->height * y_ratio),
            .w = (int)(win->width * SPRITE_WIDTH_RATIO),
            .h = (int)(win->height * SPRITE_HEIGHT_RATIO)
        };
        SDL_Rect nameRect = {
            .x = spriteRect.x,
            .y = spriteRect.y - NAME_Y_OFFSET,
            .w = spriteRect.w / 2,
            .h = NAME_HEIGHT
        };
        SDL_Rect pvRect = {
            .x = spriteRect.x,
            .y = spriteRect.y + spriteRect.h + PV_Y_OFFSET,
            .w = spriteRect.w / 3,
            .h = PV_BAR_HEIGHT
        };
        if(poke->img) destroyICMonsSprite(poke);
        poke->img = initICMonSprite(win->renderer, spriteRect, nameRect, pvRect, poke, win->LargeFont, teamFlag);
        if (!poke->img || !poke->img->ICMonTexture) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, 
                "❌ Failed to initialize sprite for %s team pokemon %d", (teamFlag == 0 ? "red" : "blue"), i);
            return -1;
        }
        
        // Start entrance animation only for the first Pokemon of each team
        if (i == 0) {
            startICMonEntranceAnimation(poke);
        }
    }
    return 0;
}

/**
 * @fn void handleEvent(Window *win, SDL_Event *event)
 * @brief Gère un événement générique.
 *
 * Cette fonction traite les événements SDL généraux (clavier, manette, souris et fenêtre).
 *
 * @param win Pointeur sur la fenêtre.
 * @param event Pointeur sur l'événement SDL à traiter.
 */
void handleEvent(Window *win, SDL_Event *event) {
    if (!win || !event) return;
    
    SDL_Surface *newCursor = game.cursor;

    /* Gestion de la fermeture de l'application */
    if (event->type == SDL_QUIT) {
        win->quit = 1;
        SDL_LogMessage(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_INFO, "Quit");
        return;
    }

    const int maxButtons = game.ui[game.gameState.currentState].buttons ? 
                             game.ui[game.gameState.currentState].buttons->size - 1 : -1;
    const int halfMaxButtons = maxButtons / 2;

    switch (event->type) {
        case SDL_KEYDOWN: {
            SDL_Keycode key = event->key.keysym.sym;
            
            /* Touches globales */
            if (key == SDLK_DELETE) {
                win->quit = 1;
                SDL_LogMessage(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_INFO, "Quit");
                return;
            }

            /* Navigation dans les boutons */
            if (game.ui[game.gameState.currentState].buttons) {
                switch (key) {
                    case SDLK_UP: 
                        game.currentButton = (game.currentButton > 0) ? game.currentButton - 1 : game.currentButton; 
                        break;
                    case SDLK_DOWN: 
                        game.currentButton = (game.currentButton < maxButtons) ? game.currentButton + 1 : game.currentButton; 
                        break;
                    case SDLK_LEFT: 
                        game.currentButton = (game.currentButton >= halfMaxButtons) ? game.currentButton - halfMaxButtons : game.currentButton; 
                        break;
                    case SDLK_RIGHT: 
                        game.currentButton = (game.currentButton + halfMaxButtons <= maxButtons) ? game.currentButton + halfMaxButtons : game.currentButton; 
                        break;
                    case SDLK_c: 
                        ButtonClicked(game.ui[game.gameState.currentState].buttons->buttons[game.currentButton], -1, -1, win); 
                        break;
                    case SDLK_F1:
                        changeState(win, &game.stateHandlers[9].state);
                        break;
                }
            }
        } break;
        
        case SDL_CONTROLLERBUTTONDOWN: {
            if (!game.ui[game.gameState.currentState].buttons) break;
            
            switch (event->cbutton.button) {
                case SDL_CONTROLLER_BUTTON_A: 
                    ButtonClicked(game.ui[game.gameState.currentState].buttons->buttons[game.currentButton], -1, -1, win); 
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_UP: 
                    game.currentButton = (game.currentButton > 0) ? game.currentButton - 1 : game.currentButton; 
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN: 
                    game.currentButton = (game.currentButton < maxButtons) ? game.currentButton + 1 : game.currentButton; 
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT: 
                    game.currentButton = (game.currentButton >= halfMaxButtons) ? game.currentButton - halfMaxButtons : game.currentButton; 
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: 
                    game.currentButton = (game.currentButton + halfMaxButtons <= maxButtons) ? game.currentButton + halfMaxButtons : game.currentButton; 
                    break;
            }
        } break;

        case SDL_MOUSEMOTION:
            if (game.ui[game.gameState.currentState].buttons) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                for (int i = 0; i < game.ui[game.gameState.currentState].buttons->size; i++) {
                    Button *button = game.ui[game.gameState.currentState].buttons->buttons[i];
                    if (x >= button->rect.x && x <= button->rect.x + button->rect.w &&
                        y >= button->rect.y && y <= button->rect.y + button->rect.h) {
                        game.currentButton = i;
                        newCursor = game.cursor_hover;
                        break;
                    }
                }
                SDL_SetCursor(SDL_CreateColorCursor(newCursor, 0, 0));
            }
            break;

        case SDL_WINDOWEVENT:
            if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
                handleWindowSizeChange(win);
                updateCameraViewport(game.gameData.camera, win->width, win->height);
            }
            break;
    }
}

/**
 * @fn void handleWindowSizeChange(Window *win)
 * @brief Gère le redimensionnement de la fenêtre.
 *
 * Met à jour les dimensions de la fenêtre et réajuste les positions et tailles des éléments de l'interface.
 *
 * @param win Pointeur sur la fenêtre.
 */
void handleWindowSizeChange(Window *win) {
    SDL_GetWindowSize(win->window, &win->width, &win->height);
    float scaleX = (float)win->width / win->InitialWidth;
    float scaleY = (float)win->height / win->InitialHeight;
    for (int i = 1; i < game.nbMenu; i++) {
        if (game.ui[i].buttons) updateButtonPosition(game.ui[i].buttons, scaleX, scaleY);
        if (game.ui[i].sliders) updateSliderPosition(game.ui[i].sliders, scaleX, scaleY);
    }
    updateTextPosition(&NewGameText, scaleX, scaleY);
    updateTextPosition(&title, scaleX, scaleY);
    updateTextPosition(game.windowText, scaleX, scaleY);

    for (int i = 0; i < 3; i++) {
         if (game.touche[i]) {            
            updateImageSize(game.touche[i], scaleX, scaleY);
        }
    }
    for (int i = 0; i < game.battleState.rouge.nb_poke; i++) {
        updateICMonsSprite(&(game.battleState.rouge.team[i]), scaleX, scaleY);
    }
    for (int i = 0; i < game.battleState.bleu.nb_poke; i++) {
        updateICMonsSprite(&(game.battleState.bleu.team[i]), scaleX, scaleY);
    }
    
}

/**
 * @fn void handleICMonsEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements spécifiques à la sélection des ICMons.
 *
 * Traite les clics souris et la touche Échap lors de la sélection.
 *
 * @param win Pointeur sur la fenêtre.
 * @param event Pointeur sur l'événement SDL.
 */
void handleICMonsEvent(Window *win, SDL_Event *event) {
    if (!win || !event) return;
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[game.gameState.currentState].buttons->size; i++) {
            ButtonClicked(game.ui[game.gameState.currentState].buttons->buttons[i], x, y, win);
        }
    }
    if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
        changeState(win, &game.stateHandlers[2].state);
    }
    handleEvent(win, event);
}

/**
 * @fn void handleIntermediateEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements dans l'état intermédiaire.
 *
 * Traite notamment les clics sur les boutons et délègue le reste au gestionnaire générique.
 *
 * @param win Pointeur sur la fenêtre.
 * @param event Pointeur sur l'événement SDL.
 */
void handleIntermediateEvent(Window *win, SDL_Event *event) {
    if (!win || !event) return;
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[game.gameState.currentState].buttons->size; i++) {
            ButtonClicked(game.ui[game.gameState.currentState].buttons->buttons[i], x, y, win);
        }
    }
    const Uint8 *keyState = SDL_GetKeyboardState(NULL);

    // Si le joueur appuie sur E, lancer le combat
    if (keyState[SDL_SCANCODE_E]) {
        changeState(win, &game.stateHandlers[GAME].state); // Passer à l'état de combat
    }
    // Si le joueur appuie sur Échap, annuler l'interaction
    else if (keyState[SDL_SCANCODE_ESCAPE]) {
        changeState(win, &game.stateHandlers[MAP].state);
    }
    handleEvent(win, event);
}

/** 
 * @fn void handleSwapEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements d'échange entre un ICmon du joueur et de l'IA.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void handleSwapEvent(Window *win, SDL_Event *event) {
    if (!win || !event) return;
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[game.gameState.currentState].buttons->size; i++) {
            ButtonClicked(game.ui[game.gameState.currentState].buttons->buttons[i], x, y, win);
        }
    }
    handleEvent(win, event);
}

/**
 * @fn void handleStartersEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements liés au choix des starters.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void handleStartersEvent(Window *win, SDL_Event *event) {
    if (!win || !event) return;
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[game.gameState.currentState].buttons->size; i++) {
            ButtonClicked(game.ui[game.gameState.currentState].buttons->buttons[i], x, y, win);
        }
    }
    handleEvent(win, event);
}

/** 
 * @fn void handleLearningEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements lorsque le joueur apprend une nouvelle capacité.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void handleLearningEvent(Window *win, SDL_Event *event){
    if (!win || !event) return;
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[game.gameState.currentState].buttons->size; i++) {
            ButtonClicked(game.ui[game.gameState.currentState].buttons->buttons[i], x, y, win);
        }
    }
    handleEvent(win, event);
}

/**
 * @fn void handleResumeEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements liés à la reprise d'une partie.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void handleResumeEvent(Window *win, SDL_Event *event){
    if (!win || !event) return;
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[game.gameState.currentState].buttons->size; i++) {
            ButtonClicked(game.ui[game.gameState.currentState].buttons->buttons[i], x, y, win);
        }
    }
    handleEvent(win, event);
}

/**
 * @fn void handleMenuEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements du menu.
 *
 * Traite les clics sur les boutons et délègue le reste au gestionnaire générique.
 *
 * @param win Pointeur sur la fenêtre.
 * @param event Pointeur sur l'événement SDL.
 */
void handleMenuEvent(Window *win, SDL_Event *event) {
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[game.gameState.currentState].buttons->size; i++) {
            ButtonClicked(game.ui[game.gameState.currentState].buttons->buttons[i], x, y, win);
        }
    }
    handleEvent(win, event);
}

/**
 * @fn void handleGameEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements durant le jeu.
 *
 * Contrôle la transition d'état en fonction de la vie des équipes et gère la navigation (clavier/souris).
 *
 * @param win Pointeur sur la fenêtre.
 * @param event Pointeur sur l'événement SDL.
 */
void handleGameEvent(Window *win, SDL_Event *event) {
    if (!game.scrollingTextIntro->isComplete) return;
    if ((!isTeamAlive(&game.battleState.rouge) || !isTeamAlive(&game.battleState.bleu)) && game.battleState.turnState == TURN_NONE) {
        /* Réinitialisation de l'état du jeu */
        game.gameState.playerTurn = 0;
        AppState newState = isTeamAlive(&game.battleState.rouge) ? INTER : QUIT;
        if (newState == QUIT) saveDestroy(&game.battleState.rouge);
        win->state = newState;
        game.gameState.currentState = newState;
        //game.isInDuel = false;
    }

    if (!game.gameState.playerTurn && isTeamAlive(&game.battleState.rouge) && isTeamAlive(&game.battleState.bleu)) {
        if (!isAlive(&(game.battleState.rouge.team[0]))) {
            game.gameState.currentState = ICMONS;
            win->state = ICMONS;
            return;
        }
        game.gameState.playerTurn = 1;
    }
    if (game.battleState.turnState != TURN_NONE) return;
    if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
        changeState(win, &game.stateHandlers[SETTINGS].state);
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (game.battleState.turnState == TURN_NONE) {
            for (int i = 0; i < game.ui[game.gameState.currentState].buttons->size; i++) {
                ButtonClicked(game.ui[game.gameState.currentState].buttons->buttons[i], x, y, win);
            }
        }
    }
    handleEvent(win, event);
}

/**
 * @fn void handleSettingsEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements dans les paramètres.
 *
 * Traite les événements liés aux sliders et aux boutons de l'écran de réglages.
 *
 * @param win Pointeur sur la fenêtre.
 * @param event Pointeur sur l'événement SDL.
 */
void handleSettingsEvent(Window *win, SDL_Event *event) {
    if (!win || !event || !game.ui[game.gameState.currentState].sliders->sliders ||
        game.ui[game.gameState.currentState].sliders->size <= 0)
        return;
    handleSliderEvent(game.ui[game.gameState.currentState].sliders->sliders[0], event);
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[game.gameState.currentState].buttons->size; i++) {
            ButtonClicked(game.ui[game.gameState.currentState].buttons->buttons[i], x, y, win);
        }
        
    }

    handleEvent(win, event);
}

/**
 * @fn void handleQuitEvent(Window *win, SDL_Event *event)
 * @brief Gère l'événement de fermeture de l'application.
 *
 * Marque la fenêtre pour la fermeture et délègue le traitement générique.
 *
 * @param win Pointeur sur la fenêtre.
 * @param event Pointeur sur l'événement SDL.
 */
void handleQuitEvent(Window *win, SDL_Event *event) {
    win->quit = 1;
    handleEvent(win, event);
}

/**
 * @fn void handleNewGameEvent(Window *win, SDL_Event *event)
 * @brief Gère le démarrage d'une nouvelle partie.
 *
 * Initialise les données, les équipes et les sprites correspondants si le jeu n'est pas déjà initialisé.
 * Sinon, change l'état vers le menu approprié.
 *
 * @param win Pointeur sur la fenêtre.
 * @param event Pointeur sur l'événement SDL.
 */
void handleNewGameEvent(Window *win, SDL_Event *event) {
    handleEvent(win, event);
    
    if (!game.gameState.initialized) {
        initData();
        srand(time(NULL));
        initTeam(&game.battleState.rouge, 3);
        strcpy(game.battleState.rouge.trainerName, "GoofyStudent");
        initBlueTeam(&game.battleState.bleu, &game.battleState.rouge);
        game.battleState.ia = (t_AI){10, damageOnly, &game.battleState.bleu};
        
        if (initTeamSprites(win, &game.battleState.rouge, RED_SPRITE_X_RATIO, RED_SPRITE_Y_RATIO, 0) != 0)
            return;
        if (initTeamSprites(win, &game.battleState.bleu, BLUE_SPRITE_X_RATIO, BLUE_SPRITE_Y_RATIO, 1) != 0)
            return;
        
        updateICButtons(win, &game.battleState.rouge);
        
        game.gameState.initialized = 1;
    } else {
        changeState(win, &game.stateHandlers[GAME].state);
    }
}



/**
 * @fn void handleLoadGameEvent(Window *win, SDL_Event *event)
 * @brief Gère le chargement d'une partie sauvegardée.
 *
 * Charge les données sauvegardées, initialise les équipes et leurs sprites, puis met à jour l'interface.
 *
 * @param win Pointeur sur la fenêtre.
 * @param event Pointeur sur l'événement SDL.
 */
 /* 
void handleLoadGameEvent(Window *win, SDL_Event *event) {
    
    if (!win || !event) return;
    srand(time(NULL));
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        int clickedButtonIndex = -1;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[game.gameState.currentState].buttons->size; i++) {
            ButtonClicked(game.ui[game.gameState.currentState].buttons->buttons[i], x, y, win);
            clickedButtonIndex = i; 
        }
        initData();
        if(clickedButtonIndex == 0)charger("Save_1", &game.battleState.rouge, &game.battleState.bleu);  
        else if(clickedButtonIndex == 1)charger("Save_2", &game.battleState.rouge, &game.battleState.bleu);  
        initBlueTeam(&game.battleState.bleu, &game.battleState.rouge);
        game.battleState.ia = (t_AI){10, damageOnly, &game.battleState.bleu};
        if (initTeamSprites(win, &game.battleState.rouge, RED_SPRITE_X_RATIO, RED_SPRITE_Y_RATIO, 0) != 0)return;
        if (initTeamSprites(win, &game.battleState.bleu, BLUE_SPRITE_X_RATIO, BLUE_SPRITE_Y_RATIO, 1) != 0)return;
        updateICButtons(win, &game.battleState.rouge);
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, 
        "Red team PV: %d, Blue team PV: %d", 
        game.battleState.rouge.team[0].current_pv,
        game.battleState.bleu.team[0].current_pv);        
        game.gameState.initialized = 1; 
    }
    handleEvent(win, event);
}*/
void handleLoadGameEvent(Window *win, SDL_Event *event) {
    
    
    if (!win || !event) return;
    srand(time(NULL));
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[game.gameState.currentState].buttons->size; i++) {
            ButtonClicked(game.ui[game.gameState.currentState].buttons->buttons[i], x, y, win);

        }
    } 
    handleEvent(win, event);
}

/** 
 * @fn void loadFile(Window *win, void *event)
 * @brief Gère les événements liés au chargement d'une partie sauvegardée.
 * 
 * @param win Pointeur vers la structure Window contenant l'état du jeu.
 * @param event Pointeur vers l'événement SDL à traiter.
 */
void loadFile(Window *win, void *event){
    if(!game.gameState.initialized){
        initData();
        char data[50];
        snprintf(data,50,"%s",(char*)event);
        if((charger(data, &game.battleState.rouge, &game.battleState.bleu))==-1){
            initTeam(&game.battleState.rouge, 3);
            return;
        }

        initBlueTeam(&game.battleState.bleu, &game.battleState.rouge);
        game.battleState.ia = (t_AI){10, damageOnly, &game.battleState.bleu};
        
        if (initTeamSprites(win, &game.battleState.rouge, RED_SPRITE_X_RATIO, RED_SPRITE_Y_RATIO, 0) != 0)
            return;
        if (initTeamSprites(win, &game.battleState.bleu, BLUE_SPRITE_X_RATIO, BLUE_SPRITE_Y_RATIO, 1) != 0)
            return;
        
        updateICButtons(win, &game.battleState.rouge);
        
        game.gameState.initialized = 1;
        changeState(win, &game.stateHandlers[MAP].state);
        handleEvent(win, event);

    }
    handleEvent(win, event);
}



/**
 * @fn void handlePauseEvent(Window *win, SDL_Event *event)
 * @brief Gère les événements de pause.
 *
 * Permet de quitter le mode pause en appuyant sur la touche Échap.
 *
 * @param win Pointeur sur la fenêtre.
 * @param event Pointeur sur l'événement SDL.
 */
void handlePauseEvent(Window *win, SDL_Event *event) {
    if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
        changeState(win, &game.stateHandlers[2].state);
    }
    handleEvent(win, event);
}

/**
 * @fn void handlePlayerEvent(Window *win, SDL_Event *event)
 * @brief Gère les déplacements et actions du joueur.
 *
 * Cette fonction vérifie les touches fléchées pour déplacer le joueur si le déplacement est autorisé,
 * met à jour la position cible et initialise l'animation du mouvement.
 * Elle vérifie également si le joueur est sur une case spéciale pour changer de carte.
 *
 * @param win Pointeur sur la fenêtre.
 * @param event Pointeur sur l'événement SDL.
 */
void handlePlayerEvent(Window *win, SDL_Event *event) {
    (void)win;
    (void)event;
    
    // Si l'état du jeu n'est pas MAP, ne pas traiter les entrées de déplacement
    if (game.gameState.currentState != MAP) {
        return;
    }
    
    // Vérifier si le joueur est sur une case de transition après un mouvement
    if (!game.gameData.player->isMovingToTarget) {
        Map *currentMap = game.gameData.maps[game.gameData.player->mapIndex];
        if (currentMap) {
                   
            // Vérifier si une transition doit être effectuée
            if (checkAndLoadNewMap(game.gameData.player, currentMap, game.gameData.camera, game.win->renderer)) {
                return;
            }
        }
    }
    
    const Uint8 *keyState = SDL_GetKeyboardState(NULL);
    int newMatrixX = game.gameData.player->matrixX;
    int newMatrixY = game.gameData.player->matrixY;
    PlayerState newState = game.gameData.player->state;
    bool shouldMove = false;

    if (keyState[SDL_SCANCODE_RIGHT]) {
        if (newMatrixX < MAP_WIDTH - 1 && !game.gameData.player->isMovingToTarget) {
            newMatrixX++;
            newState = WALK_RIGHT;
            shouldMove = true;
        }
    }
    else if (keyState[SDL_SCANCODE_LEFT]) {
        if (newMatrixX > 0 && !game.gameData.player->isMovingToTarget) {
            newMatrixX--;
            newState = WALK_LEFT;
            shouldMove = true;
        }
    }
    else if (keyState[SDL_SCANCODE_DOWN]) {
        if (newMatrixY < MAP_HEIGHT - 1 && !game.gameData.player->isMovingToTarget) {
            newMatrixY++;
            newState = WALK_DOWN;
            shouldMove = true;
        }
    }
    else if (keyState[SDL_SCANCODE_UP]) {
        if (newMatrixY > 0 && !game.gameData.player->isMovingToTarget) {
            newMatrixY--;
            newState = WALK_UP;
            shouldMove = true;
        }
    }
    else if (keyState[SDL_SCANCODE_ESCAPE]) {
        changeState(game.win, &game.stateHandlers[MENU].state);
    }
    else if (keyState[SDL_SCANCODE_DELETE]) {
        changeState(game.win, &game.stateHandlers[QUIT].state);
    }
    else if (keyState[SDL_SCANCODE_E] && game.gameData.maps[game.gameData.player->mapIndex]->mat[newMatrixY-1][newMatrixX] == 6 && game.gameState.currentState == MAP) {
            nextDuel(game.win, NULL);
            game.isInDuel = true;
    }
    
    if (shouldMove && game.gameData.maps[game.gameData.player->mapIndex]->mat[newMatrixY][newMatrixX] != COLLISION && game.gameData.maps[game.gameData.player->mapIndex]->mat[newMatrixY][newMatrixX] != DUEL) {
        game.gameData.player->startX = game.gameData.player->position.x;
        game.gameData.player->startY = game.gameData.player->position.y;
        game.gameData.player->targetMatrixX = newMatrixX;
        game.gameData.player->targetMatrixY = newMatrixY;
        game.gameData.player->targetX = newMatrixX * TILE_SIZE_W_SCALE;
        game.gameData.player->targetY = newMatrixY * TILE_SIZE_H_SCALE;
        game.gameData.player->state = newState;
        game.gameData.player->isMovingToTarget = true;
        game.gameData.player->interpolationTime = 0.0f;
    }
}
