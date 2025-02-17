#include "../include/Buttons.h"
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdlib.h>

Button *createButton(char *text, Window *win, int x, int y, int w, int h, SDL_Color color, SDL_Color textcolor, void (*onClick)(Window *, void *), void *data, TTF_Font *font) {
    Button *button = malloc(sizeof(Button));
    if (!button) {
        SDL_Log("❌ Erreur d'allocation pour le bouton.");
        return NULL;
    }
    button->text = text;
    button->textcolor = textcolor;
    button->rect = (SDL_Rect){x, y, w, h};
    button->initialRect = button->rect;
    button->color = color;
    button->texture = NULL;
    button->renderer = win->renderer;
    button->onClick = onClick;
    button->data = data;
    button->font = font ? font : win->font;

    SDL_Surface *textSurface = TTF_RenderText_Solid(button->font, text, textcolor);
    if (!textSurface) {
        SDL_Log("❌ Erreur lors de la création de la surface du texte : %s", TTF_GetError());
        free(button);
        return NULL;
    }
    button->textTexture = SDL_CreateTextureFromSurface(win->renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!button->textTexture) {
        SDL_Log("❌ Erreur lors de la création de la texture du texte : %s", SDL_GetError());
        free(button);
        return NULL;
    }
    return button;
}


void destroyButton(Button *button) {
    if (!button) return;

    // Free only the allocated textures, not the alias pointer 'texture'
    if (button->initialTexture) {
        SDL_DestroyTexture(button->initialTexture);
        button->initialTexture = NULL;
    }
    if (button->selectedTexture) {
        SDL_DestroyTexture(button->selectedTexture);
        button->selectedTexture = NULL;
    }
    if (button->textTexture) {
        SDL_DestroyTexture(button->textTexture);
        button->textTexture = NULL;
    }
    free(button);
}


void addListButton(ButtonList *list, Button **buttons, int count) {
    if (list->buttons) {
        for (int i = 0; i < list->size; i++) destroyButton(list->buttons[i]);
        free(list->buttons);
    }
    list->buttons = malloc(count * sizeof(Button *));
    if (!list->buttons) {
        SDL_Log("❌ Erreur : allocation de mémoire pour les boutons échouée !");
        return;
    }
    for (int i = 0; i < count; i++) list->buttons[i] = buttons[i];
    list->size = count;
}


void destroyButtonList(ButtonList *list) {
    if (list->buttons) {
        for (int i = 0; i < list->size; i++) {
            destroyButton(list->buttons[i]);
        }
        free(list->buttons);
        list->buttons = NULL; 
    }
    

    list->size = 0;
}



void InitTextureButton(Button *button, SDL_Renderer *renderer, const char *imagePath) {
    if (!button || !renderer || !imagePath) return;
    SDL_Surface *surface = IMG_Load(imagePath);
    if (!surface) return;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) return;
    if (button->texture) SDL_DestroyTexture(button->texture);
    button->texture = texture;
    button->initialTexture = texture;

    surface = IMG_Load("assets/User Interface/Blue/button_rectangle_depth_flat.png");
    if (!surface) {
        SDL_Log("❌ Erreur lors du chargement de l'image : %s", IMG_GetError());
        return;
    }
    button->selectedTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!button->selectedTexture) {
        SDL_Log("❌ Erreur lors de la création de la texture : %s", SDL_GetError());
    }
}


//----------------------------------------------------------------------------------------

// Rendering functions


void renderButtonList(ButtonList *B)
{
    if (!B) return;
    for (int i = 0; i < B->size; i++)
        renderButton(B->buttons[i]);
}

