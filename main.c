// Douglas Black HW 8

#include "mylib.h"
#include "main.h"
#include "input.h"
#include <debugging.h>

int tetriminos[7][16] =							// Tetrimino matrix
{	{	0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0	},		// O 
	{	0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0	},		// I
	{	0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0	},		// L
	{	0,0,0,0,0,0,1,0,0,0,1,0,0,1,1,0	},		// J
	{	0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1	},		// T
	{	0,0,0,0,0,1,0,0,0,1,1,0,0,0,1,0	},		// S
	{	0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0	}	};	// Z

u16 colors[7] = {RED, BLUE, GREEN, YELLOW, MAGENTA, CYAN, WHITE}; // Tetrimino colors

int frame = 0;	// video frame
tetrimino key;	// initial tetrimino, this is the one that falls.
tetrimino next;	// next tetrimino to always be displayed top right of board
int keyLastR;
int keyLastC;
int *keyLastT;
int movedYet = 0;
int rotatedYet = 0;
int updateSpeed = 20;
int main() 
{ 
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	setupInterrupts();

	// Generate random tetrimino to start falling and
	// Generate random tetrimino to show as next tetrmino
	int *t;
	int tetType = qran_range(0,7);
	t = tetriminos[tetType];
	int nextType = qran_range(0,7);
	int *n;
    n = tetriminos[nextType];
	next.t = n;
	next.color = colors[nextType];
	setNextPiece(next);
	key.t = t;
	key.color = colors[tetType];
	key.r = 0;
	key.c = 5;
	keyLastR = 0;
	keyLastC = 5;
	keyLastT = key.t;
	drawMatrixBorders();
	DEBUG_PRINT("\n[DEBUG]\n");
	while(1); 

}

void setupInterrupts(void)
{
	REG_IME = 0x0; //disable interrupts
	REG_INTERRUPT = (u32)interruptHandler; //set int handler
	enableVBlankInterrupt();
	enableButtonInterrupt();
	REG_IME = 0x1; //enable interrupts
}

void interruptHandler()
{
	REG_IME = 0x0;	//	disable interrupts
	if ((REG_IF & INT_BUTTON) && (movedYet == 0) && (rotatedYet == 0))
	{
		printMatrix(key.t);
		if (KEY_DOWN_NOW(KEY_RIGHT))
			keyRight();
		else if (KEY_DOWN_NOW(KEY_LEFT))
			keyLeft();
		else if (KEY_DOWN_NOW(KEY_UP) && (rotatedYet == 0)) {
			rotatedYet = 1;
			keyRotateLeft();
		}
		else if (KEY_DOWN_NOW(KEY_DOWN))
			keySoftDrop();
		else if (KEY_DOWN_NOW(KEY_A))
			keyRotateRight();
		else if (KEY_DOWN_NOW(KEY_B))
			keyRotateLeft();
		else if (KEY_DOWN_NOW(KEY_SELECT))
			showMenu();
		else if (KEY_DOWN_NOW(KEY_START))
			pause();
	} 	
	if (REG_IF & INT_VB)
	{
		frame++;
		if ((frame % updateSpeed)==0) 
		{
			clearTetrimino(keyLastR, keyLastC, keyLastT);
			if (checkBoundBottom(key) != 1) {
				key.r = key.r + 1;
				drawTetrimino(key);
			} else {
				placeKey();
			}
			drawMatrix();
			keyLastR = key.r;
			keyLastC = key.c;
			keyLastT = key.t;
			// Move falling tetrimino down every second 
			movedYet = 0;
			rotatedYet = 0;
		}
	} 
	REG_IF = REG_IF;
	REG_IME = 0x1;
}

void enableVBlankInterrupt()
{
	REG_IE = REG_IE | INT_VB;      // Enable vblank interrupt detection
	REG_DISPSTAT |= INT_VBLANK_ENABLE; // Make display generate vblank interrupts
}

void enableButtonInterrupt()
{
	REG_IE = REG_IE | INT_BUTTON;	// Enable button interrupt detection
	REG_KEYCNT |= INT_BUTTON_ENABLE; // Make key input generate button interrupts
	REG_KEYCNT |= KEY_UP;
	REG_KEYCNT |= KEY_RIGHT;
	REG_KEYCNT |= KEY_LEFT;
	REG_KEYCNT |= KEY_DOWN;
	REG_KEYCNT |= KEY_A;
	REG_KEYCNT |= KEY_B;
}

void keyLeft() 
{ 
	storeKeyPosition();
	key.c -= 1; 
	movedYet = 1;
}
void keyRight()
{ 
	storeKeyPosition();
	key.c += 1; 
	movedYet = 1;
}
void keySoftDrop() 
{ 
	storeKeyPosition();
	key.r += 1; 
	movedYet = 1;
}
void keyHardDrop() {}
void keyRotateLeft() {
	printMatrix(key.t);
	clearTetrimino(key.r, key.c, key.t);
	storeKeyPosition();
	int a[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
	a[0] = key.t[12];
	a[1] = key.t[8];
	a[2] = key.t[4];
	a[3] = key.t[0];
	a[4] = key.t[13];
	a[5] = key.t[9];
	a[6] = key.t[5];
	a[7] = key.t[1];
	a[8] = key.t[14];
	a[9] = key.t[10];
	a[10] = key.t[6];
	a[11] = key.t[2];
	a[12] = key.t[15];
	a[13] = key.t[11];
	a[14] = key.t[7];
	a[15] = key.t[3];
	key.t = a;
	DEBUG_PRINT("A:\n");
	printMatrix(a);
	DEBUG_PRINT("T:\n");
	printMatrix(key.t);
}
void keyRotateRight() {}
void showMenu() {}
void pause() {}
void storeKeyPosition() 
{
	keyLastR = key.r;
	keyLastC = key.c;
}

void setNextPiece(tetrimino next) {
	drawRect(20, 170, 24, 24, BLACK);
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y<4; y++) {
			if (next.t[x*4+y] == 1)
				drawRect(20 + x*6, 170+y*6, 4, 4, next.color);
		}
	}
}


void placeKey() {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (key.t[x*4+y] == 1)
				matrix[key.r+x][key.c+y] = 1;
		}
	}
	key.t = next.t;
	key.color = next.color;
	key.r = 0;
	key.c = 5;
	int nextType = qran_range(0,7);
	int *n;
	n = tetriminos[nextType];
	next.t = n;
	next.color = colors[nextType];
	setNextPiece(next);
}

void drawMatrixBorders() {
	for (int i = 14; i <= 148; i++) {
		setPixel(i, 88, YELLOW);
		setPixel(i, 152, YELLOW);	
	} 
	for (int i = 88; i <= 152; i++) {
		setPixel(14, i, YELLOW);
		setPixel(148, i, YELLOW);
	}
}

void printMatrix(int *m) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			DEBUG_PRINTF("%d ", m[4*i+j]);
		}
		DEBUG_PRINT("\n");
	}
	DEBUG_PRINT("=======\n");
}
