#include "PVCPhotoViewer.h"
#define pi 3.1415926535898 

struct photoNode* now;
PHdc hdcBmp;

int WDHEIGHT=500;
int WDWIDTH=500;

PBitmap nBmp;
PBitmap orgnBmp;
struct DisplayState{
	double sizeRate;
	double degree;
}DisplayState = {1,0};

double degreeArray[12] = {0, pi/6, 2*pi/6, 3*pi/6, 4*pi/6, 5*pi/6, 6*pi/6, 7*pi/6, 8*pi/6, 9*pi/6, 10*pi/6, 11*pi/6};
int degreeIndex = 0;

unsigned char * c;
int imgsize;
int _argc=1;
char * _argv;

PBitmap loadJpg(char *filename)
{
	char ZZ[64] = { 0, 1, 8, 16, 9, 2, 3, 10, 17, 24, 32, 25, 18,
        11, 4, 5, 12, 19, 26, 33, 40, 48, 41, 34, 27, 20, 13, 6, 7, 14, 21, 28, 35,
        42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51, 58, 59, 52, 45,
        38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63 };
    Context* ctx=malloc(sizeof(struct Context));
    memset(ctx, 0, sizeof(Context));

    int f;
    f = open(filename, O_RDONLY);
    unsigned char * buf = (unsigned char *)malloc(MAX_JPEG_FILE_SIZE/*size*/);

    read(f, buf, MAX_JPEG_FILE_SIZE/*size*/);
    
    close(f);

    _Decode(ctx, ZZ, buf, MAX_JPEG_FILE_SIZE/*size*/);

   
	PBitmap bmp;
	bmp.height=0;
	bmp.width=0;
	bmp.data=0;
	
	int imgsize = GetImageSize(ctx);
    	unsigned char * c = GetImage(ctx);
    	bmp.width = GetWidth(ctx);
    	bmp.height = GetHeight(ctx);
	int n = bmp.width * bmp.height;
    	bmp.data = (PColor *)malloc(sizeof(PColor) * n);
   	for (int i = 0; i < imgsize; i += 3)
   	{
      		  bmp.data[i / 3].r = c[i];
     	 	  bmp.data[i / 3].g = c[i + 1];
      		  bmp.data[i / 3].b = c[i + 2];
   	}

	// printf(1,"bmp size %d %d",bmp.width,bmp.height);
	return bmp;
}

//由文件名读取png返回Bitmap句柄的函数
PBitmap loadPng(char *filename){
    // unsigned char * buf = (unsigned char *)malloc(MAX_JPEG_FILE_SIZE/*size*/);
	unsigned char* image = 0;
  	unsigned width, height;
	//使用适配过的lodepng库读取png文件
	lodepng_decode24_file(&image, &width, &height, filename);
	// int err = lodepng_decode24_file(&image, &width, &height, filename);

	// printf(1, "after loadepng decode 32 file err: %d\n", err);
	// printf(1, "png size width:%d, height:%d\n", width, height);
	PBitmap bmp;
	bmp.height=height;
	bmp.width=width;
	bmp.data=0;

	// int imgsize = width * height;
	int n = bmp.width * bmp.height;
    bmp.data = (PColor *)malloc(sizeof(PColor) * n);
	// printf(1, "create bmp width:%d, height:%d\n", bmp.width,  bmp.height);

	//这里是用decode24,所以是3个3个的读取
   	for (int i = 0; i < n; i += 1)
   	{
		unsigned char r = image[i*3];
		unsigned char g = image[i*3 + 1];
		unsigned char b = image[i*3 + 2];
		bmp.data[i].r = r;
		bmp.data[i].g = g;
		bmp.data[i].b = b;

   	}
	// printf(1, "read all png data.\n");
	free(image);
	return bmp;

}

void photoListInit(struct photoList* pl)
{
	pl->head=0;
	pl->nd=0;
	pl->tail=0;
}
void update(PHwnd hwnd,char * name)
{
	printf(1,"updata name:%s",name);
	orgnBmp=loadImg(name);
	// printf(1,"check bitmap %d %d, height %d width %d",orgnBmp.data[0].r,orgnBmp.data[1].g,orgnBmp.height,orgnBmp.width);
	// printf(1,"orgnBmp %d %d",orgnBmp.width,orgnBmp.height);
	// memcpy(&orgnBmp,&nBmp,sizeof(nBmp));
	hdcBmp = pvcCreateCompatibleDCFromBitmap(orgnBmp);
}
int type(char *filename)
{
	int len=strlen(filename);
	if(filename[len-1]=='g'&&filename[len-2]=='p'&&filename[len-3]=='j') return JPG;
	if(filename[len-1]=='p'&&filename[len-2]=='m'&&filename[len-3]=='b') return BMP;
	if(filename[len-1]=='g'&&filename[len-2]=='n'&&filename[len-3]=='p') return PNG;
	else return NOTIMG;
}

