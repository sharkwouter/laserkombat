#ifndef OBJECT_H
#define OBJECT_H

// base.h
// This is the base class for all game objects

#include <vector>
#include <SDL.h>

#include "Input.h"

class Object {
    
public:

    virtual void handleInput(std::vector<Input> input) { }

    virtual void update() { }

    virtual void draw(SDL_Renderer * renderer) = 0;
};


#endif // OBJECT_H