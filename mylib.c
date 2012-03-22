// mylib.c
#include "mylib.h"

u16 *videoBuffer = (u16*) 0x6000000;
int matrix[22][10];
int __qran_seed = 10123;
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
    for (int i = r; i <= (r+width); i++) {
        for (int j = c; j <= (c+height); j++) {
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
			if (key.t[x*4+y] == 1)
				matrix[x+key.r][y+key.c] = 1;
		}
	}
}

void drawMatrix()
{
	for (int x = 0; x < 22; x++) {
		for (int y = 0; y < 10; y++) {
			if (matrix[x][y]==1)
				drawRect(20+x*6, 90+y*6, 6, 6, RED); 
			else 
				drawRect(20+x*6, 90+y*6, 6, 6, LIGHTBLUE);
		}
	}
}

void clearTetrimino(int r, int c, int *t)
{
	for (int x=0;x<4;x++) {
		for (int y=0;y<4;y++) {
			if (t[x*4+y] == 1)
				matrix[x+r][y+c] = 0;
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

