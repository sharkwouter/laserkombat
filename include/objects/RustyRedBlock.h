#ifndef RUSTYREDBLOCK_H
#define RUSTYREDBLOCK_H

#include <SDL.h>

#include "objects/Object.h"
#include "Textures.h"

class RustyRedBlock : public Object {

public:

    RustyRedBlock(Textures * textures, int rotation);
    ~RustyRedBlock();

    void draw(SDL_Renderer * renderer, int x, int y);

};

#endif // RUSTYREDBLOCK_H