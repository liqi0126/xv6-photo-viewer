#include "PVCTurnScale.h"

// 排除边界，获取对应的PColor，对于错误的x，y，将isInPic置位0
PColor * getColor(PBitmap* pic, int x, int y, int *isInPic)
{
    if(x<0||x>=pic->width||y<0||y>=pic->height)
    {
        *isInPic = 0;
        return (pic->data);
    }

    *isInPic = 1;
    return (pic->data + x * pic->width + y);
    
}

int setColor(PColor* src, PColor* dst) 
{
    dst->r = src->r;
    dst->g = src->g;
    dst->b = src->b;

    return 1;
}

// 获取经过二次插值之后的PColor
int mixColor(PBitmap* src, float fx, float fy, PColor* dst)
{
    int x = (int) fx;
    int y = (int) fy;
    if(x > fx) x--;
    if(y > fy) y--;

    int flag[4];
    PColor *Color0 = getColor(src, x, y, &flag[0]);
    PColor *Color1 = getColor(src, x + 1, y, &flag[1]);
    PColor *Color2 = getColor(src, x, y + 1, &flag[2]);
    PColor *Color3 = getColor(src, x + 1, y + 1, &flag[3]);

    float u = fx - x;
    float v = fy - y;

    float pm3 = u * v;
    float pm2 = u * (1 - v);
    float pm1 = (1 - u) * v;
    float pm0 = (1 - u) * (1 - v);

    dst->r = (int)(Color0->r * pm0 * flag[0] + Color1->r * pm1 * flag[1] + Color2->r * pm2 * flag[2] + Color3->r * pm3 * flag[3]);
    dst->g = (int)(Color0->g * pm0 * flag[0] + Color1->g * pm1 * flag[1] + Color2->g * pm2 * flag[2] + Color3->g * pm3 * flag[3]);
    dst->b = (int)(Color0->b * pm0 * flag[0] + Color1->b * pm1 * flag[1] + Color2->b * pm2 * flag[2] + Color3->b * pm3 * flag[3]);

    return 1;
}

int picScale(PBitmap* src, PBitmap* dst)
{
    float xScale = (float)src->width / (float)dst->width;
    float yScale = (float)src->height / (float)dst->height;
    
    int flag = 0;
    for(int x = 0; x < dst->height; x++)
    {
        for(int y = 0; y < dst->width; y++)
        {
            mixColor(src, (float)x * xScale, (float)y * yScale, (dst->data+ x * dst->width + y));
        }
    }
    
    return 1;
}

int getTurnSize(int* width, int* height, float angle) 
{
    int w = (int)(*width * mycos(angle) + *height * mysin(angle));
    int h = (int)(*width * mysin(angle) + *height * mycos(angle));

    *height = h;
    *width = w;
    return 1;
}

int picTurn(PBitmap* src, PBitmap* dst, float angle) {
    
    // 以左上角为旋转中心，要找到目标图中的旋转中心
    double my = src->height / 2;
    double mx = src->width / 2;

    const double COSX = mycos(-angle);
    const double SINX = mysin(-angle);

    int d = 0;
    float srcI = 0;
    float srcJ = 0;

    for (int i = 0; i < dst->height; i++)
    {
        for (int j = 0; j < dst->width; j++) 
        {
            srcI = (i -my) * COSX + (j - mx) * SINX + my;
            srcJ = (j - mx) * COSX - (i - my) * SINX + mx;
            if (!(srcI >= 0 && srcI < src->height && srcJ >= 0 && srcJ < src->width))
                continue;
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

