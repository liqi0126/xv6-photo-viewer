struct RGB;
struct RGBA;
struct Size;
struct Point;

extern struct RGB* screen;
extern struct RGB* screen_wo_focus;
extern struct RGB* screen_buf;

void drawCharacterToScreen(RGB *buf, Size s, Point p, char ch, RGBA color);
void drawStringToScreen(RGB *buf, Size s, Point p, char *str, RGBA color);
void drawBitmapToScreen(RGB *buf, RGB *img, Point p, Size s);
void drawPartBitmapToScreen(RGB *buf, RGB *img, Point screen_p, Point img_p, Size img_s, Size draw_s);
void drawTransparentBitmapToScreen(RGB *buf, RGB *img, Point p, Size s);
void drawMouseToScreen(RGB *buf, int mode, int x, int y);
void drawScreenToScreen(RGB* buf, RGB* img);
void clearMouse(RGB *buf, RGB *no_mouse_buf, int x, int y);