#ifndef OBJECT_H
#define OBJECT_H

// base.h
// This is the base class for all game objects

#include <SDL.h>

#include "BlockType.h"

class Object {
    
public:

    virtual void update() { }

    virtual void draw(SDL_Renderer * renderer, int x, int y) = 0;

    virtual BlockType GetBlockType() {return blocktype;}

	virtual int GetRotation() {return rotation;}

protected:

    BlockType blocktype;

    int rotation;

    SDL_Texture * texture;

};


#endif // OBJECT_H