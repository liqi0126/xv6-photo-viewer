#ifndef PVCLOADER_H
#define PVCLOADER_H

#include "PVCObject.h"

enum imgType{
    JPG,
    BMP,
    PNG,
    NONE,
};

int type(char* filename);
PBitmap LoadBmp(char* filename);
PBitmap LoadJpeg(char* filename);
PBitmap LoadImg(char* filename);


#endif