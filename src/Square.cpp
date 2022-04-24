#include "Square.h"

Square::Square(Tile* b, Tile* g) : block(b), ground(g) {

}

Square::~Square() {
    if (block) {
        delete block; block=NULL;
    }
    if (ground) {
        delete ground; ground=NULL;
    }
    if (over) {
        delete over; over=NULL;
    }
}
