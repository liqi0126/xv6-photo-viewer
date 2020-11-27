#include "PVCTurnScale.h"

double myabs(double x) 
{
	if (x > 0) return x;
	return -x;
}

double mysin(double x)
{
    double fl = 1;
    if (x > 2 * PI || x < -2 * PI) x -= (int)(x / (2 * PI)) * 2 * PI;
    if (x > PI) x -= 2 * PI;
    if (x < -PI) x += 2 * PI;
    if (x > PI / 2)
    {
        x -= PI;
        fl *= -1;
    }
    if (x < -PI / 2)
    {
        x += PI;
        fl *= -1;
    }
    if (x < 0)
    {
        x *= -1;
        fl *= -1;
    }//处理区间
    if (x > PI / 4) return fl * mycos(PI / 2 - x);
    else return fl * (x - mypow(x, 3) / 6 + mypow(x, 5) / 120 - mypow(x, 7) / 5040 + mypow(x, 9) / 362880);//泰勒公式
}

double mycos(double x)
{
    double fl = 1;
    if (x > 2 * PI || x < -2 * PI) x -= (int)(x / (2 * PI)) * 2 * PI;
    if (x > PI) x -= 2 * PI;
    if (x < -PI) x += 2 * PI;
    if (x > PI / 2)
    {
        x -= PI;
        fl *= -1;
    }
    if (x < -PI / 2)
    {
        x += PI;
        fl *= -1;
    }//处理区间
    if (x > PI / 4) return fl * mysin(PI / 2 - x);
    else return fl * (1 - mypow(x, 2) / 2 + mypow(x, 4) / 24 - mypow(x, 6) / 720 + mypow(x, 8) / 40320);//泰勒公式
}


double mypow(double a, int b)
{
	float r = a;
	if (b > 0) {
		while (--b)
			r *= a;
	}
	else if (b < 0) {
		while (++b)
			r *= a;
		r = 1.0 / r;
	}
	else r = 0;
	return r;
}

float mysqrt(float number) {
	float new_guess;
	float last_guess;

	if (number < 0) {
		return -1;
	}

	new_guess = 1;
	do {
		last_guess = new_guess;
		new_guess = (last_guess + number / last_guess) / 2;
	} while (new_guess != last_guess);

	return new_guess;
}