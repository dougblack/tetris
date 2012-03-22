// input.h

//buttons
#define KEY_A       0x0001
#define KEY_B       0x0002
#define KEY_SELECT  0x0004
#define KEY_START   0x0008
#define KEY_RIGHT	0x0010
#define KEY_LEFT    0x0020
#define KEY_UP      0x0040
#define KEY_DOWN    0x0080
#define KEY_R       0x0100
#define KEY_L       0x0200

#define KEY_MASK	0x03FF

#define KEY_DOWN_NOW(key)	(~(REG_KEYINPUT) & key)