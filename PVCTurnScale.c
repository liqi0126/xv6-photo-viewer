#include "PVCTurnScale.h"

// 排除边界，获取对应的PColor，对于错误的x，y，将isInPic置位0
PColor * getColor(PBitmap* pic, int x, int y, int *isInPic){
    if(x<0||x>pic->width||y<0||y>pic->height)
    {
        isInPic = 0;
        return (pic->data);
    }

    isInPic = 1;
    return (pic->data + x * pic->width + y);
    
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

    dst->r = (int)Color0->r * pm0 * flag[0] + Color1->r * pm1 * flag[1] + Color2->r * pm2 * flag[2] + Color3->r * pm3 * flag[3];
    dst->g = (int)Color0->g * pm0 * flag[0] + Color1->g * pm1 * flag[1] + Color2->g * pm2 * flag[2] + Color3->g * pm3 * flag[3];
    dst->b = (int)Color0->b * pm0 * flag[0] + Color1->b * pm1 * flag[1] + Color2->b * pm2 * flag[2] + Color3->b * pm3 * flag[3];

    return 1;
}

int PicScale(PBitmap* src, PBitmap* dst)
{
    float xScale = (float)src->width / (float)dst->width;
    float yScale = (float)src->height / (float)dst->height;
    
    int flag = 0;
    for(int x = 0; x < dst->width; x++)
    {
        for(int y = 0; y < dst->height; y++)
        {
            mixColor(src, x * xScale, y * yScale, (dst->data+ x * dst->width + y));
        }
    }
    
    return 1;
}