#include "WaterTile.h"

#include "constants.h"

WaterTile::WaterTile(Textures * textures, int rotation) : texture(textures->getWaterSprite()) {
    this->blocktype = BlockType::WATER;
    this->rotation = rotation;
}

WaterTile::~WaterTile() {

}

void WaterTile::draw(SDL_Renderer * renderer, int x, int y) {
    SDL_Log("Does this happen?");
    animation++;
    animation%=40;
    x= animation%10 *IMAGE_WIDTH;
    y=animation/10 * IMAGE_WIDTH;
    int dx=x*IMAGE_WIDTH;
    int dy=y*IMAGE_WIDTH;
    SDL_Rect sr={x,y,x+IMAGE_WIDTH, y+IMAGE_WIDTH};
    SDL_Rect rect={dx,dy,dx+IMAGE_WIDTH, dy+IMAGE_WIDTH};
    SDL_RenderCopy(renderer, texture, &sr, &rect);
}
