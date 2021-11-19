#ifndef TILE_H
#define TILE_H

#include <SDL.h>

#include "BlockType.h"
#include "Draw.h"
#include "Textures.h"
#include "constants.h"

class Tile {

public:

    Tile(int rotation=0) {
        SetRotation(rotation);
        moved = false;
    };
    ~Tile() {};

    virtual BlockType GetBlockType() {return BlockType::NONE;};
    virtual int GetRotation() {return rotation;}

    virtual bool SetRotation(int r) {
		rotation=r;
		rotation%=4;
		return true;
	}

    virtual bool RotateLeft() {
		rotation--;
		if (rotation<0) rotation=0;
		return true;
	}
    virtual bool RotateRight() {
		rotation++;
		SetRotation(rotation);
		return true;
	}

    virtual void Update(SDL_Renderer * renderer, int x, int y) {
        Draw(renderer, x, y);
        moved = false;
		
	}

protected:
    virtual void Draw(SDL_Renderer * renderer, int x, int y) {
        Draw::BlackSquare(renderer, x, y);
    }

    int rotation;

    bool moved;

};


class GroundTile : public Tile {

public:
    GroundTile(Textures * textures, int rotation=33) : Tile(rotation) {
        SetRotation(rotation);
        if(!surface) SetSurface(textures);
    }

    virtual BlockType GetBlockType() {return BlockType::GROUND;};

    virtual bool SetRotation(int r) {
		rotation=r;
		return true;
	}

    virtual void SetOtherBlocks(unsigned int around) {
		around%=256;
		otherblocks=around+around*256;	
	}

    static bool SetChanged(bool change) {
		bool ret=changed;
		changed=change;
		return ret;
	}

protected:

    virtual void Draw(SDL_Renderer * renderer, int x, int y) {
        if (rotation!=33) {
            int r;
            if (rotation>=100) r=33-rotation/100, rotation-=100; 
            else r=rotation;
            if (surface) Draw::BlitSquare(renderer, surface, r%11 ,r/11, x, y);
        } else {
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

            if (surface) {
                for (int i=0; i<4; i++) {
                    Draw::BlitOther(renderer, surface,
                        corners[i]*IMAGE_WIDTH + ((i==0||i==3)?0:(IMAGE_WIDTH/2)),
                        3*IMAGE_WIDTH + ((i<2)?0:(IMAGE_WIDTH/2)),
                        x*IMAGE_WIDTH + ((i==0||i==3)?0:(IMAGE_WIDTH/2)),
                        y*IMAGE_WIDTH + ((i<2)?0:(IMAGE_WIDTH/2)),
                        IMAGE_WIDTH/2, IMAGE_WIDTH/2);
                }
            }
        }
    }

private:

    virtual void SetSurface(Textures * textures) {
		surface=textures->getGroundSprites();
	}

    SDL_Texture * surface = NULL;

    inline static bool changed=false;
    unsigned int otherblocks;
};

class Water : public GroundTile
{
public:
	Water(Textures * textures, int rotation=0) : GroundTile(textures, rotation) {
		SetRotation(rotation);
        if (!surface) SetSurface(textures);
	}

	virtual bool SetRotation(int r) {
		rotation=r;
		return true;
	}
	virtual BlockType GetBlockType() {
		return BlockType::WATER;
	}

	static void IncStaticRotation() {
		static bool chop=true;
		chop=!chop;
		if (chop) {
			Water::static_rotation++;
			Water::static_rotation%=40;
		}
	}

protected: //functions
	virtual void Water::Draw(SDL_Renderer * renderer, int x, int y) {
		//Exception::Output("Tank::draw called.");
		if (rotation>=100) {
			GroundTile::Draw(renderer, x, y);
			return;
		}
		if (surface) Draw::BlitSquare(renderer, surface, static_rotation%10 ,static_rotation/10, x, y); 
	}

private: // functions
	virtual void SetSurface(Textures * textures) {
		surface=textures->getWaterSprite();
	}

private: //members
	inline static int static_rotation=0;

    SDL_Texture * surface = NULL;
};

#endif // TILE_H