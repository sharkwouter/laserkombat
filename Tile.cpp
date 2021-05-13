//Tile.cpp
#include <ddraw.h>
#include "pix.h"
#include "sound.h"

#include "Exception.h"
#include "draw.h"
#include "Tile.h"

#include "square.h"
#include "board.h"

LPDIRECTDRAWSURFACE Tile::surface=NULL;
LPDIRECTDRAWSURFACE Tank::surface=draw.GetSurface("TANK");
LPDIRECTDRAWSURFACE GroundTile::surface=draw.GetSurface("GROUND");
LPDIRECTDRAWSURFACE RedBlock::surface=draw.GetSurface("REDBLOCK");
LPDIRECTDRAWSURFACE Water::surface=draw.GetSurface("WATER");
LPDIRECTDRAWSURFACE Static::surface=draw.GetSurface("STATIC");
LPDIRECTDRAWSURFACE Mirror::surface=draw.GetSurface("MIRROR");
LPDIRECTDRAWSURFACE Tee::surface=draw.GetSurface("TEE");
LPDIRECTDRAWSURFACE Nuke::surface=draw.GetSurface("NUKE");
LPDIRECTDRAWSURFACE Rusty::surface=draw.GetSurface("RUSTY");
LPDIRECTDRAWSURFACE Message::surface=draw.GetSurface("MESSAGE");
LPDIRECTDRAWSURFACE BarsVert::surface=draw.GetSurface("BARS");

LPDIRECTDRAWSURFACE Tile::beamSurface=NULL;

bool GroundTile::changed=true;

int Water::static_rotation=0;

/*bool Tank::Kill() {
	int x,y;
	board.GetTankXY(x,y);
	bool ret;
	if ((ret=ATile::Kill()) && x==x_pos && y==y_pos) board.YouDied();
	return ret;
}*/

void Tile::AddDead(Tile* tile) {
	if(!tile) return;
	Tile *current_tile=this;
	if (tile->GetBlockType()==ENEMYTANK) board.DecreaseEnemyCount();
	while (current_tile->deadBlock) {
		current_tile=current_tile->deadBlock;
	}
	current_tile->deadBlock=tile;
}


 
bool Tile::Kill() {
	if (!board.array[x_pos][y_pos]->block) return true;
	Tile* temp=board.array[x_pos][y_pos]->block;
	board.array[x_pos][y_pos]->block=NULL;
	//Schedule for deletion
	board.array[x_pos][y_pos]->ground->AddDead(this);
	return true;
}

bool Tile::KillOver() {
	if (!board.array[x_pos][y_pos]->over) return true;
	Tile* temp=board.array[x_pos][y_pos]->over;
	board.array[x_pos][y_pos]->over=NULL;
	//Schedule for deletion
	board.array[x_pos][y_pos]->ground->AddDead(this);	
	return true;
}

bool Tile::WillKill(int flag) {
	board.array[x_pos][y_pos]->block=NULL;
	if (flag==1) willKillFlag=flag+10*BEAM_PERSISTANCE;
	else willKillFlag=flag;	//don't persist long for plain kill
	Tile* temp=board.array[x_pos][y_pos]->over;
	if (temp) {
		temp->FinalizeOver();
	}
	board.array[x_pos][y_pos]->over=this;
	return true;
}

void Tile::FinalizeOver() {// Get the over tile out of the way
	if (willKillFlag%10==2) {
		KillOver();
	}
	else if (willKillFlag%10==1) {
		DrownOver();
		if (!board.IsOver())PreSeeMe();
	}
}

void Tile::ClearOver() {//make sure the over layer is clear
	Tile* temp=board.array[x_pos][y_pos]->over;
	if (temp) {
		temp->FinalizeOver();
	}
	
}

