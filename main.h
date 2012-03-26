// main.h

#include "input.h"
#include "video.h"
#include "green.h"
#include "black.h"
#include <debugging.h>

extern int matrix[26][10];

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
void setNextPiece(tetrimino next);
void placeKey();
void drawMatrixBorders();
void printMatrix(int *m);
void storeMatrix(int *to, int *from);
