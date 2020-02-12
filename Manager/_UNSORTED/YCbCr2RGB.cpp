#include <iostream>
#include <algorithm>


#include "general_types.h"
#include "YCbCr2RGB.h"

//#define clip(n, lower, upper) if (n < lower) n= lower; else if (n > upper) n= upper

using namespace std;

static inline char clip(float n)
{
	if (n < 0)
	{
		return 0;
	}
	else if (n > 255)
	{
		return 255;
	}
	return n;
}


int YCbCr2RGB(unsigned char* Y, unsigned char* Cb, unsigned char* Cr, rgb_value*RGB)
{
	RGB->red	= clip(FIRST_TERM[*Y]						+ R_THIRD_TERM[*Cr]);//RED
	RGB->green	= clip(FIRST_TERM[*Y]	- G_SECOND_TERM[*Cb]	- G_THIRD_TERM[*Cr]);
	RGB->blue	= clip(FIRST_TERM[*Y]	+ B_SECOND_TERM[*Cb]);

	return 0;
}