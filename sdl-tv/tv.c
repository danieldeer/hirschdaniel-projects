#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow("SDL Drawing Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 700, 0);

	SDL_Surface *surface = SDL_GetWindowSurface(window);	
	SDL_Rect rect = (SDL_Rect) {40,40,40,200};
	SDL_FillRect(surface, &rect, (Uint32) 0xffffffff);

	SDL_UpdateWindowSurface(window);
	SDL_Delay(5000);
}
