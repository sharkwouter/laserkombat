#include "Board.h"

#include <SDL.h>

#include "utils.h"

Board::Board(Draw * draw, Textures * textures, Sound * sound) : help_keys(true), help_blocks(false), rows(ROWS), cols(COLUMNS), tank_x(0), tank_y(0), level(0), draw(draw), textures(textures), sound(sound) {
	CreateArray();
	LoadLevel();	
}

Board::~Board() {
	ClearArray();
}

void Board::FillArray(bool credits, RestorePoint * rp)
{
	ClearArray();
	if (!credits) {
		number_of_enemies=0;
		died=defeated=finished=0;
		if(rp) {
			tank_x=rp->tank_x;
			tank_y=rp->tank_y;
		} else {
			tank_x=origin_x;
			tank_y=origin_y;
		}
	}
	GroundTile::SetChanged(1);


	for (int y=0; y<ROWS; y++) {
		for (int x=0; x<COLUMNS; x++) {
			CreateSquare(x, y, rp);
		}
	}
	if (!credits) {
		if (number_of_enemies<1) YouDefeated();
	}
}

void Board::CreateSquare(int x, int y, RestorePoint * rp)
{
	if (array[x][y]) return;
	Tile* ground=NULL;
	Tile* block=NULL;
	BlockType type;
	int r;
	if(rp) {
		type = rp->groundTypeArray[x][y];
		r = rp->groundRotationArray[x][y];
	} else {
		type = groundTypeArray[x][y];
		r = groundRotationArray[x][y];
	}

	switch(type) {
	default:
	case BlockType::GROUND:		ground=	new GroundTile(x, y, draw, textures, sound, this, r); break;
	case BlockType::WATER:			ground=	new Water(x, y, draw, textures, sound, this, r); break;
	}

	if(rp) {
		type = rp->blockTypeArray[x][y];
		r = rp->blockRotationArray[x][y];
	} else {
		type = blockTypeArray[x][y];
		r = blockRotationArray[x][y];
	}

	switch(type) {
	default:			            block= NULL; break;
	case BlockType::REDBLOCK:		block=	new RedBlock(x, y, draw, textures, sound, this, r); break;
	case BlockType::RUSTYREDBLOCK:	block=	new RustyRedBlock(x, y, draw, textures, sound, this, r); break;
	case BlockType::WHITEBLOCK:	    block=	new WhiteBlock(x, y, draw, textures, sound, this, r); break;
	case BlockType::TEE:			block=  new Tee(x, y, draw, textures, sound, this, r); break;
	case BlockType::MIRROR:		    block=	new Mirror(x, y, draw, textures, sound, this, r); break;
	case BlockType::NUKE:			block=	new Nuke(x, y, draw, textures, sound, this, r); break;
	case BlockType::ENEMYNUKE:		block=	new EnemyNuke(x, y, draw, textures, sound, this, r); break;
	case BlockType::TANK:			block=	new Tank(x, y, draw, textures, sound, this, r); break;
	case BlockType::ENEMYTANK:		block=	new EnemyTank(x, y, draw, textures, sound, this, r); number_of_enemies++; break;
	case BlockType::STATIC:		    block=	new Static(x, y, draw, textures, sound, this, r); break;
	case BlockType::RUSTY:			block=	new Rusty(x, y, draw, textures, sound, this, r); break;
	case BlockType::BARSVERT:		block=	new BarsVert(x, y, draw, textures, sound, this, r); break;
	case BlockType::BARSHORIZ:		block=	new BarsHoriz(x, y, draw, textures, sound, this, r); break;
	case BlockType::BARSCROSS:		block=	new BarsCross(x, y, draw, textures, sound, this, r); break;
	case BlockType::TRIANGLE:		block=	new Triangle(x, y, draw, textures, sound, this, r); break;
	case BlockType::RUSTYTRIANGLE:	block=	new RustyTriangle(x, y, draw, textures, sound, this, r); break;
	case BlockType::RUSTYBARSVERT:	block=	new RustyBarsVert(x, y, draw, textures, sound, this, r); break;
	case BlockType::RUSTYBARSHORIZ: block=	new RustyBarsHoriz(x, y, draw, textures, sound, this, r); break;
	case BlockType::RUSTYWHITEBLOCK:block= new RustyWhiteBlock(x, y, draw, textures, sound, this, r); break;
	}

	if (!ground) ground= new GroundTile(x, y, draw, textures, sound, this, 33);
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
	undo_list.clear();

	std::string filename = "level";
	filename += std::string(3 - std::min(3, int(std::to_string(level).length())), '0') + std::to_string(level);
	filename += ".lvl";
	std::string path = getAssetPath("levels", filename);

	FILE *file=fopen(path.c_str(), "rb");
	if (file) {
		info = true;
		int size= fread( &origin_x, sizeof(origin_y), 1, file);
		if (size) size= fread( &origin_y, sizeof(origin_x), 1, file);
		origin_x%=COLUMNS;
		origin_y%=ROWS;

		if (size) size= fread(groundTypeArray, sizeof(BlockType), COLUMNS*ROWS, file);
		if (size==COLUMNS*ROWS) size= fread(groundRotationArray, sizeof(int), COLUMNS*ROWS, file);
		if (size==COLUMNS*ROWS) size= fread(blockTypeArray, sizeof(BlockType), COLUMNS*ROWS, file);
		if (size==COLUMNS*ROWS) size= fread(blockRotationArray, sizeof(int), COLUMNS*ROWS, file);
		if (size==COLUMNS*ROWS) {
			int duplicateObjectArray[COLUMNS][ROWS]; // This array has 1 for each water tile and numbers for object. No idea what for
			size= fread(duplicateObjectArray, sizeof(int), COLUMNS*ROWS, file);
		}
		if (size==COLUMNS*ROWS) {
			fread(&description, sizeof(description), 1, file);
			fread(&author, sizeof(author), 1, file);
		}
		
		fclose(file);
		FillArray(false);
		return (size==COLUMNS*ROWS);

	}
	else FillDefault();
	return false;
}

