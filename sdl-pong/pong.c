#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

static int MOVEMENT_SPEED = 3;

static int RIGHT_INNER_BORDER = 560;
static int BOTTOM_INNER_BORDER = 480;
static int SURFACE_WIDTH = 640;
static int SURFACE_HEIGHT = 480;
static int LEFT_INNER_BORDER = 80;
static int TOP_INNER_BORDER = 0;
static int BALL_DIAMETER = 10;
static int PL_WIDTH = 20;
static int PL_HEIGHT = 60;

typedef struct Speed
{
	int x;
	int y;
} Speed;

typedef struct Score
{
	int pl1;
	int pl2;
} Score;

void move_rect(SDL_Surface* surface, SDL_Rect* rect, Speed* speed)
{
	SDL_FillRect(surface, rect, 0x00000000);
	rect->x += speed->x;
	rect->y += speed->y;
	SDL_FillRect(surface, rect, 0xffffffff);
}

void move_ball(SDL_Surface* surface, SDL_Rect* ball, SDL_Rect* pl1, SDL_Rect* pl2, Speed* ball_speed, Score* score)
{
	if(ball->y <= TOP_INNER_BORDER)
		ball_speed->y = -ball_speed->y;
	if(ball->y + ball->h >= BOTTOM_INNER_BORDER)
		ball_speed->y = -ball_speed->y;

	if(ball->x <= LEFT_INNER_BORDER)
		if ((ball->y + ball->h > pl1->y) && (ball->y < pl1->y + pl1->h))
		{
			ball_speed->x = -ball_speed->x;
			int ball_center_y = (ball->y+ball->h/2);
			int pl_center_y = (pl1->y + pl1->h/2);
			double hit_fraction = (ball_center_y - pl_center_y) / (double) PL_HEIGHT * 2;
			ball_speed->y = (double) (hit_fraction * (double) MOVEMENT_SPEED);
		}
		else
		{
			score->pl2++;
		}

			
	if(ball->x + ball->w >= RIGHT_INNER_BORDER)
		if ((ball->y + ball->h > pl2->y) && (ball->y < pl2->y + pl2->h))
		{
			ball_speed->x = -ball_speed->x;
			int ball_center_y = (ball->y+ball->h/2);
			int pl_center_y = (pl2->y + pl2->h/2);
			double hit_fraction = (ball_center_y - pl_center_y) / (double) PL_HEIGHT * 2;
			ball_speed->y = (double) (hit_fraction * (double) MOVEMENT_SPEED);
		}
		else
		{
			score->pl1++;
		}

	move_rect(surface, ball, ball_speed);
		
}

/* int direction shall be +-1 */
void move_player(SDL_Surface* surface, SDL_Rect* pl, int direction)
{
	if(direction < 0 && pl->y <= 0)
		return;
	if(direction > 0 && pl->y >= (surface->h - pl->h))
		return;
	Speed speed = (Speed){0, direction*MOVEMENT_SPEED};
	move_rect(surface, pl, &speed);	
}

void draw_init_game(SDL_Surface* surface, SDL_Rect* pl1, SDL_Rect* pl2, SDL_Rect* ball, SDL_Rect* border)
{
	SDL_Rect black_surface = (SDL_Rect){0,0,SURFACE_WIDTH, SURFACE_HEIGHT};
	SDL_FillRect(surface, &black_surface, 0x00000000);
	*pl1 = (SDL_Rect) {LEFT_INNER_BORDER - PL_WIDTH,100,PL_WIDTH,PL_HEIGHT};
	Uint32 color = 0xffffffff;
	SDL_FillRect(surface, pl1, color);

	*pl2 = (SDL_Rect) {RIGHT_INNER_BORDER,100,PL_WIDTH,PL_HEIGHT};
	SDL_FillRect(surface, pl2, color);

	*ball = (SDL_Rect) {(LEFT_INNER_BORDER + RIGHT_INNER_BORDER)/2 , (BOTTOM_INNER_BORDER + TOP_INNER_BORDER) / 2, BALL_DIAMETER, BALL_DIAMETER};
	SDL_FillRect(surface, ball, color);
	
	*border = (SDL_Rect) {SURFACE_WIDTH/2,0,1,SURFACE_HEIGHT};
	// Redraw border
	SDL_FillRect(surface, border, 0xffffffff);
}

int main()
{
	SDL_InitSubSystem(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("Pong Game 1972", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SURFACE_WIDTH, BOTTOM_INNER_BORDER, 0);
	SDL_Surface *surface = SDL_GetWindowSurface(window);

	SDL_Rect pl1;
	SDL_Rect pl2;
	SDL_Rect ball;
	SDL_Rect border;
	draw_init_game(surface, &pl1, &pl2, &ball, &border);
	SDL_UpdateWindowSurface(window);

	Score score = (Score){0,0};

	int running = 1;
	int local_pl1_score = 0;
	int local_pl2_score = 0;

	Speed ball_speed = (Speed){MOVEMENT_SPEED, 0};

	SDL_Event event;
	while(running)
	{
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
		{
			running = 0;	
		}
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LALT)
		{
			move_player(surface, &pl1, 1);
		}
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LCTRL)
		{
			move_player(surface, &pl1, -1);
		}
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN)
		{
			move_player(surface, &pl2, 1);
		}
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP)
		{
			move_player(surface, &pl2, -1);
		}

		move_ball(surface, &ball, &pl1, &pl2, &ball_speed, &score);
		// Check if someone scored, redraw if so
		int score_changed = 0;
		if(score.pl1 != local_pl1_score)
		{
			local_pl1_score = score.pl1;
			draw_init_game(surface, &pl1, &pl2, &ball, &border);	
			score_changed = 1;
		}
		if(score.pl2 != local_pl2_score)
		{
			local_pl2_score = score.pl2;
			draw_init_game(surface, &pl1, &pl2, &ball, &border);	
			score_changed = 1;
		}
		if (score_changed)
		{
			printf("NEW SCORE: pl1: %d, pl2: %d\n", score.pl1, score.pl2);
			// Generate new startig speed
			int dir = (rand() % 2) * 2 - 1;
			ball_speed.y=0;
			ball_speed.x=dir*MOVEMENT_SPEED;
			
			// Waiting for user to resume game
			int waiting = 1;
			while(waiting)
			{
				SDL_PollEvent(&event);
				if (event.type == SDL_QUIT)
				{
					running = 0;	
					waiting = 0;	
				}
				if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
				{
					waiting = 0;
				}
				draw_init_game(surface, &pl1, &pl2, &ball, &border);	
				SDL_UpdateWindowSurface(window);
			}
		}
		if (score_changed && (score.pl1 == 5 || score.pl2 == 5))
		{
			printf("GAME END, PLAYER WON.\n");
			score.pl1 = 0;
			score.pl2 = 0;
			draw_init_game(surface, &pl1, &pl2, &ball, &border);	
		}
		// Redraw border
		SDL_FillRect(surface, &border, 0xffffffff);

		SDL_UpdateWindowSurface(window);
		SDL_Delay(10);
	}
}
