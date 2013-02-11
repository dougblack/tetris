// Douglas Black HW 8

#include "main.h"

/*
 * Tetrimino art. :)
 */
int tetriminos[7][16] =
{	{	0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0	},		// O 
  {	0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0	},		// I
  {	0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0	},		// L
  {	0,0,0,0,0,0,1,0,0,0,1,0,0,1,1,0	},		// J
  {	0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1	},		// T
  {	0,0,0,0,0,1,0,0,0,1,1,0,0,0,1,0	},		// S
  {	0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0	}	};	// Z

u16 colors[7] = {YELLOW, CYAN, LIGHTRED, BLUE, MAGENTA, GREEN, RED}; // Tetrimino colors

int frame = 0;	// video frame
tetrimino key;	// initial tetrimino, this is the one that falls.
tetrimino next;	// next tetrimino to always be displayed top right of board
int keyLastR, keyLastC;
int keyLastT[16];
int movedYet = 0;
int rotatedYet = 0;
int droppedYet = 0;
int clearedLines = 0;
int level = 1;
int fallSpeed = 40;
int inputSpeed = 10;
int rotateSpeed = 20;
int placed = 0;
int gameover = 0;


int main() 
{ 
  setupBoard();
  titleScreen();
  mainRunLoop();
  gameOverScreen();
}

/* ===============================
 * BOARD SETUP
 * =============================== */
void setupBoard() { 
  REG_DISPCTL = MODE3 | BG2_ENABLE;
  int tetType = qran_range(0,7);
  int nextType = qran_range(0,7);
  int *t;
  int *n;
  t = tetriminos[tetType];
  n = tetriminos[nextType];

  // Generate random tetrimino to start falling and
  // generate random tetrimino to show as next tetrmino
  key.t = t;
  next.t = n;
  next.color = colors[nextType];
  key.color = colors[tetType];

  key.r = 0;
  key.c = 3;
  keyLastR = 0;
  keyLastC = 3;

  for (int i = 0; i < 16; i++) {
    keyLastT[i] = key.t[i];
  }


  /* Draw back screen */
  dma_mem[3].cnt = 0;
  u16 *black = BLACK;
  dma_mem[3].src = black;
  dma_mem[3].dst = videoBuffer;
  dma_mem[3].cnt = 38400 | DMA_ON | DMA_SOURCE_FIXED;

  /* Draw board and pieces */
  drawMatrixBorders();
  setNextPiece(next);
}

/* ==================================
 * TITLE SCREEN
 * ================================== */
void titleScreen() {
  drawImage3(0,0,240,160,title);
  while(1)
  {
    key_poll();
    if (key_hit(KEY_START))
      break;
  }
}

/* ===================================
 * MAIN RUN LOOP
 * =================================== */
void mainRunLoop() {
  while(!gameover) 
  {
    /* Get and process input */
    key_poll();		
    if (key_hit(KEY_RIGHT)) 
      keyRight();
    else if (key_hit(KEY_LEFT))
      keyLeft();
    else if (key_hit(KEY_UP)) {
      droppedYet = 1;
      keyHardDrop();
    }
    else if (key_hit(KEY_DOWN))
      keySoftDrop();
    else if (key_hit(KEY_A)) {
      rotatedYet = 1;
      keyRotateRight();
    }
    else if (key_hit(KEY_B)) {
      rotatedYet = 1;
      keyRotateLeft();
    }
    else if (key_hit(KEY_SELECT))
      showMenu();
    else if (key_hit(KEY_START))
      pause();

    waitForVBlank();
    frame++;
    /* Calculate layout of next frame */
    if ((frame % fallSpeed)==0) 
    {
      placed = 0;
      clearTetrimino(keyLastR, keyLastC, keyLastT);
      if (checkBoundBottom(key) != 1) {
        key.r = key.r + 1;
        drawTetrimino(key);
      } else {
        placeKey();
        placed = 1;
      }
    }
    if ((frame % inputSpeed)==0)
    {
      if (placed == 0) {
        clearTetrimino(keyLastR, keyLastC, keyLastT);
        drawTetrimino(key);
      }
      keyLastR = key.r;
      keyLastC = key.c;
      for (int i = 0; i < 16; i++) 
        keyLastT[i] = key.t[i];
      movedYet = 0;
    }
    if ((frame % rotateSpeed)==0)
      rotatedYet = 0;
  }
}

/* =================================
 * GAME OVER SCREEN
 * ================================= */
void gameOverScreen() {
  drawImage3(0,0,240,160,gameoverscreen);
  while(1)
  {
    key_poll();
    if (key_hit(KEY_START))
      break;
  }
}

/* VBA SETUP METHODS */
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

/* INPUT METHODS */
void keyLeft() 
{ 
  storeKeyPosition();
  if (checkBoundLeft(key) != 1)
    key.c -= 1; 
  movedYet = 1;
}
void keyRight()
{ 
  storeKeyPosition();
  if (checkBoundRight(key) != 1)
    key.c += 1; 
  movedYet = 1;
}
void keySoftDrop() 
{ 
  storeKeyPosition();
  if (checkBoundBottom(key) != 1)
    key.r += 1; 
  movedYet = 1;
}
void keyHardDrop() {
  storeKeyPosition();
  while (checkBoundBottom(key) != 1)
    key.r+=1;
  movedYet = 1;	
}
void keyRotateLeft() {
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
  dma_memcpy(key.t, a, 16*sizeof(int));
}
void keyRotateRight() {
  clearTetrimino(key.r, key.c, key.t);
  storeKeyPosition();
  int a[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
  a[0] = key.t[3];
  a[1] = key.t[7];
  a[2] = key.t[11];
  a[3] = key.t[15];
  a[4] = key.t[2];
  a[5] = key.t[6];
  a[6] = key.t[10];
  a[7] = key.t[14];
  a[8] = key.t[1];
  a[9] = key.t[5];
  a[10] = key.t[9];
  a[11] = key.t[13];
  a[12] = key.t[0];
  a[13] = key.t[4];
  a[14] = key.t[8];
  a[15] = key.t[12];
  dma_memcpy(key.t, a, 16*sizeof(int));
}
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
      if (key.t[x*4+y] == 1) {
        if (key.r+x <=4)
          gameOver();
        matrix[key.r+x][key.c+y] = 1;
        colorMatrix[key.r+x][key.c+y] = key.color;
      }
    }
  }
  checkForScore();
  key.t = next.t;
  key.color = next.color;
  key.r = 0;
  key.c = 3;
  int nextType = qran_range(0,7);
  int *n;
  n = tetriminos[nextType];
  next.t = n;
  next.color = colors[nextType];
  drawMatrix();
  setNextPiece(next);
  droppedYet = 0;
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

void gameOver() {
  gameover = 1;
}

void waitForVBlank()
{
  while(SCANLINECOUNTER > 160);
  while(SCANLINECOUNTER < 160);
}