void Board::Save() {
	SaveToRestorePoint(&restorePoint);
}

void Board::Undo() {
	if(!undo_list.empty()) {
		RestorePoint rp = undo_list.back();
		FillArray(false, &rp);
		undo_list.pop_back();
	}
}

void Board::UpdateUndoList() {
	RestorePoint rp;
	SaveToRestorePoint(&rp);
	undo_list.push_back(rp);
}

void Board::SaveToRestorePoint(RestorePoint * rp) {
	for (int x=0; x<COLUMNS; x++) {
		for (int y=0; y<ROWS; y++) {
			if (array[x][y]->ground) {
				rp->groundTypeArray[x][y] = array[x][y]->ground->GetBlockType();
				rp->groundRotationArray[x][y] = array[x][y]->ground->GetRotation();
			} else {
				rp->groundTypeArray[x][y] = BlockType::NONE;
				rp->groundRotationArray[x][y] = 33;
			}
			if (array[x][y]->block) {
				rp->blockTypeArray[x][y] = array[x][y]->block->GetBlockType();
				rp->blockRotationArray[x][y] = array[x][y]->block->GetRotation();
			} else {
				rp->blockTypeArray[x][y] = BlockType::NONE;
				rp->blockRotationArray[x][y] = 0;
			}
		}
	}
	rp->tank_x = tank_x;
	rp->tank_y = tank_y;
}

void Board::Restore() {
	FillArray(false, &restorePoint);
}

void Board::YouDied() {
	if (died) return;
	for (int y=ROWS/2-2, yy=0; y<ROWS/2+1; y++, yy++) {
		for (int x=COLUMNS/2-3, xx=0; x<COLUMNS/2+3; x++, xx++) {
			Tile* temp=array[x][y]->over;
			array[x][y]->over=new Message(x,y,draw,textures,sound,this,yy*14+xx);
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
			array[x][y]->over=new Message(x,y,draw,textures,sound,this, yy*14+xx);
			if (temp) array[x][y]->ground->AddDead(temp);
		}
	}
	defeated=true;
}

