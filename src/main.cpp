#include <SDL.h>
#include <vector>

#include "Window.h"
#include "Textures.h"
#include "Sound.h"
#include "Board.h"
#include "constants.h"

Window window(TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
Textures textures(window.renderer);
Sound sound();
Board board(&textures, &sound);

bool running = false;

void handleInput(std::vector<Input> input) {
    for (Input i: input) {
        int any_ret;
        if ((any_ret=board.AnyKey(i))==666) {
            running = false;
        } else if (any_ret) {
            switch (i)
            {
            case Input::RIGHT:
                board.Right(); break;
            case Input::LEFT:
                board.Left(); break;
            case Input::UP:
                board.Up(); break;
            case Input::DOWN:
                board.Down(); break;
            case Input::FIRE:
                board.Fire(); break;
            case Input::RESTART:
                board.Restart(); break;
            case Input::NEXTLEVEL:
                board.NextLevel(); break;
            case Input::PREVIOUSLEVEL:
                board.Previous(); break;
            case Input::EXIT:
                running = false; break;
            }
        }
    }

}

int main(int, char**) {
    running = true;
    while(running) {
        handleInput(window.getInput());
        board.Animate(window.renderer);
    }

    return 0;
}