#include "msg.h"
#include "user.h"
#include "types.h"
#include "color.h"
#include "fcntl.h"
#include "fs.h"
#include "stat.h"
#include "xv6_api.h"

#define PI 3.1415926536

RGB whiteRGB={255,255,255};

// 排除边界，获取对应的RGB，对于错误的x，y，将isInPic置位0
struct RGB * getColor(PBitmap* pic, int x, int y, int *isInPic)
{
    if(x<0||x>pic->width||y<0||y>pic->height)
    {
        *isInPic = 1;
        return (&whiteRGB);
    }

    if(x == pic->width) x--;
    if(y == pic->height) y--;

    *isInPic = 1;
    return (pic->data + x * pic->width + y);
    
}

int setColor(RGB* src, RGB* dst) 
{
    dst->R = src->R;
    dst->G = src->G;
    dst->B = src->B;

    return 1;
}

// 获取经过二次插值之后的RGB
int mixColor(PBitmap* src, float fx, float fy, RGB* dst)
{
    int x = (int) fx;
    int y = (int) fy;
    if(x > fx) x--;
    if(y > fy) y--;

    int flag[4];
    RGB *Color0 = getColor(src, x, y, &flag[0]);
    RGB *Color1 = getColor(src, x + 1, y, &flag[1]);
    RGB *Color2 = getColor(src, x, y + 1, &flag[2]);
    RGB *Color3 = getColor(src, x + 1, y + 1, &flag[3]);

    float u = fx - x;
    float v = fy - y;

    float pm3 = u * v;
    float pm2 = u * (1 - v);
    float pm1 = (1 - u) * v;
    float pm0 = (1 - u) * (1 - v);

    dst->R = (int)(Color0->R * pm0 * flag[0] + Color1->R * pm1 * flag[1] + Color2->R * pm2 * flag[2] + Color3->R * pm3 * flag[3]);
    dst->G = (int)(Color0->G * pm0 * flag[0] + Color1->G * pm1 * flag[1] + Color2->G * pm2 * flag[2] + Color3->G * pm3 * flag[3]);
    dst->B = (int)(Color0->B * pm0 * flag[0] + Color1->B * pm1 * flag[1] + Color2->B * pm2 * flag[2] + Color3->B * pm3 * flag[3]);

    return 1;
}

int picScale(PBitmap* src, PBitmap* dst)
{
    float xScale = (float)src->width / (float)dst->width;
    float yScale = (float)src->height / (float)dst->height;
    
    for(int x = 0; x < dst->height; x++)
    {
        for(int y = 0; y < dst->width; y++)
        {
            mixColor(src, (float)(x + 0.49999) * xScale - 0.5, (float)(y + 0.49999) * yScale - 0.5, (dst->data+ x * dst->width + y));
        }
    }
    
    return 1;
}

int getTurnSize(int* width, int* height, float angle) 
{
    int w = (int)(*width * abs(cos(angle)) + *height * abs(sin(angle)));
    int h = (int)(*width * abs(sin(angle)) + *height * abs(cos(angle)));

    *height = h;
    *width = w;
    return 1;
}

int picTurn(PBitmap* src, PBitmap* dst, float angle) {
    
    // 以左上角为旋转中心，要找到目标图中的旋转中心
    float my = (float)src->height / 2;
    float mx = (float)src->width / 2;

    const float COSX = cos(-angle);
    const float SINX = sin(-angle);

    float srcI = 0;
    float srcJ = 0;

    for (int i = 0; i < dst->height; i++)
    {
        for (int j = 0; j < dst->width; j++) 
        {
            srcI = ((float)i - my) * COSX + ((float)j - mx) * SINX + my;
            srcJ = ((float)j - mx) * COSX - ((float)i - my) * SINX + mx;
            if (!(srcI >= 0 && srcI < src->height && srcJ >= 0 && srcJ < src->width))
            {
                setColor(&whiteRGB, (dst->data + i * dst->width + j));
            }
            mixColor(src, srcI , srcJ , (dst->data + i * dst->width + j));
        }
    }

    
    return 1;

}

// （上下）翻转
int picRollingOver(PBitmap* src, PBitmap* dst)
{
    // 如果原图与目标图尺寸不一样，则返回 0；
    if (src->width != dst->width || src->height != dst->height)
        return 0;

    int h = src->height;
    int w = src->width;

    for (int i = 0;i < h;i++)
    {
        for (int j = 0;j < w;j++)
        {
            setColor((src->data + i * w + j), (dst->data + (h - i - 1) * w + j));
        }
    }

    return 1;
}

int picTurnAround(PBitmap* src, PBitmap* dst)
{
    // 如果原图与目标图尺寸不一样，则返回 0；
    if (src->width != dst->width || src->height != dst->height)
        return 0;

    int h = src->height;
    int w = src->width;

    for (int i = 0;i < h;i++)
    {
        for (int j = 0;j < w;j++)
        {
            setColor((src->data + i * w + j), (dst->data + i * w + (w - j - 1)));
        }
    }

    return 1;
}

