#include "Window.h"

#include <climits>

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
                input.push_back(Input::CLOSE);
                break;
            case SDL_KEYDOWN:
                input.push_back(getInputForKeyboardKey(event.key.keysym.sym));
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                input.push_back(getInputForControllerButton(event.cbutton.button));
                break;
            case SDL_CONTROLLERAXISMOTION: {
                    Input direction = getInputForControllerAxis(event.caxis.axis, event.caxis.value);
                    if(direction != Input::NONE) input.push_back(direction);
                }
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
        case SDLK_LEFT:
            input = Input::LEFT;
            break;

        case SDLK_UP:
            input = Input::UP;
            break;

        case SDLK_RIGHT:
            input = Input::RIGHT;
            break;

        case SDLK_DOWN:
            input = Input::DOWN;
            break;

        case SDLK_SPACE:
            input = Input::FIRE;
            break;

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

        case SDLK_u:
            input = Input::UNDO;
            break;

        case SDLK_s:
            input = Input::SAVESTATE;
            break;

        case SDLK_r:
            input = Input::RESTORESTATE;
            break;

        case SDLK_F1:
            input = Input::HELPKEYS;
            break;

        case SDLK_F2:
            input = Input::HELPBLOCKS;
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
            input = Input::HELPKEYS;
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

Input Window::getInputForControllerAxis(Uint32 axis, Sint16 value) {
    Input input = Input::NONE;

    switch (axis) {
        case SDL_CONTROLLER_AXIS_LEFTX:
            if (value > (SHRT_MAX*stick_deadzone_multiplier)) {
                if (this->returned_to_horizontal_center) {
                    input = Input::RIGHT;
                    this->returned_to_horizontal_center = false;
                }
            } else if (value < (SHRT_MIN*stick_deadzone_multiplier)) {
                if(this->returned_to_horizontal_center) {
                    input = Input::LEFT;
                    this->returned_to_horizontal_center = false;
                }
            } else {
                this->returned_to_horizontal_center = true;
            }
            break;
        case SDL_CONTROLLER_AXIS_LEFTY:
            if (value > (SHRT_MAX*stick_deadzone_multiplier)) {
                if(this->returned_to_vertical_center) {
                    input = Input::DOWN;
                    this->returned_to_vertical_center = false;
                }
            } else if (value < (SHRT_MIN*stick_deadzone_multiplier)) {
                if(this->returned_to_vertical_center) {
                    input = Input::UP;
                    this->returned_to_vertical_center = false;
                }
            } else {
                this->returned_to_vertical_center = true;
            }
            break;
        default:
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
