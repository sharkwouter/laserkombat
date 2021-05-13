#include <SDL2/SDL.h>
#include <stdio.h>
#include "constants.h"
#include "Board.h"

SDL_Window *window;
SDL_Renderer *renderer;

bool running;

Board board;

void init() {
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) < 0){
        printf("Couldn't init SDL: %s", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WOLD_MAX_X, WOLD_MAX_Y, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        printf("Couldn't create window: %s", SDL_GetError());
        exit(2);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        printf("Couldn't create renderer: %s", SDL_GetError());
        exit(3);
    }
}

void handle_input() {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_RIGHT:
                    board.Right(); break;
                case SDLK_LEFT:
                    board.Left(); break;
                case SDLK_UP:
                    board.Up(); break;
                case SDLK_DOWN:
                    board.Down(); break;
                case SDLK_SPACE:
                    board.Fire(); break;
                case SDLK_RETURN:
                    board.Restart(); break;
                case SDLK_PAGEUP:
                    board.NextLevel(); break;
                case SDLK_PAGEDOWN:
                    board.Previous(); break;
                case SDLK_ESCAPE:
                case SDLK_F12:
                    running = false;
                    break;
                }
        }
    }
}

int main() {
    init();

    running = true;
    while(running) {
        handle_input();
        board.Animate();
    }
}