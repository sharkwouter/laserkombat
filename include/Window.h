#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <vector>
#include <string>

#include "Input.h"

class Window {

public:

    Window(const std::string &title, int width, int height);
    ~Window();

    std::vector<Input> getInput();

    SDL_Renderer * renderer;

private:

    SDL_Event event;

    SDL_Window * window;

    Input getInputForKeyboardKey(SDL_Keycode);
    Input getInputForControllerButton(Uint8);
    Input getInputForControllerAxis(Uint32 axis, Sint16 value);

    void openGameController(Sint32);
    void closeDisconnectedGameControllers();
    void closeAllGameControllers();

    bool returned_to_horizontal_center;
    bool returned_to_vertical_center;
    float stick_deadzone_multiplier = 0.5;

    std::vector<SDL_GameController*> gameControllers;
};

#endif // WINDOW_H
