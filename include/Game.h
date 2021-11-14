#ifndef GAME_H
#define GAME_H

#include "Object.h"
#include "Window.h"
#include "Textures.h"
// #include "Board.h"

class Game {
    
public:

    Game(Window * window);
    ~Game();

    void run();

private:

    Window * window;

    Textures textures;

    // Board board;

    bool running = false;
};

#endif // GAME_H