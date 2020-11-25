#include "PVCLoader.h"
#include "fcntl.h"

PBitmap LoadBitmap(char* filename){
    PBitmap bmp = {0, 0, 0};
    int fd;
    if((fd = open(filename, O_RDONLY)) < 0){
        printf(1, "Can't open %s\n", filename);
        return bmp;
    }
    PBitmapFileHeader fileHeader;
    read(fd, (char*)&fileHeader, sizeof(fileHeader));
    
    PBitmapInfoHeader infoHeader;
    read(fd, (char*)&infoHeader, sizeof(infoHeader));
    printf(1, "\n info: %x %d %d %d %d, totalsize %d\n", fileHeader.bfType, fileHeader.bfSize, 
	fileHeader.bfOffBits, fileHeader.bfReserved1, fileHeader.bfReserved2,sizeof(fileHeader)+sizeof(infoHeader));
    
    bmp.w = infoHeader.biWidth;
    bmp.h = infoHeader.biHeight;
    bmp.data = (PColor*)malloc(bmp.w * bmp.h * sizeof(PColor));
    
    int count = infoHeader.biBitCount;
    int length = (((bmp.w * count) + 31) >> 5) << 2;
    int size = length * bmp.h;
    printf(1, "load bitmap l: %d s: %d c: %d width: %d height: %d\n",length,size,count,bmp.w,bmp.h);

    int wastedLen = fileHeader.bfOffBits - sizeof(fileHeader) - sizeof(infoHeader);
    uchar* waste  = (uchar*)malloc(sizeof(uchar) * wastedLen);
    read(fd, (char*)waste, wastedLen);

    uchar* data = (uchar*)malloc(sizeof(uchar) * fileHeader.bfSize);
    read(fd, (char*)data, sizeof(uchar) * fileHeader.bfSize);

    int bits = infoHeader.biBitCount / 8;
    for(int j=0; j<bmp.h; ++j){
        int offset = (bmp.h - j - 1) * bmp.w;
        int dataOffset = j * length;
        for(int i=0; i<bmp.w; ++i){
            int specOffset = dataOffset + bits * (i + 1);
            bmp.data[offset+i].r = (int)data[specOffset - 1];
            bmp.data[offset+i].g = (int)data[specOffset - 2];
            bmp.data[offset+i].b = (int)data[specOffset - 3];
        }
    }
    close(fd);
    return bmp;
}