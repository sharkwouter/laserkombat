#ifndef TILE_H
#define TILE_H

#include <SDL.h>

#include "constants.h"
#include "BlockType.h"
#include "Textures.h"
#include "Sound.h"
#include "Draw.h"

class Board;
class Tile;

struct Blocks {
	Tile* right;
	Tile* left;
	Tile* above;
	Tile* below;
	Tile* belowright;
	Tile* belowleft;
	Tile* aboveright;
	Tile* aboveleft;
};

class Tile
{
	friend class ATile;
public:
	Tile(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : x_pos(x), y_pos(y), draw(draw), textures(textures), sound(sound), board(board) {
		SetRotation(r);
		FiringUp=FiringDown=FiringLeft=FiringRight=Firing=0;
		WasHitLeft=WasHitRight=WasHitTop=WasHitBottom=WasHit=0;
		HadFiredLeft=HadFiredRight=HadFiredUp=HadFiredDown=HadFired=0;
		moved=false;
	}
	virtual ~Tile() {
		if (deadBlock) delete deadBlock;
	}

	void SetXY(int x, int y) {x_pos=x, y_pos=y;}
	void SetXY(SDL_Point xy) {x_pos=xy.x, y_pos=xy.y;}
	SDL_Point getXY() {return {x_pos, y_pos};}

	virtual void SetOtherBlocks (unsigned int) {}

	virtual bool Kill();
	virtual bool KillOver();

	virtual bool Drown() {Tile::Kill(); return true;}
	virtual bool DrownOver() {Tile::KillOver(); return true;}

	virtual bool WillKill(int flag=2);

	virtual BlockType GetBlockType() {
		return BlockType::NONE;
	}

	virtual bool BlockOver(Tile* &, Tile* &ground)
	{
		if (ground) ground->ClearOver();
		return false;
	}

	void ClearOver();	//make sure the over layer is clear


	int GetRotation() {return rotation;}
	virtual bool SetRotation(int r) {
		rotation=r;
		rotation%=4;
		return true;
	}

	virtual bool HitLeft() {
		Tile* ground=GetGround();
		if (ground) ground->WasHit=ground->WasHitLeft=BEAM_PERSISTANCE;
		return true;
	}
	virtual bool HitTop() {
		Tile* ground=GetGround();
		if (ground) ground->WasHit=ground->WasHitTop=BEAM_PERSISTANCE;
		return true;
	}
	virtual bool HitRight() {
		Tile* ground=GetGround();
		if (ground) ground->WasHit=ground->WasHitRight=BEAM_PERSISTANCE;
		return true;
	}
	virtual bool HitBottom() {
		Tile* ground=GetGround();
		if (ground) ground->WasHit=ground->WasHitBottom=BEAM_PERSISTANCE;
		return true;
	}

	virtual bool LookLeft(BlockType, int) {return false;}
	virtual bool LookTop(BlockType, int) {return false;}
	virtual bool LookRight(BlockType, int) {return false;}
	virtual bool LookBottom(BlockType, int) {return false;}


	virtual bool SeeMeUp(BlockType type, int dist);
	virtual bool SeeMeDown(BlockType type, int dist);
	virtual bool SeeMeRight(BlockType type, int dist);
	virtual bool SeeMeLeft(BlockType type, int dist);

	virtual bool ShootUp();
	virtual bool ShootDown();
	virtual bool ShootRight();
	virtual bool ShootLeft();


	virtual bool PushLeft() {return MoveRight();}
	virtual bool PushRight() {return MoveLeft();}
	virtual bool PushTop() {return MoveDown();}
	virtual bool PushBottom() {return MoveUp();}

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

	virtual void Update() {
		Display();
		BeamState();
		DisplayBeam();
		if (Firing) Firing=FiringRight=FiringLeft=FiringUp=FiringDown=false;
		moved=false;
	}

	virtual bool PreSeeMe() {
		return false;
	}

	virtual bool AfterAnimate() {
		if (willKillFlag<10) FinalizeOver();
		else {willKillFlag-=10; return true;}
		return false;
	}

	void FinalizeOver();

	void AddDead(Tile* tile);

public: // members
	bool moved;

	bool FiringUp, FiringDown, FiringLeft, FiringRight, Firing;
	int WasHitLeft, WasHitRight, WasHitTop, WasHitBottom, WasHit;
	int HadFiredLeft, HadFiredRight, HadFiredUp, HadFiredDown, HadFired;

protected: //functions
	virtual void Display() {
		BeamState();
		DisplayBeam();
		draw->BlackSquare(x_pos, y_pos);
	}
	virtual int BeamState();
	bool SeeMe(BlockType type) {
		SeeMeLeft(type, 0);
		SeeMeUp(type, 0);
		SeeMeRight(type, 0);
		SeeMeDown(type, 0);
		return true;
	}

	static bool CheckInRange(int x, int y) {
		if (x>=COLUMNS||x<0) return false;
		if (y>=ROWS||y<0) return false;
		return true;
	}
	Blocks GetBlocks();
	Blocks GetGrounds();
	Tile*& GetGround();
	Tile* GetBlock();

	bool RightBlock(Tile** p);
	bool LeftBlock(Tile** p);
	bool TopBlock(Tile** p);
	bool BottomBlock(Tile** p);

	bool RightGround(Tile** p);
	bool LeftGround(Tile** p);
	bool TopGround(Tile** p);
	bool BottomGround(Tile** p);

	void DisplayBeam() {
		if (HadFiredLeft||WasHitLeft)
			draw->BlitSquare(textures->getBeamSprites(), 0 ,1, x_pos, y_pos);
		if (HadFiredUp||WasHitTop)
			draw->BlitSquare(textures->getBeamSprites(), 1 ,1, x_pos, y_pos);
		if (HadFiredRight||WasHitRight)
			draw->BlitSquare(textures->getBeamSprites(), 2 ,1, x_pos, y_pos);
		if (HadFiredDown||WasHitBottom)
			draw->BlitSquare(textures->getBeamSprites(), 3 ,1, x_pos, y_pos);
	}

	virtual bool MoveUp();
	virtual bool MoveDown();
	virtual bool MoveRight();
	virtual bool MoveLeft();

protected: //members
	int cols;
	int rows;

	int x_pos, y_pos;
	int rotation;

	Draw * draw;
	Textures * textures;
	Sound * sound;
	Board * board;

private: // functions
	//Set for killing, just not immediatly.

private: //members
	int willKillFlag = 0;
	Tile* deadBlock = NULL;


};

// Tile.h

class GroundTile : public Tile
{
public:
	GroundTile(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=33) : Tile(x,y,draw,textures,sound,board,r), otherblocks(0) {
		SetRotation(r);

	}
	virtual bool SetRotation(int r) {
		rotation=r;
		return true;
	}