PBitmap loadImg(char* filename)
{
	int t=type(filename);
	PBitmap pm;
	switch(t){
		
	case JPG:
	return loadJpg(filename);
	break;
	
	case BMP:
	return pvcLoadBitmap(filename);
	break;
	
	case PNG:
	return loadPng(filename);
	break;

	default :
	return pm;
	}
	
	
}

PBitmap* resize(PBitmap* src)
{
	if(DisplayState.sizeRate==1.0){ // if resize rate is close to 1 or equal to 1
		return src;
	}
	PBitmap *des = (PBitmap* )malloc(sizeof(PBitmap));
	//printf(1,"herein reduce,%d %d ",src->width,src->height);

	int deswidth=toInt((double)src->width*DisplayState.sizeRate);
	int desheight=toInt((double)src->height*DisplayState.sizeRate);
	des->width=deswidth>WDWIDTH?WDWIDTH:deswidth;
	des->height=desheight>WDHEIGHT?WDHEIGHT:desheight;	
	PColor* data=(PColor *)malloc(des->height*des->width*sizeof(PColor));
	for(int i=0;i<des->height*des->width;i++)
	{
		int l=i/des->width;
		int r=i%des->width;
		int remap_l = toInt((double)l/DisplayState.sizeRate);
		int remap_r = toInt((double)r/DisplayState.sizeRate);
		int orgidx = remap_l*src->width+remap_r;
		data[i].r=src->data[orgidx].r;
		data[i].g=src->data[orgidx].g;
		data[i].b=src->data[orgidx].b;
	}
	des->data = data;
	printf(1,"resize width %d,desheight %d\n",deswidth,desheight);
	return des;
}

PBitmap* rotate(PBitmap* src)
{
	if (DisplayState.degree==0.0){
		return src;
	}
	PBitmap *des = (PBitmap* )malloc(sizeof(PBitmap));
	printf(1,"herein reduce,%d %d \n",src->width,src->height);
	double floatHeight = (double)src->height;
	double floatWidth = (double)src->width;
	double cosTheta = cos(DisplayState.degree);
	double sinTheta = sin(DisplayState.degree);
	int tmpwidth = floatHeight*abs(sinTheta) + floatWidth*abs(cosTheta);   
	int tmpheight = floatHeight*abs(cosTheta) + floatWidth*abs(sinTheta);
	des->width=tmpwidth>WDWIDTH?WDWIDTH:tmpwidth;
	des->height=tmpheight>WDHEIGHT?WDHEIGHT:tmpheight; 
	PColor* data=(PColor *)malloc(des->height*des->width*sizeof(PColor));

	printf(1,"cosTheta: %d  sinTheta: %d\n",toInt(1000*cosTheta),toInt(1000*sinTheta));
	for(int i=0;i<des->height*des->width;i++)
	{
		
		int y0=i/des->width;
		int x0=i%des->width;
		int x1 = x0 - 0.5*des->width;
		int y1 = y0 - 0.5*des->height;
		int x2 = x1*cosTheta + y1*sinTheta; 
		int y2 = x1*sinTheta - y1*cosTheta; 

		int x3 = toInt(x2+0.5*src->width);
		int y3 = toInt(-y2+0.5*src->height);
		int orgIndex = x3 + y3* src->width;
		if(x2<-0.5*src->width||x2>0.5*src->width||y2<-0.5*src->height||y2>0.5*src->height||orgIndex>src->height*src->width){
			data[i].r = 255;
			data[i].g = 255;
			data[i].b = 255;
		}else{
			data[i].r = src->data[orgIndex].r;
			data[i].g = src->data[orgIndex].g;
			data[i].b = src->data[orgIndex].b;
		}
	
	}
	des->data = data;
	return des;	
}

