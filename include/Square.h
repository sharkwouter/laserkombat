#ifndef SQUARE_H
#define SQUARE_H

#include "Tile.h"

class Square {

public:

	Square(Tile* b, Tile* g);
	~Square();

	Tile* over = nullptr;
	Tile* block = nullptr;
	Tile* ground = nullptr;

};

#endif // SQUARE_H