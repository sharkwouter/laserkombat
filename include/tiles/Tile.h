#ifndef TILE_H
#define TILE_H

#include <SDL.h>

#include "BlockType.h"

class Tile {

public:

	virtual void draw(SDL_Renderer * renderer, int x, int y) = 0;

	virtual BlockType GetBlockType() {return blocktype;}

	virtual int GetRotation() {return rotation;}

	virtual void SetOtherBlocks(unsigned int around) {
		around%=256;
		otherblocks=around+around*256;	
	}

protected:

	BlockType blocktype;

	int rotation;

	unsigned int otherblocks;

};
#endif // TILE_H