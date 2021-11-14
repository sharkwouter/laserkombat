#include "Game.h"

Game::Game(Window * window) {
    this->window = window;
}

Game::~Game() {
    this->running = false;
}

void Game::run() {
    this->running = true;
    while(this->running) {
        std::vector<Input> input = this->window->getInput();
        for (Input i: input) {
            if (i == Input::EXIT) {
                this->running = false;
            }
        }
    }
}
