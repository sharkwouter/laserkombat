#ifndef RESTOREPOINT_H
#define RESTOREPOINT_H

#include "constants.h"
#include "BlockType.h"

struct RestorePoint {
	BlockType groundTypeArray[COLUMNS][ROWS];
	BlockType blockTypeArray[COLUMNS][ROWS];

	int groundRotationArray[COLUMNS][ROWS];
	int blockRotationArray[COLUMNS][ROWS];

	int tank_x;
	int tank_y;
};

#endif // RESTOREPOINT_H
