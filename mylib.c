// mylib.c
#include "mylib.h"
#include "green.h"
#include "black.h"

u16 *videoBuffer = (u16*) 0x6000000;
int matrix[26][10];
int clearedLines;
int level;
u16 colorMatrix[26][10];
int __qran_seed = 10;
/* FUNCTION DECLARATIONS */

// Sets the pixel at row r, column c to color
void setPixel(int r, int c, u16 color)
{
   videoBuffer[(r)*(240)+(c)] = color; 
}

// Draws a rectange starting at top left corner at (r,c) with
// the specified width and height and color
void drawRect(int r, int c, int width, int height, u16 color)
{
    for (int i = r; i < (r+width); i++) {
        for (int j = c; j < (c+height); j++) {
            setPixel(i,j,color);
        }
    }
}

// A simple dummy for-loop to "pause" execution
void sleep(int length) {
    for (int i = 0; i < length; i++) {}
}

void drawTetrimino(tetrimino key) 
{
	for (int x=0;x<4;x++) {
		for (int y=0;y<4;y++) {
			if (key.t[x*4+y] == 1 && (key.r+x>3)) {
//				drawRect(15+(key.r+x-4)*6, 90+(key.c+y)*6, 4,4, key.color);
				drawImage3(15+(key.r+x-4)*6, 90+(key.c+y)*6, 5, 5, green);
			}
		}
	}
}

void drawMatrix()
{
	for (int x = 4; x < 26; x++) {
		for (int y = 0; y < 10; y++) {
			if (matrix[x][y]==1)
				drawRect(15+(x-4)*6, 90+y*6, 5, 5, colorMatrix[x][y]); 
			else 
				drawRect(15+(x-4)*6, 90+y*6, 6, 6, BLACK);
		}
	}
}

void clearTetrimino(int r, int c, int *t)
{
	for (int x=0;x<4;x++) {
		for (int y=0;y<4;y++) {
			if (t[x*4+y] == 1 && (r+x>3)) {
//				drawRect(15+(r+x-4)*6, 90+(c+y)*6, 5, 5, BLACK);
				drawImage3(15+(r+x-4)*6, 90+(c+y)*6, 5, 5, black);
			}
		}
	}
}

void rotateLeft(tetrimino key) 
{
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			int save = key.t[4*x+y];
			key.t[4*x+y] = key.t[4*y+x];
			key.t[4*y+x] = save;
		}
	}
}


void rotateRight(tetrimino key)
{ 
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			int save = key.t[4*y+x];
			key.t[4*y+x] = key.t[4*x+y];
			key.t[4*x+y] = save;
		}
	}
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
				return 1;
		}
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

void dma_memcpy(void *dst, const void *src, u16 count)
{
	dma_mem[3].cnt = 0;
	dma_mem[3].src = src;
	dma_mem[3].dst = dst;
	dma_mem[3].cnt = count | DMA_ON;
}

void drawImage3(int r, int c, int width, int height, const u16* image)
{
	int rowWidth = width;
	for (int i = 0; i < height; i++) {
		const u16* row = image+i*rowWidth;
		u16 *dest = videoBuffer+((r+i)*240+c);
		dma_memcpy(dest, row, rowWidth);
	}
}