void addPhotoByFileName(char *s,struct photoList* pl)
{
	struct photoNode *n=(struct photoNode*)malloc(sizeof(struct photoNode));
	n->name=s;
	if(pl->tail==0)
	{
		pl->head=n;
		pl->tail=n;
		pl->head->next=0;
		pl->head->prev=0;
	}
	else{
		struct photoNode* f=pl->tail;
		pl->tail->next=n;
		pl->tail=n;
		pl->tail->prev=f;
		pl->tail->next=0;
	}
}

void Render(PHwnd hwnd)
{
	PHdc hdc = pvcGetDC(hwnd);
	hdc->pen.color = COLOR_NULL;
    	hdc->brush.color = RGB(235, 235, 235);
    	pvcDrawRect(hdc, 0, 0, hwnd->dc.size.cx, 52);
	hdc->brush.color = RGB(255, 255, 255);
	pvcDrawRect(hdc, 0, WND_EDGE_SIZE, hwnd->dc.size.cx, hwnd->dc.size.cy);
    	PHdc hdcBmp;
	    hdcBmp = pvcCreateCompatibleDCFromBitmap(m_lastPhoto.hBmp);
		pvcTransparentBit(hdc, m_lastPhoto.pos.x, m_lastPhoto.pos.y,
		hdcBmp, 0, 0, m_lastPhoto.size.cx, m_lastPhoto.size.cy,
		RGB(255, 255, 255));

	    hdcBmp = pvcCreateCompatibleDCFromBitmap(m_nextPhoto.hBmp);
		pvcTransparentBit(hdc, m_nextPhoto.pos.x, m_nextPhoto.pos.y,
		hdcBmp, 0, 0, m_nextPhoto.size.cx, m_nextPhoto.size.cy,
		RGB(255, 255, 255));

	    hdcBmp = pvcCreateCompatibleDCFromBitmap(m_reducePhoto.hBmp);
		pvcTransparentBit(hdc, m_reducePhoto.pos.x, m_reducePhoto.pos.y,
		hdcBmp, 0, 0, m_reducePhoto.size.cx, m_reducePhoto.size.cy,
		RGB(255, 255, 255));

		hdcBmp = pvcCreateCompatibleDCFromBitmap(m_enlargePhoto.hBmp);
		pvcTransparentBit(hdc, m_enlargePhoto.pos.x, m_enlargePhoto.pos.y,
		hdcBmp, 0, 0, m_enlargePhoto.size.cx, m_enlargePhoto.size.cy,
		RGB(255, 255, 255));

	    hdcBmp = pvcCreateCompatibleDCFromBitmap(m_clockwise30Degree.hBmp);
		pvcTransparentBit(hdc, m_clockwise30Degree.pos.x, m_clockwise30Degree.pos.y,
		hdcBmp, 0, 0, m_clockwise30Degree.size.cx, m_clockwise30Degree.size.cy,
		RGB(255, 255, 255));

	    hdcBmp = pvcCreateCompatibleDCFromBitmap(m_anticlockwise30Degree.hBmp);
		pvcTransparentBit(hdc, m_anticlockwise30Degree.pos.x, m_anticlockwise30Degree.pos.y,
		hdcBmp, 0, 0, m_anticlockwise30Degree.size.cx, m_anticlockwise30Degree.size.cy,
		RGB(255, 255, 255));

	    hdcBmp = pvcCreateCompatibleDCFromBitmap(m_clockwise90Degree.hBmp);
		pvcTransparentBit(hdc, m_clockwise90Degree.pos.x, m_clockwise90Degree.pos.y,
		hdcBmp, 0, 0, m_clockwise90Degree.size.cx, m_clockwise90Degree.size.cy,
		RGB(255, 255, 255));

		hdcBmp = pvcCreateCompatibleDCFromBitmap(m_anticlockwise90Degree.hBmp);
		pvcTransparentBit(hdc, m_anticlockwise90Degree.pos.x, m_anticlockwise90Degree.pos.y,
		hdcBmp, 0, 0, m_anticlockwise90Degree.size.cx, m_anticlockwise90Degree.size.cy,
		RGB(255, 255, 255));

		hdcBmp = pvcCreateCompatibleDCFromBitmap(m_rotate180Degree.hBmp);
		pvcTransparentBit(hdc, m_rotate180Degree.pos.x, m_rotate180Degree.pos.y,
		hdcBmp, 0, 0, m_rotate180Degree.size.cx, m_rotate180Degree.size.cy,
		RGB(255, 255, 255));

		hdcBmp = pvcCreateCompatibleDCFromBitmap(m_openFile.hBmp);
		pvcTransparentBit(hdc, m_openFile.pos.x, m_openFile.pos.y,
		hdcBmp, 0, 0, m_openFile.size.cx, m_openFile.size.cy,
		RGB(255, 255, 255));
}

