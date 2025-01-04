
#include <SDL2/SDL.h>
#include <stdlib.h>
void fill_surface_with_random_data(SDL_Surface *surface) {

    int num_pixels = surface->w * surface->h;

    Uint8 *pixels = (Uint8 *)surface->pixels;

    for (int i = 0; i < num_pixels; ++i) {
        Uint32 random_pixel;

        switch (surface->format->BytesPerPixel) {
            case 1:  
                pixels[i] = (Uint8)(rand() % 256); 
                break;
            case 2: 
                *(Uint16 *)&pixels[i * 2] = (Uint16)(rand() % 65536);
                break;
            case 4:
                *(Uint32 *)&pixels[i * 4] = (Uint32)rand(); // Random value
                break;
        }
    }
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("SDL2 Window",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          680, 480,
                                          0);

    SDL_Surface *window_surface = SDL_GetWindowSurface(window);
    SDL_Event event;
    int running = 1;
    while(running)
    {
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT)
		running = 0;

        fill_surface_with_random_data(window_surface);

        SDL_UpdateWindowSurface(window);
        SDL_Delay(10);
    }

}

