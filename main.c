// Douglas Black HW 8

#include "mylib.h"
#include "main.h"

int tetriminos[7][16] = 
{	{	0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0	},		// O 
	{	0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0	},		// I
	{	0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0	},		// L
	{	0,0,0,0,0,0,1,0,0,0,1,0,0,1,1,0	},		// J
	{	0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1	},		// T
	{	0,0,0,0,0,1,0,0,0,1,1,0,0,0,1,0	},		// S
	{	0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0	}	};	// Z
u16 colors[7] = {RED, BLUE, GREEN, YELLOW, MAGENTA, CYAN, WHITE};
int frame = 0;
tetrimino key; 
int main() 
{ 
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	setupInterrupts();
	int *t;
	t = tetriminos[2];
	key.t = t;
	key.color = colors[2];
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
		if (KEY_DOWN_NOW(KEY_RIGHT))
			key.c = key.c + 20;
		else if (KEY_DOWN_NOW(KEY_LEFT))
			key.c = key.c - 20;
		else
			key.r = key.r + 50;	
	} 	
	if (REG_IF == INT_VB)
	{
		frame++;
		if ((frame % 60)==0) 
		{	
			clearTetrimino(key);
			key.r = key.r+10;
			drawTetrimino(key);
		}
	}
	REG_IF = REG_IF;
	REG_IME = 0x1;
}

void enableVBlankInterrupt()
{
	REG_IE = REG_IE | INT_VB;      // Enable vblank interrupt detection
	REG_DISPSTAT |= INT_VBLANK_ENABLE; // Make display generate
	REG_KEYCNT |= INT_BUTTON_ENABLE;	
	// vblank interrupts
}

void enableButtonInterrupt()
{
	REG_IE = REG_IE | INT_BUTTON;
}