void renderButton(Button *button) {
    if (!button) return;
    if (button->texture) SDL_RenderCopy(button->renderer, button->texture, NULL, &button->rect);
    else {
        SDL_SetRenderDrawColor(button->renderer, button->color.r, button->color.g, button->color.b, button->color.a);
        SDL_RenderFillRect(button->renderer, &button->rect);
    }
    if (button->textTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(button->textTexture, NULL, NULL, &textWidth, &textHeight);
        
        // Calculs corrects pour centrer horizontalement et verticalement
        SDL_Rect textRect = {
            button->rect.x + (button->rect.w - textWidth) / 2,  // Centrage horizontal
            button->rect.y + ((button->rect.h - textHeight) / 2)* 0.8, // Ajustement vertical (ajouter un décalage)
            textWidth,
            textHeight
        };
        
        SDL_RenderCopy(button->renderer, button->textTexture, NULL, &textRect);
    }
}



//--------------------------------------------------------------------------

// Button handling functions


void ButtonClicked(Button *button, int mouseX, int mouseY, Window *win) {
    if (button && (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &button->rect)|| (mouseX == -1 && mouseY == -1) )&& button->onClick) {
        button->onClick(win, button->data);
    }
}


void updateButtonPosition(ButtonList *buttons, float Scalex, float Scaley)
{
    for (int i = 0; i < buttons->size; i++)
    {
        if(!buttons->buttons[i]){
            SDL_Log("❌ Invalid button at index %d", i);
            continue;
        }
        buttons->buttons[i]->rect.w = buttons->buttons[i]->initialRect.w * Scalex;
        buttons->buttons[i]->rect.h = buttons->buttons[i]->initialRect.h * Scaley;
        buttons->buttons[i]->rect.x = buttons->buttons[i]->initialRect.x * Scalex;
        buttons->buttons[i]->rect.y = buttons->buttons[i]->initialRect.y * Scaley;
        
    }
}

void setButtonText(Button *button, const char *text, SDL_Renderer *renderer) {
    if (!button || !text || !renderer) {
        SDL_Log("❌ Erreur : Paramètre NULL dans setButtonText");
        return;
    }

    // Détruire l'ancienne texture texte si elle existe
    if (button->textTexture) {
        SDL_DestroyTexture(button->textTexture);
        button->textTexture = NULL;
    }

    // Générer une nouvelle texture texte
    SDL_Surface *textSurface = NULL;
    if (text && text[0] == '\0') textSurface = TTF_RenderText_Solid(button->font, " ", button->textcolor);
    else textSurface = TTF_RenderText_Solid(button->font, text, button->textcolor);
    if (!textSurface) {
        SDL_Log("❌ Erreur lors de la création de la surface du texte : %s", TTF_GetError());
        return;
    }

    button->textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    if (!button->textTexture) {
        SDL_Log("❌ Erreur lors de la création de la texture du texte : %s", SDL_GetError());
    }
}

//-----------------------------------------------

// Slider functions

Slider *createSlider(SDL_Renderer *renderer, int x, int y, int w, int h, SDL_Color color, SDL_Color cursorColor) {
    Slider *slider = malloc(sizeof(Slider));
    if (!slider) {
        SDL_Log("Erreur d'allocation pour le slider.");
        return NULL;
    }
    slider->value = 0.5f;
    slider->rect = (SDL_Rect){x, y, w, h};
    slider->initialBar = slider->rect;
    int cursorW = 10;
    slider->cursor = (SDL_Rect){slider->rect.x + (slider->value * slider->rect.w) - (slider->cursor.w / 2), y-5, cursorW, h+12};
    slider->initialCursor = slider->cursor;
    slider->color = color;
    slider->cursorColor = cursorColor;
    slider->renderer = renderer;

    return slider;
}


void destroySlider(Slider *slider) {
    if (slider) {
        free(slider);
    }
}

void destroySliderList(SliderList *list) {
    if (list->sliders) {
        for (int i = 0; i < list->size; i++) {
            destroySlider(list->sliders[i]);
        }
        free(list->sliders);
        list->sliders = NULL;
    }
    list->size = 0;
}

