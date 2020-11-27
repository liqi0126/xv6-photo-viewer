#ifndef PVCTURNSCALE_H
#define PVCTURNSCALE_H

typedef struct PColor
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
}PColor;

typedef struct PBitmap
{
    int width;
    int height;
    PColor * data;
}PBitmap;


int picScale(PBitmap* src, PBitmap* dst);
int mixColor(PBitmap* src, float fx, float fy, PColor* dst);
PColor* getColor(PBitmap* pic, int x, int y, int* isInPic);

double myabs(double x);
double mysin(double x);
double mycos(double x);
double mypow(double a, int b);
float mysqrt(float number);
int picTurn(PBitmap* src, PBitmap* dst, float angle);
int getTurnSize(int* width, int* height, float angle);
int picRollingOver(PBitmap* src, PBitmap* dst);
int picTurnAround(PBitmap* src, PBitmap* dst);
int setColor(PColor* src, PColor* dst);

#define PI 3.1415926536

#endif PVCTURNSCALE_H