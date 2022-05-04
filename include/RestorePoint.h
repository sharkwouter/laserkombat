#ifndef RESTOREPOINT_H
#define RESTOREPOINT_H

#include "constants.h"
#include "BlockType.h"

struct RestorePoint {
	BlockType groundTypeArray[COLUMNS][ROWS];
	BlockType blockTypeArray[COLUMNS][ROWS];

	int groundRotationArray[COLUMNS][ROWS];
	int blockRotationArray[COLUMNS][ROWS];
};

#endif // RESTOREPOINT_H
