#include "Window.h"

#ifdef NXDK
    #include <hal/video.h>
#endif

Window::Window(const std::string &title, int width, int height) {
    #ifdef NXDK
        XVideoSetMode(width, height, 16, REFRESH_DEFAULT);
    #endif

    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER|SDL_INIT_GAMECONTROLLER) < 0){
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "Couldn't init SDL: %s", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_RESIZABLE);
    if (this->window == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "Couldn't create window: %s", SDL_GetError());
        exit(2);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (this->renderer == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_RENDER, "Couldn't create renderer: %s", SDL_GetError());
        exit(3);
    }
}

Window::~Window() {
    closeAllGameControllers();
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

std::vector<Input> Window::getInput() {
    std::vector<Input> input;

    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;
            case SDL_KEYDOWN:
                input.push_back(getInputForKeyboardKey(event.key.keysym.sym));
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                input.push_back(getInputForControllerButton(event.cbutton.button));
                break;
            case SDL_CONTROLLERDEVICEADDED:
                openGameController(event.cdevice.which);
                break;
            case SDL_CONTROLLERDEVICEREMOVED:
                closeDisconnectedGameControllers();
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
            input = Input::FIRE;
            break;

        case SDLK_r:
        case SDLK_RETURN:
        case SDLK_RETURN2:
            input = Input::RESTART;
            break;

        case SDLK_PAGEUP:
            input = Input::PREVIOUSLEVEL;
            break;

        case SDLK_PAGEDOWN:
            input = Input::NEXTLEVEL;
            break;

        case SDLK_F1:
        case SDLK_h:
            input = Input::HELP;
            break;

        case SDLK_ESCAPE:
        case SDLK_F12:
            input = Input::EXIT;
            break;

        default:
            input = Input::ANY;
            break;
    }

    return input;
}

Input Window::getInputForControllerButton(Uint8 button) {
    Input input;

    switch (button) {
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            input = Input::LEFT;
            break;

        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            input = Input::UP;
            break;

        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            input = Input::RIGHT;
            break;

        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            input = Input::DOWN;
            break;

        case SDL_CONTROLLER_BUTTON_A:
            input = Input::FIRE;
            break;

        case SDL_CONTROLLER_BUTTON_START:
            input = Input::RESTART;
            break;

        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
            input = Input::NEXTLEVEL;
            break;

        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
            input = Input::PREVIOUSLEVEL;
            break;

        case SDL_CONTROLLER_BUTTON_Y:
            input = Input::HELP;
            break;

        case SDL_CONTROLLER_BUTTON_BACK:
            input = Input::EXIT;
            break;

        default:
            input = Input::ANY;
            break;
    }

    return input;
}

void Window::openGameController(Sint32 index) {
     if (SDL_IsGameController(index)) {
        SDL_GameController * controller = SDL_GameControllerOpen(index);
        SDL_Log("Adding controller: %s", SDL_GameControllerName(controller));
        gameControllers.push_back(controller);
    }
}

void Window::closeDisconnectedGameControllers() {
    std::vector<SDL_GameController*> currentControllers;
    for(SDL_GameController * controller : gameControllers) {
        if (!SDL_GameControllerGetAttached(controller)) {
            SDL_Log("Removing controller: %s", SDL_GameControllerName(controller));
            SDL_GameControllerClose(controller);
            controller = NULL;
        } else {
            currentControllers.push_back(controller);
        }
    }

    gameControllers = currentControllers;
}

void Window::closeAllGameControllers() {
    for (unsigned int i = 0; i < gameControllers.size(); i++) {
        SDL_Log("Removing controller: %s", SDL_GameControllerName(gameControllers[i]));
        SDL_GameControllerClose(gameControllers[i]);
        gameControllers[i] = NULL;
    }
}