bool wndProc(PHwnd hwnd, PMessage msg)
{
	switch(msg.type)
	{
	case MSG_CREATE:
		//加载图片的位图
		m_hReduceBmp =  pvcLoadBitmap("reduce.bmp");
		m_hEnlargeBmp =  pvcLoadBitmap("enlarge.bmp");
		m_hLastPhotoBmp = pvcLoadBitmap("left.bmp");
		m_hNextPhotoBmp = pvcLoadBitmap("right.bmp");
		m_hClockwise30DegreeBmp = pvcLoadBitmap("clockwise30.bmp");
		m_hAnticlockwise30DegreeBmp = pvcLoadBitmap("anticlockwise30.bmp");
		m_hClockwise90DegreeBmp = pvcLoadBitmap("clockwise90.bmp");
		m_hAnticlockwise90DegreeBmp = pvcLoadBitmap("anticlockwise90.bmp");
		m_h180DegreeBmp = pvcLoadBitmap("rotate.bmp");
		m_hOpenFileBmp = pvcLoadBitmap("file.bmp");

		//创建图片位置
		m_lastPhoto = CreateResIconLastPhoto(2, 202, 48, 48, m_hLastPhotoBmp);
		m_nextPhoto = CreateResIconNextPhoto(445, 202, 48, 48, m_hNextPhotoBmp);
		m_reducePhoto = CreateResIconReducePhoto(102, 2, 48, 48, m_hReduceBmp);
		m_enlargePhoto = CreateResIconEnlargePhoto(152, 2, 48, 48, m_hEnlargeBmp);
		m_anticlockwise30Degree = CreateResIconAnticlockwise30(202, 2, 48, 48, m_hAnticlockwise30DegreeBmp);
		m_clockwise30Degree = CreateResIconClockwise30(252, 2, 48, 48, m_hClockwise30DegreeBmp);
		m_anticlockwise90Degree = CreateResIconAnticlockwise90(302, 2, 48, 48, m_hAnticlockwise90DegreeBmp);
		m_clockwise90Degree = CreateResIconClockwise90(352, 2, 48, 48, m_hClockwise90DegreeBmp);
		m_rotate180Degree = CreateResIconRotate180(402, 2, 48, 48, m_h180DegreeBmp);
		m_openFile = CreateResIconOpenFile(52, 2, 48, 48, m_hOpenFileBmp);

		if (_argc==1){
		init(hwnd);
		}
		else{
			initWithArgs(hwnd,_argv);
		}

		break;
	case MSG_PAINT:
		Render(hwnd);
		draw(hwnd);
		break;
	case MSG_LBUTTON_DOWN:
		//左鼠标事件
		leftbuttondown(hwnd, msg);
		break;
	case MSG_KEY_DOWN:
		keydown(hwnd, msg);
		break;
	case MSG_WHEEL_UP:
		pvcInvalidate(hwnd);
		break;
	default:
		break;
	}
	return pvcWndProc(hwnd, msg);
}

