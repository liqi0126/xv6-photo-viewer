#ifndef PVCOBJECT_H
#define PVCOBJECT_H

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
    uchar r;
    uchar g;
    uchar b;
}PColor;

typedef struct PBitmap
{
  int w;
  int h;
  PColor * data;
} PBitmap;

static inline PColor RGB(uchar r, uchar g, uchar b){
    PColor rgb = {r, g, b};
    return rgb;
}

#pragma pack(2)
typedef struct PBitmapFileHeader{
    ushort bfType;
    ulong bfSize;
    ushort bfReserved1;
    ushort bfReserved2;
    ulong bfOffBits;
}PBitmapFileHeader;

typedef struct PBitmapInfoHeader{
	ulong biSize;
	long biWidth;
	long biHeight;
	ushort biPlanes;
	ushort biBitCount;
	ulong biCompression;
	ulong biSizeImage;
	long biXPelsPerMeter;
	long biYPelsPerMeter;
	ulong biClrUsed;
	ulong biClrImportant;
} PBitmapInfoHeader;

#endif