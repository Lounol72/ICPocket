<<<<<<< Updated upstream
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include "boutons.h"
#include "inputs.h"
#include "menu.h"
#include "state.h"
#include "utils.h"

void mainLoop(SDL_Window* window, SDL_Surface* image, int* backgroundColor, State* currentState);

Bouton param = {50, 450, 200, 100}; // x ,y largeur, hauteur

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Surface* menu = NULL;
    SDL_Surface* parametre = NULL;
    
    int backgroundColor = 0; // 0 for black, 1 for white
    State currentState = MENU;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) quitSDL(1,NULL);
    if (SDL_Init(SDL_INIT_AUDIO) < 0) quitSDL(1,NULL);
    if (Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0) // création de la configuration de la carte son
    {
        SDL_Log("Erreur initialisation SDL_mixer : %s", Mix_GetError());
        SDL_Quit();
        return -1;
    }
    window = SDL_CreateWindow("ICPocket", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 720, 600, SDL_WINDOW_SHOWN);

    SDL_Surface* image = IMG_Load("assets/MENU_TEST.png");
    if(!image)
    {
        printf("Erreur de chargement de l'image : %s",SDL_GetError());
        return -1;
    }
    
    if (window == NULL)quitSDL(0,window);
    
    mainLoop(window, image, &backgroundColor, &currentState);

    SDL_FreeSurface(menu);
    SDL_FreeSurface(parametre);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    quitSDL(0,window);

    return 0;
}

void mainLoop(SDL_Window* window, SDL_Surface* image, int* backgroundColor, State* currentState) {
    SDL_Surface* menu = NULL;

    SDL_AudioSpec spec;

    SDL_memset(&spec, 0, sizeof(spec));

    spec.freq = 96000; // 4 100 Hz, 48 000 Hz, 96 000 Hz, 192 000 Hz (standard) 
    spec.format = AUDIO_F32SYS;
    spec.channels = 1;
    spec.samples = 4096; // Oublier pas que ce sa doit être en puissance de deux 2^n
    spec.callback = [](void* param, Uint8* stream, int len)
    {
        // Envoyez les données dans notre buffer...
    };

    SDL_AudioDeviceID dev = SDL_OpenAudioDevice(nullptr, 0, &spec, &spec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);

    SDL_PauseAudioDevice(dev, SDL_FALSE);

    while (1) {
        menu = SDL_GetWindowSurface(window);
        if (*backgroundColor == 0) {
            SDL_FillRect(menu, NULL, SDL_MapRGB(menu->format, 0, 0, 0));
        } else {
            SDL_FillRect(menu, NULL, SDL_MapRGB(menu->format, 255, 255, 255));
        }
        if (*currentState == MENU) {
            drawMenu(menu, image);
        } else if (*currentState == PARAMETRE) {
            drawParametre(menu);
        }
        SDL_UpdateWindowSurface(window);
        handleInputs(window, backgroundColor, currentState);
    }
}
=======
#include <SDL2/SDL.h>
#include <stdio.h>

void quitSDL(int codeError, SDL_Window* window);
void handleInputs(SDL_Window* window);
void getMousePosition(SDL_Window* window);

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) quitSDL(1,NULL);

    window = SDL_CreateWindow("ICPocket", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);

    if (window == NULL)quitSDL(0,window);
    while(1){
        screenSurface = SDL_GetWindowSurface(window);
        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
        handleInputs(window);
        SDL_UpdateWindowSurface(window);
        handleInputs(window);
    }
    quitSDL(0,window);

    return 0;
}


void quitSDL(int codeError, SDL_Window* window) {
    switch (codeError)
    {
    case 1:
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        break;
    case 2:
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        break;
    default:
        break;
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}

void handleInputs(SDL_Window* window)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                // Quit the game
                SDL_DestroyWindow(window);
                SDL_Quit();
                exit(0); 
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_a:
                        // Bouton d'action lorsque A est pressé || attaque 1
                        printf("A \n");
                        break;
                    case SDLK_z:
                        // Bouton d'action lorsque Z est pressé || attaque 2
                        printf("Z \n");
                        break;
                    case SDLK_e:
                        // Bouton d'action lorsque E est pressé || attaque 3
                        printf("E \n");
                        break;
                    case SDLK_r:
                        // Bouton d'action lorsque R est pressé || attaque 4
                        printf("R \n");
                        break;
                    case SDLK_ESCAPE:
                        // Pause la partie
                        printf("Pause \n");
                        break;
                    
                    default:
                        break;
                }
                // 
                case SDL_MOUSEBUTTONDOWN:
                        // Donner les coordonnées de la souris lors du clic
                        printf("Mouse clicked\n");
                        getMousePosition(window);
                        break; 
                break;
            default:
                break;
        }
    }
}

// recuperation de coodonnées de la souris 
void getMousePosition(SDL_Window* window)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    printf("Mouse position: x=%d y=%d\n", x, y);
}
>>>>>>> Stashed changes
