#ifndef SQUARE_H
#define SQUARE_H

#include "Tile.h"

class Square {

public:

	Square(Tile* b, Tile* g);
	~Square();

	Tile* over;
	Tile* block;
	GroundTile* ground;

};

#endif // SQUARE_H