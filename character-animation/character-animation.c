#include <stdio.h>
#include <unistd.h>
#include <math.h>

#define WIDTH 70
#define HEIGHT 14
#define NUM_CIRCLES 3
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_WHITE "\x1b[37m"
#define ANSI_RESET_ALL "\x1b[0m"

struct Circle
{
    double x,y,r,v_x,v_y;
};

void clear_screen()
{
    printf("\e[2J");
}

void move_cursor_beginning_of_screen()
{
    printf("\e[H");
}

void visualize(struct Circle circles[NUM_CIRCLES], char* colors[])
{
    for(int y=0; y<HEIGHT; y++)
    {
        for(int x=0; x<WIDTH; x++)
        {

            int filled = 0;
            for (int i=0; i<NUM_CIRCLES && !filled; i++)
            {
                struct Circle circle = circles[i];
                double distance = sqrt( pow(x-circle.x, 2) + pow(y-circle.y,2));
                if (distance < circle.r)
                {
                    filled = 1;
                    printf(colors[i]);
                    printf("#");
                    printf(ANSI_RESET_ALL);
                }
            }

            if(!filled)
                printf(" ");
        }
        printf("\n");
    } 
}

void step(struct Circle circles[NUM_CIRCLES])
{
    for(int i=0; i<NUM_CIRCLES; i++)
    {
        struct Circle* circle = &circles[i];
        if (circle->x - circle->r < 0)
        {
            circle->v_x = - circle->v_x;
            circle->x = circle->r;
        }
        if (circle->x + circle->r > WIDTH)
        {
            circle->v_x = - circle->v_x;
            circle->x = WIDTH - circle->r;
        }
        if (circle->y - circle->r < 0)
        {
            circle->v_y = - circle->v_y;
            circle->y = circle->r;
        }
        if (circle->y + circle->r > HEIGHT)
        {
            circle->v_y = - circle->v_y;
            circle->y = HEIGHT - circle->r;
        }
        circle->x += circle->v_x;
        circle->y += circle->v_y;
    }

}

int main()
{
    struct Circle circle = {10,10,5,0.7,0.3};
    struct Circle circle2 = {10,10,3,-0.5,-0.2};
    struct Circle circle3 = {10,10,4,-0.1,-0.2};

    struct Circle circles[] = {circle, circle2, circle3};
    clear_screen();
    
    char* colors[] = {ANSI_COLOR_BLUE, ANSI_COLOR_RED, ANSI_COLOR_WHITE};
    while ( 1 )
    {
        printf("Terminal Animiation programmed in pure C\n");
        move_cursor_beginning_of_screen();
        step(circles);
        visualize(circles, colors);
        usleep(50000);
    }

}

