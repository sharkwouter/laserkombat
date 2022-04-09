#include "Board.h"

#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "Exception.h"
#include "Sound.h"
#include "Draw.h"

#include "BlockType.h"
#include "Square.h"
#include "Tile.h"

Board::Board(Textures * textures) : help(true), cols(COLUMNS), rows(ROWS), tank_x(0), tank_y(0), level(0), textures(textures) {
	LoadLevel();	
}

Board::~Board() {
	for (int i=0; i<ROWS; i++) {
		for (int j=0; j<COLUMNS; j++) {
			if (array[j][i] != NULL) delete array[j][i];
			array[j][i]=NULL;
		}
	}
}

void Board::handleInput(std::vector<Input> input) {
	for (auto i : input) {
		if (help) {
			help = false;
			return;
		}
		switch (i)
		{
		case Input::HELP:
			if (!help)
				help = true;
			break;
		case Input::LEFT:
			Left();
			break;
		case Input::RIGHT:
			Right();
			break;
		case Input::UP:
			Up();
			break;
		case Input::DOWN:
			Down();
			break;
		case Input::SHOOT:
			Fire();
			break;
		case Input::NEXTLEVEL:
			LoadLevel();
			break;
		case Input::PREVIOUSLEVEL:
			Previous();
			break;
		case Input::RESTART:
			Restart();
			break;
		}
	}
	
}

void Board::FillArray(bool credits)
{
	ClearArray();
	if (!credits) {
		number_of_enemies=0;
		died=defeated=finished=0;
		tank_x=origin_x;
		tank_y=origin_y;
	}
	GroundTile::SetChanged(1);


	for (int y=0; y<ROWS; y++) {
		for (int x=0; x<COLUMNS; x++) {
			array[x][y] = NULL;
			CreateSquare(x, y);
		}
	}
	CheckArray();	//Make sure no empty spots
	if (!credits) {
		if (number_of_enemies<1) YouDefeated();
	}
}

void Board::CreateSquare(int x, int y)
{
	if (array[x][y]) return;
	Tile* ground=NULL;
	Tile* block=NULL;
	BlockType type= groundTypeArray[x][y];
	int r= groundRotationArray[x][y];

	switch(type) {
	default:
	case BlockType::GROUND:		ground=	new GroundTile(x, y, textures, r); break;
	case BlockType::WATER:			ground=	new Water(x, y, textures, r); break;
	}

	type= blockTypeArray[x][y];
	r= blockRotationArray[x][y];

	switch(type) {
	default:			            block= NULL; break;
	case BlockType::REDBLOCK:		block=	new RedBlock(x, y, textures, r); break;
	case BlockType::RUSTYREDBLOCK:	block=	new RustyRedBlock(x, y, textures, r); break;
	case BlockType::WHITEBLOCK:	    block=	new WhiteBlock(x, y, textures, r); break;
	case BlockType::TEE:			block=  new Tee(x, y, textures, r); break;
	case BlockType::MIRROR:		    block=	new Mirror(x, y, textures, r); break;
	case BlockType::NUKE:			block=	new Nuke(x, y, textures, r); break;
	case BlockType::ENEMYNUKE:		block=	new EnemyNuke(x, y, textures, r); break;
	case BlockType::TANK:			block=	new Tank(x, y, textures, r); break;
	case BlockType::ENEMYTANK:		block=	new EnemyTank(x, y, textures, r); number_of_enemies++; break;
	case BlockType::STATIC:		    block=	new Static(x, y, textures, r); break;
	case BlockType::RUSTY:			block=	new Rusty(x, y, textures, r); break;
	case BlockType::BARSVERT:		block=	new BarsVert(x, y, textures, r); break;
	case BlockType::BARSHORIZ:		block=	new BarsHoriz(x, y, textures, r); break;
	case BlockType::BARSCROSS:		block=	new BarsCross(x, y, textures, r); break;
	case BlockType::TRIANGLE:		block=	new Triangle(x, y, textures, r); break;
	case BlockType::RUSTYTRIANGLE:	block=	new RustyTriangle(x, y, textures, r); break;
	case BlockType::RUSTYBARSVERT:	block=	new RustyBarsVert(x, y, textures, r); break;
	case BlockType::RUSTYBARSHORIZ: block=	new RustyBarsHoriz(x, y, textures, r); break;
	case BlockType::RUSTYWHITEBLOCK:block= new RustyWhiteBlock(x, y, textures, r); break;
	}

	if (!ground) ground= new GroundTile(x, y, textures, 33);
	array[x][y] = new Square(block, ground);
}

