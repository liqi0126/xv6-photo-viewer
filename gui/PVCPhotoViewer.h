#ifndef PVCPHOTOVIEWER_H
#define PVCPHOTOVIEWER_H

#include"PVC.h"
#include"PVCDecodeJPEG.h"
#include"math.h"
#include "PVCDecodePNG.h"
PBitmap m_hBackgroundBmp;
PBitmap m_hBuildingBmp;
int  nowBmp=0;

struct photoNode
{
	int tag1;
	int tag2;
	char *name;
	int type;
	struct photoNode* prev;
	struct photoNode* next;
};

struct photoList
{
	struct photoNode* head;
	struct photoNode* tail;
	struct photoNode* nd;
};

enum IMGTYPE{
	JPG,
	BMP,
	PNG,
	NOTIMG,
};

typedef struct
{
	PBitmap	hBmp;
	PPoint	pos;
	PSize	size;
}ReducePhoto, EnlargePhoto, LastPhoto, NextPhoto, Clockwise30, Anticlockwise30, Clockwise90, Anticlockwise90, Rotate180, OpenFile;

ReducePhoto CreateResIconReducePhoto(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp);

EnlargePhoto CreateResIconEnlargePhoto(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp);

LastPhoto CreateResIconLastPhoto(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp);

NextPhoto CreateResIconNextPhoto(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp);

Clockwise30 CreateResIconClockwise30(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp);

Anticlockwise30 CreateResIconAnticlockwise30(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp);

Clockwise90 CreateResIconClockwise90(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp);

Anticlockwise90 CreateResIconAnticlockwise90(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp);

Rotate180 CreateResIconRotate180(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp);

OpenFile CreateResIconOpenFile(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp);

void Render(PHwnd hwnd);

/*声明位图句柄*/
// Building	m_building;
PBitmap m_hReduceBmp;
PBitmap m_hEnlargeBmp;
PBitmap m_hLastPhotoBmp;
PBitmap m_hNextPhotoBmp;
PBitmap m_hClockwise30DegreeBmp;
PBitmap m_hAnticlockwise30DegreeBmp;
PBitmap m_hClockwise90DegreeBmp;
PBitmap m_hAnticlockwise90DegreeBmp;
PBitmap m_h180DegreeBmp;
PBitmap m_hOpenFileBmp;

/*状态*/
ReducePhoto m_reducePhoto;
EnlargePhoto m_enlargePhoto;
LastPhoto m_lastPhoto;
NextPhoto m_nextPhoto;
Clockwise30 m_clockwise30Degree;
Anticlockwise30 m_anticlockwise30Degree;
Clockwise90 m_clockwise90Degree;
Anticlockwise90 m_anticlockwise90Degree;
Rotate180 m_rotate180Degree;
OpenFile m_openFile;

struct photoList *PL;
bool wndProc(PHwnd hwnd, PMessage msg);
void keydown(PHwnd hwnd, PMessage msg);
void draw(PHwnd hwnd);
void init(PHwnd hwnd);
void photoListInit(struct photoList* pl);
void addPhotoByFileName(char *s,struct photoList* pl);
void update(PHwnd hwnd,char * name);
PHdc pvcLeftRotatePdc(PHdc src);
PHdc pvcRightRotatePdc(PHdc src);
PBitmap loadJpg(char *filename);
PBitmap loadPng(char *filename);
int type(char *filename);
PHdc enlarge(PHdc src);
PBitmap loadImg(char* filename);

void initWithArgs(PHwnd hwnd,char* filename);

void LButtonDownReducePhoto(PHwnd hwnd, PMessage msg);

void LButtonDownEnlargePhoto(PHwnd hwnd, PMessage msg);

void LButtonDownLastPhoto(PHwnd hwnd, PMessage msg);

void LButtonDownNextPhoto(PHwnd hwnd, PMessage msg);

void LButtonDownClockwise30(PHwnd hwnd, PMessage msg);

void LButtonDownAnticlockwise30(PHwnd hwnd, PMessage msg);

void LButtonDownClockwise90(PHwnd hwnd, PMessage msg);

void LButtonDownAnticlockwise90(PHwnd hwnd, PMessage msg);

void LButtonDownRotate180(PHwnd hwnd, PMessage msg);

void LButtonDownOpenFile(PHwnd hwnd, PMessage msg);

void leftbuttondown(PHwnd hwnd, PMessage msg);









#endif
