#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_test_font.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int init(SDL_Window **window, SDL_Renderer **renderer, int width, int height);
void closeSDL(SDL_Window *window, SDL_Renderer* renderer);

#endif