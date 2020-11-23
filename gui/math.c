#include "math.h"
#define pi 3.1415926535898 
double abs(double x)
{
	if (x < 0)
		return x * -1;
	else
		return x;
}

double pow(double x, double y)
{
	if(x==0 && y!=0) return 0;
	else if(x==0 && y==0) return 1;
	else if(y<0) return 1/pow(x,-y);//把指数小于0的情况转为1/x^-y计算
	else if(x<0 && y-(int)y!=0) return 0;//若x为负，且y不为整数数，则出错，返回0  
	else if(x<0 && y-(int)y==0)//若x为负，且y为整数数，则用循环计算 
	{
		double powint=1;
		int i;
		for(i=1;i<=y;i++) powint*=x;
		return powint;
	}
	return exp(y*ln(x));
}

double sqrt(double x)
{
    long i;
	float x2, y;
	const float threehalfs = 1.5F; 
	x2 =  (float)x * 0.5F;
	y   = x;
	i   = * ( long * ) & y;
	// evil floating point bit level hacking
	
	i   = 0x5f3759df - ( i >> 1 );
	// what the fuck?
	y   = * ( float * ) & i;
	y   = y * ( threehalfs - ( x2 * y * y ) );
	// 1st iteration 
	y   = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed
	return (double)1/y;
}

// ln(x) = 2 arctanh((x-1)/(x+1))
// 调用了Arctanh(double) 方法
double ln(double x)
{
	double y=x-1,ln_p1=0,ln_p2=0,ln_p3=0,ln_px=0,ln_tmp=1,dln_px=1,tmp;
	int l;
	if(x==1) return 0;
	else if(x>2) return -ln(1/x);
	else if(x<.1)
	{
		double n=-1;
		double a;
		do
		{
			n=n-.6;
			a=x/exp(n);
		}
		while(a>2 || a<1);
		return ln(a)+n;
	}
	for(l=1,tmp=1;(ln_px-ln_tmp)>1e-9 || (ln_px-ln_tmp)<-1e-9;l++)
	{
		ln_tmp=ln_px;
		tmp*=y;
		if(l==1) tmp=tmp/l;
		else tmp=tmp/-l;
		ln_p1+=tmp;
		ln_p2=ln_p1+-1*tmp*y*l/(l+1);
		ln_p3=ln_p2+tmp*y*y*l/(l+2);
		dln_px=ln_p3-ln_p2;
		ln_px=ln_p3-dln_px*dln_px/(ln_p3-2*ln_p2+ln_p1);
		tmp*=l;
	}
	return ln_px;
}

// 求e^x 用于Pow( double, double )调用
// e^x = 1+x+(x^2)/2!+(x^3)/3!+...
// 精度为7位
double exp( double x )
{
	double y=x,ex_p1=0,ex_p2=0,ex_p3=0,ex_px=0,ex_tmp=1,dex_px=1,tmp;
	int l;
	if(x==0) return 1;
	if(x<0) return 1/exp(-x); 
	for(l=1,tmp=1;((ex_px-ex_tmp)>1e-10 || (ex_px-ex_tmp)<-1e-10) && dex_px>1e-10;l++)
	{
		ex_tmp=ex_px;
		tmp*=y;
		tmp=tmp/l;
		ex_p1+=tmp;
		ex_p2=ex_p1+tmp*y/(l+1);
		ex_p3=ex_p2+tmp*y*y/(l+1)/(l+2);
		dex_px=ex_p3-ex_p2;
		ex_px=ex_p3-dex_px*dex_px/(ex_p3-2*ex_p2+ex_p1);
	}
	return ex_px+1;
}

int toInt(double x)
{
	int result;
	int intPart = (int)x;
	double pointPart;
	if(x>=0)
	{
		pointPart = x - intPart;
		if(pointPart>=0.5)
		{
			result = intPart + 1;
		}else
		{
			result = intPart;
		}
	}else
	{
		pointPart = intPart - x;
		if(pointPart>=0.5)
		{
			result = intPart - 1;
		}else
		{
			result = intPart;
		}
	}
	return result;
}

double sin(double x)
{
    double fl = 1;
    if(x>2*pi || x<-2*pi) x -= (int)(x/(2*pi))*2*pi;
    if(x>pi) x -= 2*pi;
    if(x<-pi) x += 2*pi;
    if(x>pi/2)
    {
        x -= pi;
        fl *= -1;
    }
    if(x<-pi/2)
    {
        x += pi;
        fl *= -1;
    }
    if(x<0)
    {
        x*=-1;
        fl*=-1;
    }//处理区间
    if(x>pi/4) return fl*cos(pi/2-x);
    else return fl*(x - pow(x,3)/6 + pow(x,5)/120 - pow(x,7)/5040 +pow(x,9)/362880);//泰勒公式
}

double cos(double x)
{
    double fl = 1;
    if(x>2*pi || x<-2*pi) x -= (int)(x/(2*pi))*2*pi;
    if(x>pi) x -= 2*pi;
    if(x<-pi) x += 2*pi;
    if(x>pi/2)
    {
        x -= pi;
        fl *= -1;
    }
    if(x<-pi/2)
    {
        x += pi;
        fl *= -1;
    }//处理区间
    if(x>pi/4) return fl*sin(pi/2-x);
    else return fl*(1 - pow(x,2)/2 + pow(x,4)/24 - pow(x,6)/720 + pow(x,8)/40320);//泰勒公式
}

double tan(double x)  
{  
	return sin(x)/cos(x);  
} 