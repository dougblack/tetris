// mylib.h

#define REG_DISPCTL *(unsigned short *)0x4000000

#define MODE3           3
#define BG2_ENABLE      (1<<10)

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
#define LIGHTRED        RGB(10,0,0)
#define BLACK			RGB(0,0,0) 

typedef unsigned short u16;
extern u16 *videoBuffer;
extern int matrix[22][10];

typedef struct {
	int *t;
	u16 color;
	int r;
	int c;
} tetrimino;

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

int sqran(int seed);
int qran();
int qran_range(int min, int max);
