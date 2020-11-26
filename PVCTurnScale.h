#ifndef PVCTURNSCALE_H
#define PVCTURNSCALE_H

typedef struct PColor
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
}PColor;

typedef struct PBitmap
{
    int width;
    int height;
    PColor * data;
}PBitmap;


int PicScale(PBitmap* src, PBitmap* dst);


#endif PVCTURNSCALE_H