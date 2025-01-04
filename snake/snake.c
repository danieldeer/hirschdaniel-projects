#include <stdio.h>
#include <SDL2/SDL.h>
#include <assert.h>
#include <stdlib.h>

#define WIDTH 900
#define HEIGHT 600

#define CELL_SIZE 30
#define ROWS HEIGHT/CELL_SIZE
#define COLUMNS WIDTH/CELL_SIZE
#define LINE_WIDTH 2

#define COLOR_BLACK 0x00000000
#define COLOR_GRID 0x1f1f1f1f
#define COLOR_WHITE 0xffffffff
#define COLOR_APPLE 0x00ff0000

#define SNAKE(x,y) fill_cell(psurface,x,y,COLOR_WHITE) 
#define APPLE(x,y) fill_cell(psurface,x,y,COLOR_APPLE)
#define DRAW_GRID draw_grid(psurface)

/* Like a LinkedList */
struct SnakeElement
{
    int x, y;
    /* Can be NULL (in case of last element) */
    struct SnakeElement *pnext;
};

/* Only one value should be != 0 */
struct Direction
{
    int dx,dy;
};

struct Apple
{
    int x,y;
};

int draw_grid(SDL_Surface* psurface)
{
    SDL_Rect row_line = {0,0,900,LINE_WIDTH};
    for (row_line.y = 0; row_line.y < HEIGHT; row_line.y += CELL_SIZE )
        SDL_FillRect(psurface, &row_line, COLOR_GRID);

    SDL_Rect col_line = {0,0,LINE_WIDTH, HEIGHT};
    for (col_line.x = 0; col_line.x < WIDTH; col_line.x += CELL_SIZE )
        SDL_FillRect(psurface, &col_line, COLOR_GRID);
}

void fill_cell(SDL_Surface* psurface, int x, int y, Uint32 color)
{
    SDL_Rect rect = {x*CELL_SIZE, y*CELL_SIZE, CELL_SIZE, CELL_SIZE};
    SDL_FillRect(psurface, &rect, color);
}


size_t snake_size(struct SnakeElement **ppsnake)
{
    assert(ppsnake != NULL);
    assert(*ppsnake != NULL);

    size_t list_size = 1;
    struct SnakeElement *current = *ppsnake;
    while (current->pnext != NULL)
    {
        current = current->pnext;
        list_size++;
    }
    return list_size;
}

void draw_snake(SDL_Surface* psurface, struct SnakeElement **ppsnake)
{
    assert(psurface != NULL);
    assert(ppsnake != NULL);
    assert(*ppsnake != NULL);

    struct SnakeElement *psnake = *ppsnake;
    int debug_snake_size = 0;
    while (psnake != NULL)
    {
        SNAKE(psnake->x, psnake->y);
        psnake = psnake->pnext;
        debug_snake_size++;
    }
    assert(snake_size(ppsnake) == debug_snake_size);
}

void print_snake(struct SnakeElement **ppsnake)
{
    struct SnakeElement *psnake = *ppsnake;
    int counter = 0;
    while (psnake != NULL)
    {
        printf("Element No. %d\n", counter);
        printf("Snake x=%d\n", psnake->x);
        printf("Snake y=%d\n", psnake->y);
        printf("Snake self=%p\n", psnake);
        printf("Snake pnext=%p\n", psnake->pnext);
        psnake = psnake->pnext;
        counter++;
    }
}

void move_snake(struct SnakeElement **ppsnake, struct Direction *pdirection)
{
    assert(ppsnake != NULL);
    assert(*ppsnake != NULL);

    printf("direction dx=%d, dy=%d\n", pdirection->dx, pdirection->dy);
    if(pdirection->dx == 0 && pdirection->dy == 0)
    {
        return;
    }

    size_t size = snake_size(ppsnake);
    if (size == 1)
    {
        (*ppsnake)->x += pdirection->dx;
        (*ppsnake)->y += pdirection->dy;
    }
    else
    {
        struct SnakeElement *pcurrent = *ppsnake;
        int previous_x = pcurrent->x;
        int previous_y = pcurrent->y;

        pcurrent->x += pdirection->dx;
        pcurrent->y += pdirection->dy;

        while ( pcurrent->pnext != NULL )
        {
            pcurrent = pcurrent->pnext;

            int temp_x = pcurrent->x; 
            int temp_y = pcurrent->y; 

            pcurrent->x = previous_x;
            pcurrent->y = previous_y;

            previous_x = temp_x;
            previous_y = temp_y;

        }
    }
    printf("Snake size =%zu\n", size);
}



