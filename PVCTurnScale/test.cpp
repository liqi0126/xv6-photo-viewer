#define _CRT_SECURE_NO_WARNINGS
extern "C" {
	#include "PVCTurnScale.h"
}

#include<iostream>
using namespace std;




typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef long LONG;
typedef unsigned char BYTE;

typedef struct  tagBITMAPFILEHEADER {
	//WORD bfType;//文件类型，必须是0x424D，即字符“BM”
	DWORD bfSize;//文件大小
	WORD bfReserved1;//保留字
	WORD bfReserved2;//保留字
	DWORD bfOffBits;//从文件头到实际位图数据的偏移字节数
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	DWORD biSize;//信息头大小
	LONG biWidth;//图像宽度
	LONG biHeight;//图像高度
	WORD biPlanes;//位平面数，必须为1
	WORD biBitCount;//每像素位数
	DWORD  biCompression; //压缩类型
	DWORD  biSizeImage; //压缩图像大小字节数
	LONG  biXPelsPerMeter; //水平分辨率
	LONG  biYPelsPerMeter; //垂直分辨率
	DWORD  biClrUsed; //位图实际用到的色彩数
	DWORD  biClrImportant; //本位图中重要的色彩数
}BITMAPINFOHEADER; //位图信息头定义

typedef struct tagRGBQUAD {
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
}RGBQUAD;

#define LENGTH_NAME_BMP 100
BITMAPFILEHEADER strHead;
BITMAPINFOHEADER strInfo;
RGBQUAD strPla[256];

PBitmap imagedata;

void readBmp()
{
	char strFile[LENGTH_NAME_BMP];//bmp文件名
	cout << "please write filename" << endl;
	cin >> strFile;
	FILE *fpi;
	fpi = fopen(strFile, "rb");

	while (fpi == NULL) {
		cout << "failed" << endl;
		cin >> strFile;
		fpi = fopen(strFile, "rb");
	}

	if (fpi != NULL) {
		//先读取文件类型
		WORD bfType;
		fread(&bfType, 1, sizeof(WORD), fpi);
		if (0x4d42 != bfType)
		{
			cout << "It's not a bmp file!" << endl;
			return;
		}
		//读取bmp文件的文件头和信息头
		fread(&strHead, 1, sizeof(tagBITMAPFILEHEADER), fpi);

		fread(&strInfo, 1, sizeof(tagBITMAPINFOHEADER), fpi);


		imagedata.width = strInfo.biWidth;
		imagedata.height = strInfo.biHeight;
		imagedata.data = (struct PColor*)malloc(imagedata.height * imagedata.width * sizeof(struct PColor));



		//初始化原始图片的像素数组
		for (int i = 0; i < imagedata.height; ++i)
		{
			for (int j = 0; j < imagedata.width; ++j)
			{
				(*(imagedata.data + i * imagedata.width + j)).b = 0;
				(*(imagedata.data + i * imagedata.width + j)).g = 0;
				(*(imagedata.data + i * imagedata.width + j)).r = 0;
			}
		}

		//fseek(fpi,54,SEEK_SET);
		//读出图片的像素数据
		fread(imagedata.data, sizeof(struct PColor) * imagedata.width, imagedata.height, fpi);
		fclose(fpi);
	}
	else
	{
		cout << "failed!" << endl;
		return;
	}

}

void PrintBMP(PBitmap *dstimage)
{
	FILE *fpw;
	char strFile[LENGTH_NAME_BMP];
	cout << "请输入图片名字:" << endl;
	cin >> strFile;
	int length = strlen(strFile);
	//保存bmp图片
	if ((fpw = fopen(strFile, "wb")) == NULL)
	{
		cout << "创建BMP文件失败!" << endl;
		return;
	}

	WORD bfType_w = 0x4d42;
	fwrite(&bfType_w, 1, sizeof(WORD), fpw);
	fwrite(&strHead, 1, sizeof(tagBITMAPFILEHEADER), fpw);
	strInfo.biWidth = dstimage->width;
	strInfo.biHeight = dstimage->height;
	fwrite(&strInfo, 1, sizeof(tagBITMAPINFOHEADER), fpw);


    fwrite(dstimage->data, sizeof(struct PColor) * dstimage->width, dstimage->height, fpw);

	fclose(fpw);
}



int main(){
	readBmp();
    PBitmap dstimage;
	dstimage.width = (int) (imagedata.width );
	dstimage.height = (int) (imagedata.height );
	//getTurnSize(&dstimage.width, &dstimage.height, PI/2);
	dstimage.data=(PColor*)malloc(dstimage.width * dstimage.height * sizeof(PColor));
    //picScale(&imagedata,&dstimage);
	//picTurn(&imagedata, &dstimage, PI*1.5);
	picTurnAround(&imagedata, &dstimage);
    PrintBMP(&dstimage);
	return 0;
}