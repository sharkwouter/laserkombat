#include "GroundTile.h"

#include "constants.h"

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

            x= r%11 *IMAGE_WIDTH;
            y=r/11 * IMAGE_WIDTH;
            int dx=x*IMAGE_WIDTH;
            int dy=y*IMAGE_WIDTH;
            SDL_Rect sr={x,y,x+IMAGE_WIDTH, y+IMAGE_WIDTH};
            SDL_Rect rect={dx,dy,dx+IMAGE_WIDTH, dy+IMAGE_WIDTH};
            SDL_RenderCopy(renderer, texture, &sr, &rect);
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
					x = corners[i]*IMAGE_WIDTH + ((i==0||i==3)?0:(IMAGE_WIDTH/2));
					y = 3*IMAGE_WIDTH + ((i<2)?0:(IMAGE_WIDTH/2));
					int dx = x*IMAGE_WIDTH + ((i==0||i==3)?0:(IMAGE_WIDTH/2));
					int dy = y*IMAGE_WIDTH + ((i<2)?0:(IMAGE_WIDTH/2));
                SDL_Rect rect={x,y,x+IMAGE_WIDTH/2, y+IMAGE_WIDTH/2};
                SDL_RenderCopy(renderer, texture, NULL, &rect);
			}
		}
}
