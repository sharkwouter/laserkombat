#include "GroundTile.h"

#include "constants.h"
#include "Draw.h"

GroundTile::GroundTile(Textures * textures, int rotation) : texture(textures->getGroundSprites()) {
    this->blocktype = BlockType::GROUND;
    this->rotation = rotation;
}

GroundTile::~GroundTile() {

}

void GroundTile::draw(SDL_Renderer * renderer, int x, int y) {
	if (rotation!=33) {
			int r;
			if (rotation>=100) r=33-rotation/100, rotation-=100; 
			else r=rotation;
			Draw::BlitSquare(renderer, texture, r%11 ,r/11, x, y);
		}
		else {
			//calculate pieces of images to stick together

			unsigned int mask=0x7u;
			unsigned int corners[4] = {7u,7u,1u,4u};

			corners[0] = ((otherblocks& mask));
			mask*=0x4u;
			corners[1] = ((otherblocks& mask)/0x4u);
			mask*=0x4u;
			corners[2] = ((otherblocks& mask)/0x10u);
			mask*=0x4u;
			corners[3] = ((otherblocks& mask)/0x40u);
			mask*=0x4u;

			for (int i=0; i<4; i++) {
				Draw::BlitOther(renderer, texture,
					corners[i]*IMAGE_WIDTH + ((i==0||i==3)?0:(IMAGE_WIDTH/2)),
					3*IMAGE_WIDTH + ((i<2)?0:(IMAGE_WIDTH/2)),
					x*IMAGE_WIDTH + ((i==0||i==3)?0:(IMAGE_WIDTH/2)),
					y*IMAGE_WIDTH + ((i<2)?0:(IMAGE_WIDTH/2)),
					IMAGE_WIDTH/2, IMAGE_WIDTH/2);
			}
		}
}
