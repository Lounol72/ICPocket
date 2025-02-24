#include "../include/Events.h"

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
        
        case SDL_CONTROLLERBUTTONDOWN: { int max = game.ui[game.gameState.currentState].buttons->size - 1; switch(event->cbutton.button){ case SDL_CONTROLLER_BUTTON_A: ButtonClicked(game.ui[game.gameState.currentState].buttons->buttons[game.currentButton], -1, -1, win); break; case SDL_CONTROLLER_BUTTON_DPAD_UP: game.currentButton = game.currentButton > 0 ? game.currentButton - 1 : game.currentButton; break; case SDL_CONTROLLER_BUTTON_DPAD_DOWN: game.currentButton = game.currentButton < max ? game.currentButton + 1 : game.currentButton; break; case SDL_CONTROLLER_BUTTON_DPAD_LEFT: game.currentButton = game.currentButton >= max/2 ? game.currentButton - max/2 : game.currentButton; break; case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: game.currentButton = game.currentButton + max/2 <= max ? game.currentButton + max/2 : game.currentButton; break; default: break; } } break;
        case SDL_QUIT:
            win->quit = 1;
            SDL_LogMessage(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_INFO, "Quit");
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

// Functions for the ICMons selection

void handleICMonsEvent(Window *win, SDL_Event *event) {
    if (!win || !event) return;
    if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[6].buttons->size; i++) {
            ButtonClicked(game.ui[6].buttons->buttons[i], x, y, win);
        }
    }
    if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
        changeState(win, &game.stateHandlers[2].state);
    }
    handleEvent(win, event);
}

//---------------------------------------------------------------------------------

// Functions for intermediate

void handleIntermediateEvent(Window *win, SDL_Event *event) {
    if (!win || !event) return;
    if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[7].buttons->size; i++) {
            ButtonClicked(game.ui[7].buttons->buttons[i], x, y, win);
        }
    }
    if(!game.saved){

    }
    handleEvent(win, event);
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

void handleGameEvent(Window *win, SDL_Event *event) {
    if (!isTeamAlive(&game.battleState.rouge) || !isTeamAlive(&game.battleState.bleu)) {
        
        // Reset game state
        game.gameState.initialized = 0;
        game.gameState.playerTurn = 0;
        
        // Change state
        AppState newState = isTeamAlive(&game.battleState.rouge) ? INTER : MENU;
        win->state = newState;
        game.gameState.currentState = newState;
        return;
    }

    // Rest of the function remains the same
    if (!game.gameState.playerTurn && isTeamAlive(&game.battleState.rouge) && isTeamAlive(&game.battleState.bleu)) {
        if (!isAlive(&(game.battleState.rouge.team[0]))) {
            game.gameState.currentState = ICMONS;
            win->state = ICMONS;
            return;
        }
        game.gameState.playerTurn = 1;
    }
    
    if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
        changeState(win, &game.stateHandlers[2].state);
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[3].buttons->size; i++) {
            ButtonClicked(game.ui[3].buttons->buttons[i], x, y, win);
        }
    }
    handleEvent(win, event);
}

//---------------------------------------------------------------------------------

// Functions for the settings

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

void handleQuitEvent(Window *win, SDL_Event *event) {
    win->quit = 1;
    handleEvent(win, event);
}

//---------------------------------------------------------------------------------

// Functions for new game

void handleNewGameEvent(Window * win, SDL_Event * event) {
    handleEvent(win, event);
    if (!game.gameState.initialized) {
        initData();
        initTeam(&game.battleState.rouge, 3);
        initTeam(&game.battleState.bleu, 3);
        game.battleState.ia = (t_AI){10, damageOnly, &game.battleState.bleu};
        
        // Initialize sprites for both teams
        for(int i = 0; i < game.battleState.rouge.nb_poke; i++) {
            t_Poke *poke = &(game.battleState.rouge.team[i]);
            poke->img = initICMonSprite(win->renderer, 250, 275, 200, 200, poke, win->LargeFont);
            if (!poke->img || !poke->img->ICMonTexture) {
                SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, 
                    "❌ Failed to initialize sprite for red team pokemon %d", i);
                return;
            }
        }
        
        for(int i = 0; i < game.battleState.bleu.nb_poke; i++) {
            t_Poke *poke = &(game.battleState.bleu.team[i]);
            poke->img = initICMonSprite(win->renderer, 750, 100, 200, 200, poke, win->LargeFont);
            if (!poke->img || !poke->img->ICMonTexture) {
                SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, 
                    "❌ Failed to initialize sprite for blue team pokemon %d", i);
                return;
            }
        }

        updateICButtons(win, &game.battleState.rouge);
        
        // Debug info
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, 
            "Red team PV: %d, Blue team PV: %d", 
            game.battleState.rouge.team[0].current_pv,
            game.battleState.bleu.team[0].current_pv);
            
        game.gameState.initialized = 1;
    }
}

//---------------------------------------------------------------------------------

// Functions for load game


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