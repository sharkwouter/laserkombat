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
		moved=FALSE;


	}
	virtual ~Tile() {
		if (deadBlock) delete deadBlock;
	}

	void SetXY(int x, int y) {x_pos=x, y_pos=y;}

	virtual void SetOtherBlocks (unsigned int around) {}

	virtual BOOL Kill();
	virtual BOOL KillOver();

	virtual BOOL Drown() {Tile::Kill(); return TRUE;}
	virtual BOOL DrownOver() {Tile::KillOver(); return TRUE;}

	virtual BOOL WillKill(int flag=2);

	virtual BlockType GetBlockType() {
		return NONE;
	}

	virtual BOOL BlockOver(Tile* &block, Tile* &ground)
	{
		if (ground) ground->ClearOver();
		return FALSE;
	}

	void ClearOver();	//make sure the over layer is clear


	int GetRotation() {return rotation;}
	virtual BOOL SetRotation(int r) {
		rotation=r;
		rotation%=4;
		return TRUE;
	}

	virtual BOOL PushLeft() {return MoveRight();}
	virtual BOOL PushRight() {return MoveLeft();}
	virtual BOOL PushTop() {return MoveDown();}
	virtual BOOL PushBottom() {return MoveUp();}

	virtual BOOL HitLeft() {
		Tile* ground=GetGround();
		if (ground) ground->WasHit=ground->WasHitLeft=BEAM_PERSISTANCE;
		return TRUE;
	}
	virtual BOOL HitTop() {
		Tile* ground=GetGround();
		if (ground) ground->WasHit=ground->WasHitTop=BEAM_PERSISTANCE;
		return TRUE;
	}
	virtual BOOL HitRight() {
		Tile* ground=GetGround();
		if (ground) ground->WasHit=ground->WasHitRight=BEAM_PERSISTANCE;
		return TRUE;
	}
	virtual BOOL HitBottom() {
		Tile* ground=GetGround();
		if (ground) ground->WasHit=ground->WasHitBottom=BEAM_PERSISTANCE;
		return TRUE;
	}
	/*virtual BOOL HitLeft() {
		Tile* ground=GetGround();
		if (ground) ground->HadFired=ground->HadFiredRight=BEAM_PERSISTANCE;
		return TRUE;
	}
	virtual BOOL HitTop() {
		Tile* ground=GetGround();
		if (ground) ground->HadFired=ground->HadFiredDown=BEAM_PERSISTANCE;
		return TRUE;
	}
	virtual BOOL HitRight() {
		Tile* ground=GetGround();
		if (ground) ground->HadFired=ground->HadFiredLeft=BEAM_PERSISTANCE;
		return TRUE;
	}
	virtual BOOL HitBottom() {
		Tile* ground=GetGround();
		if (ground) ground->HadFired=ground->HadFiredUp=BEAM_PERSISTANCE;
		return TRUE;
	}*/

	virtual BOOL LookLeft(BlockType type, int dist) {return FALSE;}
	virtual BOOL LookTop(BlockType type, int dist) {return FALSE;}
	virtual BOOL LookRight(BlockType type, int dist) {return FALSE;}
	virtual BOOL LookBottom(BlockType type, int dist) {return FALSE;}


	virtual BOOL SeeMeUp(BlockType type, int dist);
	virtual BOOL SeeMeDown(BlockType type, int dist);
	virtual BOOL SeeMeRight(BlockType type, int dist);
	virtual BOOL SeeMeLeft(BlockType type, int dist);

	virtual BOOL ShootUp();
	virtual BOOL ShootDown();
	virtual BOOL ShootRight();
	virtual BOOL ShootLeft();

	virtual BOOL RotateLeft() {
		rotation--;
		if (rotation<0) rotation=0;
		return TRUE;
	}
	virtual BOOL RotateRight() {
		rotation++;
		SetRotation(rotation);
		return TRUE;
	}

	virtual void Update() {
		Draw();
		DisplayBeam(BeamState());
		if (Firing) Firing=FiringRight=FiringLeft=FiringUp=FiringDown=FALSE;
		moved=FALSE;
	}

	virtual BOOL PreSeeMe() {
		return FALSE;
	}

	virtual BOOL AfterAnimate() {
		if (willKillFlag<10) FinalizeOver();
		else {willKillFlag-=10; return TRUE;}
		return FALSE;
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
	BOOL SeeMe(BlockType type) {
		SeeMeLeft(type, 0);
		SeeMeUp(type, 0);
		SeeMeRight(type, 0);
		SeeMeDown(type, 0);
		return TRUE;
	}

	static BOOL CheckInRange(int x, int y) {
		if (x>=COLUMNS||x<0) return FALSE;
		if (y>=ROWS||y<0) return FALSE;
		return TRUE;
	}
	Blocks GetBlocks();
	Blocks GetGrounds();
	Tile*& GetGround();
	Tile* Tile::GetBlock();

	BOOL RightBlock(Tile* &p);
	BOOL LeftBlock(Tile* &p);
	BOOL TopBlock(Tile* &p);
	BOOL BottomBlock(Tile* &p);

	BOOL RightGround(Tile* &p);
	BOOL LeftGround(Tile* &p);
	BOOL TopGround(Tile* &p);
	BOOL BottomGround(Tile* &p);

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

	virtual BOOL MoveUp();
	virtual BOOL MoveDown();
	virtual BOOL MoveRight();
	virtual BOOL MoveLeft();

protected: //members
	BOOL FiringUp, FiringDown, FiringLeft, FiringRight, Firing;
	int WasHitLeft, WasHitRight, WasHitTop, WasHitBottom, WasHit;
	int HadFiredLeft, HadFiredRight, HadFiredUp, HadFiredDown, HadFired;
	BOOL moved;

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
	virtual BOOL SetRotation(int r) {
		rotation=r;
		return TRUE;
	}

	virtual void SetOtherBlocks(unsigned int around) {
		around%=256;
		otherblocks=around+around*256;	
	}

	virtual BOOL Kill() {return FALSE;}

	virtual BOOL HitLeft() {Tile::HitLeft(); ShootRight(); return TRUE;}
	virtual BOOL HitTop() {Tile::HitTop(); ShootDown(); return TRUE;}
	virtual BOOL HitRight() {Tile::HitRight(); ShootLeft(); return TRUE;}
	virtual BOOL HitBottom() {Tile::HitBottom(); ShootUp(); return TRUE;}

	virtual BOOL LookLeft(BlockType type, int dist) {SeeMeRight(type, ++dist); return TRUE;}
	virtual BOOL LookTop(BlockType type, int dist) {SeeMeDown(type, ++dist); return TRUE;}
	virtual BOOL LookRight(BlockType type, int dist) {SeeMeLeft(type, ++dist); return TRUE;}
	virtual BOOL LookBottom(BlockType type, int dist) {SeeMeUp(type, ++dist); return TRUE;}

	virtual BlockType GetBlockType() {
		return GROUND;
	}

	static BOOL SetChanged(BOOL change) {
		BOOL ret=changed;
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
	static BOOL changed;
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
		if (Firing) Firing=FiringRight=FiringLeft=FiringUp=FiringDown=FALSE;
		moved=FALSE;
	}

	/*virtual BOOL HitLeft() {
		Tile* ground=GetGround();
		if (ground) ground->WasHit=ground->WasHitLeft=BEAM_PERSISTANCE;
		return TRUE;
	}
	virtual BOOL HitTop() {
		Tile* ground=GetGround();
		if (ground) ground->WasHit=ground->WasHitTop=BEAM_PERSISTANCE;
		return TRUE;
	}
	virtual BOOL HitRight() {
		Tile* ground=GetGround();
		if (ground) ground->WasHit=ground->WasHitRight=BEAM_PERSISTANCE;
		return TRUE;
	}
	virtual BOOL HitBottom() {
		Tile* ground=GetGround();
		if (ground) ground->WasHit=ground->WasHitBottom=BEAM_PERSISTANCE;
		return TRUE;
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

	virtual BOOL SetRotation(int r) {
		rotation=r%12;
		return TRUE;
	}

	virtual BOOL PreSeeMe() {
		return SeeMe(TANK);
	}

	virtual BlockType GetBlockType()
	{
		return TANK;
	}

	//virtual BOOL Kill();

	virtual BOOL ShootUp() {
	//sound.PlayASound("beam.wav");
		BOOL ret=ATile::ShootUp();
		//SeeMe(TANK);
		return ret;

	}
	virtual BOOL ShootDown() {
	//sound.PlayASound("beam.wav");
		BOOL ret=ATile::ShootDown();
		//SeeMe(TANK);
		return ret;

	}
	virtual BOOL ShootRight() {
	//sound.PlayASound("beam.wav");
		BOOL ret=ATile::ShootRight();
		//SeeMe(TANK);
		return ret;

	}
	virtual BOOL ShootLeft() {
	//sound.PlayASound("beam.wav");
		BOOL ret=ATile::ShootLeft();
		//SeeMe(TANK);
		return ret;

	}
	virtual BOOL HitLeft() {ATile::HitLeft(); sound.PlayASound("pop.wav", POP); return WillKill();}
	virtual BOOL HitTop() {ATile::HitTop(); sound.PlayASound("pop.wav", POP); return WillKill();}
	virtual BOOL HitRight() {ATile::HitRight(); sound.PlayASound("pop.wav", POP); return WillKill();}
	virtual BOOL HitBottom() {ATile::HitBottom(); sound.PlayASound("pop.wav", POP); return WillKill();}

	virtual BOOL RotateLeft() {
		int row=rotation/4;
		rotation--;
		if (rotation<0) rotation=0;
		if (rotation/4!=row) rotation+=4;

		return TRUE;
	}
	virtual BOOL RotateRight() {
		int row=rotation/4;
		rotation++;
		if (rotation/4!=row) rotation-=4;

		return TRUE;
	}
	static void RestoreSurface() {if (surface) surface->Restore();}


protected: //functions
	virtual int BeamState() {
		return 0;
	}
	virtual void Tank::Draw();
	virtual BOOL MoveUp() {
		BOOL ret=ATile::MoveUp();
		//if (ret) sound.PlayASound("tick.wav");
		//SeeMe(TANK);
		return ret;
	}
	virtual BOOL MoveDown() {
		BOOL ret=ATile::MoveDown();
		//if (ret) sound.PlayASound("tick.wav");
		//SeeMe(TANK);
		return ret;
	}
	virtual BOOL MoveRight() {
		BOOL ret=ATile::MoveRight();
		//if (ret) sound.PlayASound("tick.wav");
		//SeeMe(TANK);
		return ret;
	}
	virtual BOOL MoveLeft() {
		BOOL ret=ATile::MoveLeft();
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
	virtual BOOL SetRotation(int r) {
		rotation=0;
		return FALSE;
	}

	virtual BOOL HitLeft() {Tile::HitLeft(); PushLeft(); return TRUE;}
	virtual BOOL HitTop() {Tile::HitTop(); PushTop(); return TRUE;}
	virtual BOOL HitRight() {Tile::HitRight(); PushRight(); return TRUE;}
	virtual BOOL HitBottom() {Tile::HitBottom(); PushBottom(); return TRUE;}
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
	virtual BOOL SetRotation(int r) {
		rotation=2;
		return FALSE;
	}
	static void RestoreSurface() {if (surface) surface->Restore();}


protected: //functions
	virtual void WhiteBlock::Draw() {
		//Exception::Output("Tank::draw called.");
		if (surface) draw.BlitSquare(surface, 2 ,0, x_pos, y_pos); 
	}
	virtual BOOL MoveUp() {sound.PlayASound(NULL, SCRAPE); return Tile::MoveUp();}
	virtual BOOL MoveDown() {sound.PlayASound(NULL, SCRAPE); return Tile::MoveDown();}
	virtual BOOL MoveLeft() {sound.PlayASound(NULL, SCRAPE); return Tile::MoveLeft();}
	virtual BOOL MoveRight() {sound.PlayASound(NULL, SCRAPE); return Tile::MoveRight();}
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

	virtual BOOL BlockOver(Tile* &block, Tile* &ground);
	virtual BOOL SetRotation(int r) {
		rotation=r;
		return TRUE;
	}
	virtual BlockType GetBlockType() {
		return WATER;
	}

	static void IncStaticRotation() {
		static BOOL chop=TRUE;
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
	virtual BOOL SetRotation(int r) {
		if (r>8) r-=4;
		rotation=r;
		return TRUE;
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
	virtual BOOL PushLeft() {return FALSE;}
	virtual BOOL PushRight() {return FALSE;}
	virtual BOOL PushTop() {return FALSE;}
	virtual BOOL PushBottom() {return FALSE;}

	virtual BOOL HitLeft() {sound.PlayASound("donk.wav", DONK); return span=1;}
	virtual BOOL HitRight() {sound.PlayASound("donk.wav", DONK); return span=1;}
	virtual BOOL HitTop() {sound.PlayASound("donk.wav", DONK); return span=1;}
	virtual BOOL HitBottom() {sound.PlayASound("donk.wav", DONK); return span=1;}



protected: //members
		static LPDIRECTDRAWSURFACE surface;
private: // functions

	virtual void SetSurface() {
		//Exception::Output("Tank::setsurface() called.");
		surface=draw.GetSurface("STATIC");
	}
private: //members
	BOOL turn_right;
	int time, span;

};

class Mirror : public ATile
{
public:
	Mirror(int x, int y, int r=0) : ATile(x,y,r), rotation2(0){
		if (!surface) SetSurface();
		SetRotation(r);

	}
	virtual BOOL SetRotation(int r) {
		rotation=r;
		rotation%=16;
		return TRUE;
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


	virtual BOOL HitLeft() {
		ATile::HitLeft();
		//if (Firing) return FALSE;
		switch(rotation%4) {
		case 1: if (Firing) return FALSE;
			ShootDown();
			break;
		case 2:  if (Firing) return FALSE;
			ShootUp();
			break;
		case 0:
		case 3:
			return PushLeft();
			break;
		default:
			return FALSE;
			break;
		}
		return TRUE;

	}
	virtual BOOL HitTop() {
		ATile::HitTop();
		//if (Firing) return FALSE;
		switch(rotation%4) {
		case 0:
		case 1:
			return PushTop();
			break;
		case 2: if (Firing) return FALSE;
			ShootLeft();
			break;
		case 3: if (Firing) return FALSE;
			ShootRight();
			break;
		default:
			return FALSE;
			break;
		}
		return TRUE;

	}
	virtual BOOL HitRight() {
		ATile::HitRight();
		//if (Firing) return FALSE;
		switch(rotation%4) {
		case 0: if (Firing) return FALSE;
			ShootDown();
			break;
		case 1:
		case 2:
			return PushRight();
			break;
		case 3: if (Firing) return FALSE;
			ShootUp();
			break;
		default:
			return FALSE;
			break;
		}
		return TRUE;

	}
	virtual BOOL HitBottom() {
		ATile::HitBottom();
		//if (Firing) return FALSE;
		switch(rotation%4) {
		case 0: if (Firing) return FALSE;
			ShootRight();
			break;
		case 1: if (Firing) return FALSE;
			ShootLeft();
			break;
		case 2:
		case 3:
			return PushBottom();
			break;
		default:
			return FALSE;
			break;
		}
		return TRUE;

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

	virtual BOOL HitLeft() {
		Tile::HitLeft();
		//if (Firing) return FALSE;
		switch(rotation%7) {
		case 0: if (Firing) return FALSE;
			ShootRight();
			ShootDown();
			break;
		case 1: if (Firing) return FALSE;
			ShootUp();
			ShootDown();
			break;
		case 2: if (Firing) return FALSE;
			ShootUp();
			ShootRight();
			break;
		case 3:
		case 6:
			return PushLeft();
			break;
		case 4: if (Firing) return FALSE;
			ShootUp();
			ShootRight();
			ShootDown();
			break;
		case 5: if (Firing) return FALSE;
			ShootRight();
			break;
		default:
			return FALSE;
			break;
		}
		return TRUE;

	}
	virtual BOOL HitTop() {
		Tile::HitTop();
		//if (Firing) return FALSE;
		switch(rotation%7) {
		case 1: if (Firing) return FALSE;
			ShootLeft();
			ShootDown();
			break;
		case 0:
		case 5:
			return PushTop();
			break;
		case 2: if (Firing) return FALSE;
			ShootLeft();
			ShootRight();
			break;
		case 3: if (Firing) return FALSE;
			ShootRight();
			ShootDown();
			break;
		case 4: if (Firing) return FALSE;
			ShootLeft();
			ShootRight();
			ShootDown();
			break;
		case 6: if (Firing) return FALSE;
			ShootDown();
			break;
		default:
			return FALSE;
			break;
		}
		return TRUE;

	}
	virtual BOOL HitRight() {
		Tile::HitRight();
		//if (Firing) return FALSE;
		switch(rotation%7) {
		case 0: if (Firing) return FALSE;
			ShootLeft();
			ShootDown();
			break;
		case 1:
		case 6:
			return PushRight();
		case 2: if (Firing) return FALSE;
			ShootLeft();
			ShootUp();
			break;
		case 3: if (Firing) return FALSE;
			ShootUp();
			ShootDown();
			break;
		case 4: if (Firing) return FALSE;
			ShootLeft();
			ShootUp();
			ShootDown();
			break;
		case 5: if (Firing) return FALSE;
			ShootLeft();
			break;
		default:
			return FALSE;
			break;
		}
		return TRUE;

	}
	virtual BOOL HitBottom() {
		Tile::HitBottom();
		//if (Firing) return FALSE;
		switch(rotation%7) {
		case 0: if (Firing) return FALSE;
			ShootLeft();
			ShootRight();
			break;
		case 1: if (Firing) return FALSE;
			ShootLeft();
			ShootUp();
			break;
		case 3: if (Firing) return FALSE;
			ShootUp();
			ShootRight();
			break;
		case 2:
		case 5:
			return PushBottom();
			break;
		case 4: if (Firing) return FALSE;
			ShootLeft();
			ShootUp();
			ShootRight();
			break;
		case 6: if (Firing) return FALSE;
			ShootUp();
			break;
		default:
			return FALSE;
			break;
		}
		return TRUE;

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
	virtual BOOL SetRotation(int r) {return ATile::SetRotation(r);}

	virtual BOOL PreSeeMe() {
		return FALSE;
	}

	virtual BlockType GetBlockType() {
		return ENEMYTANK;
	}

	virtual BOOL LookLeft(BlockType type, int dist) {ShootLeft(); return TRUE;}
	virtual BOOL LookTop(BlockType type, int dist) {ShootUp(); return TRUE;}
	virtual BOOL LookRight(BlockType type, int dist) {ShootRight(); return TRUE;}
	virtual BOOL LookBottom(BlockType type, int dist) {ShootDown(); return TRUE;}

	virtual BOOL ShootUp() {SetRotation(1); return ATile::ShootUp();}
	virtual BOOL ShootDown() {SetRotation(3); return ATile::ShootDown();}
	virtual BOOL ShootRight() {SetRotation(2); return ATile::ShootRight();}
	virtual BOOL ShootLeft() {SetRotation(0); return ATile::ShootLeft();}
	static void RestoreSurface() {if (surface) surface->Restore();}

protected: //functions
	virtual void EnemyTank::Draw() {
		//Exception::Output("Tank::draw called.");
		int rotate=rand()%1000;
		if (rotate==0) RotateRight();	//make tanks rotate randomly
		else if (rotate==1) RotateLeft();
		if (surface) draw.BlitSquare(surface, rotation ,1, x_pos, y_pos); 
	}
	virtual BOOL MoveUp() {
		return ATile::MoveUp();
	}
	virtual BOOL MoveDown() {
		return ATile::MoveDown();
	}
	virtual BOOL MoveRight() {
		return ATile::MoveRight();
	}
	virtual BOOL MoveLeft() {
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
	virtual BOOL HitLeft() {ATile::HitLeft(); Kill(); return TRUE;}
	virtual BOOL HitTop() {ATile::HitTop(); Kill(); return TRUE;}
	virtual BOOL HitRight() {ATile::HitRight(); Kill(); return TRUE;}
	virtual BOOL HitBottom() {ATile::HitBottom(); Kill(); return TRUE;}

	virtual BOOL Kill();
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

	virtual BOOL HitLeft() {ATile::HitLeft(); sound.PlayASound("collapse.wav", COLLAPSE); return Kill();}
	virtual BOOL HitTop() {ATile::HitTop(); sound.PlayASound("collapse.wav", COLLAPSE); return Kill();}
	virtual BOOL HitRight() {ATile::HitRight(); sound.PlayASound("collapse.wav", COLLAPSE); return Kill();}
	virtual BOOL HitBottom() {ATile::HitBottom(); sound.PlayASound("collapse.wav", COLLAPSE); return Kill();}
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

	BOOL turn_right;

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
	virtual BOOL SetRotation(int r) {
		rotation=1;
		return FALSE;
	}
	virtual BOOL HitLeft() {ATile::HitLeft(); sound.PlayASound("pop.wav", POP); return Kill();}
	virtual BOOL HitTop() {ATile::HitTop(); sound.PlayASound("pop.wav", POP); return Kill();}
	virtual BOOL HitRight() {ATile::HitRight(); sound.PlayASound("pop.wav", POP); return Kill();}
	virtual BOOL HitBottom() {ATile::HitBottom(); sound.PlayASound("pop.wav", POP); return Kill();}
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

	virtual BOOL LookLeft(BlockType type, int dist) {if (!dist) Kill(); return TRUE;}
	virtual BOOL LookTop(BlockType type, int dist) {if (!dist) Kill(); return TRUE;}
	virtual BOOL LookRight(BlockType type, int dist) {if (!dist) Kill(); return TRUE;}
	virtual BOOL LookBottom(BlockType type, int dist) {if (!dist) Kill(); return TRUE;}

	//virtual BOOL HitLeft() {return ATile::HitLeft();}
	//virtual BOOL HitTop() {return ATile::HitTop();}
	//virtual BOOL HitRight() {return ATile::HitRight();}
	//virtual BOOL HitBottom() {return ATile::HitBottom();}
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
	virtual BOOL SetRotation(int r) {
		rotation=r;
		return TRUE;
	}
	static void RestoreSurface() {if (surface) surface->Restore();}

protected: //functions
	virtual void Message::Draw() {
		if (surface) draw.BlitSquare(surface, rotation%14 ,rotation/14, x_pos, y_pos); 
	}
	virtual BOOL PushLeft() {return FALSE;}
	virtual BOOL PushRight() {return FALSE;}
	virtual BOOL PushTop() {return FALSE;}
	virtual BOOL PushBottom() {return FALSE;}

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
	virtual BOOL SetRotation(int r) {
		rotation=0;
		return FALSE;
	}
	static void RestoreSurface() {if (surface) surface->Restore();}

protected: //functions
	virtual void BarsVert::Draw() {

		if (surface) draw.BlitSquare(surface, 0 ,0, x_pos, y_pos); 
	}

	virtual BOOL HitTop() {Tile::HitTop(); return ShootDown();}
	virtual BOOL HitBottom() {Tile::HitBottom(); return ShootUp();}

	virtual BOOL LookTop(BlockType type, int dist) {SeeMeDown(type, ++dist); return TRUE;}
	virtual BOOL LookBottom(BlockType type, int dist) {SeeMeUp(type, ++dist); return TRUE;}

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
	virtual BOOL SetRotation(int r) {
		rotation=0;
		return FALSE;
	}
	static void RestoreSurface() {if (surface) surface->Restore();}

protected: //functions
	virtual void BarsHoriz::Draw() {

		if (surface) draw.BlitSquare(surface, 1 ,0, x_pos, y_pos); 
	}
	virtual BOOL HitTop() {return Static::HitTop();}
	virtual BOOL HitBottom() {return Static::HitBottom();}
	virtual BOOL HitLeft() {Tile::HitLeft(); return ShootRight();}
	virtual BOOL HitRight() {Tile::HitRight(); return ShootLeft();}

	virtual BOOL LookLeft(BlockType type, int dist) {SeeMeRight(type, ++dist); return TRUE;}
	virtual BOOL LookTop(BlockType type, int dist) {return FALSE;}
	virtual BOOL LookRight(BlockType type, int dist) {SeeMeLeft(type, ++dist); return TRUE;}
	virtual BOOL LookBottom(BlockType type, int dist) {return FALSE;}

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
	virtual BOOL SetRotation(int r) {
		rotation=0;
		return FALSE;
	}
	static void RestoreSurface() {if (surface) surface->Restore();}

protected: //functions
	virtual void BarsCross::Draw() {

		if (surface) draw.BlitSquare(surface, 2 ,0, x_pos, y_pos); 
	}

	virtual BOOL HitTop() {Tile::HitTop(); return ShootDown();}
	virtual BOOL HitBottom() {Tile::HitBottom(); return ShootUp();}
	virtual BOOL HitLeft() {Tile::HitLeft(); return ShootRight();}
	virtual BOOL HitRight() {Tile::HitRight(); return ShootLeft();}

	virtual BOOL LookLeft(BlockType type, int dist) {SeeMeRight(type, ++dist); return TRUE;}
	virtual BOOL LookTop(BlockType type, int dist) {SeeMeDown(type, ++dist); return TRUE;}
	virtual BOOL LookRight(BlockType type, int dist) {SeeMeLeft(type, ++dist); return TRUE;}
	virtual BOOL LookBottom(BlockType type, int dist) {SeeMeUp(type, ++dist); return TRUE;}

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
	virtual BOOL SetRotation(int r) {
		rotation=r;
		rotation%=4;
		return TRUE;
	}

	virtual BOOL PushLeft() {return FALSE;}
	virtual BOOL PushRight() {return FALSE;}
	virtual BOOL PushTop() {return FALSE;}
	virtual BOOL PushBottom() {return FALSE;}

	virtual BOOL HitLeft() {sound.PlayASound("donk.wav", DONK); return Mirror::HitLeft();}
	virtual BOOL HitRight() {sound.PlayASound("donk.wav", DONK); return Mirror::HitRight();}
	virtual BOOL HitTop() {sound.PlayASound("donk.wav", DONK); return Mirror::HitTop();}
	virtual BOOL HitBottom() {sound.PlayASound("donk.wav", DONK); return Mirror::HitBottom();}

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

	virtual BOOL HitLeft() {
		Triangle::HitLeft();
		sound.PlayASound("collapse.wav", COLLAPSE); return Kill();	
	}
	virtual BOOL HitRight() {
		Triangle::HitRight();
		sound.PlayASound("collapse.wav", COLLAPSE); return Kill();	
	}
	virtual BOOL HitTop() {
		Triangle::HitTop();
		sound.PlayASound("collapse.wav", COLLAPSE); return Kill();	
	}
	virtual BOOL HitBottom() {
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

	virtual BOOL HitLeft() {
		ATile::HitLeft();
		sound.PlayASound("collapse.wav", COLLAPSE); return Kill();	
	}
	virtual BOOL HitRight() {
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

	virtual BOOL HitTop() {
		ATile::HitTop();
		sound.PlayASound("collapse.wav", COLLAPSE); return Kill();	
	}
	virtual BOOL HitBottom() {
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
	
	virtual BOOL HitLeft() {ATile::HitLeft(); sound.PlayASound("pop.wav", POP); return Kill();}
	virtual BOOL HitTop() {ATile::HitTop(); sound.PlayASound("pop.wav", POP); return Kill();}
	virtual BOOL HitRight() {ATile::HitRight(); sound.PlayASound("pop.wav", POP); return Kill();}
	virtual BOOL HitBottom() {ATile::HitBottom(); sound.PlayASound("pop.wav", POP); return Kill();}

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