bool Nuke::Kill() {
	ATile::Kill();
	int rr=0;
	Blocks blocks=GetGrounds();
	/*if (blocks.left&&blocks.left->GetBlockType()==GROUND) blocks.left->SetRotation((rr=blocks.left->GetRotation())>=19&&rr<100?rr+300:rand()%5+25+300);
	if (blocks.above&&blocks.above->GetBlockType()==GROUND) blocks.above->SetRotation((rr=blocks.above->GetRotation())>=19&&rr<100?rr+300:rand()%5+25+300);
	if (blocks.right&&blocks.right->GetBlockType()==GROUND) blocks.right->SetRotation((rr=blocks.right->GetRotation())>=19&&rr<100?rr+300:rand()%5+25+300);
	if (blocks.below&&blocks.below->GetBlockType()==GROUND) blocks.below->SetRotation((rr=blocks.below->GetRotation())>=19&&rr<100?rr+300:rand()%5+25+300);
	if (blocks.aboveleft&&blocks.aboveleft->GetBlockType()==GROUND) blocks.aboveleft->SetRotation((rr=blocks.aboveleft->GetRotation())>=19&&rr<100?rr+300:rand()%5+25+300);
	if (blocks.aboveright&&blocks.aboveright->GetBlockType()==GROUND) blocks.aboveright->SetRotation((rr=blocks.aboveright->GetRotation())>=19&&rr<100?rr+300:rand()%5+25+300);
	if (blocks.belowright&&blocks.belowright->GetBlockType()==GROUND) blocks.belowright->SetRotation((rr=blocks.belowright->GetRotation())>=19&&rr<100?rr+300:rand()%5+25+300);
	if (blocks.belowleft&&blocks.belowleft->GetBlockType()==GROUND) blocks.belowleft->SetRotation((rr=blocks.belowleft->GetRotation())>=19&&rr<100?rr+300:rand()%5+25+300);
	*/

	if (blocks.left) blocks.left->SetRotation(333);
	if (blocks.above) blocks.above->SetRotation(333);
	if (blocks.right) blocks.right->SetRotation(333);
	if (blocks.below) blocks.below->SetRotation(333);
	if (blocks.aboveleft) blocks.aboveleft->SetRotation(333);
	if (blocks.aboveright) blocks.aboveright->SetRotation(333);
	if (blocks.belowright) blocks.belowright->SetRotation(333);
	if (blocks.belowleft) blocks.belowleft->SetRotation(333);

	blocks=GetBlocks();
	if (blocks.left) blocks.left->Kill();
	if (blocks.above) blocks.above->Kill();
	if (blocks.right) blocks.right->Kill();
	if (blocks.below) blocks.below->Kill();
	if (blocks.aboveleft) blocks.aboveleft->Kill();
	if (blocks.aboveright) blocks.aboveright->Kill();
	if (blocks.belowright) blocks.belowright->Kill();
	if (blocks.belowleft) blocks.belowleft->Kill();

	Tile* &ground=GetGround();
	if (ground) {
		Tile* temp=ground;
		ground=new Water(x_pos,y_pos);
		ground->AddDead(temp);
		ground->SetRotation(ground->GetRotation()+300);

	}

	GroundTile::SetChanged(true);	//flag for changes to update images draw
	sound.PlayASound("nuke.wav", NUKE_SOUND);
	return true;
}


Tile*& Tile::GetGround()
{
	return board.array[x_pos][y_pos]->ground;
}

Tile* Tile::GetBlock() 
{
	return board.array[x_pos][y_pos]->block;
}

Blocks Tile::GetBlocks()
{
	Blocks blocks;
	if (x_pos>=COLUMNS-1) blocks.right=NULL;
	else blocks.right=board.array[x_pos+1][y_pos]->block;
	if (x_pos<=0) blocks.left=NULL;
	else blocks.left=board.array[x_pos-1][y_pos]->block;
	if (y_pos>=ROWS-1) blocks.below=NULL;
	else blocks.below=board.array[x_pos][y_pos+1]->block;
	if (y_pos<=0) blocks.above=NULL;
	else blocks.above=board.array[x_pos][y_pos-1]->block;

	if (x_pos>=COLUMNS-1||y_pos>=ROWS-1) blocks.belowright=NULL;
	else blocks.belowright=board.array[x_pos+1][y_pos+1]->block;
	if (x_pos>=COLUMNS-1||y_pos<=0) blocks.aboveright=NULL;
	else blocks.aboveright=board.array[x_pos+1][y_pos-1]->block;
	if (x_pos<=0||y_pos>=ROWS-1) blocks.belowleft=NULL;
	else blocks.belowleft=board.array[x_pos-1][y_pos+1]->block;
	if (x_pos<=0||y_pos<=0) blocks.aboveleft=NULL;
	else blocks.aboveleft=board.array[x_pos-1][y_pos-1]->block;
	return blocks;
}

