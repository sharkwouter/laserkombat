#ifndef GAME_H
#define GAME_H

#include "Object.h"
#include "Window.h"

class Game {
    
public:

    Game(Window * window);
    ~Game();

    void run();

private:

    Window * window;

    bool running = false;
};

#endif // GAME_H