bool Board::Previous()
{
	level-=2;
	if (level<0) {level=1; return false;}
	return LoadLevel();
}


bool Board::LoadLevel() {
	level++;

	std::string filename = "assets/levels/level";
	filename += std::string(3 - std::min(3, int(std::to_string(level).length())), '0') + std::to_string(level);
	filename += ".lvl";

	FILE *file=fopen(filename.c_str(), "rb");
	if (file) {
		int size= fread( &origin_x, sizeof(origin_y), 1, file);
		if (size) size= fread( &origin_y, sizeof(origin_x), 1, file);
		origin_x%=COLUMNS;
		origin_y%=ROWS;

		if (size) size= fread(groundTypeArray, sizeof(BlockType), COLUMNS*ROWS, file);
		if (size==COLUMNS*ROWS) size= fread(groundRotationArray, sizeof(int), COLUMNS*ROWS, file);
		if (size==COLUMNS*ROWS) size= fread(blockTypeArray, sizeof(BlockType), COLUMNS*ROWS, file);
		if (size==COLUMNS*ROWS) size= fread(blockRotationArray, sizeof(int), COLUMNS*ROWS, file);
		
		fclose(file);
		FillArray(false);
		return (size==COLUMNS*ROWS);

	}
	else FillDefault();
	return false;
}

void Board::YouDied() {
	if (died) return;
	for (int y=ROWS/2-2, yy=0; y<ROWS/2+1; y++, yy++) {
		for (int x=COLUMNS/2-3, xx=0; x<COLUMNS/2+3; x++, xx++) {
			Tile* temp=array[x][y]->over;
			array[x][y]->over=new Message(x,y,textures,yy*14+xx);
			if (temp) array[x][y]->ground->AddDead(temp);
		}
	}
	died=true;
}

void Board::YouDefeated() {
	if (!TankExists()) return;
	if (died) return;
	if (defeated) return;
	if (finished) return;
	for (int y=ROWS/2-2, yy=0; y<ROWS/2+1; y++, yy++) {
		for (int x=COLUMNS/2-4, xx=6; x<COLUMNS/2+4; x++, xx++) {
			Tile* temp=array[x][y]->over;
			array[x][y]->over=new Message(x,y,textures,yy*14+xx);
			if (temp) array[x][y]->ground->AddDead(temp);
		}
	}
	defeated=true;
}

void Board::Credits() {
	for (int y=ROWS/2-5, yy=3; y<ROWS/2+6; y++, yy++) {
		for (int x=COLUMNS/2-7, xx=0; x<COLUMNS/2+7; x++, xx++) {
			Tile* temp=array[x][y]->over;
			array[x][y]->over=new Message(x,y,textures,yy*14+xx);
			if (temp) array[x][y]->ground->AddDead(temp);
		}
	}
	finished=true;
}

void Board::ClearArray()
{
	for (int i=0; i<ROWS; i++) {
		for (int j=0; j<COLUMNS; j++) {
			array[j][i]=NULL;
		}
	}
}

void Board::CheckArray()
{
	for (int i=0; i<ROWS; i++) {
		for (int j=0; j<COLUMNS; j++) {
			if (!array[j][i]) throw Exception("Unfilled square.");
			if (!array[j][i]->ground) throw Exception("Missing ground.");
		}
	}
}

bool Board::swap(int x, int y, int x1, int y1)
{
	if (x>=COLUMNS||x<0) return false;
	if (y>=ROWS||y<0) return false;
	if (!array[x][y]->block) return false;	//if you are nothing you can move anywhere
	if (x1>=COLUMNS||x1<0) return false;
	if (y1>=ROWS||y1<0) return false;
	if (array[x1][y1]->block) return false; //cannot move into occupied space
	Tile* temp=array[x][y]->block;
	array[x][y]->block=array[x1][y1]->block;
	array[x1][y1]->block=temp;
	//let the ground know someone's moving on it
	if (array[x1][y1]->block) array[x1][y1]->block->SetXY(x1,y1);
	if (array[x][y]->block) array[x][y]->block->SetXY(x,y);
	if (x==tank_x && y==tank_y) tank_x=x1, tank_y=y1;
	array[x1][y1]->ground->BlockOver(array[x1][y1]->block, array[x1][y1]->ground);
	return true;
}