Blocks Tile::GetGrounds()
{
	Blocks blocks;
	if (x_pos>=COLUMNS-1) blocks.right=NULL;
	else blocks.right=board.array[x_pos+1][y_pos]->ground;
	if (x_pos<=0) blocks.left=NULL;
	else blocks.left=board.array[x_pos-1][y_pos]->ground;
	if (y_pos>=ROWS-1) blocks.below=NULL;
	else blocks.below=board.array[x_pos][y_pos+1]->ground;
	if (y_pos<=0) blocks.above=NULL;
	else blocks.above=board.array[x_pos][y_pos-1]->ground;

	if (x_pos>=COLUMNS-1||y_pos>=ROWS-1) blocks.belowright=NULL;
	else blocks.belowright=board.array[x_pos+1][y_pos+1]->ground;
	if (x_pos>=COLUMNS-1||y_pos<=0) blocks.aboveright=NULL;
	else blocks.aboveright=board.array[x_pos+1][y_pos-1]->ground;
	if (x_pos<=0||y_pos>=ROWS-1) blocks.belowleft=NULL;
	else blocks.belowleft=board.array[x_pos-1][y_pos+1]->ground;
	if (x_pos<=0||y_pos<=0) blocks.aboveleft=NULL;
	else blocks.aboveleft=board.array[x_pos-1][y_pos-1]->ground;
	return blocks;
}

bool Tile::RightBlock(Tile* &p)
{
	if (x_pos>=COLUMNS-1) {p=NULL; return false;}
	p=board.array[x_pos+1][y_pos]->block;
	return true;
}

bool Tile::LeftBlock(Tile* &p)
{
	if (x_pos<=0) {p=NULL; return false;}
	p=board.array[x_pos-1][y_pos]->block;
	return true;
}

bool Tile::TopBlock(Tile* &p)
{
	if (y_pos<=0) {p=NULL; return false;}
	p=board.array[x_pos][y_pos-1]->block;
	return true;
}

bool Tile::BottomBlock(Tile* &p)
{
	if (y_pos>=ROWS-1) {p=NULL; return false;}
	p=board.array[x_pos][y_pos+1]->block;
	return true;
}

bool Tile::RightGround(Tile* &p)
{
	if (x_pos>=COLUMNS-1) {p=NULL; return false;}
	p=board.array[x_pos+1][y_pos]->ground;
	return true;
}

bool Tile::LeftGround(Tile* &p)
{
	if (x_pos<=0) {p=NULL; return false;}
	p=board.array[x_pos-1][y_pos]->ground;
	return true;
}

bool Tile::TopGround(Tile* &p)
{
	if (y_pos<=0) {p=NULL; return false;}
	p=board.array[x_pos][y_pos-1]->ground;
	return true;
}

bool Tile::BottomGround(Tile* &p)
{
	if (y_pos>=ROWS-1) {p=NULL; return false;}
	p=board.array[x_pos][y_pos+1]->ground;
	return true;
}




bool Tile::MoveUp() {
	if (moved) return false;
	Tile* block; TopBlock(block);
	if (block) block->PushBottom();
	bool ret = board.swap(x_pos, y_pos, x_pos, y_pos-1);
	if (ret&&GetBlockType()!=TANK) sound.PlayASound("scrape.wav", SCRAPE);
	return moved=ret;
}
bool Tile::MoveDown() {
	if (moved) return false;
	Tile* block; BottomBlock(block);
	if (block) block->PushTop();
	bool ret =board.swap(x_pos, y_pos, x_pos, y_pos+1);
	if (ret&&GetBlockType()!=TANK) sound.PlayASound("scrape.wav", SCRAPE);
	return moved=ret;
}
bool Tile::MoveRight() {
	if (moved) return false;
	Tile* block; RightBlock(block);
	if (block) block->PushLeft();
	bool ret =board.swap(x_pos, y_pos, x_pos+1, y_pos);
	if (ret&&GetBlockType()!=TANK) sound.PlayASound("scrape.wav", SCRAPE);
	return moved=ret;
}
bool Tile::MoveLeft() {
	if (moved) return false;
	Tile* block; LeftBlock(block);
	if (block) block->PushRight();
	bool ret =board.swap(x_pos, y_pos, x_pos-1, y_pos);
	if (ret&&GetBlockType()!=TANK) sound.PlayASound("scrape.wav", SCRAPE);
	return moved=ret;
}

bool Water::BlockOver(Tile* &block, Tile* &ground)
{
	Tile::BlockOver(block, ground);
	if (!block) return false;
	
	// else:
	Tile* temp = block;

	if (temp->GetBlockType()==WHITEBLOCK) return true;

	if (temp->GetBlockType()==REDBLOCK) {
		ground=new GroundTile(x_pos, y_pos, 10);
		ground->AddDead(this);
	}
	block->WillKill(1); // 1 for drown
	GroundTile::SetChanged(1);
	sound.PlayASound("splash.wav", SPLASH);
	return true;
}

