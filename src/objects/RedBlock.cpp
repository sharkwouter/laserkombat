#include "objects/RedBlock.h"

#include "Draw.h"

RedBlock::RedBlock(Textures * textures, int rotation) {
    this->blocktype = BlockType::REDBLOCK;
    this->texture = textures->getRedblockSprites();
    this->rotation = rotation;
}

RedBlock::~RedBlock() {

}

void RedBlock::draw(SDL_Renderer * renderer, int x, int y) {
    Draw::BlitSquare(renderer, texture, 0 ,0, x, y);
}