void leftbuttondown(PHwnd hwnd, PMessage msg)
{
	PPoint ptMouse;
	ptMouse.x = ((msg.param >> 16) & 0xffff) - hwnd->clientPos.x;
	ptMouse.y = (msg.param & 0xffff) - hwnd->clientPos.y;
	PHdc des= (PHdc) malloc(sizeof(PDc));
	des->pen.size=0;
	des->pen.color=des->brush.color=des->font.color=COLOR_NULL;
	PBitmap* curBmp;
	// LButtonDownReducePhoto(hwnd, msg);
	if (ptMouse.x > m_reducePhoto.pos.x && ptMouse.x < m_reducePhoto.pos.x + m_reducePhoto.size.cx && ptMouse.y > m_reducePhoto.pos.y && ptMouse.y < m_reducePhoto.pos.y + m_reducePhoto.size.cy)
	{
		DisplayState.sizeRate*=0.8;
		// curBmp=resize(&orgnBmp);
	}
	// LButtonDownEnlargePhoto(hwnd, msg);
	if (ptMouse.x > m_enlargePhoto.pos.x && ptMouse.x < m_enlargePhoto.pos.x + m_enlargePhoto.size.cx && ptMouse.y > m_enlargePhoto.pos.y && ptMouse.y < m_enlargePhoto.pos.y + m_enlargePhoto.size.cy)
	{
		DisplayState.sizeRate*=1.25;
		// curBmp=resize(&orgnBmp);			
	}
	// LButtonDownLastPhoto(hwnd, msg);
	if (ptMouse.x > m_lastPhoto.pos.x && ptMouse.x < m_lastPhoto.pos.x + m_lastPhoto.size.cx && ptMouse.y > m_lastPhoto.pos.y && ptMouse.y < m_lastPhoto.pos.y + m_lastPhoto.size.cy)
	{
		if(now->prev!=0) now=now->prev;
		update(hwnd,now->name);
		DisplayState.sizeRate = 1;
		degreeIndex = 0;
		DisplayState.degree = degreeArray[degreeIndex];
		// curBmp=resize(&orgnBmp);
	}
	// LButtonDownNextPhoto(hwnd, msg);
	if (ptMouse.x > m_nextPhoto.pos.x && ptMouse.x < m_nextPhoto.pos.x + m_nextPhoto.size.cx && ptMouse.y > m_nextPhoto.pos.y && ptMouse.y < m_nextPhoto.pos.y + m_nextPhoto.size.cy)
	{
		if(now->next!=0) now=now->next;
		update(hwnd,now->name);
		DisplayState.sizeRate = 1;
		degreeIndex = 0;
		DisplayState.degree = degreeArray[degreeIndex];
		// curBmp=resize(&orgnBmp);
	}
	// LButtonDownClockwise30(hwnd, msg);
	if (ptMouse.x > m_clockwise30Degree.pos.x && ptMouse.x < m_clockwise30Degree.pos.x + m_clockwise30Degree.size.cx && ptMouse.y > m_clockwise30Degree.pos.y && ptMouse.y < m_clockwise30Degree.pos.y + m_clockwise30Degree.size.cy)
	{
		degreeIndex = (degreeIndex+1)%12;
		printf(1,"degree index %d\n",degreeIndex);
		DisplayState.degree = degreeArray[degreeIndex];
		// curBmp=rotate(&orgnBmp);
	}
	// LButtonDownAnticlockwise30(hwnd, msg);
	if (ptMouse.x > m_anticlockwise30Degree.pos.x && ptMouse.x < m_anticlockwise30Degree.pos.x + m_anticlockwise30Degree.size.cx && ptMouse.y > m_anticlockwise30Degree.pos.y && ptMouse.y < m_anticlockwise30Degree.pos.y + m_anticlockwise30Degree.size.cy)
	{
		if(degreeIndex == 0)
		{
			degreeIndex = 11;
		}else
		{
			degreeIndex = (degreeIndex-1)%12;
		}
		printf(1,"degree index %d\n",degreeIndex);
		DisplayState.degree = degreeArray[degreeIndex];
		// curBmp=rotate(&orgnBmp);
	}
	// LButtonDownClockwise90(hwnd, msg);
	if (ptMouse.x > m_clockwise90Degree.pos.x && ptMouse.x < m_clockwise90Degree.pos.x + m_clockwise90Degree.size.cx && ptMouse.y > m_clockwise90Degree.pos.y && ptMouse.y < m_clockwise90Degree.pos.y + m_clockwise90Degree.size.cy)
	{
		degreeIndex = (degreeIndex+3)%12;
		printf(1,"degree index %d\n",degreeIndex);
		DisplayState.degree = degreeArray[degreeIndex];
		// curBmp=rotate(&orgnBmp);
	}
	// LButtonDownAnticlockwise90(hwnd, msg);
	if (ptMouse.x > m_anticlockwise90Degree.pos.x && ptMouse.x < m_anticlockwise90Degree.pos.x + m_anticlockwise90Degree.size.cx && ptMouse.y > m_anticlockwise90Degree.pos.y && ptMouse.y < m_anticlockwise90Degree.pos.y + m_anticlockwise90Degree.size.cy)
	{
		degreeIndex = (degreeIndex+9)%12;
		printf(1,"degree index %d\n",degreeIndex);
		DisplayState.degree = degreeArray[degreeIndex];
		// curBmp=rotate(&orgnBmp);
	}
	// LButtonDownRotate180(hwnd, msg);
	if (ptMouse.x > m_rotate180Degree.pos.x && ptMouse.x < m_rotate180Degree.pos.x + m_rotate180Degree.size.cx && ptMouse.y > m_rotate180Degree.pos.y && ptMouse.y < m_rotate180Degree.pos.y + m_rotate180Degree.size.cy)
	{
		degreeIndex = (degreeIndex+6)%12;
		printf(1,"degree index %d\n",degreeIndex);
		DisplayState.degree = degreeArray[degreeIndex];
		// curBmp=rotate(&orgnBmp);	
	}
	// LButtonDownOpenFile(hwnd, msg);
	if (ptMouse.x > m_openFile.pos.x && ptMouse.x < m_openFile.pos.x + m_openFile.size.cx && ptMouse.y > m_openFile.pos.y && ptMouse.y < m_openFile.pos.y + m_openFile.size.cy)
	{
		
	}
	curBmp=resize(&orgnBmp);
	curBmp=rotate(curBmp);
	des->size.cx = curBmp->width;
	des->size.cy = curBmp->height;
	des->content=curBmp->data;
	hdcBmp = des;
	pvcInvalidate(hwnd);
}

