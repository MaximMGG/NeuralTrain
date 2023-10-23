#include "snake.h"
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_keycode.h>
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

typedef struct {
    int x;
    int y;
} apple;
apple Apple;

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

void move_snake() {

    int prev_x = head->x;
    int prev_y = head->y;

    switch(head->dir) {
        case SNAKE_UP:
            head->y--;
            break;
        case SNAKE_DOWN:
            head->y++;
            break;
        case SNAKE_LEFT:
            head->x--;
            break;
        case SNAKE_RIGHT:
            head->x++;
            break;

    }

    Snake *track = head;

    if(track->next != NULL) {
        track = track->next;
    }

    while(track != NULL) {

        int save_x = track->x;
        int save_y = track->y;

        track->x = prev_x;
        track->y = prev_y;

        track = track->next;

        prev_x = save_x;
        prev_y = save_y;
    }
}


void render_snake(SDL_Renderer *renderer, int x, int y) {

    SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 255);

    int seq_size = GRID_DIM / GRID_SIZE;
    SDL_Rect seq;

    seq.w = seq_size;
    seq.h = seq_size;

    Snake *track = head;

    while(track != NULL) {
        seq.x =  x + track->x * seq_size;
        seq.y =  y + track->y * seq_size;

        SDL_RenderFillRect(renderer, &seq);

        track = track->next;
    }




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

void gen_apple() {
    Apple.x = rand() % GRID_SIZE;
    Apple.y = rand() % GRID_SIZE;
}

void render_apple(SDL_Renderer *renderer, int x, int y) {

    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 255);
    SDL_Rect app;

    int apple_size = GRID_DIM / GRID_SIZE;

    app.w = apple_size;
    app.h = apple_size;
    app.x = x + Apple.x * apple_size;
    app.y = y + Apple.y * apple_size;

    SDL_RenderFillRect(renderer, &app);



}


void detect_apple() {
    if (head->x == Apple.x && head->y == Apple.y) {
        gen_apple();
        increase_snake();
    }
}

int WinMain(int args, int **argv) {

    srand(time(NULL));

    init_snake();
    increase_snake();
    increase_snake();
    increase_snake();

    gen_apple();



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
                        case SDLK_UP:
                            head->dir = SNAKE_UP;
                            break;
                        case SDLK_DOWN:
                            head->dir = SNAKE_DOWN;
                            break;
                        case SDLK_LEFT:
                            head->dir = SNAKE_LEFT;
                            break;
                        case SDLK_RIGHT:
                            head->dir = SNAKE_RIGHT;
                            break;
                    }
                    break;
            }
        } 

        SDL_RenderClear(renderer);
        //render loop start 
            

        move_snake();
        render_grid(renderer, grid_x, grid_y);
        render_snake(renderer, grid_x, grid_y);
        render_apple(renderer, grid_x, grid_y);

        detect_apple();


        //render loop end
        SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 255);
        SDL_RenderPresent(renderer);

        SDL_Delay(200);

    }




    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
