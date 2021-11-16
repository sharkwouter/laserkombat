#ifndef WATERTILE_H
#define WATERTILE_H

#include <SDL.h>

#include "tiles/Tile.h"
#include "Textures.h"

class WaterTile : public Tile {

public:

	WaterTile(Textures * textures, int rotation);
	~WaterTile();

	void draw(SDL_Renderer * renderer, int x, int y);

private:

    int animation;

	SDL_Texture * texture;

};
#endif // WATERTILE_H