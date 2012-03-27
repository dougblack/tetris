// mylib.c

#include "mylib.h"

int clearedLines;
int level;
int matrix[26][10];
u16 colorMatrix[26][10];
int __qran_seed = 10;

// A simple dummy for-loop to "pause" execution
void sleep(int length) {
    for (int i = 0; i < length; i++) {}
}

int sqran(int seed)
{
	int old = __qran_seed;
	__qran_seed = seed;
	return old;
}

int qran()
{
	__qran_seed = 1664525 * __qran_seed+1013904223;
	return (__qran_seed >> 16) & 0x7FFF;
}

int qran_range(int min, int max)
{
	return (qran() * (max-min)>>15) + min;
}

int checkBoundBottom(tetrimino key) {
	for (int x = 0; x<4; x++) {	
		for (int y = 0; y<4; y++) {
			if ((key.t[4*x+y] == 1) && 
				((key.r+x+1 > 25) || 
					((key.t[4*(x+1) + y] == 0) && (matrix[key.r+x+1][key.c+y] == 1))))
				return 1;
		}
	}
		return 0;
}

int checkBoundRight(tetrimino key) {
	for (int x=0; x<4; x++) {
		for (int y=0; y<4; y++) {
			if ((key.t[4*x+y] == 1) && 
				((key.c+y+1 > 9) || ((key.t[4*x+y+1]==0) && (matrix[key.r+x][key.c+y+1]==1))))
				return 1;
		}
	}
	return 0;
}


int checkBoundLeft(tetrimino key) {
	for (int x=0; x<4; x++) {
		for (int y=0; y<4; y++) {
			if ((key.t[4*x+y] == 1) && 
				((key.c+y-1 < 0) || ((key.t[4*x+y-1]==0) && (matrix[key.r+x][key.c+y-1]==1))))
				return 1; }
	}
	return 0;
}

void checkForScore() {
//	DEBUG_PRINT("CHECKING FOR SCORE");
	for (int i = 4; i < 26; i++) {
		for (int j = 0; j < 10; j++) {
			if (matrix[i][j] == 0)  {
				break;
			}
			else if(j==9)  {
				clearRow(i);
//				DEBUG_PRINTF("Row: %d cleared", i);
			}
		}
//		DEBUG_PRINTF("Row: %d checked", i);
	}
}

void clearRow(int row) {
	int tempMatrix[26][10];
	//Store offset matrix to tempMatrix.
	for (int i = 25; i > 3; i--) {
		if (i > row) {
			for (int j = 0; j<10; j++) {
				tempMatrix[i][j] = matrix[i][j];
			}
		} else {
			for (int j = 0; j<10; j++) {
				tempMatrix[i][j] = matrix[i-1][j];
			}
		}
	}
	// Copy matrix back
	for (int i = 4; i < 26; i++) {
		for (int j = 0; j < 10; j++) {
			if (i == 0)
				matrix[i][j] = 0;
			else
				matrix[i][j] = tempMatrix[i][j];	
		}
	}
	incrementLines();
}

void incrementLines() {
	clearedLines++;
	if (clearedLines % 5) {
		level++;
		fallSpeed-=5;
	}
}
