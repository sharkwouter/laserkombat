#include "Window.h"

Window::Window(const std::string &title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) < 0){
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "Couldn't init SDL: %s", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (this->window == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "Couldn't create window: %s", SDL_GetError());
        exit(2);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (this->renderer == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_RENDER, "Couldn't create renderer: %s", SDL_GetError());
        exit(3);
    }
}

Window::~Window() {
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

std::vector<Input> Window::getInput() {
    std::vector<Input> input;

    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                input.push_back(Input::EXIT);
                break;
            case SDL_KEYDOWN:
                Input i = getInputForKeyboardKey(event.key.keysym.sym);
                if (i != Input::NONE) {
                    input.push_back(i);
                }
                break;
        }
    }

    return input;
}

Input Window::getInputForKeyboardKey(SDL_Keycode key) {
    Input input;
    
    switch (key) {
        case SDLK_a:
        case SDLK_LEFT:
            input = Input::LEFT;
            break;

        case SDLK_w:
        case SDLK_UP:
            input = Input::UP;
            break;

        case SDLK_d:
        case SDLK_RIGHT:
            input = Input::RIGHT;
            break;

        case SDLK_s:
        case SDLK_DOWN:
            input = Input::DOWN;
            break;

        case SDLK_SPACE:
        case SDLK_LCTRL:
        case SDLK_RCTRL:
            input = Input::SHOOT;
            break;

        case SDLK_r:
        case SDLK_RETURN:
        case SDLK_RETURN2:
            input = Input::RESTART;
            break;

        case SDLK_PAGEUP:
            input = Input::NEXTLEVEL;
            break;

        case SDLK_PAGEDOWN:
            input = Input::PREVIOUSLEVEL;
            break;

        case SDLK_F1:
        case SDLK_h:
            input = Input::HELP;
            break;

        case SDLK_ESCAPE:
            input = Input::EXIT;
            break;

        default:
            input = Input::NONE;
            break;
    }

    return input;
}