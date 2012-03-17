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
	key.color = colors[];
	key.r = 0;
	key.c = 50;

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
	if (REG_IF == INT_BUTTON)
	{
		int i = 0;
		if (KEY_DOWN_NOW(KEY_RIGHT))
			i=1;	// Move falling tetrimino right 
		else if (KEY_DOWN_NOW(KEY_LEFT))
			i=2;	// Move falling tetrimino left
		else if (KEY_DOWN_NOW(KEY_UP))
			i=3;	// Hard drop falling tetrimino
		else if (KEY_DOWN_NOW(KEY_DOWN))
			i=4;	// Soft drop falling tetrimino
		else if (KEY_DOWN_NOW(KEY_A))
			i=5;	// Roate falling tetrimino right 90 degrees
		else if (KEY_DOWN_NOW(KEY_B))
			i=6;	// Rotate falling tetrimino left 90 degrees
		else if (KEY_DOWN_NOW(KEY_SELECT))
			i=7;	// Return to main menu
		else if (KEY_DOWN_NOW(KEY_START))
			i=8;	// Pause?
	} 	
	if (REG_IF == INT_VB)
	{
		frame++;
		if ((frame % 60)==0) 
		{
			clearTetrimino(key);
			key.r = key.r + 20;
			drawTetrimino(key);	
			// Move falling tetrimino down every second 
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
}
