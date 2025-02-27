#include "../include/Events.h"

// Constants for sprite positioning and sizing
#define RED_SPRITE_X_RATIO 0.23f
#define RED_SPRITE_Y_RATIO 0.35f
#define BLUE_SPRITE_X_RATIO 0.60f 
#define BLUE_SPRITE_Y_RATIO 0.15f
#define SPRITE_WIDTH_RATIO 0.2f
#define SPRITE_HEIGHT_RATIO 0.25f
#define NAME_Y_OFFSET 25
#define PV_Y_OFFSET 25
#define NAME_HEIGHT 20
#define PV_BAR_HEIGHT 15


void handleEvent(Window *win, SDL_Event *event) {
    // Early exit if no window or event
    if (!win || !event) return;

    // Handle quit events first
    if (event->type == SDL_QUIT) {
        win->quit = 1;
        SDL_LogMessage(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_INFO, "Quit");
        return;
    }


    const int maxButtons = game.ui[game.gameState.currentState].buttons ? game.ui[game.gameState.currentState].buttons->size - 1 : -1;
    const int halfMaxButtons = maxButtons / 2;

    switch(event->type) {
        case SDL_KEYDOWN: {
            SDL_Keycode key = event->key.keysym.sym;
            
            // Handle global keys first
            if (key == SDLK_DELETE) {
                win->quit = 1;
                SDL_LogMessage(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_INFO, "Quit");
                return;
            }
            
            if (key == SDLK_ESCAPE && game.gameState.currentState == MAP) {
                changeState(win, &game.stateHandlers[2].state);
                return;
            }

            // Handle button navigation if buttons exist
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
            
            switch(event->cbutton.button) {
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
                        break;
                    }
                }
            }
            break;

        case SDL_WINDOWEVENT:
            if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
                handleWindowSizeChange(win);
            }
            break;
    }
}

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
    
    for(int i = 0; i < game.battleState.rouge.nb_poke; i++) {
        updateICMonsSprite(&(game.battleState.rouge.team[i]), scaleX, scaleY);
    }
    for(int i = 0; i < game.battleState.bleu.nb_poke; i++) {
        updateICMonsSprite(&(game.battleState.bleu.team[i]), scaleX, scaleY);
    }
    
}

// Functions for the ICMons selection

void handleICMonsEvent(Window *win, SDL_Event *event) {
    if (!win || !event) return;
    if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
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

//---------------------------------------------------------------------------------

// Functions for intermediate

void handleIntermediateEvent(Window *win, SDL_Event *event) {
    if (!win || !event) return;
    if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[game.gameState.currentState].buttons->size; i++) {
            ButtonClicked(game.ui[game.gameState.currentState].buttons->buttons[i], x, y, win);
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
        for (int i = 0; i < game.ui[game.gameState.currentState].buttons->size; i++) {
            ButtonClicked(game.ui[game.gameState.currentState].buttons->buttons[i], x, y, win);
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
        for (int i = 0; i < game.ui[game.gameState.currentState].buttons->size; i++) {
            ButtonClicked(game.ui[game.gameState.currentState].buttons->buttons[i], x, y, win);
        }
    }
    handleEvent(win, event);
}

//---------------------------------------------------------------------------------

// Functions for the settings

void handleSettingsEvent(Window *win, SDL_Event *event) {
    if (!win || !event || !game.ui[game.gameState.currentState].sliders->sliders || game.ui[game.gameState.currentState].sliders->size <= 0) return;
    // Parcourt et gère les événements des sliders
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[game.gameState.currentState].sliders->size; i++) {
            if (handleSliderEvent(game.ui[game.gameState.currentState].sliders->sliders[i], x,y)) break;
        }
        for(int i = 0; i < game.ui[game.gameState.currentState].buttons->size; i++){
            ButtonClicked(game.ui[game.gameState.currentState].buttons->buttons[i], x, y, win);
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
            SDL_Rect spriteRect = {
                win->width * RED_SPRITE_X_RATIO, 
                win->height * RED_SPRITE_Y_RATIO,
                win->width * SPRITE_WIDTH_RATIO,
                win->height * SPRITE_HEIGHT_RATIO
            };
            SDL_Rect nameRect = {
                spriteRect.x,
                spriteRect.y - NAME_Y_OFFSET,
                spriteRect.w / 2,
                NAME_HEIGHT
            };
            SDL_Rect pvRect = {
                spriteRect.x,
                spriteRect.y + spriteRect.h + PV_Y_OFFSET,
                spriteRect.w / 3,
                PV_BAR_HEIGHT
            };
            poke->img = initICMonSprite(win->renderer, spriteRect, nameRect, pvRect, poke, win->LargeFont, 0);
            if (!poke->img || !poke->img->ICMonTexture) {
                SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, 
                    "❌ Failed to initialize sprite for red team pokemon %d", i);
                return;
            }
        }
        
        for(int i = 0; i < game.battleState.bleu.nb_poke; i++) {
            t_Poke *poke = &(game.battleState.bleu.team[i]);
            SDL_Rect spriteRect = {
                win->width * BLUE_SPRITE_X_RATIO,
                win->height * BLUE_SPRITE_Y_RATIO,
                win->width * SPRITE_WIDTH_RATIO,
                win->height * SPRITE_HEIGHT_RATIO
            };
            SDL_Rect nameRect = {
                spriteRect.x,
                spriteRect.y - NAME_Y_OFFSET,
                spriteRect.w/2,
                NAME_HEIGHT
            };
            SDL_Rect pvRect = {
                spriteRect.x,
                spriteRect.y + spriteRect.h + PV_Y_OFFSET,
                spriteRect.w / 3,
                PV_BAR_HEIGHT
            };
            poke->img = initICMonSprite(win->renderer, spriteRect, nameRect, pvRect, poke, win->LargeFont, 1);
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
    }else{
        changeState(win, &game.stateHandlers[3].state);
    }
}

