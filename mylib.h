// mylib.h

#define REG_DISPCTL *(unsigned short *)0x4000000

#define MODE3           3
#define BG2_ENABLE      (1<<10)

#define DMATRANSFER(_dst, _src, _count, _ch, _mode) \
do { \
	dma_mem[_ch].cnt = 0; \
	dma_mem[_ch].src = (const void*) (_src); \
	dma_mem[_ch].dst = (void*) (_dst); \
	dma_mem[_ch].cnt = (_count) | (_mode); \
} while(0)

#define REG_DMA0SAD         *(unsigned int*)0x40000B0  // source address
#define REG_DMA0DAD         *(unsigned int*)0x40000B4  // destination address
#define REG_DMA0CNT         *(unsigned int*)0x40000B8  // control register

/* DMA channel 1 register definitions */
#define REG_DMA1SAD         *(unsigned int*)0x40000BC  // source address
#define REG_DMA1DAD         *(unsigned int*)0x40000C0  // destination address
#define REG_DMA1CNT         *(unsigned int*)0x40000C4  // control register

/* DMA channel 2 register definitions */
#define REG_DMA2SAD         *(unsigned int*)0x40000C8  // source address
#define REG_DMA2DAD         *(unsigned int*)0x40000CC  // destination address
#define REG_DMA2CNT         *(unsigned int*)0x40000D0  // control register

/* DMA channel 3 register definitions */
#define REG_DMA3SAD         *(unsigned int*)0x40000D4  // source address
#define REG_DMA3DAD         *(unsigned int*)0x40000D8  // destination address
#define REG_DMA3CNT         *(unsigned int*)0x40000DC  // control register

/* Defines */
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

// Note: The next 4 lines can tell you the default: DMA_DESTINATION_INCREMENT
// as well as which bits are used for this (i.e. 22 and 21)
#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)  // Used for sound

#define DMA_16 (0 << 26)  // Typically use this
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28) // Typically use this
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)  // The on switch!!!!

#define RGB(r,g,b)		((r) | ((g)<<5) | ((b)<<10))
#define RED             RGB(31,0,0)
#define BLUE            RGB(0,0,31)
#define GREEN           RGB(0,31,0)
#define YELLOW          RGB(31,31,0)
#define MAGENTA         RGB(31,0,31)
#define CYAN            RGB(0,31,31)
#define WHITE           RGB(31,31,31)
#define LIGHTBLUE       RGB(0,0,10)
#define LIGHTGREEN      RGB(0,10,0)
#define LIGHTRED        RGB(31,10,0)
#define BLACK			RGB(0,0,0) 

typedef unsigned short u16;
typedef unsigned int u32;
extern u16 *videoBuffer;
extern int matrix[26][10];
extern u16 colorMatrix[26][10];
extern int clearedLines;
extern int level;
extern int fallSpeed;

typedef struct {
	int *t;
	u16 color;
	int r;
	int c;
} tetrimino;

typedef struct
{
	const volatile void *src;
	volatile void *dst;
	volatile unsigned int cnt;
} DMAREC;

#define dma_mem			((volatile DMAREC*) 0x040000B0)

/* FUNCTION PROTOTYPES */
void setPixel(int r, int c, u16 color);
void drawRect(int r, int c, int width, int height, u16 color);
void sleep(int length);
void drawTetrimino(tetrimino key);
void clearTetrimino(int r, int c, int *t);
void drawMatrix();
void rotateLeft(tetrimino key);
void rotateRight(tetrimino key);
int checkBoundBottom(tetrimino key);
int checkBoundLeft(tetrimino key);
int checkBoundRight(tetrimino key);
void checkForScore();
void clearRow(int row);
void incrementLines();
void dma_memcpy(void *dst, const void *src, u16 count);
void drawImage3(int r, int c, int width, int height, const u16* image);

int sqran(int seed);
int qran();
int qran_range(int min, int max);
