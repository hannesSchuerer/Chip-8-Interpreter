#include "chip.h"

#define NO_ROM_INSERTED 1

const int PIXEL_SIZE = 20;
const int CHIP_WIDTH = 64;
const int CHIP_HEIGHT = 32;

const int SCREEN_WIDTH = CHIP_WIDTH * PIXEL_SIZE;
const int SCREEN_HEIGHT = CHIP_HEIGHT * PIXEL_SIZE;

int main(int argc, char **argv)
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Event event;
	
	const uint8_t *state = SDL_GetKeyboardState(NULL);
	int quit = 0;
	
	/*Initializes random number generator*/
	time_t t;
	srand((unsigned) time(&t));
	
	/*Time relatet*/
	int fps = 60;
	int numerOfOpcodes = 400;
	int numOfFrames = numerOfOpcodes / fps;
	float interval = 1000;
	interval /= fps;
	
	unsigned int time2 = SDL_GetTicks();
	/*Time relatet*/
	
	Chip *chip = (Chip*)malloc(sizeof(Chip));
	chipInitialize(chip);
	
	loadProgramInMemory(chip, "./Roms/WIPEOFF");
	
    if(!init(&window, &renderer, SCREEN_WIDTH, SCREEN_HEIGHT))
    {
        printf( "Failed to initialize!\n" );
    }
	else
	{	
		while(!quit)
		{	
			while(SDL_PollEvent(&event) != 0)
			{
				if(event.type == SDL_QUIT)
				{
					quit = 1;
				}
			}
			
			// Clear screen
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);
			
			checkKeyState(chip, state);
			
			unsigned int current = SDL_GetTicks();
			
			if((time2 + interval) < current)
			{
				decreaseTimers(chip);
				
				for(int i = 0; i < numOfFrames; i++)
				{
					printf("Program Counter: %x, ", chip->pc);
					executionCycle(chip);
					printf("Opcode: %x, Memory: %x, StackPointer %d\n", chip->opcode, chip->memory[chip->pc], chip->sp);	
				}
				
				for(int i = 0; i < CHIP_WIDTH; i++)
				{
					for(int j = 0; j < CHIP_HEIGHT; j++)
					{
						if(chip->gfx[i][j] > 0x00)
						{
							SDL_Rect fillRect = {i * PIXEL_SIZE, j * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE};
							SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
							SDL_RenderFillRect(renderer, &fillRect);
						}
					}
				}
				SDL_RenderPresent(renderer);
				time2 = current;
			}	
		}
	}

	closeSDL(window, renderer);
				
	free(chip);
	
	return 0;
}