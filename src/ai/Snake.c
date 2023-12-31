#include "snake.h"
#include <SDL.h>
#include <SDL_blendmode.h>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_render.h>
#include <SDL_video.h>

#define WINDOW_X 0
#define WINDOW_Y 0
#define WINDOW_WIDTH 750 
#define WINDOW_HEIGHT 750 

#define GRID_SIZE 28
#define GRID_DIM 560 
#define DELAY 40

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
  new->x = rand() % (GRID_SIZE / 2) + (GRID_SIZE / 4);
  new->y = rand() % (GRID_SIZE / 2) + (GRID_SIZE / 4);
  new->dir = SNAKE_UP;
  new->next = NULL;

  head = new;
  tail = new;
}

void increase_snake() {

    Snake *new = malloc(sizeof(Snake));

    switch(tail->dir) {
        case SNAKE_UP:
            new->x = tail->x;
            new->y = tail->y + 1;
            break;
        case SNAKE_DOWN:
            new->x = tail->x;
            new->y = tail->y - 1;
            break;
        case SNAKE_LEFT:
            new->x = tail->x + 1;
            new->y = tail->y;
            break;
        case SNAKE_RIGHT:
            new->x = tail->x - 1;
            new->y = tail->y;
            break;
    
    }



    new->x = tail->x;
    new->y = tail->y;//-1
    new->dir = tail->dir;

    new->next = NULL;
    tail->next = new;

    tail = new;
}

void move_snake() {

  int prev_x = head->x;
  int prev_y = head->y;
  int prev_dir = head->dir;

  switch (head->dir) {
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

  if (track->next != NULL) {
    track = track->next;
  }

  while (track != NULL) {

    int save_x = track->x;
    int save_y = track->y;
    int save_dir = track->dir;

    track->x = prev_x;
    track->y = prev_y;
    track->dir = prev_dir;

    track = track->next;

    prev_x = save_x;
    prev_y = save_y;
    prev_dir = save_dir;
  }
}


void reset_snake() {
    Snake *track = head;
    Snake *temp;

    while(track != NULL) {
        temp = track;
        track = track->next;
        free(temp);
    }

    init_snake();
    increase_snake();
    increase_snake();
    increase_snake();
}

void render_snake(SDL_Renderer *renderer, int x, int y) {

    SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 255);

    int seq_size = GRID_DIM / GRID_SIZE;

    SDL_Rect seq;
    seq.w = seq_size - 2;
    seq.h = seq_size - 2;

    SDL_Rect seq_out;
    seq_out.w = seq_size;
    seq_out.h = seq_size;

    Snake *track = head;

    int bright = 255;
    int b_dir = 0;

    while (track != NULL) {

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, bright, 255);
        seq_out.x = x + track->x * seq_size;
        seq_out.y = y + track->y * seq_size;

        SDL_RenderFillRect(renderer, &seq_out);

        SDL_SetRenderDrawColor(renderer, 0x00, bright, 0x00, 255);
        seq.x = x + track->x * seq_size + 1;
        seq.y = y + track->y * seq_size + 1;

        SDL_RenderFillRect(renderer, &seq);

        track = track->next;
        if (b_dir == 0) {
            bright -= 5;
            if (bright < 150) {
                b_dir = 1;
            }
        }
        if (b_dir == 1) {
            bright += 5;
            if (bright > 250) {
                b_dir = 0;
            }
        }
    }
}

void render_grid(SDL_Renderer *renderer, int x, int y) {

    SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0xff, 255);

#if 0

    int cell_size = GRID_DIM / GRID_SIZE;
    SDL_Rect cell;
    cell.w = cell_size;
    cell.h = cell_size;

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            cell.x = x + (i * cell_size);
            cell.y = y + (j * cell_size);
            SDL_RenderDrawRect(renderer, &cell);
        }
    }
#else

    for(int i = 0; i < 255; i++) {
    
        SDL_SetRenderDrawColor(renderer, 0, 0, 0xff, 255 - i);

        SDL_Rect outline;
        outline.x = x - i;
        outline.y = y - i;
        outline.w = GRID_DIM + i + i;
        outline.h = GRID_DIM + i + i;

        SDL_RenderDrawRect(renderer, &outline);
    }
#endif

    return;
}

