#ifndef GUI_BASE_H
#define GUI_BASE_H

#define GUI_BUF 0x9000

#ifndef __ASSEMBLER__

ushort SCREEN_WIDTH;
ushort SCREEN_HEIGHT;
int screen_size; 

#define USCREEN_WIDTH 800
#define USCREEN_HEIGHT 600
#define UTITLE_HEIGHT 30

// 24 bit RGB. used in GUI Utility
typedef struct RGB {
    unsigned char B;
    unsigned char G;
    unsigned char R;
} RGB;

// 32 bit RGBA. used above GUI Utility
typedef struct RGBA {
    unsigned char A;
    unsigned char B;
    unsigned char G;
    unsigned char R;
} RGBA;

typedef struct Rect {
    int x;
    int y;
    int h;
    int w;
} Rect;

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct Size {
    int h;
    int w;
} Size;

typedef struct Window {
    int hwnd;
    Point pos;
    Size size;
    char* title;
    struct RGB* content;
    struct RGB* wholeContent;
} Window;

void drawPoint(RGB* color, RGB origin);
void drawPointAlpha(RGB* color, RGBA origin);
void drawCharacter(RGB *buf, Size s, Point p, char ch, RGBA color);
void drawString(RGB *buf, Size s, Point p, char *str, RGBA color);
void drawRect(RGB *buf, Point p, Size s, RGB color, Size rect_size);
void drawBitmap(struct RGB* tgt, struct RGB* cont, Point pt, Point pc, Size st, Size sc, Size s);
void drawTransparentBitmap(struct RGB* tgt, struct RGB* cont, Point pt, Point pc, Size st, Size sc, Size s);
void copyContent(RGB* tgt, RGB* src, Point p, Size s, Size copy_size);
void drawMouse(RGB *buf, int mode, int x, int y);

#endif
#endif
