#include "Game.h"

Game::Game(Window * window) : textures(window->renderer), board(&textures) {
    this->window = window;
}

Game::~Game() {
    this->running = false;
}

void Game::run() {
    this->running = true;
    while(this->running) {
        std::vector<Input> input = this->window->getInput();

        board.handleInput(input);
        board.update();
        
        SDL_SetRenderDrawColor(this->window->renderer, 255, 255, 255, 255);
        SDL_RenderClear(this->window->renderer);

        board.draw(window->renderer);

        SDL_RenderPresent(this->window->renderer);
        
        // Check if exit has been used
        for (Input i: input) {
            if (i == Input::EXIT) {
                this->running = false;
            }
        }
    }
}
