#include "math.h"

static unsigned int g_seed = 0;

int fastrand() {
    printf(1, "G seed %d\n", g_seed);
    ++g_seed;
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}