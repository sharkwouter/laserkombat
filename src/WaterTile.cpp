#include "WaterTile.h"

#include "constants.h"
#include "Draw.h"

WaterTile::WaterTile(Textures * textures, int rotation) : texture(textures->getWaterSprite()) {
    this->blocktype = BlockType::WATER;
    this->rotation = rotation;
}

WaterTile::~WaterTile() {

}

void WaterTile::draw(SDL_Renderer * renderer, int x, int y) {
    Draw::BlitSquare(renderer, texture, Draw::animation%10 ,Draw::animation/10, x, y);
}