bool Tile::ShootUp()
{
	Tile* ground=GetGround();
	if (ground) ground->HadFired=ground->HadFiredUp=BEAM_PERSISTANCE;
	Firing=FiringUp=true;
	HadFired=HadFiredUp=BEAM_PERSISTANCE;
	Tile* block; TopBlock(block);
	if (block) block->HitBottom();
	else {
		TopGround(block);
		if (block) block->HitBottom();
	}
	return true;
}

bool Tile::ShootDown()
{
	Tile* ground=GetGround();
	if (ground) ground->HadFired=ground->HadFiredDown=BEAM_PERSISTANCE;
	Firing=FiringDown=true;
	HadFired=HadFiredDown=BEAM_PERSISTANCE;
	Tile* block; BottomBlock(block);
	if (block) block->HitTop();
	else {
		BottomGround(block);
		if (block) block->HitTop();
	}
	return true;
}
bool Tile::ShootRight()
{
	Tile* ground=GetGround();
	if (ground) ground->HadFired=ground->HadFiredRight=BEAM_PERSISTANCE;
	Firing=FiringRight=true;
	HadFired=HadFiredRight=BEAM_PERSISTANCE;
	Tile* block; RightBlock(block);
	if (block) block->HitLeft();
	else {
		RightGround(block);
		if (block) block->HitLeft();
	}
	return true;
}
bool Tile::ShootLeft()
{
	Tile* ground=GetGround();
	if (ground) ground->HadFired=ground->HadFiredLeft=BEAM_PERSISTANCE;
	Firing=FiringLeft=true;
	HadFired=HadFiredLeft=BEAM_PERSISTANCE;
	Tile* block; LeftBlock(block);
	if (block) block->HitRight();
	else {
		LeftGround(block);
		if (block) block->HitRight();
	}
	return true;
}

bool Tile::SeeMeUp(BlockType type, int dist)
{
	Tile* block; TopBlock(block);
	if (block) block->LookBottom(type, dist);
	else {
		TopGround(block);
		if (block) block->LookBottom(type, dist);
	}
	return true;
}

bool Tile::SeeMeDown(BlockType type, int dist)
{
	Tile* block; BottomBlock(block);
	if (block) block->LookTop(type, dist);
	else {
		BottomGround(block);
		if (block) block->LookTop(type, dist);
	}
	return true;
}
bool Tile::SeeMeRight(BlockType type, int dist)
{
	Tile* block; RightBlock(block);
	if (block) block->LookLeft(type, dist);
	else {
		RightGround(block);
		if (block) block->LookLeft(type, dist);
	}
	return true;
}
bool Tile::SeeMeLeft(BlockType type, int dist)
{
	Tile* block; LeftBlock(block);
	if (block) block->LookRight(type, dist);
	else {
		LeftGround(block);
		if (block) block->LookRight(type, dist);
	}
	return true;
}

void Tank::Draw() {
	if (!board.IsYou(x_pos, y_pos)) { 
		int rotate=rand()%1000;
		if (rotate==0) RotateRight();	//make tanks rotate randomly
		else if (rotate==1) RotateLeft();
	}
	if (surface) draw.BlitSquare(surface, rotation%4 ,rotation/4, x_pos, y_pos); 
}

int Tile::BeamState() {
	//int horiz=0;
	//int verti=0;
	if (HadFired) {
		//horiz=(HadFiredRight||HadFiredLeft);
		//verti=(HadFiredUp||HadFiredDown);
		if (HadFiredRight&&!board.IsOver()) HadFiredRight--;
		if (HadFiredDown&&!board.IsOver()) HadFiredDown--;
		if (HadFiredLeft&&!board.IsOver()) HadFiredLeft--;
		if (HadFiredUp&&!board.IsOver()) HadFiredUp--;

		if (!Firing&&HadFired&&!board.IsOver()) HadFired--;
	}
	if (WasHit) {
		//int direction=WasHitLeft?0:(WasHitTop?1:(WasHitRight?2:(WasHitBottom?3:4)));
		if (WasHitLeft&&!board.IsOver()) WasHitLeft--;
		if (WasHitTop&&!board.IsOver()) WasHitTop--;
		if (WasHitRight&&!board.IsOver()) WasHitRight--;
		if (WasHitBottom&&!board.IsOver()) WasHitBottom--;

		if (WasHit&&!board.IsOver()) WasHit--;
		//return direction+4;
	}


	//if (horiz) horiz=1;
	//if (verti) verti=2;
	//return horiz+verti;		//returns 0,1,2, or 3
	return 0;
}

int Mirror::BeamState() {
	if (!Firing&&HadFired&&!board.IsOver()) HadFired--;
	return 0;
}


