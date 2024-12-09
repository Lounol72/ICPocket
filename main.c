#include <SDL2/SDL.h>
#include <stdio.h>

void quitSDL(int codeError, SDL_Window* window);
void handleInputs(SDL_Window* window);


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


#include <stdio.h>

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
                    case SDLK_UP:
                        // Move the player up
                        printf("Up arrow pressed\n");
                        break;
                    case SDLK_DOWN:
                        // Move the player down
                        printf("Down arrow pressed\n");
                        break;
                    case SDLK_LEFT:
                        // Move the player left
                        printf("Left arrow pressed\n");
                        break;
                    case SDLK_RIGHT:
                        // Move the player right
                        printf("Right arrow pressed\n");
                        break;
                    case SDLK_ESCAPE:
                        // Pause the game
                        printf("Pause the game\n");
                        break;
                    
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}