bool Board::MoveRight(Tile* p)
{
	SDL_Point pos = p->getXY();
	if (p->moved) return false;
	Tile* block = NULL; RightBlock(pos, &block);
	if (block && block->CanMove()) MoveRight(block);
	bool ret =swap(pos.x, pos.y, pos.x+1, pos.y);
	if (ret&&p->GetBlockType()!=BlockType::TANK) Sound::PlayASound("scrape.wav", SoundPriority::SCRAPE);
	return p->moved=ret;
}

bool Board::MoveLeft(Tile* p)
{
	SDL_Point pos = p->getXY();
	if (p->moved) return false;
	Tile* block = NULL; LeftBlock(pos, &block);
	if (block && block->CanMove()) MoveLeft(block);
	bool ret =swap(pos.x, pos.y, pos.x-1, pos.y);
	if (ret&&p->GetBlockType()!=BlockType::TANK) Sound::PlayASound("scrape.wav", SoundPriority::SCRAPE);
	return p->moved=ret;
}

bool Board::MoveUp(Tile* p)
{
	SDL_Point pos = p->getXY();
	if (p->moved) return false;
	Tile* block = NULL; TopBlock(pos, &block);
	if (block && block->CanMove()) MoveUp(block);
	bool ret =swap(pos.x, pos.y, pos.x, pos.y-1);
	if (ret&&p->GetBlockType()!=BlockType::TANK) Sound::PlayASound("scrape.wav", SoundPriority::SCRAPE);
	return p->moved=ret;
}

bool Board::MoveDown(Tile* p)
{
	SDL_Point pos = p->getXY();
	if (p->moved) return false;
	Tile* block = NULL; BottomBlock(pos, &block);
	if (block && block->CanMove()) MoveDown(block);
	bool ret =swap(pos.x, pos.y, pos.x, pos.y+1);
	if (ret&&p->GetBlockType()!=BlockType::TANK) Sound::PlayASound("scrape.wav", SoundPriority::SCRAPE);
	return p->moved=ret;
}

bool Board::RightBlock(SDL_Point pos, Tile** p)
{
	if (pos.x>=COLUMNS-1) {p=NULL; return false;}
	*p=array[pos.x+1][pos.y]->block;
	return true;
}

bool Board::LeftBlock(SDL_Point pos, Tile** p)
{
	if (pos.x<=0) {p=NULL; return false;}
	*p=array[pos.x-1][pos.y]->block;
	return true;
}

bool Board::TopBlock(SDL_Point pos, Tile** p)
{
	if (pos.y<=0) {p=NULL; return false;}
	*p=array[pos.x][pos.y-1]->block;
	return true;
}

bool Board::BottomBlock(SDL_Point pos, Tile** p)
{
	if (pos.y>=ROWS-1) {p=NULL; return false;}
	*p=array[pos.x][pos.y+1]->block;
	return true;
}

bool Board::RightGround(SDL_Point pos, Tile** p)
{
	if (pos.x>=COLUMNS-1) {p=NULL; return false;}
	*p=array[pos.x+1][pos.y]->ground;
	return true;
}

bool Board::LeftGround(SDL_Point pos, Tile** p)
{
	if (pos.x<=0) {p=NULL; return false;}
	*p=array[pos.x-1][pos.y]->ground;
	return true;
}

bool Board::TopGround(SDL_Point pos, Tile** p)
{
	if (pos.y<=0) {p=NULL; return false;}
	*p=array[pos.x][pos.y-1]->ground;
	return true;
}

bool Board::BottomGround(SDL_Point pos, Tile** p)
{
	if (pos.y>=ROWS-1) {p=NULL; return false;}
	*p=array[pos.x][pos.y+1]->ground;
	return true;
}

