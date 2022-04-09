#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include "constants.h"
#include "Input.h"
#include "Textures.h"
#include "Sound.h"
#include "BlockType.h"
#include "Tile.h"
#include "Square.h"

class Board {
public:
	Board(Textures * textures);
	~Board();


	int DecreaseEnemyCount() {--number_of_enemies; return number_of_enemies;}
	void GetTankXY(int &x, int& y) {x=tank_x; y=tank_y;}

	void handleInput(std::vector<Input> input);
	void NextLevel() {LoadLevel();}


	void Restart() {
		FillArray();
	}

	void Animate(SDL_Renderer * renderer);		//Update tiles

	void See() {
		for (int i=0; i<ROWS; i++) {
			for (int j=0; j<COLUMNS; j++) {
				if (array[j][i]&&array[j][i]->block) array[j][i]->block->PreSeeMe();
			}
		}
	}

	bool IsOver() {return died||defeated;}

	void AfterAnimate() {	//called to change things that should be changed only after redraw
		Sound::Play();	//play the queued sound
		for (int i=0; i<ROWS; i++) {
			for (int j=0; j<COLUMNS; j++) {
				if (array[j][i]&&array[j][i]->over) array[j][i]->over->AfterAnimate();
			}
		}

		if (finished) MoveForCredits();
		Water::IncStaticRotation(); //all water moves synchronously.
		if (!IsOver()) See(); //uncomment this if see changes need to be made 
		if (number_of_enemies<1) YouDefeated();
	}


	bool IsYou(int x, int y) {
		if (x==tank_x&&y==tank_y) return true;
		return false;
	}

	bool CheckInRange(int x, int y) {
		if (x>=COLUMNS||x<0) return false;
		if (y>=ROWS||y<0) return false;
		return true;
	}


	bool TankExists() {
		bool ret;
		if (!CheckInRange(tank_x, tank_y)) ret= false;
		else ret= ((array[tank_x][tank_y]->block)?true:false);

		if (!ret) YouDied();
		return ret;
	}

	void Right() {	//user presses right arrow, ect.
		if (!TankExists()) return;
		if (array[tank_x][tank_y]->block->GetRotation()==2)
		{MoveRight(array[tank_x][tank_y]->block); See();}
		else array[tank_x][tank_y]->block->SetRotation(2);
		TankExists();
	}
	void Left() {
		if (!TankExists()) return;
		if (array[tank_x][tank_y]->block->GetRotation()==0)
		{MoveLeft(array[tank_x][tank_y]->block), See();}
		else array[tank_x][tank_y]->block->SetRotation(0);
		TankExists();
	}
	void Up() {
		if (!TankExists()) return;
		if (array[tank_x][tank_y]->block->GetRotation()==1)
		{MoveUp(array[tank_x][tank_y]->block); See();}
		else array[tank_x][tank_y]->block->SetRotation(1);
		TankExists();
	}
	void Down() {
		if (!TankExists()) return;
		if (array[tank_x][tank_y]->block->GetRotation()==3)
		{MoveDown(array[tank_x][tank_y]->block); See();}
		else array[tank_x][tank_y]->block->SetRotation(3);
		TankExists();
	}
	void Fire() {         //user presses fire button
		if (!TankExists()) return;
		int rotation= array[tank_x][tank_y]->block->GetRotation();
		Tile* tank=array[tank_x][tank_y]->block;
		switch(rotation) {
		case 0:
			ShootLeft(tank);
			break;
		case 1:
			ShootUp(tank);
			break;
		case 2:
			ShootRight(tank);
			break;
		case 3:
			ShootDown(tank);
			break;
		default:
			break;
		}
		See();
		TankExists();
	}

	bool Previous();

private:
	void MoveForCredits();

	void Delay();
	bool LoadLevel();
	void FillDefault();
	void CreateSquare(int x, int y);

	void YouDied();
	void YouDefeated();
	void Credits();

	void SetGroundTypes();
	unsigned int GetGroundBits(unsigned a[COLUMNS+2][ROWS+2], int x, int y);
	bool swap(int x, int y, int x1, int y1);

	void FillArray(bool credits=false);
	void ClearArray();
	void CheckArray();

	bool MoveUp(Tile* p);
	bool MoveDown(Tile* p);
	bool MoveRight(Tile* p);
	bool MoveLeft(Tile* p);

	bool RightBlock(SDL_Point pos, Tile** p);
	bool LeftBlock(SDL_Point pos, Tile** p);
	bool TopBlock(SDL_Point pos, Tile** p);
	bool BottomBlock(SDL_Point pos, Tile** p);

	bool RightGround(SDL_Point pos, Tile** p);
	bool LeftGround(SDL_Point pos, Tile** p);
	bool TopGround(SDL_Point pos, Tile** p);
	bool BottomGround(SDL_Point pos, Tile** p);

	bool ShootUp(Tile* p);
	bool ShootDown(Tile* p);
	bool ShootRight(Tile* p);
	bool ShootLeft(Tile* p);

	void DisplayHelp(SDL_Renderer * renderer);

private: //data

	bool help;
	bool died;
	bool defeated;
	bool finished;
	bool changed;

	Textures * textures;

	Square* array[COLUMNS][ROWS];
	int rows, cols;
	int tank_x, tank_y, origin_x, origin_y;
	int level;

	BlockType groundTypeArray[COLUMNS][ROWS];
	BlockType blockTypeArray[COLUMNS][ROWS];

	int groundRotationArray[COLUMNS][ROWS];
	int blockRotationArray[COLUMNS][ROWS];

	int number_of_enemies;
	float seconds_ago;
};

#endif // BOARD_H
