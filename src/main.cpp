#include "constants.h"
#include "Window.h"
#include "Game.h"

int main() {
    Window window("Laser Kombat Revised", WINDOW_WIDTH, WINDOW_HEIGHT);
    Game game(&window);

    game.run();

    return 0;
}