void keydown(PHwnd hwnd, PMessage msg)
{
	
	PHdc des= (PHdc) malloc(sizeof(PDc));
	des->pen.size=0;
	des->pen.color=des->brush.color=des->font.color=COLOR_NULL;
	PBitmap* curBmp;
	switch(msg.param)
	{
	case VK_UP:
		if(now->prev!=0) now=now->prev;
		update(hwnd,now->name);
		DisplayState.sizeRate = 1;
		degreeIndex = 0;
		DisplayState.degree = degreeArray[degreeIndex];
		break;
	case VK_DOWN:
		if(now->next!=0) now=now->next;
		update(hwnd,now->name);
		DisplayState.sizeRate = 1;
		degreeIndex = 0;
		DisplayState.degree = degreeArray[degreeIndex];
		break;
	case VK_LEFT:
		if(degreeIndex == 0)
		{
			degreeIndex = 11;
		}else
		{
			degreeIndex = (degreeIndex-1)%12;
		}
		printf(1,"degree index %d\n",degreeIndex);
		DisplayState.degree = degreeArray[degreeIndex];
		break;
	case VK_RIGHT:
		degreeIndex = (degreeIndex+1)%12;
		printf(1,"degree index %d\n",degreeIndex);
		DisplayState.degree = degreeArray[degreeIndex];
		break;
	case VK_F1:
		DisplayState.sizeRate*=0.8;
		break;
	case VK_F2:
		DisplayState.sizeRate*=1.25;
		break;
	default:
		return;
	}
	curBmp=resize(&orgnBmp);
	curBmp=rotate(curBmp);
	des->size.cx = curBmp->width;
	des->size.cy = curBmp->height;
	des->content=curBmp->data;
	hdcBmp = des;
	pvcInvalidate(hwnd);
}

// 图标加载函数
ReducePhoto CreateResIconReducePhoto(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp)
{
	ReducePhoto nextMusic;
	nextMusic.pos.x = posX;
	nextMusic.pos.y = posY;
	nextMusic.size.cx = sizeX;
	nextMusic.size.cy = sizeY;
	nextMusic.hBmp = hBmp;
	return nextMusic;
}

EnlargePhoto CreateResIconEnlargePhoto(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp)
{
	EnlargePhoto nextMusic;
	nextMusic.pos.x = posX;
	nextMusic.pos.y = posY;
	nextMusic.size.cx = sizeX;
	nextMusic.size.cy = sizeY;
	nextMusic.hBmp = hBmp;
	return nextMusic;
}

LastPhoto CreateResIconLastPhoto(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp)
{
	LastPhoto nextMusic;
	nextMusic.pos.x = posX;
	nextMusic.pos.y = posY;
	nextMusic.size.cx = sizeX;
	nextMusic.size.cy = sizeY;
	nextMusic.hBmp = hBmp;
	return nextMusic;
}

NextPhoto CreateResIconNextPhoto(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp)
{
	NextPhoto nextMusic;
	nextMusic.pos.x = posX;
	nextMusic.pos.y = posY;
	nextMusic.size.cx = sizeX;
	nextMusic.size.cy = sizeY;
	nextMusic.hBmp = hBmp;
	return nextMusic;
}

