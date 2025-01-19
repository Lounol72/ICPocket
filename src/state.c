#include "include/state.h"
#include "menu.h"

extern Bouton retourMenu;


void drawCurrentState(SDL_Surface* menu, Window* win) {
    if (*currentState == MENU) {
            if (!win->musicPlaying) {
                if (Mix_PlayMusic(win->music, -1) == -1) {
                    SDL_Log("Error playing music: %s", Mix_GetError());
                } else {
                    win->musicPlaying = 1;
                }
            }
            drawMenu(menu, win->image);
        }else if(*currentState == GAME){
            updateSizeBoutons(&retourMenu, 25, 600);
            drawGame(menu);
        } else {
            if (win->musicPlaying) {
                Mix_HaltMusic();
                win->musicPlaying = 0;
            }
            
            if (*currentState == SETTINGS) {
            
                drawSettings(menu);
                drawVolumeControl(menu, win->musicVolume);
            }
        }
}