void Board::Credits() {
	for (int y=ROWS/2-5, yy=3; y<ROWS/2+6; y++, yy++) {
		for (int x=COLUMNS/2-7, xx=0; x<COLUMNS/2+7; x++, xx++) {
			Tile* temp=array[x][y]->over;
			array[x][y]->over=new Message(x,y,draw,textures,sound,this,yy*14+xx);
			if (temp) array[x][y]->ground->AddDead(temp);
		}
	}
	finished=true;
}

void Board::ClearArray()
{
	for (int i=0; i<ROWS; i++) {
		for (int j=0; j<COLUMNS; j++) {
			if (array[j][i]) delete array[j][i];
			array[j][i]=NULL;
		}
	}
}

void Board::CreateArray()
{
	for (int i=0; i<ROWS; i++) {
		for (int j=0; j<COLUMNS; j++) {
			array[j][i]=NULL;
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

bool Board::MoveRight()
{
	return array[tank_x][tank_y]->block->PushLeft();
}

bool Board::MoveLeft()
{
	return array[tank_x][tank_y]->block->PushRight();

}

bool Board::MoveUp()
{
	return array[tank_x][tank_y]->block->PushBottom();
}

bool Board::MoveDown()
{
	return array[tank_x][tank_y]->block->PushTop();
}

void Board::Animate() {
	Delay();
	SetGroundTypes(); //Make ground tiles all line up nice

	for (int i=0; i<ROWS; i++) {
		for (int j=0; j<COLUMNS; j++) {
			if (!array[j][i]) continue;
			if (array[j][i]->ground) {array[j][i]->ground->Update();};
			if (array[j][i]->block) {array[j][i]->block->Update();};
			if (array[j][i]->over) {array[j][i]->over->Update();};
		}
	}
	DisplayKeysHelp();
	DisplayBlocksHelp();
	DisplayLevelInfo();
	draw->Flip();
	AfterAnimate();
}

void Board::SetGroundTypes()
{
	if (!GroundTile::SetChanged(0)) return;
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

int Board::AnyKey(Input key) {
	if (key==Input::HELPKEYS&&!help_keys) {help_keys=true;help_blocks=false; return 0;}
	if (key==Input::HELPBLOCKS&&!help_blocks) {help_blocks=true;help_keys=false; return 0;}
	if (help_keys) {help_keys=false; return 0;}
	if (help_blocks) {help_blocks=false; return 0;}
	if (info&&!finished&&key!=Input::PREVIOUSLEVEL&&key!=Input::NEXTLEVEL) {info=false; return 0;}
	if (key==Input::EXIT) return 666;
	if (finished&&key==Input::RESTART) return 666;
	if (died&&key==Input::RESTART) {Restart(); return 0;}
	if (defeated&&(key==Input::RESTART)) {LoadLevel(); return 0;}
	else if (defeated) return 0;
	else if (finished&&key!=Input::PREVIOUSLEVEL) return 0;

	return 1;
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
	unsigned int current_time=SDL_GetTicks();
	unsigned int difference=current_time-seconds_ago;
	if (difference < 25) {
		int sleep = 25 - difference;
		if (sleep>25) sleep=25;
		SDL_Delay(sleep);
	}
	seconds_ago= current_time;
}

void Board::DisplayKeysHelp()
{
	if (!help_keys) return;
	draw->BlitOther(textures->getMainKeysSprite(), 0, 0, 120, 40, 560, 520);
}

void Board::DisplayBlocksHelp()
{
	if (!help_blocks) return;
	draw->BlitOther(textures->getMainBlocksSprite(), 0, 0, 120, 40, 560, 520);
}

void Board::DisplayLevelInfo()
{
	if (help_keys || help_blocks || finished ||!info) return;
	draw->BlitOther(textures->getLevelInfoSprite(), 0, 0, 240, 220, 320, 160);

	char level_string[4];
	sprintf(level_string, "%03d", level);
	for(int i=0; i<3; i++) {
		int number = level_string[i] - '0';
		draw->BlitOther(textures->getLevelInfoSprite(), number*17, 160, 240+171+i*16, 220+18, 16, 23);
	}

	draw->BlitText(description, 240+160, 220+59);
	draw->BlitText(author, 240+160, 220+114);
}
