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
int matrix[22][10];
int keyLastR;
int keyLastC;
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
	key.t = t;
	key.color = colors[tetType];
	key.r = 0;
	key.c = 50;
	keyLastR = 0;
	keyLastC = 50;
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
			clearTetrimino(keyLastR, keyLastC);
			key.r = key.r + 5;
			drawTetrimino(key);	
			keyLastR = key.r;
			keyLastC = key.c;
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
	key.c -= 5; 
	movedYet = 1;
}
void keyRight()
{ 
	storeKeyPosition();
	key.c += 5; 
	movedYet = 1;
}
void keySoftDrop() 
{ 
	storeKeyPosition();
	key.r += 5; 
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