void gen_apple() {

    bool in_snake;
    do {
        in_snake = false;
        Apple.x = rand() % GRID_SIZE;
        Apple.y = rand() % GRID_SIZE;

        Snake *track = head;

        while(track != NULL) {
            if (track->x == Apple.x && track->y == Apple.y) {
                in_snake = true;
            }
            track = track->next;
        }
    }
    while(in_snake);
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


void detect_creach() {
    if (head->x < 0 || head->x >= GRID_SIZE || head->y < 0 || head->y >= GRID_SIZE) {
        reset_snake();
    }

    Snake *track = head;
    if (track->next != NULL) {
        track = track->next;
    }

    while (track != NULL) {
        if (track->x == head->x && track->y == head->y) {
            reset_snake();
            break;
        }
        track = track->next;
    }


}
//ai begin

void turn_left() {
    switch (head->dir) {
        case SNAKE_UP:
            head->dir = SNAKE_LEFT;
            break;
        case SNAKE_DOWN:
            head->dir = SNAKE_RIGHT;
            break;
        case SNAKE_LEFT:
            head->dir = SNAKE_DOWN;
            break;
        case SNAKE_RIGHT:
            head->dir = SNAKE_UP;
            break;
    }
}

void turn_right() {
    switch (head->dir) {
        case SNAKE_UP:
            head->dir = SNAKE_RIGHT;
            break;
        case SNAKE_DOWN:
            head->dir = SNAKE_LEFT;
            break;
        case SNAKE_LEFT:
            head->dir = SNAKE_UP;
            break;
        case SNAKE_RIGHT:
            head->dir = SNAKE_DOWN;
            break;
    }
}

enum {
    TRY_FORWARD,
    TRY_LEFT,
    TRY_RIGHT,
};


int state(int try) {
    int reward = 0;

    int try_x = head->x;
    int try_y = head->y;

    switch (head->dir) {
        case SNAKE_UP:
            switch(try) {
                case TRY_FORWARD:
                    try_y--;
                    break;
                case TRY_LEFT:
                    try_x--;
                    break;
                case TRY_RIGHT:
                    try_x++;
                    break;
            }
            break;
        case SNAKE_DOWN:
            switch(try) {
                case TRY_FORWARD:
                    try_y++;
                    break;
                case TRY_LEFT:
                    try_x++;
                    break;
                case TRY_RIGHT:
                    try_x--;
                    break;
            }
            break;
        case SNAKE_LEFT:
            switch(try) {
                case TRY_FORWARD:
                    try_x--;
                    break;
                case TRY_LEFT:
                    try_y++;
                    break;
                case TRY_RIGHT:
                    try_y--;
                    break;
            }
            break;
        case SNAKE_RIGHT:
            switch(try) {
                case TRY_FORWARD:
                    try_x++;
                    break;
                case TRY_LEFT:
                    try_y--;
                    break;
                case TRY_RIGHT:
                    try_y++;
                    break;
            }
            break;
        
    }

    //detect wall crach
    if (try_x < 0 || try_x > GRID_SIZE - 1) {
        reward += -100;
    }
    if (try_y < 0 || try_y > GRID_SIZE - 1) {
        reward += -100;
    }
    //detect apple
    if (try_x == Apple.x && try_y == Apple.y) {
        reward += 100;
    }
    //move towards apple
    int diff_x = abs(head->x - Apple.x);
    int diff_y = abs(head->y - Apple.y);
    int try_diff_x = abs(try_x - Apple.x);
    int try_diff_y = abs(try_y - Apple.y);

    if (try_diff_x < diff_x) {
        reward += 5;
    }
    if (try_diff_y < diff_y) {
        reward += 5;
    }


    //detect tail

    Snake *track = head;

    if (track->next != NULL) {
        track = track->next;
    }

    while(track != NULL) {
        if (try_x == track->x && try_y == track->y) {
            reward += -100;
        }
        track = track->next;
    }
    
    return reward;
}

void ai() {
    int try_f = state(TRY_FORWARD);
    int try_l = state(TRY_LEFT);
    int try_r = state(TRY_RIGHT);
    
    if (try_f >= try_l && try_f >= try_r) {
        //continue forward
    }else {
        if (try_l > try_r) {
            turn_left();
        } else {
            turn_right();
        }
    }

}

//ai end

int WinMain() {

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

  window = SDL_CreateWindow("Snake", WINDOW_X, WINDOW_Y, WINDOW_WIDTH,
                            WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);

  if (!window) {
    fprintf(stderr, "ERROR: window not create");
  }


  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (!renderer) {
    fprintf(stderr, "ERROR: no renderer");
  }

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  int grid_x = (WINDOW_WIDTH / 2 - GRID_DIM / 2);
  int grid_y = (WINDOW_HEIGHT / 2 - GRID_DIM / 2);

  bool quit = false;
  SDL_Event event;

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        quit = true;
        break;
      case SDL_KEYUP:
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
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
    // render loop start

    move_snake();
    detect_apple();
    detect_creach();

    ai();


    render_grid(renderer, grid_x, grid_y);
    render_snake(renderer, grid_x, grid_y);
    render_apple(renderer, grid_x, grid_y);



    // render loop end
    SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 255);
    SDL_RenderPresent(renderer);

    SDL_Delay(DELAY);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
