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

    void detectControllers();

};

#endif // WINDOW_H
