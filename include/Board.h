#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include "constants.h"
#include "Input.h"
#include "Textures.h"
#include "Sound.h"
#include "Sound.h"
#include "BlockType.h"
#include "Tile.h"
#include "Square.h"

class Board {
public:
	Board(Textures * textures, Sound * sound);
	~Board();


	int DecreaseEnemyCount() {--number_of_enemies; return number_of_enemies;}
	void GetTankXY(int &x, int& y) {x=tank_x; y=tank_y;}

	int AnyKey(Input key);
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
		sound->Play();	//play the queued sound
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
		{MoveRight(); See();}
		else array[tank_x][tank_y]->block->SetRotation(2);
		TankExists();
	}
	void Left() {
		if (!TankExists()) return;
		if (array[tank_x][tank_y]->block->GetRotation()==0)
			MoveLeft(), See();
		else array[tank_x][tank_y]->block->SetRotation(0);
		TankExists();
	}
	void Up() {
		if (!TankExists()) return;
		if (array[tank_x][tank_y]->block->GetRotation()==1)
		{MoveUp(); See();}
		else array[tank_x][tank_y]->block->SetRotation(1);
		TankExists();
	}
	void Down() {
		if (!TankExists()) return;
		if (array[tank_x][tank_y]->block->GetRotation()==3)
		{MoveDown(); See();}
		else array[tank_x][tank_y]->block->SetRotation(3);
		TankExists();
	}
	void Fire() {         //user presses fire button
		if (!TankExists()) return;
		int rotation= array[tank_x][tank_y]->block->GetRotation();
		Tile* tank=array[tank_x][tank_y]->block;
		switch(rotation) {
		case 0:
			tank->ShootLeft();
			break;
		case 1:
			tank->ShootUp();
			break;
		case 2:
			tank->ShootRight();
			break;
		case 3:
			tank->ShootDown();
			break;
		default:
			break;
		}
		See();
		TankExists();
	}

	bool Previous();
	bool swap(int x, int y, int x1, int y1);

public:
	Square* array[COLUMNS][ROWS];

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

	void FillArray(bool credits=false);
	void ClearArray();
	void CheckArray();

	bool MoveUp();
	bool MoveDown();
	bool MoveRight();
	bool MoveLeft();

	void DisplayHelp(SDL_Renderer * renderer);

private: //data

	bool help;
	bool died;
	bool defeated;
	bool finished;

	Textures * textures;
	Sound * sound;

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