bool Board::ShootUp(Tile* p)
{
	SDL_Point pos = p->getXY();
	p->SetRotation(1);
	Tile* ground=array[pos.x][pos.y]->ground;
	if (ground) ground->HadFired=ground->HadFiredUp=BEAM_PERSISTANCE;
	p->Firing=p->FiringUp=true;
	p->HadFired=p->HadFiredUp=BEAM_PERSISTANCE;
	Tile* block = NULL; TopBlock(pos, &block);
	if (block) block->HitBottom();
	else {
		TopGround(pos, &block);
		if (block) block->HitBottom();
	}
	return true;
}

bool Board::ShootDown(Tile* p)
{
	SDL_Point pos = p->getXY();
	p->SetRotation(3);
	Tile* ground=array[pos.x][pos.y]->ground;
	if (ground) ground->HadFired=ground->HadFiredDown=BEAM_PERSISTANCE;
	p->Firing=p->FiringDown=true;
	p->HadFired=p->HadFiredDown=BEAM_PERSISTANCE;
	Tile* block = NULL; BottomBlock(pos, &block);
	if (block) block->HitTop();
	else {
		BottomGround(pos, &block);
		if (block) block->HitTop();
	}
	return true;
}

bool Board::ShootRight(Tile* p)
{
	SDL_Point pos = p->getXY();
	p->SetRotation(2);
	Tile* ground=array[pos.x][pos.y]->ground;
	if (ground) ground->HadFired=ground->HadFiredRight=BEAM_PERSISTANCE;
	p->Firing=p->FiringRight=true;
	p->HadFired=p->HadFiredRight=BEAM_PERSISTANCE;
	Tile* block; RightBlock(pos, &block);
	if (block) block->HitLeft();
	else {
		RightGround(pos, &block);
		if (block) block->HitLeft();
	}
	return true;
}

bool Board::ShootLeft(Tile* p)
{
	SDL_Point pos = p->getXY();
	p->SetRotation(0);
	Tile* ground=array[pos.x][pos.y]->ground;
	if (ground) ground->HadFired=ground->HadFiredLeft=BEAM_PERSISTANCE;
	p->Firing=p->FiringLeft=true;
	p->HadFired=p->HadFiredLeft=BEAM_PERSISTANCE;
	Tile* block; LeftBlock(pos, &block);
	if (block) block->HitRight();
	else {
		LeftGround(pos, &block);
		if (block) block->HitRight();
	}
	return true;
}

// bool Board::SeeMeUp(BlockType type, int dist)
// {
// 	Tile* block = NULL; TopBlock(block);
// 	if (block) block->LookBottom(type, dist);
// 	else {
// 		TopGround(block);
// 		if (block) block->LookBottom(type, dist);
// 	}
// 	return true;
// }

// bool Board::SeeMeDown(BlockType type, int dist)
// {
// 	Tile* block = NULL; BottomBlock(block);
// 	if (block) block->LookTop(type, dist);
// 	else {
// 		BottomGround(block);
// 		if (block) block->LookTop(type, dist);
// 	}
// 	return true;
// }
// bool Board::SeeMeRight(BlockType type, int dist)
// {
// 	Tile* block = NULL; RightBlock(block);
// 	if (block) block->LookLeft(type, dist);
// 	else {
// 		RightGround(block);
// 		if (block) block->LookLeft(type, dist);
// 	}
// 	return true;
// }
// bool Board::SeeMeLeft(BlockType type, int dist)
// {
// 	Tile* block = NULL; LeftBlock(block);
// 	if (block) block->LookRight(type, dist);
// 	else {
// 		LeftGround(block);
// 		if (block) block->LookRight(type, dist);
// 	}
// 	return true;
// }

// bool Board::Kill(Tile *p) {
// 	if (!board.array[x_pos][y_pos]->block) return true;
// 	Tile* temp=board.array[x_pos][y_pos]->block;
// 	board.array[x_pos][y_pos]->block=NULL;
// 	//Schedule for deletion
// 	board.array[x_pos][y_pos]->ground->AddDead(this);
// 	return true;
// }

