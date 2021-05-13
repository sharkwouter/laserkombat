// Tile.h

enum BlockType{NONE, TANK, WATER, GROUND, REDBLOCK,
	ENEMYTANK, NUKE, ENEMYNUKE, MIRROR, TEE, STATIC,
	RUSTYREDBLOCK, RUSTY, BARSVERT, BARSHORIZ, BARSCROSS,
	WHITEBLOCK, TRIANGLE, RUSTYTRIANGLE, RUSTYBARSVERT,
	RUSTYBARSHORIZ, RUSTYWHITEBLOCK};


class Tile;
class Draw;

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

//#include "pix.h"

class Tile
{
	friend class ATile;
public:
	Tile(int x, int y, int r=0) : x_pos(x), y_pos(y), deadBlock(0), willKillFlag(0) {
		if (!beamSurface) SetBeamSurface();

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

	virtual void SetOtherBlocks (unsigned int around) {}

	virtual bool Kill();
	virtual bool KillOver();

	virtual bool Drown() {Tile::Kill(); return true;}
	virtual bool DrownOver() {Tile::KillOver(); return true;}

	virtual bool WillKill(int flag=2);

	virtual BlockType GetBlockType() {
		return NONE;
	}

	virtual bool BlockOver(Tile* &block, Tile* &ground)
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

	virtual bool PushLeft() {return MoveRight();}
	virtual bool PushRight() {return MoveLeft();}
	virtual bool PushTop() {return MoveDown();}
	virtual bool PushBottom() {return MoveUp();}

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
	/*virtual bool HitLeft() {
		Tile* ground=GetGround();
		if (ground) ground->HadFired=ground->HadFiredRight=BEAM_PERSISTANCE;
		return true;
	}
	virtual bool HitTop() {
		Tile* ground=GetGround();
		if (ground) ground->HadFired=ground->HadFiredDown=BEAM_PERSISTANCE;
		return true;
	}
	virtual bool HitRight() {
		Tile* ground=GetGround();
		if (ground) ground->HadFired=ground->HadFiredLeft=BEAM_PERSISTANCE;
		return true;
	}
	virtual bool HitBottom() {
		Tile* ground=GetGround();
		if (ground) ground->HadFired=ground->HadFiredUp=BEAM_PERSISTANCE;
		return true;
	}*/

	virtual bool LookLeft(BlockType type, int dist) {return false;}
	virtual bool LookTop(BlockType type, int dist) {return false;}
	virtual bool LookRight(BlockType type, int dist) {return false;}
	virtual bool LookBottom(BlockType type, int dist) {return false;}


	virtual bool SeeMeUp(BlockType type, int dist);
	virtual bool SeeMeDown(BlockType type, int dist);
	virtual bool SeeMeRight(BlockType type, int dist);
	virtual bool SeeMeLeft(BlockType type, int dist);

	virtual bool ShootUp();
	virtual bool ShootDown();
	virtual bool ShootRight();
	virtual bool ShootLeft();

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
		Draw();
		DisplayBeam(BeamState());
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
	static void RestoreBeamSurface() {if (beamSurface) beamSurface->Restore();}
	static void RestoreSurface() {if (surface) surface->Restore();}



protected: //functions
	virtual void Draw() {
		if (surface) draw.BlackSquare(x_pos, y_pos); 
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
	Tile* Tile::GetBlock();

	bool RightBlock(Tile* &p);
	bool LeftBlock(Tile* &p);
	bool TopBlock(Tile* &p);
	bool BottomBlock(Tile* &p);

	bool RightGround(Tile* &p);
	bool LeftGround(Tile* &p);
	bool TopGround(Tile* &p);
	bool BottomGround(Tile* &p);

	void DisplayBeam(int type) {

		//if (type&&beamSurface)
		if (HadFiredLeft||WasHitLeft)
			draw.BlitSquare(beamSurface, 0 ,1, x_pos, y_pos);
		if (HadFiredUp||WasHitTop)
			draw.BlitSquare(beamSurface, 1 ,1, x_pos, y_pos);
		if (HadFiredRight||WasHitRight)
			draw.BlitSquare(beamSurface, 2 ,1, x_pos, y_pos);
		if (HadFiredDown||WasHitBottom)
			draw.BlitSquare(beamSurface, 3 ,1, x_pos, y_pos);	
	
	
	}

	virtual bool MoveUp();
	virtual bool MoveDown();
	virtual bool MoveRight();
	virtual bool MoveLeft();

protected: //members
	bool FiringUp, FiringDown, FiringLeft, FiringRight, Firing;
	int WasHitLeft, WasHitRight, WasHitTop, WasHitBottom, WasHit;
	int HadFiredLeft, HadFiredRight, HadFiredUp, HadFiredDown, HadFired;
	bool moved;

	static LPDIRECTDRAWSURFACE surface;
	static LPDIRECTDRAWSURFACE beamSurface;
	int cols;
	int rows;

	int x_pos, y_pos;
	int rotation;

private: // functions
	//Set for killing, just not immediatly.

	static void SetBeamSurface()
	{
		beamSurface=draw.GetSurface("BEAM");
	}
private: //members
	int willKillFlag;
	Tile* deadBlock;


};

// Tile.h

class GroundTile : public Tile
{
public:
	GroundTile(int x, int y, int r=33) : Tile(x,y,r), otherblocks(0) {
		SetRotation(r);
		if (!surface) SetSurface();

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
		return GROUND;
	}

	static bool SetChanged(bool change) {
		bool ret=changed;
		changed=change;
		return ret;
	}
	static void RestoreSurface() {if (surface) surface->Restore();}


protected: //functions
	virtual void GroundTile::Draw() {
		//Exception::Output("Tank::draw called.");
		if (rotation!=33) {
			int r;
			if (rotation>=100) r=33-rotation/100, rotation-=100; 
			else r=rotation;
			if (surface) draw.BlitSquare(surface, r%11 ,r/11, x_pos, y_pos);
		}
		else {		//calculate pieces of images to stick together
			//otherblocks=0x1f1fu;
			
			unsigned int mask=0x7u;
			unsigned int corners[4] = {7u,7u,1u,4u};
			//for (unsigned int i=0, j=0; i<4; i++) {
				
				corners[0] = ((otherblocks& mask));
				mask*=0x4u;
				corners[1] = ((otherblocks& mask)/0x4u);
				mask*=0x4u;
				corners[2] = ((otherblocks& mask)/0x10u);
				mask*=0x4u;
				corners[3] = ((otherblocks& mask)/0x40u);
				mask*=0x4u;

				//otherblocks = (otherblocks >> 2u); 
				//j++; j++;
			//}


			if (surface) {
				//draw.BlitOther(surface, 400, 0, x_pos*40, y_pos*40, 40, 40);
				for (int i=0; i<4; i++) {
					if (surface) { draw.BlitOther(surface,
						corners[i]*IMAGE_WIDTH + ((i==0||i==3)?0:(IMAGE_WIDTH/2)),
						3*IMAGE_WIDTH + ((i<2)?0:(IMAGE_WIDTH/2)),
						x_pos*IMAGE_WIDTH + ((i==0||i==3)?0:(IMAGE_WIDTH/2)),
						y_pos*IMAGE_WIDTH + ((i<2)?0:(IMAGE_WIDTH/2)),
						IMAGE_WIDTH/2, IMAGE_WIDTH/2);
					}
				}
			}
		}
	}
protected: //members
	static LPDIRECTDRAWSURFACE surface;

private: // functions
	virtual void SetSurface() {
		surface=draw.GetSurface("GROUND");
	}
private: //members
	static bool changed;
	unsigned int otherblocks;	
};

class ATile : public Tile
{
public:

	ATile(int x, int y, int r=0) : Tile(x,y,r) {

	}

	virtual void Update() {
		Draw();
		BeamState();
		if (Firing) Firing=FiringRight=FiringLeft=FiringUp=FiringDown=false;
		moved=false;
	}

	/*virtual bool HitLeft() {
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
	}*/


protected: //functions
protected: //members
private: // functions
private: //members


};

class Tank : public ATile
{
public:
	Tank(int x, int y, int r=0) : ATile(x,y,r) {
		if (!surface) SetSurface();
		SetRotation(r);

	}

	virtual bool SetRotation(int r) {
		rotation=r%12;
		return true;
	}

	virtual bool PreSeeMe() {
		return SeeMe(TANK);
	}

	virtual BlockType GetBlockType()
	{
		return TANK;
	}

	//virtual bool Kill();

	virtual bool ShootUp() {
	//sound.PlayASound("beam.wav");
		bool ret=ATile::ShootUp();
		//SeeMe(TANK);
		return ret;

	}
	virtual bool ShootDown() {
	//sound.PlayASound("beam.wav");
		bool ret=ATile::ShootDown();
		//SeeMe(TANK);
		return ret;

	}
	virtual bool ShootRight() {
	//sound.PlayASound("beam.wav");
		bool ret=ATile::ShootRight();
		//SeeMe(TANK);
		return ret;

	}
	virtual bool ShootLeft() {
	//sound.PlayASound("beam.wav");
		bool ret=ATile::ShootLeft();
		//SeeMe(TANK);
		return ret;

	}
	virtual bool HitLeft() {ATile::HitLeft(); sound.PlayASound("pop.wav", POP); return WillKill();}
	virtual bool HitTop() {ATile::HitTop(); sound.PlayASound("pop.wav", POP); return WillKill();}
	virtual bool HitRight() {ATile::HitRight(); sound.PlayASound("pop.wav", POP); return WillKill();}
	virtual bool HitBottom() {ATile::HitBottom(); sound.PlayASound("pop.wav", POP); return WillKill();}

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
	static void RestoreSurface() {if (surface) surface->Restore();}


protected: //functions
	virtual int BeamState() {
		return 0;
	}
	virtual void Tank::Draw();
	virtual bool MoveUp() {
		bool ret=ATile::MoveUp();
		//if (ret) sound.PlayASound("tick.wav");
		//SeeMe(TANK);
		return ret;
	}
	virtual bool MoveDown() {
		bool ret=ATile::MoveDown();
		//if (ret) sound.PlayASound("tick.wav");
		//SeeMe(TANK);
		return ret;
	}
	virtual bool MoveRight() {
		bool ret=ATile::MoveRight();
		//if (ret) sound.PlayASound("tick.wav");
		//SeeMe(TANK);
		return ret;
	}
	virtual bool MoveLeft() {
		bool ret=ATile::MoveLeft();
		//if (ret) sound.PlayASound("tick.wav");
		//SeeMe(TANK);
		return ret;

	}
protected: //members
		static LPDIRECTDRAWSURFACE surface;



private: // functions
	virtual void SetSurface() {
		//Exception::Output("Tank::setsurface() called.");
		surface=draw.GetSurface("TANK");
	}
private: //members

};

class RedBlock : public ATile
{
public:
	RedBlock(int x, int y, int r=0) : ATile(x,y,0) {
		if (!surface) SetSurface();
		SetRotation(r);

	}
	virtual BlockType GetBlockType() {
		return REDBLOCK;
	}
	virtual bool SetRotation(int r) {
		rotation=0;
		return false;
	}

	virtual bool HitLeft() {Tile::HitLeft(); PushLeft(); return true;}
	virtual bool HitTop() {Tile::HitTop(); PushTop(); return true;}
	virtual bool HitRight() {Tile::HitRight(); PushRight(); return true;}
	virtual bool HitBottom() {Tile::HitBottom(); PushBottom(); return true;}
	static void RestoreSurface() {if (surface) surface->Restore();}

protected: //functions
	virtual void RedBlock::Draw() {
		//Exception::Output("Tank::draw called.");
		if (surface) draw.BlitSquare(surface, 0 ,0, x_pos, y_pos); 
	}

protected: //members
		static LPDIRECTDRAWSURFACE surface;
private: // functions
	virtual void SetSurface() {
		//Exception::Output("Tank::setsurface() called.");
		surface=draw.GetSurface("REDBLOCK");
	}
private: //members

};

class WhiteBlock : public RedBlock
{
public:
	WhiteBlock(int x, int y, int r=0) : RedBlock(x,y,2) {
		if (!surface) SetSurface();
		SetRotation(r);

	}
	virtual BlockType GetBlockType() {
		return WHITEBLOCK;
	}
	virtual bool SetRotation(int r) {
		rotation=2;
		return false;
	}
	static void RestoreSurface() {if (surface) surface->Restore();}


protected: //functions
	virtual void WhiteBlock::Draw() {
		//Exception::Output("Tank::draw called.");
		if (surface) draw.BlitSquare(surface, 2 ,0, x_pos, y_pos); 
	}
	virtual bool MoveUp() {sound.PlayASound(NULL, SCRAPE); return Tile::MoveUp();}
	virtual bool MoveDown() {sound.PlayASound(NULL, SCRAPE); return Tile::MoveDown();}
	virtual bool MoveLeft() {sound.PlayASound(NULL, SCRAPE); return Tile::MoveLeft();}
	virtual bool MoveRight() {sound.PlayASound(NULL, SCRAPE); return Tile::MoveRight();}
protected: //members
private: // functions
	virtual void SetSurface() {
		//Exception::Output("Tank::setsurface() called.");
		surface=draw.GetSurface("REDBLOCK");
	}
private: //members

};

class Water : public GroundTile
{
public:
	Water(int x, int y, int r=0) : GroundTile(x,y,r) {
		SetRotation(r);
		if (!surface) SetSurface();

	}

	virtual bool BlockOver(Tile* &block, Tile* &ground);
	virtual bool SetRotation(int r) {
		rotation=r;
		return true;
	}
	virtual BlockType GetBlockType() {
		return WATER;
	}

	static void IncStaticRotation() {
		static bool chop=true;
		chop=!chop;
		if (chop) {
			static_rotation++;
			static_rotation%=40;
		}
	}
	static void RestoreSurface() {if (surface) surface->Restore();}

protected: //functions
	virtual void Water::Draw() {
		//Exception::Output("Tank::draw called.");
		if (rotation>=100) {
			GroundTile::Draw();
			return;
		}
		if (surface) draw.BlitSquare(surface, static_rotation%10 ,static_rotation/10, x_pos, y_pos); 
	}
protected: //members
	static LPDIRECTDRAWSURFACE surface;

private: // functions
	virtual void SetSurface() {
		surface=draw.GetSurface("WATER");
	}
private: //members
	static int static_rotation;
};

class Static : public ATile
{
public:
	Static(int x, int y, int r=0) : 
	  ATile(x,y,r), turn_right(r>8?0:1) , time(0), span(0) {
		if (!surface) SetSurface();
		SetRotation(r);

	}
	virtual bool SetRotation(int r) {
		if (r>8) r-=4;
		rotation=r;
		return true;
	}
	static void RestoreSurface() {if (surface) surface->Restore();}

protected: //functions
	virtual void Static::Draw() {
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
		//Exception::Output("Tank::draw called.");
		if (surface) draw.BlitSquare(surface, rotation%9 ,0, x_pos, y_pos); 
	}
	virtual bool PushLeft() {return false;}
	virtual bool PushRight() {return false;}
	virtual bool PushTop() {return false;}
	virtual bool PushBottom() {return false;}

	virtual bool HitLeft() {sound.PlayASound("donk.wav", DONK); return span=1;}
	virtual bool HitRight() {sound.PlayASound("donk.wav", DONK); return span=1;}
	virtual bool HitTop() {sound.PlayASound("donk.wav", DONK); return span=1;}
	virtual bool HitBottom() {sound.PlayASound("donk.wav", DONK); return span=1;}



protected: //members
		static LPDIRECTDRAWSURFACE surface;
private: // functions

	virtual void SetSurface() {
		//Exception::Output("Tank::setsurface() called.");
		surface=draw.GetSurface("STATIC");
	}
private: //members
	bool turn_right;
	int time, span;

};

class Mirror : public ATile
{
public:
	Mirror(int x, int y, int r=0) : ATile(x,y,r), rotation2(0){
		if (!surface) SetSurface();
		SetRotation(r);

	}
	virtual bool SetRotation(int r) {
		rotation=r;
		rotation%=16;
		return true;
	}
	static void RestoreSurface() {if (surface) surface->Restore();}

protected: //functions
	virtual void Mirror::Draw() {
		//Exception::Output("Tank::draw called.");
		rotation2++;
		if (rotation2>=7) {
			rotation2=0;
			rotation+=4;
			rotation%=16;
		}
		if (surface) draw.BlitSquare(surface, rotation%4 , HadFired?4:(rotation/4), x_pos, y_pos); 
	}
	virtual int BeamState();


	virtual bool HitLeft() {
		ATile::HitLeft();
		//if (Firing) return false;
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
		//if (Firing) return false;
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
		//if (Firing) return false;
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
		//if (Firing) return false;
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
		static LPDIRECTDRAWSURFACE surface;
		int rotation2;

private: // functions

	virtual void SetSurface() {
		//Exception::Output("Tank::setsurface() called.");
		surface=draw.GetSurface("MIRROR");
	}
private: //members
};

class Tee : public Mirror
{
public:
	Tee(int x, int y, int r=0) : Mirror(x,y,r) {
		if (!surface) SetSurface();
		SetRotation(r);

	}
	static void RestoreSurface() {if (surface) surface->Restore();}

protected: //functions

	virtual void Tee::Draw() {
		//Exception::Output("Tank::draw called.");
		rotation2++;
		if (rotation2>=7) {
			rotation2=0;
			rotation+=7;
			rotation%=28;
		}
		if (surface) draw.BlitSquare(surface, rotation%7 , HadFired?4:(rotation/7), x_pos, y_pos); 
	}

	virtual bool HitLeft() {
		Tile::HitLeft();
		//if (Firing) return false;
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
		//if (Firing) return false;
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
		//if (Firing) return false;
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
		//if (Firing) return false;
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
		static LPDIRECTDRAWSURFACE surface;
private: // functions

	virtual void SetSurface() {
		//Exception::Output("Tank::setsurface() called.");
		surface=draw.GetSurface("TEE");
	}
private: //members
};

class EnemyTank : public Tank
{
public:
	EnemyTank(int x, int y, int r=0) : Tank(x,y,r) {

	}
	virtual bool SetRotation(int r) {return ATile::SetRotation(r);}

	virtual bool PreSeeMe() {
		return false;
	}

	virtual BlockType GetBlockType() {
		return ENEMYTANK;
	}

	virtual bool LookLeft(BlockType type, int dist) {ShootLeft(); return true;}
	virtual bool LookTop(BlockType type, int dist) {ShootUp(); return true;}
	virtual bool LookRight(BlockType type, int dist) {ShootRight(); return true;}
	virtual bool LookBottom(BlockType type, int dist) {ShootDown(); return true;}

	virtual bool ShootUp() {SetRotation(1); return ATile::ShootUp();}
	virtual bool ShootDown() {SetRotation(3); return ATile::ShootDown();}
	virtual bool ShootRight() {SetRotation(2); return ATile::ShootRight();}
	virtual bool ShootLeft() {SetRotation(0); return ATile::ShootLeft();}
	static void RestoreSurface() {if (surface) surface->Restore();}

protected: //functions
	virtual void EnemyTank::Draw() {
		//Exception::Output("Tank::draw called.");
		int rotate=rand()%1000;
		if (rotate==0) RotateRight();	//make tanks rotate randomly
		else if (rotate==1) RotateLeft();
		if (surface) draw.BlitSquare(surface, rotation ,1, x_pos, y_pos); 
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
	Nuke(int x, int y, int r=0) : ATile(x,y,r) {
		if (!surface) SetSurface();

	}
	virtual bool HitLeft() {ATile::HitLeft(); Kill(); return true;}
	virtual bool HitTop() {ATile::HitTop(); Kill(); return true;}
	virtual bool HitRight() {ATile::HitRight(); Kill(); return true;}
	virtual bool HitBottom() {ATile::HitBottom(); Kill(); return true;}

	virtual bool Kill();
	static void RestoreSurface() {if (surface) surface->Restore();}
protected: //functions
	virtual void Nuke::Draw() {
		//Exception::Output("Tank::draw called.");
		if (surface) draw.BlitSquare(surface, 0 ,0, x_pos, y_pos); 
	}

protected: //members
		static LPDIRECTDRAWSURFACE surface;
private: // functions
	virtual void SetSurface() {
		//Exception::Output("Tank::setsurface() called.");
		surface=draw.GetSurface("NUKE");
	}
private: //members

};

class Rusty : public Static
{
public:
	Rusty(int x, int y, int r=0) : Static(x,y,r), turn_right(r>8?0:1) {
		if (!surface) SetSurface();

	}

	virtual bool HitLeft() {ATile::HitLeft(); sound.PlayASound("collapse.wav", COLLAPSE); return Kill();}
	virtual bool HitTop() {ATile::HitTop(); sound.PlayASound("collapse.wav", COLLAPSE); return Kill();}
	virtual bool HitRight() {ATile::HitRight(); sound.PlayASound("collapse.wav", COLLAPSE); return Kill();}
	virtual bool HitBottom() {ATile::HitBottom(); sound.PlayASound("collapse.wav", COLLAPSE); return Kill();}
	static void RestoreSurface() {if (surface) surface->Restore();}
protected: //functions
	virtual void Rusty::Draw() {
		if (rotation>=5) {
			if (turn_right) {rotation++;
				if (rotation>=9) rotation=5;
			} 
			else {
					rotation--;
					if (rotation<=4) rotation=8;
			}
		}
		if (surface) draw.BlitSquare(surface, rotation%9 ,0, x_pos, y_pos); 
	}



protected: //members
		static LPDIRECTDRAWSURFACE surface;
private: // functions

	bool turn_right;

	virtual void SetSurface() {
		//Exception::Output("Tank::setsurface() called.");
		surface=draw.GetSurface("RUSTY");
	}
};

class RustyRedBlock :public RedBlock
{
public:
	RustyRedBlock(int x, int y, int r=0) : RedBlock(x,y,1) {
		if (!surface) SetSurface();
		SetRotation(r);

	}
	virtual BlockType GetBlockType() {
		return REDBLOCK;
	}
	virtual bool SetRotation(int r) {
		rotation=1;
		return false;
	}
	virtual bool HitLeft() {ATile::HitLeft(); sound.PlayASound("pop.wav", POP); return Kill();}
	virtual bool HitTop() {ATile::HitTop(); sound.PlayASound("pop.wav", POP); return Kill();}
	virtual bool HitRight() {ATile::HitRight(); sound.PlayASound("pop.wav", POP); return Kill();}
	virtual bool HitBottom() {ATile::HitBottom(); sound.PlayASound("pop.wav", POP); return Kill();}
	static void RestoreSurface() {if (surface) surface->Restore();}

protected: //functions
	virtual void RustyRedBlock::Draw() {
		//Exception::Output("Tank::draw called.");
		if (surface) draw.BlitSquare(surface, 1 ,0, x_pos, y_pos); 
	}

protected: //members
private: // functions
	virtual void SetSurface() {
		//Exception::Output("Tank::setsurface() called.");
		surface=draw.GetSurface("REDBLOCK");
	}
private: //members

};

class EnemyNuke : public Nuke
{
public:
	EnemyNuke(int x, int y, int r=0) : Nuke(x,y,r) {
		if (!surface) SetSurface();

	}

	virtual bool LookLeft(BlockType type, int dist) {if (!dist) Kill(); return true;}
	virtual bool LookTop(BlockType type, int dist) {if (!dist) Kill(); return true;}
	virtual bool LookRight(BlockType type, int dist) {if (!dist) Kill(); return true;}
	virtual bool LookBottom(BlockType type, int dist) {if (!dist) Kill(); return true;}

	//virtual bool HitLeft() {return ATile::HitLeft();}
	//virtual bool HitTop() {return ATile::HitTop();}
	//virtual bool HitRight() {return ATile::HitRight();}
	//virtual bool HitBottom() {return ATile::HitBottom();}
	static void RestoreSurface() {if (surface) surface->Restore();}

protected: //functions
	virtual void EnemyNuke::Draw() {
		//Exception::Output("Tank::draw called.");
		if (surface) draw.BlitSquare(surface, 1 ,0, x_pos, y_pos); 
	}

protected: //members
private: // functions
	virtual void SetSurface() {
		surface=draw.GetSurface("NUKE");
	}
private: //members

};

class Message : public Static
{
public:
	Message(int x, int y, int r=0) : Static(x,y,r) {
		if (!surface) SetSurface();
		SetRotation(r);

	}
	virtual bool SetRotation(int r) {
		rotation=r;
		return true;
	}
	static void RestoreSurface() {if (surface) surface->Restore();}

protected: //functions
	virtual void Message::Draw() {
		if (surface) draw.BlitSquare(surface, rotation%14 ,rotation/14, x_pos, y_pos); 
	}
	virtual bool PushLeft() {return false;}
	virtual bool PushRight() {return false;}
	virtual bool PushTop() {return false;}
	virtual bool PushBottom() {return false;}

protected: //members
		static LPDIRECTDRAWSURFACE surface;
private: // functions

	virtual void SetSurface() {
		//Exception::Output("Tank::setsurface() called.");
		surface=draw.GetSurface("MESSAGE");
	}
private: //members

};

class BarsVert : public Static
{
public:
	BarsVert(int x, int y, int r=0) : 
	  Static(x,y,r) {
		if (!surface) SetSurface();
		SetRotation(0);

	}
	virtual bool SetRotation(int r) {
		rotation=0;
		return false;
	}
	static void RestoreSurface() {if (surface) surface->Restore();}

protected: //functions
	virtual void BarsVert::Draw() {

		if (surface) draw.BlitSquare(surface, 0 ,0, x_pos, y_pos); 
	}

	virtual bool HitTop() {Tile::HitTop(); return ShootDown();}
	virtual bool HitBottom() {Tile::HitBottom(); return ShootUp();}

	virtual bool LookTop(BlockType type, int dist) {SeeMeDown(type, ++dist); return true;}
	virtual bool LookBottom(BlockType type, int dist) {SeeMeUp(type, ++dist); return true;}

protected: //members
		static LPDIRECTDRAWSURFACE surface;
private: // functions

	virtual void SetSurface() {
		//Exception::Output("Tank::setsurface() called.");
		surface=draw.GetSurface("BARS");
	}
private: //members

};

class BarsHoriz : public BarsVert
{
public:
	BarsHoriz(int x, int y, int r=0) : 
	  BarsVert(x,y,r) {
		if (!surface) SetSurface();
		SetRotation(0);

	}
	virtual bool SetRotation(int r) {
		rotation=0;
		return false;
	}
	static void RestoreSurface() {if (surface) surface->Restore();}

protected: //functions
	virtual void BarsHoriz::Draw() {

		if (surface) draw.BlitSquare(surface, 1 ,0, x_pos, y_pos); 
	}
	virtual bool HitTop() {return Static::HitTop();}
	virtual bool HitBottom() {return Static::HitBottom();}
	virtual bool HitLeft() {Tile::HitLeft(); return ShootRight();}
	virtual bool HitRight() {Tile::HitRight(); return ShootLeft();}

	virtual bool LookLeft(BlockType type, int dist) {SeeMeRight(type, ++dist); return true;}
	virtual bool LookTop(BlockType type, int dist) {return false;}
	virtual bool LookRight(BlockType type, int dist) {SeeMeLeft(type, ++dist); return true;}
	virtual bool LookBottom(BlockType type, int dist) {return false;}

protected: //members
private: // functions

	virtual void SetSurface() {
		//Exception::Output("Tank::setsurface() called.");
		surface=draw.GetSurface("BARS");
	}
private: //members

};

class BarsCross : public BarsVert
{
public:
	BarsCross(int x, int y, int r=0) : 
	  BarsVert(x,y,r) {
		if (!surface) SetSurface();
		SetRotation(0);

	}
	virtual bool SetRotation(int r) {
		rotation=0;
		return false;
	}
	static void RestoreSurface() {if (surface) surface->Restore();}

protected: //functions
	virtual void BarsCross::Draw() {

		if (surface) draw.BlitSquare(surface, 2 ,0, x_pos, y_pos); 
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

	virtual void SetSurface() {
		//Exception::Output("Tank::setsurface() called.");
		surface=draw.GetSurface("BARS");
	}
private: //members

};

class Triangle : public Mirror
{
public:
	Triangle(int x, int y, int r=0) : Mirror(x,y,r) {
		if (!surface) SetSurface();
		SetRotation(r);

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

	virtual bool HitLeft() {sound.PlayASound("donk.wav", DONK); return Mirror::HitLeft();}
	virtual bool HitRight() {sound.PlayASound("donk.wav", DONK); return Mirror::HitRight();}
	virtual bool HitTop() {sound.PlayASound("donk.wav", DONK); return Mirror::HitTop();}
	virtual bool HitBottom() {sound.PlayASound("donk.wav", DONK); return Mirror::HitBottom();}

protected: //functions
	virtual void Triangle::Draw() {
		//Exception::Output("Tank::draw called.");
		if (surface) draw.BlitSquare(surface, rotation%4 , 5, x_pos, y_pos); 
	};



protected: //members

private: // functions

	virtual void SetSurface() {
		//Exception::Output("Tank::setsurface() called.");
		surface=draw.GetSurface("MIRROR");
	}
private: //members
};

class RustyTriangle : public Triangle
{
public:
	RustyTriangle(int x, int y, int r=0) : Triangle(x,y,r) {
		if (!surface) SetSurface();
		SetRotation(r);

	}

	virtual bool HitLeft() {
		Triangle::HitLeft();
		sound.PlayASound("collapse.wav", COLLAPSE); return Kill();	
	}
	virtual bool HitRight() {
		Triangle::HitRight();
		sound.PlayASound("collapse.wav", COLLAPSE); return Kill();	
	}
	virtual bool HitTop() {
		Triangle::HitTop();
		sound.PlayASound("collapse.wav", COLLAPSE); return Kill();	
	}
	virtual bool HitBottom() {
		Triangle::HitBottom();
		sound.PlayASound("collapse.wav", COLLAPSE); return Kill();	
	}

protected: //functions
	virtual void RustyTriangle::Draw() {
		//Exception::Output("Tank::draw called.");
		if (surface) draw.BlitSquare(surface, rotation%4 , 6, x_pos, y_pos); 
	};



protected: //members

private: // functions

	virtual void SetSurface() {
		//Exception::Output("Tank::setsurface() called.");
		surface=draw.GetSurface("MIRROR");
	}
private: //members
};

class RustyBarsVert : public BarsVert
{
public:
	RustyBarsVert(int x, int y, int r=0) : 
	  BarsVert(x,y,r) {
		if (!surface) SetSurface();
		SetRotation(0);

	}
	static void RestoreSurface() {if (surface) surface->Restore();}

	virtual bool HitLeft() {
		ATile::HitLeft();
		sound.PlayASound("collapse.wav", COLLAPSE); return Kill();	
	}
	virtual bool HitRight() {
		ATile::HitRight();
		sound.PlayASound("collapse.wav", COLLAPSE); return Kill();	
	}

protected: //functions
	virtual void RustyBarsVert::Draw() {

		if (surface) draw.BlitSquare(surface, 0 ,1, x_pos, y_pos); 
	}



protected: //members
private: // functions

	virtual void SetSurface() {
		//Exception::Output("Tank::setsurface() called.");
		surface=draw.GetSurface("BARS");
	}
private: //members

};

class RustyBarsHoriz : public BarsHoriz
{
public:
	RustyBarsHoriz(int x, int y, int r=0) : 
	  BarsHoriz(x,y,r) {
		if (!surface) SetSurface();
		SetRotation(0);

	}
	static void RestoreSurface() {if (surface) surface->Restore();}

	virtual bool HitTop() {
		ATile::HitTop();
		sound.PlayASound("collapse.wav", COLLAPSE); return Kill();	
	}
	virtual bool HitBottom() {
		ATile::HitBottom();
		sound.PlayASound("collapse.wav", COLLAPSE); return Kill();	
	}

protected: //functions
	virtual void RustyBarsHoriz::Draw() {

		if (surface) draw.BlitSquare(surface, 1 ,1, x_pos, y_pos); 
	}



protected: //members
private: // functions

	virtual void SetSurface() {
		//Exception::Output("Tank::setsurface() called.");
		surface=draw.GetSurface("BARS");
	}
private: //members

};

class RustyWhiteBlock : public WhiteBlock
{
public:
	RustyWhiteBlock(int x, int y, int r=0) : WhiteBlock(x,y,3) {
		if (!surface) SetSurface();
		SetRotation(r);

	}
	static void RestoreSurface() {if (surface) surface->Restore();}
	
	virtual bool HitLeft() {ATile::HitLeft(); sound.PlayASound("pop.wav", POP); return Kill();}
	virtual bool HitTop() {ATile::HitTop(); sound.PlayASound("pop.wav", POP); return Kill();}
	virtual bool HitRight() {ATile::HitRight(); sound.PlayASound("pop.wav", POP); return Kill();}
	virtual bool HitBottom() {ATile::HitBottom(); sound.PlayASound("pop.wav", POP); return Kill();}

protected: //functions
	virtual void RustyWhiteBlock::Draw() {
		//Exception::Output("Tank::draw called.");
		if (surface) draw.BlitSquare(surface, 3 ,0, x_pos, y_pos); 
	}

protected: //members
private: // functions
	virtual void SetSurface() {
		//Exception::Output("Tank::setsurface() called.");
		surface=draw.GetSurface("REDBLOCK");
	}
private: //members

};


