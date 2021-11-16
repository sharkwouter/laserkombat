#ifndef GROUNDTILE_H
#define GROUNDTILE_H

#include <SDL.h>

#include "Tile.h"
#include "Textures.h"

class GroundTile : public Tile {

public:

	GroundTile(Textures * textures, int rotation);
	~GroundTile();

	void draw(SDL_Renderer * renderer, int x, int y);

private:

	SDL_Texture * texture;

};
#endif // GROUNDTILE_H