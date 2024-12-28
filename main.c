#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
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
                        printf("A pressed\n");
                        break;
                    case SDLK_z:
                        // Bouton d'action lorsque Z est pressé || attaque 2
                        printf("Z pressed\n");
                        break;
                    case SDLK_e:
                        // Bouton d'action lorsque E est pressé || attaque 3
                        printf("E pressed\n");
                        break;
                    case SDLK_r:
                        // Bouton d'action lorsque R est pressé || attaque 4
                        printf("R pressed\n");
                        break;
                    case SDLK_ESCAPE:
                        // Pause la partie
                        printf("Pause the game\n");
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