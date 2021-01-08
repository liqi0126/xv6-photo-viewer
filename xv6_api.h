#ifndef XV6_API_H
#define XV6_API_H

#ifndef __ASSEMBLER__
struct RGB;
struct RGBA;
struct Rect;
struct Point;
struct Size;
struct Image;
struct ImageList;
struct PBitmap;

#define USCREEN_WIDTH 800
#define USCREEN_HEIGHT 600
#define UTITLE_HEIGHT 30

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

typedef struct Image
{
	char* image_name;
    char* image_type;
    int image_size;
    struct RGB* data;
    int scale_needed;
    struct RGB* scale_data;
    int h;
    int scale_h;
    int w;
    int scale_w;
    int save_time;
	struct Image* prev;
	struct Image* next;
}Image;

typedef struct ImageList
{
	struct Image* head;
	struct Image* tail;
}ImageList;

typedef struct PBitmap
{
    int width;
    int height;
    struct RGB* data;
}PBitmap;

#endif
#endif
