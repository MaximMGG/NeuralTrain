#include "snake.h"
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_video.h>

#define WINDOW_X 0
#define WINDOW_Y 0
#define WINDOW_WIDTH 750 
#define WINDOW_HEIGHT 750 


#define GRID_SIZE 20 
#define GRID_DIM 660 

enum {
    SNAKE_UP,
    SNAKE_DOWN,
    SNAKE_LEFT,
    SNAKE_RIGHT,
};

struct snake {
    int x;
    int y;
    int dir;

    struct snake *next;
};
typedef struct snake Snake;

Snake *head;
Snake *tail;

void init_snake() {
    Snake *new = malloc(sizeof(Snake));
    new->x = rand() % (GRID_SIZE / 2) + (GRID_SIZE /4);
    new->y = rand() % (GRID_SIZE / 2) + (GRID_SIZE /4);
    new->dir = SNAKE_UP;
    new->next = NULL;

    head = new;
    tail = new;
}

void increase_snake() {

    Snake *new = malloc(sizeof(Snake));
    new->x = tail->x;
    new->y = tail->y - 1;
    new->dir = tail->dir;

    new->next = NULL;
    tail->next = new;

    tail = new;

}

void render_grid(SDL_Renderer *renderer, int x, int y) {
    
    SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 255);
    int cell_size = GRID_DIM / GRID_SIZE;


    SDL_Rect cell;
    cell.w = cell_size;
    cell.h = cell_size;

    for(int i = 0; i < GRID_SIZE; i++){
        for(int j = 0; j < GRID_SIZE; j++){
            cell.x = x + (i * cell_size);
            cell.y = y + (j * cell_size);
            SDL_RenderDrawRect(renderer, &cell);
        }
    }
   return; 
}


int WinMain(int args, int **argv) {

    init_snake();
    increase_snake();

    SDL_Window *window;
    SDL_Renderer *renderer;

    if (SDL_INIT_VIDEO < 0) {
        fprintf(stderr, "ERROR: SDL_INIT_VIDEO");
    }

    window = SDL_CreateWindow("Snake",
                            WINDOW_X,
                            WINDOW_Y,
                            WINDOW_WIDTH,
                            WINDOW_HEIGHT,
                            SDL_WINDOW_BORDERLESS);

    if (!window) {
        fprintf(stderr, "ERROR: window not create");
    }

    renderer = SDL_CreateRenderer(window, -1 , SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        fprintf(stderr, "ERROR: no renderer");
    }

    int grid_x = (WINDOW_WIDTH / 2 - GRID_DIM / 2);
    int grid_y = (WINDOW_HEIGHT / 2 - GRID_DIM / 2);



    bool quit = false;
    SDL_Event event;


    while(!quit) {
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYUP:
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            quit = true;
                            break;
                    }
                    break;
            }
        } 

        SDL_RenderClear(renderer);
        //render loop start 
            

        render_grid(renderer, grid_x, grid_y);


        //render loop end
        SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 255);
        SDL_RenderPresent(renderer);
    }




    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
