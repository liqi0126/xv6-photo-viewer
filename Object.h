#ifndef OBJECT_H
#define OBJECT_H

#include "types.h"

typedef struct PPoint{
    int x;
    int y;
}PPoint;

typedef struct PRect{
    int x;
    int y;
    int w;
    int h;
}PRect;

typedef struct PColor{
    uchar b;
    uchar g;
    uchar r;
}PColor;

typedef struct PBitmap
{
  int w;
  int h;
  PColor * data;
} PBitmap;

static inline PColor RGB(uchar r, uchar g, uchar b){
    PColor rgb;
    rgb.r = r;
    rgb.g = g;
    rgb.b = b;
    return rgb;
}

#pragma pack(2)
typedef struct PBitmapFileHeader{
    ushort bfType;
    uint bfSize;
    ushort bfReserved1;
    ushort bfReserved2;
    uint bfOffBits;
}PBitmapFileHeader;

typedef struct PBitmapInfoHeader{
    uint biSize;
    int biWidth;
    int biHeight;
    ushort biPlanes;
    ushort biBitCount;
    uint biCompression;
    uint biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    uint biClrUsed;
    uint biClrImportant;
} PBitmapInfoHeader;

typedef struct APNGFrame{
    PBitmap* bmp;
    uchar dispose_op;
    uchar blend_op;
}APNGFrame;

typedef struct APNG{
    APNGFrame* frames;
}APNG;

#endif
