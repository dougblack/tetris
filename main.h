// main.h

//primary interrupt locations
#define REG_IME *(u16*)0x4000208
#define REG_IE *(u16*)0x4000200
#define REG_IF *(volatile u16*)0x4000202
#define REG_INTERRUPT *(u32*)0x3007FFC
#define REG_DISPSTAT *(u16*)0x4000004
#define REG_KEYINPUT *(u16*)0x4000130
#define REG_KEYCNT *(u16*)0x4000132

//interupt constants for turning them on
#define INT_VBLANK_ENABLE 1 << 3
#define INT_BUTTON_ENABLE 1 << 14

//interrupt constants for checking which type of interrupt happened
#define INT_VB     1 <<  0	// VB – vertical blank interrupt
#define INT_HB     1 <<  1   // HB – horizontal blank interrupt
#define INT_VC     1 <<  2	// VC – vertical scanline count interrupt
#define INT_T0     1 <<  3	// T0 – timer 0 interrupt
#define INT_T1     1 <<  4	// T1 – timer 1 interrupt
#define INT_T2     1 <<  5	// T2 – timer 2 interrupt
#define INT_T3     1 <<  6	// T3 – timer 3 interrupt
#define INT_COM    1 <<  7	// COM – serial communication interrupt
#define INT_DMA0   1 <<  8	// DMA0 – DMA0 finished interrupt
#define INT_DMA1   1 <<  9	// DMA1 – DMA1 finished interrupt
#define INT_DMA2   1 << 10	// DMA2 – DMA2 finished interrupt
#define INT_DMA3   1 << 11	// DMA3 – DMA3 finished interrupt
#define INT_BUTTON 1 << 12	// BUTTON – button interrupt
#define INT_CART   1 << 13	// CART – game cartridge interrupt


typedef unsigned int u32;

void setupInterrupts(void);
void enableButtonInterrupt();
void enableVBlankInterrupt();
void interruptHandler();

void keyLeft();
void keyRight();
void keySoftDrop();
void keyHardDrop();
void keyRotateLeft();
void keyRotateRight();
void showMenu();
void pause();
void storeKeyPosition();
