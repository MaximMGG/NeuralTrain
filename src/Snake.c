#include "snake.h"
#include <SDL.h>
#include <SDL_video.h>

#define WINDOW_X 0
#define WINDOW_Y 0
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000



int WinMain(int args, int **argv) {


    SDL_Window *window;
    SDL_Renderer *rendere;

    if (SDL_INIT_VIDEO < 0) {
        fprintf(stderr, "ERROR: SDL_INIT_VIDEO");
    }

    window = SDL_CreateWindow("Snake",
                            WINDOW_X,
                            WINDOW_Y,
                            WINDOW_WIDTH,
                            WINDOW_HEIGHT,
                            SDL_WINDOW_BORDERLESS);

    return 0;
}
