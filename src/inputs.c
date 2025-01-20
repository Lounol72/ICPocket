#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include "include/inputs.h"
#include "include/menu.h"
#include "include/parametre.h"
#include "include/boutons.h"
#include "utils.h"
#include "state.h"

// Set the extern variables
extern Bouton pageParam;
extern Bouton retourMenu;
extern Bouton jouer;
extern Slider volumeSlider;
extern State valuseState;


void handleInputs(Window* win, State* currentState, SDL_Event event, int* dragging) {
    switch (event.type) {
        case SDL_QUIT:
            quitSDL(win, 0);
            break;

        case SDL_KEYDOWN:
            handleKeyDown(event.key.keysym.sym, currentState);
            break;

        case SDL_MOUSEBUTTONDOWN:
            handleMouseButtonDown(event, currentState, dragging);
            break;

        case SDL_MOUSEBUTTONUP:
            *dragging = 0;
            break;

        case SDL_MOUSEMOTION:
            handleMouseMotion(event, win, dragging);
            break;

        default:
            break;
    }
}

void handleKeyDown(SDL_Keycode key, State* currentState) {
    switch (key) {
        case SDLK_z:
            printf("Z pressed\n"); // Go up
            break;
        case SDLK_q:
            printf("Q pressed\n"); // Go to the left
            break;
        case SDLK_s:
            printf("S pressed\n"); // Go down
            break;
        case SDLK_d:
            printf("D pressed\n"); // Go to the right
            break;
        case SDLK_a:
            printf("A pressed\n"); // Click button
            break;
        case SDLK_ESCAPE:
            printf("Pause the game\n");
            if (*currentState == GAME) retourMenu.action(retourMenu.actionParam);
            break;
        default:
            break;
    }
}

void handleMouseButtonDown(SDL_Event event, State* currentState, int* dragging) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    if (event.button.button == SDL_BUTTON_LEFT) { // Left button clicked
        handleButtonClick(&pageParam, x, y, currentState, MENU);
        handleButtonClick(&retourMenu, x, y, currentState, SETTINGS);
        handleButtonClick(&jouer, x, y, currentState, MENU);
        handleButtonClick(&retourMenu, x, y, currentState, GAME);
    }

    if (*currentState == SETTINGS && SDL_PointInRect(&(SDL_Point){x, y}, &(volumeSlider.bar))) {
        *dragging = 1;
    }
}

void handleMouseMotion(SDL_Event event, Window* win, int* dragging) {
    if (*dragging) {
        int x = event.motion.x;
        win->musicVolume = SDL_clamp((x - volumeSlider.bar.x) * MIX_MAX_VOLUME / volumeSlider.bar.w, 0, MIX_MAX_VOLUME);
        Mix_VolumeMusic(win->musicVolume);

        // Update the cursor position
        volumeSlider.cursor.x = volumeSlider.bar.x + (win->musicVolume * volumeSlider.bar.w / MIX_MAX_VOLUME) - (volumeSlider.cursor.w / 2);
    }
}

void handleButtonClick(Bouton* button, int x, int y, State* currentState, State targetState) {
    if (Cliqued(button, x, y) && *currentState == targetState) {
        button->action(button->actionParam);
    }
}