Clockwise30 CreateResIconClockwise30(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp)
{
	Clockwise30 nextMusic;
	nextMusic.pos.x = posX;
	nextMusic.pos.y = posY;
	nextMusic.size.cx = sizeX;
	nextMusic.size.cy = sizeY;
	nextMusic.hBmp = hBmp;
	return nextMusic;
}

Anticlockwise30 CreateResIconAnticlockwise30(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp)
{
	Anticlockwise30 nextMusic;
	nextMusic.pos.x = posX;
	nextMusic.pos.y = posY;
	nextMusic.size.cx = sizeX;
	nextMusic.size.cy = sizeY;
	nextMusic.hBmp = hBmp;
	return nextMusic;
}

Clockwise90 CreateResIconClockwise90(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp)
{
	Clockwise90 nextMusic;
	nextMusic.pos.x = posX;
	nextMusic.pos.y = posY;
	nextMusic.size.cx = sizeX;
	nextMusic.size.cy = sizeY;
	nextMusic.hBmp = hBmp;
	return nextMusic;
}

Anticlockwise90 CreateResIconAnticlockwise90(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp)
{
	Anticlockwise90 nextMusic;
	nextMusic.pos.x = posX;
	nextMusic.pos.y = posY;
	nextMusic.size.cx = sizeX;
	nextMusic.size.cy = sizeY;
	nextMusic.hBmp = hBmp;
	return nextMusic;
}

Rotate180 CreateResIconRotate180(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp)
{
	Rotate180 nextMusic;
	nextMusic.pos.x = posX;
	nextMusic.pos.y = posY;
	nextMusic.size.cx = sizeX;
	nextMusic.size.cy = sizeY;
	nextMusic.hBmp = hBmp;
	return nextMusic;
}

OpenFile CreateResIconOpenFile(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp)
{
	OpenFile nextMusic;
	nextMusic.pos.x = posX;
	nextMusic.pos.y = posY;
	nextMusic.size.cx = sizeX;
	nextMusic.size.cy = sizeY;
	nextMusic.hBmp = hBmp;
	return nextMusic;
}

void init(PHwnd hwnd)
{
	PL=(struct photoList* )malloc(sizeof(struct photoList));
	photoListInit(PL);
	addPhotoByFileName("test.png", PL);
	addPhotoByFileName("test.jpg", PL);
	addPhotoByFileName("Snake.bmp", PL);
	addPhotoByFileName("Clock.bmp", PL);
	addPhotoByFileName("NotePad.bmp", PL);
	addPhotoByFileName("FileManager.bmp", PL);
	addPhotoByFileName("Shell.bmp", PL);
	addPhotoByFileName("last.bmp", PL);
	addPhotoByFileName("next.bmp", PL);
	now=PL->head;
	orgnBmp=loadImg(now->name);
	hdcBmp=pvcCreateCompatibleDCFromBitmap(orgnBmp);
}

void initWithArgs(PHwnd hwnd,char* filename)
{
	PL=(struct photoList* )malloc(sizeof(struct photoList));
	photoListInit(PL);
	addPhotoByFileName(filename, PL);
	now=PL->head;
	orgnBmp=loadImg(now->name);
	hdcBmp=pvcCreateCompatibleDCFromBitmap(orgnBmp);
}

void draw(PHwnd hwnd)
{
	printf(1,"draw ");
	PHdc hdc = pvcGetDC(hwnd);
	pvcTransparentBit(
		hdc, 250-0.5*hdcBmp->size.cx, 250-0.5*hdcBmp->size.cy,
		hdcBmp, 0, 0, hdcBmp->size.cx, hdcBmp->size.cy,
		RGB(255, 255, 255)
		);
}

int main(int argc,char* argv[])
{

  printf(1,"\nPVCPhotoViews got argc %d",argc);
  for(int i =0;i<argc;i++){
	  printf(1,"argv[%d]:%s",i,argv[i]);
  }
  PHwnd hwnd = pvcCreateWindow("PhotoViewer", 0, 100, 100, WDWIDTH, WDHEIGHT);
  if(argc!=1){
	_argc=argc;
	_argv=argv[1];
  }
  pvcWndExec(hwnd, wndProc);
  

  exit();
}