	virtual void SetOtherBlocks(unsigned int around) {
		around%=256;
		otherblocks=around+around*256;	
	}

	virtual bool Kill() {return false;}

	virtual bool HitLeft() {Tile::HitLeft(); ShootRight(); return true;}
	virtual bool HitTop() {Tile::HitTop(); ShootDown(); return true;}
	virtual bool HitRight() {Tile::HitRight(); ShootLeft(); return true;}
	virtual bool HitBottom() {Tile::HitBottom(); ShootUp(); return true;}

	virtual bool LookLeft(BlockType type, int dist) {SeeMeRight(type, ++dist); return true;}
	virtual bool LookTop(BlockType type, int dist) {SeeMeDown(type, ++dist); return true;}
	virtual bool LookRight(BlockType type, int dist) {SeeMeLeft(type, ++dist); return true;}
	virtual bool LookBottom(BlockType type, int dist) {SeeMeUp(type, ++dist); return true;}

	virtual BlockType GetBlockType() {
		return BlockType::GROUND;
	}

	static bool SetChanged(bool change) {
		bool ret=changed;
		changed=change;
		return ret;
	}

protected: //functions
	virtual void Display() {
		if (rotation!=33) {
			int r;
			if (rotation>=100) r=33-rotation/100, rotation-=100; 
			else r=rotation;
			draw->BlitSquare(textures->getGroundSprites(), r%11 ,r/11, x_pos, y_pos);
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
				draw->BlitOther(textures->getGroundSprites(),
					corners[i]*IMAGE_WIDTH + ((i==0||i==3)?0:(IMAGE_WIDTH/2)),
					3*IMAGE_WIDTH + ((i<2)?0:(IMAGE_WIDTH/2)),
					x_pos*IMAGE_WIDTH + ((i==0||i==3)?0:(IMAGE_WIDTH/2)),
					y_pos*IMAGE_WIDTH + ((i<2)?0:(IMAGE_WIDTH/2)),
					IMAGE_WIDTH/2, IMAGE_WIDTH/2);
			}
		}
	}
protected: //members

private: // functions
private: //members
	static bool changed;
	unsigned int otherblocks;
};