void addListSlider(SliderList *S, Slider *sliders[], int size) {
    if (!S || !sliders) return;
    Slider **newSliders = (Slider **)realloc(S->sliders, (S->size + size) * sizeof(Slider *));
    if (!newSliders) return;
    S->sliders = newSliders;
    for (int i = 0; i < size; i++) S->sliders[S->size + i] = sliders[i];
    S->size += size;
}

void renderSliderList(SliderList *S) {
    if (!S) return;
    for (int i = 0; i < S->size; i++)
        renderSlider(S->sliders[i]);
}

void renderSlider(Slider *slider) {
    if (!slider) return;

    // Fonction utilitaire pour définir la couleur
    void setColor(SDL_Renderer *renderer, SDL_Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    }

    // Dessiner le slider
    setColor(slider->renderer, slider->color);
    SDL_RenderFillRect(slider->renderer, &slider->rect);

    // Dessiner le curseur
    setColor(slider->renderer, slider->cursorColor);
    
    SDL_RenderFillRect(slider->renderer, &slider->cursor);
}

int handleSliderEvent(Slider *slider, int x, int y) {
    if (!slider) return 0;
    if (SDL_PointInRect(&(SDL_Point){x, y}, &(slider->rect))) {
        slider->value = (float)(x - slider->rect.x) / slider->rect.w;
        slider->cursor.x = slider->rect.x + (slider->value * slider->rect.w) - (slider->cursor.w / 2);
        int volume = (int)(slider->value * SDL_MIX_MAXVOLUME);
        Mix_VolumeMusic(volume);
        return 1; // Interaction détectée
    }
    return 0;
}

void updateSliderPosition(SliderList *sliders, float Scalex, float Scaley) {
    if (!sliders) return;
    for (int i = 0; i < sliders->size; i++) {
        if (!sliders->sliders[i]) {
            SDL_Log("❌ Invalid slider at index %d", i);
            continue;
        }
        sliders->sliders[i]->rect.w = sliders->sliders[i]->initialBar.w * Scalex;
        sliders->sliders[i]->rect.h = sliders->sliders[i]->initialBar.h * Scaley;
        sliders->sliders[i]->rect.x = sliders->sliders[i]->initialBar.x * Scalex;
        sliders->sliders[i]->rect.y = sliders->sliders[i]->initialBar.y * Scaley;
        sliders->sliders[i]->cursor.w = 10;
        sliders->sliders[i]->cursor.h = sliders->sliders[i]->initialBar.h + (sliders->sliders[i]->initialCursor.h * Scaley) * 0.4;
        sliders->sliders[i]->cursor.x = sliders->sliders[i]->rect.x + (sliders->sliders[i]->value * sliders->sliders[i]->rect.w) - (sliders->sliders[i]->cursor.w / 2);
        sliders->sliders[i]->cursor.y = sliders->sliders[i]->rect.y - 5;
    }
}

void changeState(Window *win, void *data) {
    AppState newState = *(AppState *)data;
    win->state = newState;
    game.gameState.currentState = newState;
    game.currentButton = 0;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Changement d'état : %d", newState);
}

