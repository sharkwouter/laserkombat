#include "WaterTile.h"

#include "constants.h"

WaterTile::WaterTile(Textures * textures, int rotation) : texture(textures->getWaterSprite()) {
    this->blocktype = BlockType::WATER;
    this->rotation = rotation;
}

WaterTile::~WaterTile() {

}

void WaterTile::draw(SDL_Renderer * renderer, int x, int y) {
    animation++;
    animation%=40;
    int dx=x*IMAGE_WIDTH;
    int dy=y*IMAGE_WIDTH;
    int sx= animation%10 *IMAGE_WIDTH;
    int sy=animation/10 * IMAGE_WIDTH;
    SDL_Rect sr={sx,sy,IMAGE_WIDTH, IMAGE_WIDTH};
    SDL_Rect rect={dx,dy,IMAGE_WIDTH, IMAGE_WIDTH};
    SDL_RenderCopy(renderer, texture, &sr, &rect);
}
