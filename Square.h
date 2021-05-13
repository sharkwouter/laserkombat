// square.h

class Tile;

class Square
{
public:
	Square(Tile* b, Tile* g) : block(b), ground(g), over(0) {}
	~Square() {
		if (block) { delete block; block=NULL; }
		if (ground) { delete ground; ground=NULL; }
		if (over) { delete over; over=NULL; }
	}
	Tile* over;
	Tile* block;
	Tile* ground;
};