void Board::Animate(SDL_Renderer * renderer) {
	Delay();
	SetGroundTypes(); //Make ground tiles all line up nice

	if (!array) return;
	for (int i=0; i<ROWS; i++) {
		for (int j=0; j<COLUMNS; j++) {
			if (!array[j][i]) continue;
			if (array[j][i]->ground) {array[j][i]->ground->Update(); array[j][i]->ground->draw(renderer);};
			if (array[j][i]->block) {array[j][i]->block->Update(); array[j][i]->block->draw(renderer);};
			if (array[j][i]->over) {array[j][i]->over->Update(); array[j][i]->over->draw(renderer);};
		}
	}
	DisplayHelp(renderer);
	Draw::Flip(renderer);
	AfterAnimate();
}

void Board::SetGroundTypes()
{
	if (GroundTile::SetChanged(0)) return;
	static unsigned int groundArray[COLUMNS+2][ROWS+2];
	for (int i=0; i<COLUMNS+2; i++) groundArray[i][0]=1;
	for (int i=0; i<COLUMNS+2; i++) groundArray[i][ROWS+1]=1;
	for (int i=0; i<ROWS+2   ; i++) groundArray[COLUMNS+1][i]=1;
	for (int i=0; i<ROWS+2   ; i++) groundArray[0][i]=1;

	int r;

	for (int y=1; y<ROWS+1; y++) {
		for (int x=1; x<COLUMNS+1; x++) {
			if (array[x-1][y-1]->ground->GetBlockType()==BlockType::GROUND) {
				r=array[x-1][y-1]->ground->GetRotation();
				
				groundArray[x][y]=r;
			}
			else groundArray[x][y]=0x0u;
		}
	}
	for (int y=1; y<ROWS+1; y++) {
		for (int x=1; x<COLUMNS+1; x++) {
			r= groundArray[x][y]%100;
			if (r>=30&&r<=33) array[x-1][y-1]->ground->SetOtherBlocks(GetGroundBits(groundArray, x, y));
		}
	}
}

unsigned int Board::GetGroundBits(unsigned int a[COLUMNS+2][ROWS+2], int x, int y)
{
	unsigned int bits[8];
	bits[0]=(a[x-1][y]?1u:0u);
	bits[1]=(a[x-1][y-1]?1u:0u);
	bits[2]=(a[x][y-1]?1u:0u);
	bits[3]=(a[x+1][y-1]?1u:0u);
	bits[4]=(a[x+1][y]?1u:0u);
	bits[5]=(a[x+1][y+1]?1u:0u);
	bits[6]=(a[x][y+1]?1u:0u);
	bits[7]=(a[x-1][y+1]?1u:0u);

	unsigned int allbits = 0x0u;
	for (unsigned int i=0; i<8; i++)
		allbits |= (bits[i]<<i);
	return allbits;
}

void Board::FillDefault()
{
	defeated=0;
	finished=1;
	for (int i=0; i<ROWS; i++) {
		for (int j=0; j<COLUMNS; j++) {
				groundTypeArray[j][i]=BlockType::GROUND;
				groundRotationArray[j][i]=33;
				blockTypeArray[j][i]=BlockType::NONE;
				blockRotationArray[j][i]=0;
		}
	}
	blockTypeArray[1][1]=BlockType::TANK;
	blockRotationArray[1][1]=2;
	tank_x=tank_y=origin_x=origin_y=1;
	FillArray(true);
	Credits();
}

void Board::MoveForCredits() {
	static int chop=0;
	chop++;
	chop%=4;
	if (!chop) {
		if (tank_x==2&& tank_y>1) Up();
		else if (tank_x==COLUMNS-3 && tank_y< ROWS-2) Down();
		else if (tank_y==1&& tank_x< COLUMNS-3) Right();
		else if (tank_y==ROWS-2 && tank_x> 2) Left();
	}
}

void Board::Delay()
{
	float current_time=(float(clock()))/(float(CLOCKS_PER_SEC));
	float difference=current_time-seconds_ago;
	if (difference < .025) {
		int sleep = long((.025 - difference)*1000);
		if (sleep>25) sleep=25;
		SDL_Delay(sleep);
	}
	seconds_ago= current_time;
}

void Board::DisplayHelp(SDL_Renderer * renderer)
{
	if (!help) return;
	Draw::BlitOther(renderer, textures->getMainSprite(), 0, 0, 120, 40, 560, 520);
}
