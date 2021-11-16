#include "objects/RustyRedBlock.h"

#include "Draw.h"

RustyRedBlock::RustyRedBlock(Textures * textures, int rotation) {
    this->blocktype = BlockType::RUSTYREDBLOCK;
    this->texture = textures->getRedblockSprites();
    this->rotation = rotation;
}

RustyRedBlock::~RustyRedBlock() {

}

void RustyRedBlock::draw(SDL_Renderer * renderer, int x, int y) {
    Draw::BlitSquare(renderer, texture, 1 ,0, x, y);
}