class ATile : public Tile
{
public:

	ATile(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : Tile(x,y,draw,textures,sound,board,r) {

	}

	virtual void Update() {
		Display();
		BeamState();
		if (Firing) Firing=FiringRight=FiringLeft=FiringUp=FiringDown=false;
		moved=false;
	}


protected: //functions
protected: //members
private: // functions
private: //members


};

class Tank : public ATile
{
public:
	Tank(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : ATile(x,y,draw,textures,sound,board,r) {
		SetRotation(r);

	}

	virtual bool SetRotation(int r) {
		rotation=r%12;
		return true;
	}

	virtual bool PreSeeMe() {
		return SeeMe(BlockType::TANK);
	}

	virtual BlockType GetBlockType()
	{
		return BlockType::TANK;
	}

	virtual bool ShootUp() {
		bool ret=ATile::ShootUp();
		return ret;

	}
	virtual bool ShootDown() {
		bool ret=ATile::ShootDown();
		return ret;

	}
	virtual bool ShootRight() {
		bool ret=ATile::ShootRight();
		return ret;

	}
	virtual bool ShootLeft() {
		bool ret=ATile::ShootLeft();
		return ret;

	}
	virtual bool HitLeft() {ATile::HitLeft(); sound->PlayASound("pop.wav", POP); return WillKill();}
	virtual bool HitTop() {ATile::HitTop(); sound->PlayASound("pop.wav", POP); return WillKill();}
	virtual bool HitRight() {ATile::HitRight(); sound->PlayASound("pop.wav", POP); return WillKill();}
	virtual bool HitBottom() {ATile::HitBottom(); sound->PlayASound("pop.wav", POP); return WillKill();}

	virtual bool RotateLeft() {
		int row=rotation/4;
		rotation--;
		if (rotation<0) rotation=0;
		if (rotation/4!=row) rotation+=4;

		return true;
	}
	virtual bool RotateRight() {
		int row=rotation/4;
		rotation++;
		if (rotation/4!=row) rotation-=4;

		return true;
	}


protected: //functions
	virtual int BeamState() {
		return 0;
	}
	virtual void Display() {
		draw->BlitSquare(textures->getTankSprites(), rotation%4 ,rotation/4, x_pos, y_pos);
	}
	virtual bool MoveUp() {
		bool ret=ATile::MoveUp();
		return ret;
	}
	virtual bool MoveDown() {
		bool ret=ATile::MoveDown();
		return ret;
	}
	virtual bool MoveRight() {
		bool ret=ATile::MoveRight();
		return ret;
	}
	virtual bool MoveLeft() {
		bool ret=ATile::MoveLeft();
		return ret;

	}
protected: //members


private: // functions
private: //members

};

class RedBlock : public ATile
{
public:
	RedBlock(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : ATile(x,y,draw,textures,sound,board,0) {
		SetRotation(r);

	}
	virtual BlockType GetBlockType() {
		return BlockType::REDBLOCK;
	}
	virtual bool SetRotation(int) {
		rotation=0;
		return false;
	}

	virtual bool HitLeft() {Tile::HitLeft(); PushLeft(); return true;}
	virtual bool HitTop() {Tile::HitTop(); PushTop(); return true;}
	virtual bool HitRight() {Tile::HitRight(); PushRight(); return true;}
	virtual bool HitBottom() {Tile::HitBottom(); PushBottom(); return true;}

protected: //functions
	virtual void Display() {
		draw->BlitSquare(textures->getRedblockSprites(), 0 ,0, x_pos, y_pos);
	}

protected: //members
private: // functions
private: //members

};

class WhiteBlock : public RedBlock
{
public:
	WhiteBlock(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : RedBlock(x,y,draw,textures,sound,board,2) {
		SetRotation(r);

	}
	virtual BlockType GetBlockType() {
		return BlockType::WHITEBLOCK;
	}
	virtual bool SetRotation(int) {
		rotation=2;
		return false;
	}

protected: //functions
	virtual void Display() {
		draw->BlitSquare(textures->getRedblockSprites(), 2 ,0, x_pos, y_pos);
	}
	virtual bool MoveUp() {sound->PlayASound("thunk.wav", THUNK); return Tile::MoveUp();}
	virtual bool MoveDown() {sound->PlayASound("thunk.wav", THUNK); return Tile::MoveDown();}
	virtual bool MoveLeft() {sound->PlayASound("thunk.wav", THUNK); return Tile::MoveLeft();}
	virtual bool MoveRight() {sound->PlayASound("thunk.wav", THUNK); return Tile::MoveRight();}
protected: //members
private: // functions
private: //members

};

class Water : public GroundTile
{
public:
	Water(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : GroundTile(x,y,draw,textures,sound,board,r) {
		SetRotation(r);

	}

	virtual bool BlockOver(Tile* &block, Tile* &ground);
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
			static_rotation++;
			static_rotation%=40;
		}
	}

protected: //functions
	virtual void Display() {
		if (rotation>=100) {
			GroundTile::Display();
			return;
		}
		draw->BlitSquare(textures->getWaterSprite(), static_rotation%10 ,static_rotation/10, x_pos, y_pos);
	}
protected: //members

private: // functions
private: //members
	static int static_rotation;
};

class Static : public ATile
{
public:
	Static(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : 
	  ATile(x,y,draw,textures,sound,board,r), turn_right(r>8?0:1) , time(0), span(0) {
		SetRotation(r);

	}
	virtual BlockType GetBlockType() {
		return BlockType::STATIC;
	}
	virtual bool SetRotation(int r) {
		if (r>8) r-=4;
		rotation=r;
		return true;
	}

protected: //functions
	virtual void Display() {
		if (span>=2) {
			time++;
			time%=(span-1);
		}
		if ((!time)&&rotation>=5&&span<6) {
			if (turn_right) {rotation++;
				if (rotation>=9) rotation=5;
			} 
			else {
					rotation--;
					if (rotation<=4) rotation=8;
			}
			if (span&&!(rand()%(17-((span-1)*(span-1))*1))) span++;
		}
		draw->BlitSquare(textures->getStaticSprites(), rotation%9 ,0, x_pos, y_pos);
	}
	virtual bool PushLeft() {return false;}
	virtual bool PushRight() {return false;}
	virtual bool PushTop() {return false;}
	virtual bool PushBottom() {return false;}

	virtual bool HitLeft() {sound->PlayASound("donk.wav", DONK); return (span=1);}
	virtual bool HitRight() {sound->PlayASound("donk.wav", DONK); return (span=1);}
	virtual bool HitTop() {sound->PlayASound("donk.wav", DONK); return (span=1);}
	virtual bool HitBottom() {sound->PlayASound("donk.wav", DONK); return (span=1);}



protected: //members
private: // functions
private: //members
	bool turn_right;
	int time, span;

};

class Mirror : public ATile
{
public:
	Mirror(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : ATile(x,y,draw,textures,sound,board,r), rotation2(0){
		SetRotation(r);

	}
	virtual BlockType GetBlockType() {
		return BlockType::MIRROR;
	}
	virtual bool SetRotation(int r) {
		rotation=r;
		rotation%=16;
		return true;
	}

protected: //functions
	virtual void Display() {
		rotation2++;
		if (rotation2>=7) {
			rotation2=0;
			rotation+=4;
			rotation%=16;
		}
		draw->BlitSquare(textures->getMirrorSprites(), rotation%4 , HadFired?4:(rotation/4), x_pos, y_pos);
	}
	virtual int BeamState();


	virtual bool HitLeft() {
		ATile::HitLeft();
		switch(rotation%4) {
		case 1: if (Firing) return false;
			ShootDown();
			break;
		case 2:  if (Firing) return false;
			ShootUp();
			break;
		case 0:
		case 3:
			return PushLeft();
			break;
		default:
			return false;
			break;
		}
		return true;

	}
	virtual bool HitTop() {
		ATile::HitTop();
		switch(rotation%4) {
		case 0:
		case 1:
			return PushTop();
			break;
		case 2: if (Firing) return false;
			ShootLeft();
			break;
		case 3: if (Firing) return false;
			ShootRight();
			break;
		default:
			return false;
			break;
		}
		return true;

	}
	virtual bool HitRight() {
		ATile::HitRight();
		switch(rotation%4) {
		case 0: if (Firing) return false;
			ShootDown();
			break;
		case 1:
		case 2:
			return PushRight();
			break;
		case 3: if (Firing) return false;
			ShootUp();
			break;
		default:
			return false;
			break;
		}
		return true;

	}
	virtual bool HitBottom() {
		ATile::HitBottom();
		switch(rotation%4) {
		case 0: if (Firing) return false;
			ShootRight();
			break;
		case 1: if (Firing) return false;
			ShootLeft();
			break;
		case 2:
		case 3:
			return PushBottom();
			break;
		default:
			return false;
			break;
		}
		return true;

	}



protected: //members
		int rotation2;

private: // functions
private: //members
};

class Tee : public Mirror
{
public:
	Tee(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : Mirror(x,y,draw,textures,sound,board,r) {
		SetRotation(r);

	}
	virtual BlockType GetBlockType() {
		return BlockType::TEE;
	}
	virtual bool SetRotation(int r) {
		rotation=r;
		return true;
	}
protected: //functions
	virtual void Display() {
		rotation2++;
		if (rotation2>=7) {
			rotation2=0;
			rotation+=7;
			rotation%=28;
		}
		draw->BlitSquare(textures->getTeeSprites(), rotation%7 , HadFired?4:(rotation/7), x_pos, y_pos);
	}

	virtual bool HitLeft() {
		Tile::HitLeft();
		switch(rotation%7) {
		case 0: if (Firing) return false;
			ShootRight();
			ShootDown();
			break;
		case 1: if (Firing) return false;
			ShootUp();
			ShootDown();
			break;
		case 2: if (Firing) return false;
			ShootUp();
			ShootRight();
			break;
		case 3:
		case 6:
			return PushLeft();
			break;
		case 4: if (Firing) return false;
			ShootUp();
			ShootRight();
			ShootDown();
			break;
		case 5: if (Firing) return false;
			ShootRight();
			break;
		default:
			return false;
			break;
		}
		return true;

	}
	virtual bool HitTop() {
		Tile::HitTop();
		switch(rotation%7) {
		case 1: if (Firing) return false;
			ShootLeft();
			ShootDown();
			break;
		case 0:
		case 5:
			return PushTop();
			break;
		case 2: if (Firing) return false;
			ShootLeft();
			ShootRight();
			break;
		case 3: if (Firing) return false;
			ShootRight();
			ShootDown();
			break;
		case 4: if (Firing) return false;
			ShootLeft();
			ShootRight();
			ShootDown();
			break;
		case 6: if (Firing) return false;
			ShootDown();
			break;
		default:
			return false;
			break;
		}
		return true;

	}
	virtual bool HitRight() {
		Tile::HitRight();
		switch(rotation%7) {
		case 0: if (Firing) return false;
			ShootLeft();
			ShootDown();
			break;
		case 1:
		case 6:
			return PushRight();
		case 2: if (Firing) return false;
			ShootLeft();
			ShootUp();
			break;
		case 3: if (Firing) return false;
			ShootUp();
			ShootDown();
			break;
		case 4: if (Firing) return false;
			ShootLeft();
			ShootUp();
			ShootDown();
			break;
		case 5: if (Firing) return false;
			ShootLeft();
			break;
		default:
			return false;
			break;
		}
		return true;

	}
	virtual bool HitBottom() {
		Tile::HitBottom();
		switch(rotation%7) {
		case 0: if (Firing) return false;
			ShootLeft();
			ShootRight();
			break;
		case 1: if (Firing) return false;
			ShootLeft();
			ShootUp();
			break;
		case 3: if (Firing) return false;
			ShootUp();
			ShootRight();
			break;
		case 2:
		case 5:
			return PushBottom();
			break;
		case 4: if (Firing) return false;
			ShootLeft();
			ShootUp();
			ShootRight();
			break;
		case 6: if (Firing) return false;
			ShootUp();
			break;
		default:
			return false;
			break;
		}
		return true;

	}



protected: //members
private: // functions
private: //members
};

class EnemyTank : public Tank
{
public:
	EnemyTank(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : Tank(x,y,draw,textures,sound,board,r) {

	}
	virtual bool SetRotation(int r) {return ATile::SetRotation(r);}

	virtual bool PreSeeMe() {
		return false;
	}

	virtual BlockType GetBlockType() {
		return BlockType::ENEMYTANK;
	}

	virtual bool LookLeft(BlockType, int) {ShootLeft(); return true;}
	virtual bool LookTop(BlockType, int) {ShootUp(); return true;}
	virtual bool LookRight(BlockType, int) {ShootRight(); return true;}
	virtual bool LookBottom(BlockType, int) {ShootDown(); return true;}

	virtual bool ShootUp() {SetRotation(1); return ATile::ShootUp();}
	virtual bool ShootDown() {SetRotation(3); return ATile::ShootDown();}
	virtual bool ShootRight() {SetRotation(2); return ATile::ShootRight();}
	virtual bool ShootLeft() {SetRotation(0); return ATile::ShootLeft();}

protected: //functions
	virtual void Display() {
		int rotate=rand()%1000;
		if (rotate==0) RotateRight();	//make tanks rotate randomly
		else if (rotate==1) RotateLeft();
		draw->BlitSquare(textures->getTankSprites(), rotation ,1, x_pos, y_pos);
	}
	virtual bool MoveUp() {
		return ATile::MoveUp();
	}
	virtual bool MoveDown() {
		return ATile::MoveDown();
	}
	virtual bool MoveRight() {
		return ATile::MoveRight();
	}
	virtual bool MoveLeft() {
		return ATile::MoveLeft();
	}
protected: //members
private: // functions
private: //members

};

class Nuke : public ATile
{
public:
	Nuke(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : ATile(x,y,draw,textures,sound,board,r) {

	}
	virtual bool HitLeft() {ATile::HitLeft(); Kill(); return true;}
	virtual bool HitTop() {ATile::HitTop(); Kill(); return true;}
	virtual bool HitRight() {ATile::HitRight(); Kill(); return true;}
	virtual bool HitBottom() {ATile::HitBottom(); Kill(); return true;}

	virtual BlockType GetBlockType() {
		return BlockType::NUKE;
	}

	virtual bool Kill();
protected: //functions
	virtual void Display() {
		draw->BlitSquare(textures->getNukeSprites(), 0 ,0, x_pos, y_pos);
	}

protected: //members
private: // functions
private: //members

};

class Rusty : public Static
{
public:
	Rusty(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : Static(x,y,draw,textures,sound,board,r), turn_right(r>8?0:1) {

	}

	virtual bool HitLeft() {ATile::HitLeft(); sound->PlayASound("collapse.wav", COLLAPSE); return Kill();}
	virtual bool HitTop() {ATile::HitTop(); sound->PlayASound("collapse.wav", COLLAPSE); return Kill();}
	virtual bool HitRight() {ATile::HitRight(); sound->PlayASound("collapse.wav", COLLAPSE); return Kill();}
	virtual bool HitBottom() {ATile::HitBottom(); sound->PlayASound("collapse.wav", COLLAPSE); return Kill();}

	virtual BlockType GetBlockType() {
		return BlockType::RUSTY;
	}
protected: //functions
	virtual void Display() {
		if (rotation>=5) {
			if (turn_right) {rotation++;
				if (rotation>=9) rotation=5;
			} 
			else {
					rotation--;
					if (rotation<=4) rotation=8;
			}
		}
		draw->BlitSquare(textures->getRustySprites(), rotation%9 ,0, x_pos, y_pos);
	}



protected: //members
private: // functions

	bool turn_right;
};

class RustyRedBlock :public RedBlock
{
public:
	RustyRedBlock(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : RedBlock(x,y,draw,textures,sound,board,1) {
		SetRotation(r);

	}
	virtual BlockType GetBlockType() {
		return BlockType::RUSTYREDBLOCK;
	}
	virtual bool SetRotation(int) {
		rotation=1;
		return false;
	}
	virtual bool HitLeft() {ATile::HitLeft(); sound->PlayASound("pop.wav", POP); return Kill();}
	virtual bool HitTop() {ATile::HitTop(); sound->PlayASound("pop.wav", POP); return Kill();}
	virtual bool HitRight() {ATile::HitRight(); sound->PlayASound("pop.wav", POP); return Kill();}
	virtual bool HitBottom() {ATile::HitBottom(); sound->PlayASound("pop.wav", POP); return Kill();}

protected: //functions
	virtual void Display() {
		draw->BlitSquare(textures->getRedblockSprites(), 1 ,0, x_pos, y_pos);
	}

protected: //members
private: // functions
private: //members

};

class EnemyNuke : public Nuke
{
public:
	EnemyNuke(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : Nuke(x,y,draw,textures,sound,board,r) {

	}

	virtual bool LookLeft(BlockType, int dist) {if (!dist) Kill(); return true;}
	virtual bool LookTop(BlockType, int dist) {if (!dist) Kill(); return true;}
	virtual bool LookRight(BlockType, int dist) {if (!dist) Kill(); return true;}
	virtual bool LookBottom(BlockType, int dist) {if (!dist) Kill(); return true;}

	virtual BlockType GetBlockType() {
		return BlockType::ENEMYNUKE;
	}

protected: //functions
	virtual void Display() {
		draw->BlitSquare(textures->getNukeSprites(), 1 ,0, x_pos, y_pos);
	}

protected: //members
private: // functions
private: //members

};

class Message : public Static
{
public:
	Message(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : Static(x,y,draw,textures,sound,board,r) {
		SetRotation(r);

	}
	virtual BlockType GetBlockType() {
		return BlockType::NONE;
	}
	virtual bool SetRotation(int r) {
		rotation=r;
		return true;
	}

protected: //functions
	virtual void Display() {
		draw->BlitSquare(textures->getMessageSprite(), rotation%14 ,rotation/14, x_pos, y_pos);
	}

protected: //members
private: // functions
private: //members

};

class BarsVert : public Static
{
public:
	BarsVert(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : 
	  Static(x,y,draw,textures,sound,board,r) {
		SetRotation(0);

	}
	virtual BlockType GetBlockType() {
		return BlockType::BARSVERT;
	}
	virtual bool SetRotation(int) {
		rotation=0;
		return false;
	}

protected: //functions
	virtual void Display() {

		draw->BlitSquare(textures->getBarSprites(), 0 ,0, x_pos, y_pos);
	}

	virtual bool HitTop() {Tile::HitTop(); return ShootDown();}
	virtual bool HitBottom() {Tile::HitBottom(); return ShootUp();}

	virtual bool LookTop(BlockType type, int dist) {SeeMeDown(type, ++dist); return true;}
	virtual bool LookBottom(BlockType type, int dist) {SeeMeUp(type, ++dist); return true;}

protected: //members
private: // functions
private: //members

};

class BarsHoriz : public BarsVert
{
public:
	BarsHoriz(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : 
	  BarsVert(x,y,draw,textures,sound,board,r) {
		SetRotation(0);

	}
	virtual BlockType GetBlockType() {
		return BlockType::BARSHORIZ;
	}
	virtual bool SetRotation(int) {
		rotation=0;
		return false;
	}

protected: //functions
	virtual void Display() {

		draw->BlitSquare(textures->getBarSprites(), 1 ,0, x_pos, y_pos);
	}
	virtual bool HitTop() {return Static::HitTop();}
	virtual bool HitBottom() {return Static::HitBottom();}
	virtual bool HitLeft() {Tile::HitLeft(); return ShootRight();}
	virtual bool HitRight() {Tile::HitRight(); return ShootLeft();}

	virtual bool LookLeft(BlockType type, int dist) {SeeMeRight(type, ++dist); return true;}
	virtual bool LookTop(BlockType, int) {return false;}
	virtual bool LookRight(BlockType type, int dist) {SeeMeLeft(type, ++dist); return true;}
	virtual bool LookBottom(BlockType, int) {return false;}

protected: //members
private: // functions
private: //members

};

class BarsCross : public BarsVert
{
public:
	BarsCross(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : 
	  BarsVert(x,y,draw,textures,sound,board,r) {
		SetRotation(0);

	}
	virtual BlockType GetBlockType() {
		return BlockType::BARSCROSS;
	}
	virtual bool SetRotation(int) {
		rotation=0;
		return false;
	}

protected: //functions
	virtual void Display() {

		draw->BlitSquare(textures->getBarSprites(), 2 ,0, x_pos, y_pos);
	}

	virtual bool HitTop() {Tile::HitTop(); return ShootDown();}
	virtual bool HitBottom() {Tile::HitBottom(); return ShootUp();}
	virtual bool HitLeft() {Tile::HitLeft(); return ShootRight();}
	virtual bool HitRight() {Tile::HitRight(); return ShootLeft();}

	virtual bool LookLeft(BlockType type, int dist) {SeeMeRight(type, ++dist); return true;}
	virtual bool LookTop(BlockType type, int dist) {SeeMeDown(type, ++dist); return true;}
	virtual bool LookRight(BlockType type, int dist) {SeeMeLeft(type, ++dist); return true;}
	virtual bool LookBottom(BlockType type, int dist) {SeeMeUp(type, ++dist); return true;}

protected: //members
private: // functions
private: //members

};

class Triangle : public Mirror
{
public:
	Triangle(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : Mirror(x,y,draw,textures,sound,board,r) {
		SetRotation(r);

	}
	virtual BlockType GetBlockType() {
		return BlockType::TRIANGLE;
	}
	virtual bool SetRotation(int r) {
		rotation=r;
		rotation%=4;
		return true;
	}

	virtual bool PushLeft() {return false;}
	virtual bool PushRight() {return false;}
	virtual bool PushTop() {return false;}
	virtual bool PushBottom() {return false;}

	virtual bool HitLeft() {sound->PlayASound("donk.wav", DONK); return Mirror::HitLeft();}
	virtual bool HitRight() {sound->PlayASound("donk.wav", DONK); return Mirror::HitRight();}
	virtual bool HitTop() {sound->PlayASound("donk.wav", DONK); return Mirror::HitTop();}
	virtual bool HitBottom() {sound->PlayASound("donk.wav", DONK); return Mirror::HitBottom();}

protected: //functions
	virtual void Display() {
		draw->BlitSquare(textures->getMirrorSprites(), rotation%4 , 5, x_pos, y_pos);
	};



protected: //members

private: // functions
private: //members
};

class RustyTriangle : public Triangle
{
public:
	RustyTriangle(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : Triangle(x,y,draw,textures,sound,board,r) {
		SetRotation(r);

	}

	virtual bool HitLeft() {
		Triangle::HitLeft();
		sound->PlayASound("collapse.wav", COLLAPSE); return Kill();	
	}
	virtual bool HitRight() {
		Triangle::HitRight();
		sound->PlayASound("collapse.wav", COLLAPSE); return Kill();	
	}
	virtual bool HitTop() {
		Triangle::HitTop();
		sound->PlayASound("collapse.wav", COLLAPSE); return Kill();	
	}
	virtual bool HitBottom() {
		Triangle::HitBottom();
		sound->PlayASound("collapse.wav", COLLAPSE); return Kill();	
	}

	virtual BlockType GetBlockType() {
		return BlockType::RUSTYTRIANGLE;
	}

protected: //functions
	virtual void Display() {
		draw->BlitSquare(textures->getMirrorSprites(), rotation%4 , 6, x_pos, y_pos);
	};



protected: //members

private: // functions
private: //members
};

class RustyBarsVert : public BarsVert
{
public:
	RustyBarsVert(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : 
	  BarsVert(x,y,draw,textures,sound,board,r) {
		SetRotation(0);

	}

	virtual bool HitLeft() {
		ATile::HitLeft();
		sound->PlayASound("collapse.wav", COLLAPSE); return Kill();	
	}
	virtual bool HitRight() {
		ATile::HitRight();
		sound->PlayASound("collapse.wav", COLLAPSE); return Kill();	
	}


	virtual BlockType GetBlockType() {
		return BlockType::RUSTYBARSVERT;
	}

protected: //functions
	virtual void Display() {

		draw->BlitSquare(textures->getBarSprites(), 0 ,1, x_pos, y_pos);
	}



protected: //members
private: // functions
private: //members

};

class RustyBarsHoriz : public BarsHoriz
{
public:
	RustyBarsHoriz(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : 
	  BarsHoriz(x,y,draw,textures,sound,board,r) {
		SetRotation(0);

	}

	virtual bool HitTop() {
		ATile::HitTop();
		sound->PlayASound("collapse.wav", COLLAPSE); return Kill();	
	}
	virtual bool HitBottom() {
		ATile::HitBottom();
		sound->PlayASound("collapse.wav", COLLAPSE); return Kill();	
	}

	virtual BlockType GetBlockType() {
		return BlockType::RUSTYBARSHORIZ;
	}

protected: //functions
	virtual void Display() {

		draw->BlitSquare(textures->getBarSprites(), 1 ,1, x_pos, y_pos);
	}



protected: //members
private: // functions
private: //members

};

class RustyWhiteBlock : public WhiteBlock
{
public:
	RustyWhiteBlock(int x, int y, Draw * draw, Textures * textures, Sound * sound, Board * board, int r=0) : WhiteBlock(x,y,draw,textures,sound,board,3) {
		SetRotation(r);

	}
	
	virtual bool HitLeft() {ATile::HitLeft(); sound->PlayASound("pop.wav", POP); return Kill();}
	virtual bool HitTop() {ATile::HitTop(); sound->PlayASound("pop.wav", POP); return Kill();}
	virtual bool HitRight() {ATile::HitRight(); sound->PlayASound("pop.wav", POP); return Kill();}
	virtual bool HitBottom() {ATile::HitBottom(); sound->PlayASound("pop.wav", POP); return Kill();}

	virtual BlockType GetBlockType() {
		return BlockType::RUSTYWHITEBLOCK;
	}

protected: //functions
	virtual void Display() {
		draw->BlitSquare(textures->getRedblockSprites(), 3 ,0, x_pos, y_pos);
	}

protected: //members
private: // functions
private: //members

};

#endif // TILE_H
