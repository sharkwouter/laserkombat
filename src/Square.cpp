#include "Square.h"

Square::Square(Tile* b, Tile* g) : block(b), ground(g), over(0) {

}

Square::~Square() {
    if (block) {
        delete block; block=nullptr;
    }
    if (ground) {
        delete ground; ground=nullptr;
    }
    if (over) {
        delete over; over=nullptr;
    }
}
