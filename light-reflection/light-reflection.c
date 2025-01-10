#include <stdio.h>
#include <SDL2/SDL.h>

#define WIDTH 900
#define HEIGHT 600

#define COLOR_WHITE 0xffffffff
#define COLOR_BLUE 0x000000ff
#define COLOR_YELLOW 0x00ffff00

struct RotatedRectangle
{
    SDL_Rect rect;
    double angle;
};

struct Point
{
    double x, y;
};

struct RayGenerationRequest 
{
    struct Point point;
    double angle;
};

struct RaySegment
{
    struct Point start, end;
    struct RaySegment *pnext; 
};

struct Mirror
{
    struct Point start, end;
}

void Render_Rotated_Rectangle(SDL_Surface *psurface, struct RotatedRectangle rotatedRectangle, Uint32 color)
{
    double angle = rotatedRectangle.angle;
    for (double x=rotatedRectangle.rect.x; x < (rotatedRectangle.rect.x + rotatedRectangle.rect.w); x++)
    {
        for (double y=rotatedRectangle.rect.y; y < (rotatedRectangle.rect.y + rotatedRectangle.rect.h); y++)
        {
            // Rotate
            double x_rotated = cos(angle)*(x-rotatedRectangle.rect.x) - sin(angle)*(y-rotatedRectangle.rect.y) + rotatedRectangle.rect.x;
            double y_rotated = sin(angle)*(x-rotatedRectangle.rect.x) + cos(angle)*(y-rotatedRectangle.rect.y) + rotatedRectangle.rect.y;

            // Don't forget to apply mathematical rounding
            SDL_Rect pixel_rect = {round(x_rotated), round(y_rotated), 1, 1};
            SDL_FillRect(psurface, &pixel_rect, color);
        }
    } 
}

int main()
{
    printf("Hello Reflective Light\n");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *pwindow = SDL_CreateWindow("Light Reflections", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

    SDL_Surface *psurface = SDL_GetWindowSurface(pwindow);
    SDL_Rect mirror_rect = {500,500,200,50};
    SDL_Rect ray = {0,100, 660, 10};
    SDL_Rect reflected_ray = {560,430,800,10};
    double angle = M_PI / 4;
    
    struct RotatedRectangle rotated_mirror = {mirror_rect, -M_PI/4};
    struct RotatedRectangle rotated_ray = {ray, M_PI/6};
    struct RotatedRectangle rotated_reflected_ray = {reflected_ray, -M_PI/2};
    Render_Rotated_Rectangle(psurface, rotated_mirror, COLOR_WHITE);
    Render_Rotated_Rectangle(psurface, rotated_ray, COLOR_YELLOW);
    Render_Rotated_Rectangle(psurface, rotated_reflected_ray, COLOR_YELLOW);

    SDL_UpdateWindowSurface(pwindow);

    SDL_Delay(5000);
    return 0;
}