//---------------------------------------------------------------------------------

// Functions for load game


void handleLoadGameEvent(Window *win, SDL_Event *event) {
    if (!win || !event) return;
    
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < game.ui[game.gameState.currentState].buttons->size; i++) {
            ButtonClicked(game.ui[game.gameState.currentState].buttons->buttons[i], x, y, win);
        }
    }
    // Gérer les autres événements
    handleEvent(win, event);
    if (!game.gameState.initialized) {
        initData();
        initTeam(&game.battleState.bleu, 3);
        charger("Save_1",&game.battleState.rouge, &game.battleState.bleu);
        game.battleState.ia = (t_AI){10, damageOnly, &game.battleState.bleu};
        
        // Initialize sprites for both teams

        for(int i = 0; i < game.battleState.rouge.nb_poke; i++) {
            t_Poke *poke = &(game.battleState.rouge.team[i]);
            SDL_Rect spriteRect = {
                win->width * RED_SPRITE_X_RATIO, 
                win->height * RED_SPRITE_Y_RATIO,
                win->width * SPRITE_WIDTH_RATIO,
                win->height * SPRITE_HEIGHT_RATIO
            };
            SDL_Rect nameRect = {
                spriteRect.x,
                spriteRect.y - NAME_Y_OFFSET,
                spriteRect.w / 2,
                NAME_HEIGHT
            };
            SDL_Rect pvRect = {
                spriteRect.x,
                spriteRect.y + spriteRect.h + PV_Y_OFFSET,
                spriteRect.w / 3,
                PV_BAR_HEIGHT
            };
            poke->img = initICMonSprite(win->renderer, spriteRect, nameRect, pvRect, poke, win->LargeFont, 0);
            if (!poke->img || !poke->img->ICMonTexture) {
                SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, 
                    "❌ Failed to initialize sprite for red team pokemon %d", i);
                return;
            }
        }
        
        for(int i = 0; i < game.battleState.bleu.nb_poke; i++) {
            t_Poke *poke = &(game.battleState.bleu.team[i]);
            SDL_Rect spriteRect = {
                win->width * BLUE_SPRITE_X_RATIO,
                win->height * BLUE_SPRITE_Y_RATIO,
                win->width * SPRITE_WIDTH_RATIO,
                win->height * SPRITE_HEIGHT_RATIO
            };
            SDL_Rect nameRect = {
                spriteRect.x,
                spriteRect.y - NAME_Y_OFFSET,
                spriteRect.w/2,
                NAME_HEIGHT
            };
            SDL_Rect pvRect = {
                spriteRect.x,
                spriteRect.y + spriteRect.h + PV_Y_OFFSET,
                spriteRect.w / 3,
                PV_BAR_HEIGHT
            };
            poke->img = initICMonSprite(win->renderer, spriteRect, nameRect, pvRect, poke, win->LargeFont, 1);
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

// Functions for pause

void handlePauseEvent(Window *win, SDL_Event *event) {
    if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
        changeState(win, &game.stateHandlers[2].state);
    }
    handleEvent(win, event);
}

//---------------------------------------------------------------------------------

//Function for the Player

void handlePlayerEvent(Window *win, SDL_Event *event) {
    (void)win;
    (void)event;
    
    const Uint8 *keyState = SDL_GetKeyboardState(NULL);
    int newMatrixX = game.gameData.player->matrixX;
    int newMatrixY = game.gameData.player->matrixY;
    PlayerState newState = game.gameData.player->state;
    bool shouldMove = false;

    if (keyState[SDL_SCANCODE_RIGHT]) {
        if(newMatrixX < MAP_WIDTH - 1 && !game.gameData.player->isMovingToTarget) {
            newMatrixX++;
            newState = WALK_RIGHT;
            shouldMove = true;
        }
    }
    else if (keyState[SDL_SCANCODE_LEFT]) {
        if(newMatrixX > 0 && !game.gameData.player->isMovingToTarget) {
            newMatrixX--;
            newState = WALK_LEFT;
            shouldMove = true;
        }
    }
    else if (keyState[SDL_SCANCODE_DOWN]) {
        if(newMatrixY < MAP_HEIGHT - 1 && !game.gameData.player->isMovingToTarget) {
            newMatrixY++;
            newState = WALK_DOWN;
            shouldMove = true;
        }
    }
    else if (keyState[SDL_SCANCODE_UP]) {
        if(newMatrixY > 0 && !game.gameData.player->isMovingToTarget) {
            newMatrixY--;
            newState = WALK_UP;
            shouldMove = true;
        }
    }

    if (shouldMove && game.gameData.map->mat[newMatrixY][newMatrixX] != COLLISION) {
        // Sauvegarder la position de départ
        game.gameData.player->startX = game.gameData.player->position.x;
        game.gameData.player->startY = game.gameData.player->position.y;
        
        // Définir la cible
        game.gameData.player->targetMatrixX = newMatrixX;
        game.gameData.player->targetMatrixY = newMatrixY;
        game.gameData.player->targetX = newMatrixX * TILE_SIZE_W_SCALE;
        game.gameData.player->targetY = newMatrixY * TILE_SIZE_H_SCALE;
        
        // Initialiser le mouvement
        game.gameData.player->state = newState;
        game.gameData.player->isMovingToTarget = true;
        game.gameData.player->interpolationTime = 0.0f;
    }
    
    handleEvent(win, event);
}
