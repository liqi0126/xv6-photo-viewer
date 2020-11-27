#define _CRT_SECURE_NO_WARNINGS
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys\stat.h>
#include <sys\types.h>
#include "PVCTurnScale.h"

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef uint pde_t;

#define BITMAP_32 32
#define BITMAP_24 24

typedef struct BITMAP_FILE_HEADER {
    ushort bfType;
    uint bfSize;
    ushort bfReserved1;
    ushort bfReserved2;
    uint btOffBits;
}BITMAP_FILE_HEADER;

typedef struct BITMAP_INFO_HEADER {
    uint biSize;
    int biWidth;
    int biHeight;
    ushort biPlanes;
    ushort biBitCount;
    uint biCompression;
    uint biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    uint biCirUserd;
    uint biCirImportant;
}BITMAP_INFO_HEADER;

/*
typedef struct RGB {
    unsigned char B;
    unsigned char G;
    unsigned char R;
} RGB;
*/

// 32 bit RGBA. used above GUI Utility
typedef struct RGBA {
    unsigned char A;
    unsigned char B;
    unsigned char G;
    unsigned char R;
} RGBA;

void readBitmapHeader(int bmpFile, BITMAP_FILE_HEADER *bmpFileHeader, BITMAP_INFO_HEADER *bmpInfoHeader) {
    // Read Bitmap file header
    fread(bmpFileHeader, 1, sizeof(BITMAP_FILE_HEADER), bmpFile);
    // Read Bitmap info header
    fread(bmpInfoHeader, 1, sizeof(BITMAP_INFO_HEADER), bmpFile);
}

void write24BitmapFileHeader(int bmpFile, int height, int width) {
    int rowSize = (24 * width + 31) / 32 * 4;  // zero padding
    BITMAP_FILE_HEADER bmpFileHeader;
    bmpFileHeader.bfType = 0x4D42;  // "BM"
    bmpFileHeader.bfSize = rowSize * height + 54;
    bmpFileHeader.bfReserved1 = 0;
    bmpFileHeader.bfReserved2 = 0;
    bmpFileHeader.btOffBits = 54;  // size of header

    BITMAP_INFO_HEADER bmpInfoHeader;
    bmpInfoHeader.biSize = 40;  // header size
    bmpInfoHeader.biWidth = width;
    bmpInfoHeader.biHeight = height;
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitCount = 24;
    bmpInfoHeader.biCompression = 0;               // No Compression
    bmpInfoHeader.biSizeImage = rowSize * height;  // TODO:
    bmpInfoHeader.biXPelsPerMeter = 0;             //TODO:
    bmpInfoHeader.biYPelsPerMeter = 0;             //TODO:
    bmpInfoHeader.biCirUserd = 0;
    bmpInfoHeader.biCirImportant = 0;

    fwrite(&bmpFileHeader, 1, sizeof(BITMAP_FILE_HEADER), bmpFile);
    fwrite(&bmpInfoHeader, 1, sizeof(BITMAP_INFO_HEADER),bmpFile);
}

int readBitmapFile(char *fileName, RGBA *result, int *height, int *width) {
    int i;
    int bmpFile = open(fileName, 0);
    if (bmpFile < 0) {
        return -1;
    }

    BITMAP_FILE_HEADER bmpFileHeader;
    BITMAP_INFO_HEADER bmpInfoHeader;

    readBitmapHeader(bmpFile, &bmpFileHeader, &bmpInfoHeader);
    *width = bmpInfoHeader.biWidth;
    *height = bmpInfoHeader.biHeight;
    int column = bmpInfoHeader.biWidth;
    int row = bmpInfoHeader.biHeight;
    int bits = bmpInfoHeader.biBitCount;
    char tmpBytes[3];
    int rowBytes = column * bits / 8;
    char *buf = (char *)result;
    for (i = row - 1; i >= 0; i--) {
        if (bits == 32) {
            read(bmpFile, buf + i * rowBytes, rowBytes);
        } else {
            int j = 0;
            for (j = 0; j < column; j++) {
                read(bmpFile, buf + i * column * 4 + j * sizeof(RGBA), 3);
                *(buf + i * column * 4 + j * sizeof(RGBA) + 3) = 255;
            }
        }
        if (rowBytes % 4 > 0) {
            read(bmpFile, tmpBytes, 4 - (rowBytes % 4));
        }
    }

    close(bmpFile);
    return 0;
}

int read24BitmapFile(char *fileName, RGB *result, int *height, int *width) {
    int i;
    char headerbuf[100];
    int bmpFile = fopen(fileName, "rb");
    if (bmpFile < 0) {
        return -1;
    }

    BITMAP_FILE_HEADER bmpFileHeader;
    BITMAP_INFO_HEADER bmpInfoHeader;

    readBitmapHeader(bmpFile, &bmpFileHeader, &bmpInfoHeader);
    fread(headerbuf, 1, bmpFileHeader.btOffBits - 54, bmpFile); // read out the extra header
    *width = bmpInfoHeader.biWidth;
    *height = bmpInfoHeader.biHeight;
    result = (RGB*)malloc(*width * *height * sizeof(RGB));


    int column = bmpInfoHeader.biWidth;
    int row = bmpInfoHeader.biHeight;
    int bits = bmpInfoHeader.biBitCount;
    char tmpBytes[3];
    int rowBytes = column * 3;
    char *buf = (char *)result;
    for (i = row - 1; i >= 0; i--) {
        if (bits == 24) {
            fread(buf, sizeof(RGB)* i * rowBytes, rowBytes, bmpFile);
        } else {
            int j = 0;
            for (j = 0; j < column; j++) {
                fread(buf + i * column * 3 + j * sizeof(RGB), 1, 3, bmpFile);
                fread(tmpBytes, sizeof(tmpBytes), 1,bmpFile);
            }
        }

        if (rowBytes % 4 > 0) {
            fread(tmpBytes, 1, 4 - (rowBytes % 4), bmpFile);
        }
    }

    fclose(bmpFile);
    return 0;
}

int write24BitmapFile(char *filename, RGB *img, int height, int width) {
    int bmpFile = fopen(filename, O_CREAT | O_RDWR);
    int rowBytes = width * 3;
    char tmpBytes[3] = {0, 0, 0};

    write24BitmapFileHeader(bmpFile, height, width);
    for (int i = height - 1; i >= 0; i--) {
        fwrite(img, sizeof(RGB) * i * width, rowBytes, bmpFile);
        if (rowBytes % 4 > 0) {
            fwrite(img, tmpBytes, 4 - (rowBytes % 4), bmpFile);
        }
    }

    fclose(bmpFile);
    return 0;
}

int main() {
    /*
    RGB *results = malloc(800 * 600 * 3);
    int h, w;
    read24BitmapFile("images.bmp", results, &h, &w);
    printf("h=%d, w=%d\n", h, w);
    int j = 0;
    for (int i = 0; i < w; i++) {
        printf("(%x,%x,%x) ", results[j*w + i].R, results[j*w + i].G, results[j*w + i].B);
    }
    // write24BitmapFile("reverse.bmp", results, height, width);
    */

    PBitmap src, dst;
    read24BitmapFile("D:\\GitHub\\xv6-photo-viewer\\avatar-1.bmp", &src.data, &src.height, &src.width);
    dst.height = src.height * 2;
    dst.width = src.width * 2;
    dst.data = (RGB*)malloc(dst.height * dst.width * sizeof(RGB));
    picScale(&src, &dst);
    read24BitmapFile("D:\\GitHub\\xv6-photo-viewer\\avatar-3.bmp", &src.data, &src.height, &src.width);
    return 0;
}