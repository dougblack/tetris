// Douglas Black HW 8

#include "mylib.h"
#include "main.h"
#include "input.h"

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
tetrimino next;	// next tetrimino to always be displayed top right of bard
int keyLastR;
int keyLastC;
int *keyLastT;
int movedYet = 0;
int updateSpeed = 10;
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
	if ((REG_IF & INT_BUTTON) && (movedYet == 0))
	{
		if (KEY_DOWN_NOW(KEY_RIGHT))
			keyRight();
		else if (KEY_DOWN_NOW(KEY_LEFT))
			keyLeft();
		else if (KEY_DOWN_NOW(KEY_UP))
			keyHardDrop();
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
			if (checkBoundBottom(key) != 1) {
				clearTetrimino(keyLastR, keyLastC, keyLastT);
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
void keyRotateLeft() {}
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
				drawRect(20 + x*6, 170+y*6, 6, 6, next.color);
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
int checkBoundBottom(tetrimino key) {
		for (int x = 0; x<4; x++) {
			if ((key.t[12+x] == 1) && (matrix[key.r+4][key.c+x] == 1))
				return 1;
		}
		return 0;
}
