#include <SDL2/SDL.h>
#include <stdio.h>
#include "constants.h"
#include "Draw.h"
#include "Board.h"
#include "Sound.h"

bool running;

void handle_input(Board board) {
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
    Draw * draw = new Draw();
    Board * board = new Board(*draw);

    running = true;
    while(running) {
        handle_input(*board);
        board->Animate();
    }
}