void reset_apple(struct SnakeElement *psnake, struct Apple *papple)
{
    papple->x = COLUMNS * ((double) rand() / RAND_MAX);
    papple->y = ROWS * ((double) rand() / RAND_MAX);

    /* If apple coordinates collide with snake, try again */
    struct SnakeElement *pcurrent = psnake;
    do
    {
        if (pcurrent->x == papple->x && pcurrent->y == papple->y)
        {
            reset_apple(psnake, papple);
            break; 
        }
        pcurrent = pcurrent->pnext;
    }
    while (pcurrent != NULL);
}

void lengthen_snake(struct SnakeElement **ppsnake, struct Direction *pdirection)
{
    struct SnakeElement *head = malloc(sizeof(struct SnakeElement *));
    head->x = (*ppsnake)->x + pdirection->dx;
    head->y = (*ppsnake)->y + pdirection->dy;
    head->pnext = *ppsnake;

    *ppsnake = head;
}

int check_collision(struct SnakeElement **ppsnake)
{
    assert(ppsnake != NULL);
    assert(*ppsnake != NULL);

    struct SnakeElement snake = **ppsnake;
    if (snake.x < 0 || snake.y < 0
            || snake.x > COLUMNS || snake.y > ROWS)
    {
        printf("Snake hit boundary.\n");
        return 1;
    }

    struct SnakeElement *psnake = *ppsnake;
    while(psnake->pnext != NULL)
    {
        psnake = psnake->pnext;
        if(snake.x == psnake->x && snake.y == psnake->y)
        {      
            printf("Snake hit itself.\n");
            return 1;
        }
    }

    return 0;
}

int main()
{
    printf("Hello Snake\n");

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Classic Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

    SDL_Surface* psurface = SDL_GetWindowSurface(window);

    SDL_Event event;

    struct SnakeElement *psnake = malloc(sizeof(struct SnakeElement *));
    struct SnakeElement *psnakeTail = malloc(sizeof(struct SnakeElement *));
    struct SnakeElement *psnakeTail2 = malloc(sizeof(struct SnakeElement *));

    psnake->x = 5;
    psnake->y=5;
    psnake->pnext=psnakeTail;

    psnakeTail->x = 5;
    psnakeTail->y=6;
    psnakeTail->pnext=psnakeTail2;

    psnakeTail2->x = 5;
    psnakeTail2->y=7;
    psnakeTail2->pnext=NULL;

    print_snake(&psnake);

    

    struct SnakeElement **ppsnake = &psnake;
    struct Direction direction = {0,0};
    struct Direction *pdirection = &direction;
    struct Apple apple;
    struct Apple *papple = &apple;
    reset_apple(psnake, papple);

    SDL_Rect override_rect = {0,0,WIDTH,HEIGHT};

    int game = 1;
    while(game)
    {
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                game = 0;   
            if (event.type == SDL_KEYDOWN)
            {
                direction = (struct Direction) {0,0};
                if (event.key.keysym.sym == SDLK_RIGHT)
                    direction.dx = 1;
                if (event.key.keysym.sym == SDLK_LEFT)
                    direction.dx = -1;
                if (event.key.keysym.sym == SDLK_UP)
                    direction.dy = -1;
                if (event.key.keysym.sym == SDLK_DOWN)
                    direction.dy = +1;
            }
        }

        //print_snake(ppsnake);

        SDL_FillRect(psurface, &override_rect, COLOR_BLACK);

        printf("Moving snake\n");
        move_snake(ppsnake, pdirection);
        if(check_collision(ppsnake))
        {
            printf("Collision! Game Over\n");
            
            /* Free memory */
            struct SnakeElement *pcurrent = *ppsnake;
            struct SnakeElement *pnext;
            while (pcurrent->pnext != NULL)
            {
                pnext = pcurrent->pnext;
                free(pcurrent);
                pcurrent = pnext;
            }
            free(pnext);
            game = 0;
        }

        if (psnake->x == papple->x && psnake->y == papple->y)
        {
            reset_apple(psnake, papple);
            lengthen_snake(ppsnake, pdirection);
        }

        APPLE(papple->x, papple->y);
        draw_snake(psurface, ppsnake); 
        DRAW_GRID;

        SDL_UpdateWindowSurface(window);
        SDL_Delay(200);
    } 
}
