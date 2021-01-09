//
//  saver.c
//  imgloader
//
//  Created by 张子谦 on 2020/11/28.
//

#include "saver.h"
#include "lodepng.h"
 

 
int rgbToBmpFile(const char *pFileName, PBitmap* bmp, const int format)
{
    int  nWidth  = bmp->w;
    int  nHeight = bmp->h;
    char* data =(char*)malloc(sizeof(uchar)*nWidth*nHeight*format);
    char* pRgbaData = (char*)bmp->data;
    
    BMP_FILE_HEADER bmpHeader;
    BMP_INFO_HEADER bmpInfo;
 
    FILE* fp         = NULL;
    char* pBmpSource = NULL;
    char* pBmpData   = NULL;
 
    int i = 0, j=0;
 
    //4 bytes pack. must be 4 times per line。
    int bytesPerLine = (nWidth*BITS_PER_PIXCEL+31)/32*4;
    int pixcelBytes  = bytesPerLine*nHeight;
 
    bmpHeader.bfType        = 0x4D42;
    bmpHeader.bfReserved1   = 0;
    bmpHeader.bfReserved2   = 0;
    bmpHeader.bfOffBits     = sizeof(BMP_FILE_HEADER) + sizeof(BMP_INFO_HEADER);
    bmpHeader.bfSize        = bmpHeader.bfOffBits     + pixcelBytes;
 
    bmpInfo.biSize          = sizeof(BMP_INFO_HEADER);
    bmpInfo.biWidth         = nWidth;
    /** 这样图片才不会倒置 */
    bmpInfo.biHeight        = -nHeight;
    bmpInfo.biPlanes        = 1;
    bmpInfo.biBitCount      = BITS_PER_PIXCEL;
    bmpInfo.biCompression   = 0;
    bmpInfo.biSizeImage     = pixcelBytes;
    bmpInfo.biXPelsPerMeter = 100;
    bmpInfo.biYPelsPerMeter = 100;
    bmpInfo.biClrUsed       = 0;
    bmpInfo.biClrImportant  = 0;
 
 
    /** convert in memort, then write to file. */
    pBmpSource = (char*)malloc(pixcelBytes);
    if (!pBmpSource)
    {
        return -1;
    }
 
    /** open file */
    fp = fopen(pFileName,"wb+");
    if (!fp)
    {
        return -1;
    }
 
    fwrite(&bmpHeader, sizeof(BMP_FILE_HEADER), 1, fp);
    fwrite(&bmpInfo,   sizeof(BMP_INFO_HEADER), 1, fp);
    /** Here you should consider color format. RGBA ? RGB? BGR?
        Param format is RGBA, format for file is BGR */
    pBmpData = pBmpSource;
    for (i=0; i<nHeight; i++)
    {
        for (j=0; j<nWidth; j++)
        {
           pBmpData[0] = pRgbaData[0];
           pBmpData[1] = pRgbaData[1];
           pBmpData[2] = pRgbaData[2];
           pRgbaData  += format;
           pBmpData   += FORMAT_RGB;
        }
        //pack for 4 bytes
        pBmpData +=(bytesPerLine - nWidth*FORMAT_RGB);
    }
    fwrite(pBmpSource, pixcelBytes, 1, fp);
 
    /** close and release。 */
    fclose(fp);
    free(pBmpSource);
 
    return 0;
}

int rgbToJpgFile(const char *pFileName, PBitmap* bmp){
    
    return 0;
}

int rgbToPngFile(const char *pFileName, PBitmap* bmp){
    int  nWidth  = bmp->w;
    int  nHeight = bmp->h;
    uint size = nWidth * nHeight;
    char* data =(char*)malloc(sizeof(uchar)*nWidth*nHeight*FORMAT_RGB);
    RGB* rgb = bmp->data;
    for(int i=0; i<size; ++i){
        uchar r = rgb[i].b;
        rgb[i].b = rgb[i].r;
        rgb[i].r = r;
    }
    char* pRgbData = (char*)bmp->data;
    lodepng_encode24_file(pFileName, pRgbData, nWidth, nHeight);
    return 0;
}