void makeWindowFullScreen(Window *win, void *data) {
    (void *)data;
    SDL_SetWindowFullscreen(win->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_GetWindowSize(win->window, &win->width, &win->height);
}
void makeWindowWindowed(Window *win, void *data) {
    (void *)data;
    SDL_SetWindowFullscreen(win->window, 0);
    SDL_SetWindowSize(win->window, win->InitialWidth, win->InitialHeight);
    SDL_GetWindowSize(win->window, &win->width, &win->height);
}

void changeTextSpeed(Window *win, void *data) {
    double *speed = (double *)data;
    win->textSpeed = *speed;
}




void initAllButtons(Window * win)
{
    int nbButtonsLoad = 3;
    int nbButtonsMenu = 4;
    int nbSlidersSettings = 1;
    int nbButtonsParam = 6;
    int nbButtonsGame = 5;
    int nbButtonsICMons = 7;
    int nbButtonsInter = 2;

    Button **buttonsMenu = malloc(nbButtonsMenu * sizeof(Button *));
    Button **buttonsParam = malloc(nbButtonsParam * sizeof(Button *));
    Button **buttonsLoadGame = malloc(nbButtonsLoad * sizeof(Button *));
    Button **buttonsGame = malloc(nbButtonsGame * sizeof(Button *));
    Button **buttonsICMons = malloc(nbButtonsICMons * sizeof(Button *));
    Button **buttonsInter = malloc(nbButtonsInter * sizeof(Button *));

    Slider **sliders = malloc(nbSlidersSettings * sizeof(Slider *));
    
    

    if (!buttonsMenu || !sliders || !buttonsParam || !buttonsLoadGame || !buttonsGame ||!buttonsICMons  || !buttonsInter) {
        SDL_LogMessage(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_ERROR,"❌ Allocation de mémoire pour les boutons échouée !");
        exit(EXIT_FAILURE);
    }else{
        buttonsMenu[0] = createButton(
            "PLAY", win, 500, 150, 300, 100,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            changeState, &game.stateHandlers[4].state, win->MediumFont
        );

        buttonsMenu[1] = createButton(
            "LOAD GAME", win, 500, 300, 300, 100,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            changeState, &game.stateHandlers[5].state, win->MediumFont
        );
        buttonsMenu[2] = createButton(
            "SETTINGS", win, 500, 450, 300, 100,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            changeState, &game.stateHandlers[1].state, win->MediumFont
        );

        buttonsMenu[3] = createButton(
            "QUIT", win, 500, 600, 300, 100,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            changeState, &game.stateHandlers[0].state, win->MediumFont
        );

        buttonsParam[0] =createButton(
            "0.5", win, 100, 200, 200, 50,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            changeTextSpeed, &game.speeds[0], win->LargeFont
        );

        buttonsParam[1] =createButton(
            "1", win, 400, 200, 200, 50,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            changeTextSpeed, &game.speeds[1], win->LargeFont
        );

        buttonsParam[2] =createButton(
            "1.5", win, 700, 200, 200, 50,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            changeTextSpeed, &game.speeds[2], win->LargeFont
        );
        buttonsParam[3] =createButton(
            "Back", win, 100, 600, 300, 100,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            changeState, &game.stateHandlers[2].state, win->LargeFont
        );

        buttonsParam[4] =createButton(
            "Fullscreen", win, 100, 300, 220, 75,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            makeWindowFullScreen, NULL, win->LargeFont
        );

        buttonsParam[5] =createButton(
            "Windowed", win, 400, 300, 220, 75,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            makeWindowWindowed, NULL, win->LargeFont
        );

        buttonsLoadGame[0] = createButton(
            "Save 1", win, 500, 104, 300, 100,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            changeState, &game.stateHandlers[5].state, win->LargeFont
        );

        buttonsLoadGame[1] = createButton(
            "Save 2", win, 500, 258, 300, 100,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            changeState, &game.stateHandlers[5].state, win->LargeFont
        );

        buttonsLoadGame[2]  =createButton(
            "Back", win, 100, 600, 300, 100,
            (SDL_Color){0, 255, 255, 255}, (SDL_Color){128, 128, 128, 255},
            changeState, &game.stateHandlers[2].state, win->LargeFont
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
            changeState, &game.stateHandlers[6].state, win->LargeFont
        );

        // ICMons buttons

        buttonsICMons[0] = createButton(
            "ICMon1", win, 20, 20, 160, 100,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            NULL, 0, win->LargeFont
        );
        buttonsICMons[1] = createButton(
            "ICMon2", win, 240, 20, 160, 100,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            changePokemon, 11, win->LargeFont
        );
        buttonsICMons[2] = createButton(
            "ICMon3", win, 460, 20, 160, 100,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            changePokemon, 12, win->LargeFont
        );
        buttonsICMons[3] = createButton(
            "ICMon4", win, 680, 20, 160, 100,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            changePokemon, 13, win->LargeFont
        );
        buttonsICMons[4] = createButton(
            "ICMon5", win, 900, 20, 160, 100,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            changePokemon, 14, win->LargeFont
        );
        buttonsICMons[5] = createButton(
            "ICMon6", win, 1120, 20, 160, 100,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            changePokemon, 15, win->LargeFont
        );
        buttonsICMons[6] = createButton(
            "Back", win, 100, 600, 300, 100,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            changeState, &game.stateHandlers[3].state, win->LargeFont
        );

        buttonsInter[0] = createButton(
            "Next Duel", win, 500, 200, 300, 100,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            nextDuel, 0, win->LargeFont
        );

        buttonsInter[1] = createButton(
            "Back", win, 500, 350, 300, 100,
            (SDL_Color){128,128,128, 255}, (SDL_Color){0, 0, 0, 255},
            changeState, &game.stateHandlers[2].state, win->LargeFont
        );


        for(int i = 0; i < 4; i++) {
            InitTextureButton(buttonsMenu[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
            InitTextureButton(buttonsParam[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
            if (i != 3) InitTextureButton(buttonsLoadGame[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
            InitTextureButton(buttonsGame[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
            InitTextureButton(buttonsICMons[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
            if (i < 2) InitTextureButton(buttonsInter[i], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
        }
        InitTextureButton(buttonsParam[4], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
        InitTextureButton(buttonsParam[5], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
        InitTextureButton(buttonsICMons[4], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
        InitTextureButton(buttonsICMons[5], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");
        InitTextureButton(buttonsICMons[6], win->renderer, "assets/User Interface/Grey/button_rectangle_depth_gloss.png");

        InitTextureButton(buttonsGame[4], win->renderer, "assets/User Interface/Blue/button_square_depth_gloss.png");

        sliders[0] = createSlider(win->renderer, 100, 100, 200, 25, (SDL_Color){128,128,128, 255}, (SDL_Color){255, 0, 0, 255});

        addListSlider((game.ui[1].sliders), sliders, nbSlidersSettings);
        free(sliders);
        sliders = NULL;
        addListButton(game.ui[2].buttons, buttonsMenu, nbButtonsMenu);
        
        free(buttonsMenu);
        buttonsMenu = NULL;
        addListButton(game.ui[1].buttons, buttonsParam, nbButtonsParam);
        
        free(buttonsParam);
        buttonsParam = NULL;
        addListButton(game.ui[5].buttons, buttonsLoadGame, nbButtonsLoad);
        
        free(buttonsLoadGame);
        buttonsLoadGame = NULL;
        addListButton(game.ui[3].buttons, buttonsGame, nbButtonsGame);
        
        free(buttonsGame);
        buttonsGame = NULL;
        addListButton(game.ui[6].buttons, buttonsICMons, nbButtonsICMons);
        
        free(buttonsICMons);
        buttonsICMons = NULL;

        addListButton(game.ui[7].buttons, buttonsInter, nbButtonsInter);
        free(buttonsInter);
        buttonsInter = NULL;
    }
}

void updateICButtons(Window *win, t_Team *team) {
    if (!team || !team->team || !team->team[0].moveList) {
        SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_ERROR, "❌ Erreur : team, team->team ou moveList est NULL\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 4; i++) {
        if(team->team[0].nb_move > i) setButtonText(game.ui[3].buttons->buttons[i], team->team[0].moveList[i].name, win->renderer);
        else setButtonText(game.ui[3].buttons->buttons[i], "  ", win->renderer);
    }
    for (int i  = 0; i < 6; i++) {
        if (team->team[i].name) setButtonText(game.ui[6].buttons->buttons[i], team->team[i].name, win->renderer);
        else setButtonText(game.ui[6].buttons->buttons[i], "  ", win->renderer);
    }
}