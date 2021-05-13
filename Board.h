// Board.h



class Board
{
	friend class Tile;
public:
	Board(int width=20, int height=15) : help(TRUE), cols(width), rows(height), tank_x(0), tank_y(0), level(0) {
		LoadLevel();
	};

	int DecreaseEnemyCount() {--number_of_enemies; return number_of_enemies;}
	void GetTankXY(int &x, int& y) {x=tank_x; y=tank_y;}

	~Board() {ClearArray();}

	int AnyKey(int key);
	void NextLevel() {LoadLevel();}


	void Restart() {
		FillArray();
	}


	void Animate();		//Update tiles

	void See() {
		for (int i=0; i<ROWS; i++) {
			for (int j=0; j<COLUMNS; j++) {
				if (array[j][i]&&array[j][i]->block) array[j][i]->block->PreSeeMe();
				//if (array[j][i]&&array[j][i]->over) array[j][i]->over->PreSeeMe();
			}
		}
	}
	BOOL IsOver() {return died||defeated;}

	void AfterAnimate() {	//called to change things that should be changed only after redraw
		sound.Play();	//play the queued sound
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


	BOOL IsYou(int x, int y) {
		if (x==tank_x&&y==tank_y) return TRUE;
		return FALSE;
	}

	BOOL CheckInRange(int x, int y) {
		if (x>=COLUMNS||x<0) return FALSE;
		if (y>=ROWS||y<0) return FALSE;
		return TRUE;
	}


	BOOL TankExists() {
		BOOL ret;
		if (!CheckInRange(tank_x, tank_y)) ret= FALSE;
		else ret= ((array[tank_x][tank_y]->block)?TRUE:FALSE);

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

	BOOL Previous();

	static void RestoreHelpSurface();


private:
	void MoveForCredits();

	void Delay();
	BOOL LoadLevel();
	void FillDefault();
	void CreateSquare(int x, int y);
	char digit_to_char(int digit);
	void number_to_string(char* string, int number, int digits=3);


	void YouDied();
	void YouDefeated();
	void Credits();

	void SetGroundTypes();
	unsigned int Board::GetGroundBits(unsigned a[COLUMNS+2][ROWS+2], int x, int y);
	BOOL swap(int x, int y, int x1, int y1);

	void FillArray(BOOL credits=FALSE);
	void ClearArray();
	void CheckArray();

	BOOL MoveUp();
	BOOL MoveDown();
	BOOL MoveRight();
	BOOL MoveLeft();

	void DisplayHelp();
	void SetHelpSurface();


private: //data

	static LPDIRECTDRAWSURFACE help_surface;

	BOOL help;
	BOOL died;
	BOOL defeated;
	BOOL finished;


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
