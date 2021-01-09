#ifndef LOADER_H
#define LOADER_H

#include "Object.h"
#include <unistd.h>

enum imgType{
    JPG,
    BMP,
    PNG,
    NONE,
};

int type(char* filename);
PBitmap LoadBmp(char* filename);
PBitmap LoadJpeg(char* filename);
PBitmap LoadPng(char* filename);
PBitmap LoadImg(char* filename);


#endif
