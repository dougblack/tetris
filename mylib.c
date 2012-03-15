// Douglas Black HW 8

#include "mylib.h"

u16 *videoBuffer = (u16*) 0x6000000;

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
	for (int x=0;x<4;x++)
	{
		for (int y=0;y<4;y++)
		{
			if (key.t[x*4+y] == 1)
			{
				drawRect(x*4+key.r, y*4+key.c, 4, 4, key.color);
			}
		}
	}
}

void clearTetrimino(tetrimino key )
{
	drawRect(key.r, key.c, 16, 16, BLACK);
}

