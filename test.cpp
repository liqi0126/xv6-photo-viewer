#include<iostream>
using namespace std;
#include<string.h>
#include "PVCTurnScale.h"

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

#define LENGTH_NAME_BMP 30
BITMAPFILEHEADER strHead;
BITMAPINFOHEADER strInfo;

PBitmap imagedata;

void readBmp()
{
	char strFile[LENGTH_NAME_BMP];//bmp文件名
	cout << "请输入所要读取的文件名:" << endl;
	cin >> strFile;
	FILE *fpi;
	fpi = fopen(strFile, "rb");

	while (fpi == NULL) {
		cout << "打开文件失败! 请重新输入文件名。" << endl;
		cin >> strFile;
		fpi = fopen(strFile, "rb");
	}

	if (fpi != NULL) {
		//先读取文件类型
		WORD bfType;
		fread(&bfType, 1, sizeof(WORD), fpi);
		if (0x4d42 != bfType)
		{
			cout << "此文件不是一个BMP文件!" << endl;
			return;
		}
		//读取bmp文件的文件头和信息头
		fread(&strHead, 1, sizeof(tagBITMAPFILEHEADER), fpi);
		//showBmpHead(strHead);//显示文件头
		fread(&strInfo, 1, sizeof(tagBITMAPINFOHEADER), fpi);
		//showBmpInforHead(strInfo);//显示文件信息头

		imagedata.width = strInfo.biWidth;
		imagedata.height = strInfo.biHeight;
		imagedata.data = (PColor*)malloc(imagedata.height * imagedata.width * sizeof(PColor));

		//初始化原始图片的像素数组
		for (int i = 0; i < imagedata.height; ++i)
		{
			for (int j = 0; j < imagedata.width; ++j)
			{
				(*(imagedata.data + i * imagedata.width + j)).b = 0;
				(*(imagedata.data + i * imagedata.width + j)).g = 0;
				(*(imagedata.data + i *  imagedata.width + j)).r = 0;
			}
		}

		//fseek(fpi,54,SEEK_SET);
		//读出图片的像素数据
		fread(imagedata.data, sizeof(struct PColor) * imagedata.width, imagedata.height, fpi);
		fclose(fpi);
	}
	else
	{
		cout << "打开文件失败！" << endl;
		return;
	}

}

void PrintBMP()
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

    fwrite(imagedata.data, 1, sizeof(BYTE), fpw);

	fclose(fpw);
}



int main(){
	readBmp();
    
    PicScale(imagedata,)
    PrintBMP();
	return 1;

}