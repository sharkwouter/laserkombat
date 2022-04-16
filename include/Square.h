#ifndef SQUARE_H
#define SQUARE_H

#include "Tile.h"

class Square {

public:

	Square(Tile* b, Tile* g);
	~Square();

	Tile* over = NULL;
	Tile* block = NULL;
	Tile* ground = NULL;

};

#endif // SQUARE_H