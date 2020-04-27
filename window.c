#include "window.h"

int init(SDL_Window **owindow, SDL_Renderer **orenderer, int width, int height)
{
	//Initialization flag
    int success = 1;
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

    //Initialize SDL
    if( SDL_Init(SDL_INIT_VIDEO) < 0 && SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = 0;
    }
    else
    {
        //Create window
        window = SDL_CreateWindow( "Chip-8 Interpreter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = 0;
        }
        else
        {
			renderer = SDL_CreateRenderer(window, -1, 0);
			if(renderer == NULL)
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = 0;
            }
        }
    }
	*owindow = window;
	*orenderer = renderer;
    return success;
}

void closeSDL(SDL_Window *window, SDL_Renderer* renderer)
{
    //Destroy window
    SDL_DestroyWindow( window );
    window = NULL;

	SDL_DestroyRenderer(renderer);
	renderer = NULL;
    //Quit SDL subsystems
    SDL_Quit();
}