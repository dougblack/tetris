// mylib.h

#include "green.h"
#include "black.h"

typedef unsigned short u16;
typedef unsigned int u32;
extern int matrix[26][10];
extern u16 colorMatrix[26][10];
extern int clearedLines;
extern int level;
extern int fallSpeed;

typedef struct 
{
	int *t;
	u16 color;
	int r;
	int c;
} tetrimino;



/* FUNCTION PROTOTYPES */
void sleep(int length);
int checkBoundBottom(tetrimino key);
int checkBoundLeft(tetrimino key);
int checkBoundRight(tetrimino key);
void checkForScore();
void clearRow(int row);
void incrementLines();

int sqran(int seed);
int qran();
int qran_range(int min, int max);
