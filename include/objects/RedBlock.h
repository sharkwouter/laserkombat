#ifndef REDBLOCK_H
#define REDBLOCK_H

#include <SDL.h>

#include "objects/Object.h"
#include "Textures.h"

class RedBlock : public Object {

public:

    RedBlock(Textures * textures, int rotation);
    ~RedBlock();

    void draw(SDL_Renderer * renderer, int x, int y);

};

#endif // REDBLOCK_H