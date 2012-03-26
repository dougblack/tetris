// video.c

#include "video.h"

u16 *videoBuffer = (u16*) 0x6000000;

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
void drawImage3(int r, int c, int width, int height, const u16* image)
{
	int rowWidth = width;
	for (int i = 0; i < height; i++) {
		const u16* row = image+i*rowWidth;
		u16 *dest = videoBuffer+((r+i)*240+c);
		dma_memcpy(dest, row, rowWidth);
	}
}
