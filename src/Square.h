#ifndef SQUARE_H
#define SQUARE_H

#include "Tile.h"

class Square {

public:
	Square(Tile* b, Tile* g);
	~Square();

	static Square CreateSquare(int x, int y);

	Tile* over;
	Tile* block;
	Tile* ground;
};

#endif // SQUARE_H
