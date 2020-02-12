#pragma once

#include <stdio.h>
#include <math.h>
#include <fenv.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include "YUV_tables.h"






int max(int a, int b)
{
	if (a > b)
		return a;
	
	return b;
	
}

int min(int a, int b)
{
	if (a < b)
		return a;
	return b;
}

void YCbCr2rgb(int Y,int Cb, int Cr, int *Red, int *Green, int *Blue)
{
	int R, G, B;
	int float_factor = 10000;
	int biggest = 255 * float_factor;
	int smallest = 0;
	
	R	= RGB_db_term_one[Y] + RED_db_term_two[Cr];
	G	= RGB_db_term_one[Y] - GREEN_db_term_two[Cb] - GREEN_db_term_three[Cr];
	B	= RGB_db_term_one[Y] + BLUE_db_term_two[Cb];
	
	*Red	= max(smallest, min(R, biggest)) / float_factor;
	*Green	= max(smallest, min(G, biggest)) / float_factor;
	*Blue	= max(smallest, min(B, biggest)) / float_factor;
}

void slow_YCbCr2rgb(int Y, int Cb, int Cr, int *Red, int *Green, int *Blue)
{
	int R, G, B;
	
	int biggest = 255*10000;
	int smallest = 0;
	
	B	= 1164*(Y - 16) + 2018*(Cb - 128);
	G	= 1164*(Y - 16) - 813*(Cr - 128) - 391*(Cb - 128);
	R	= 1164*(Y - 16) + 1596*(Cr - 128);
	
	*Red	= max(smallest, min(R, biggest)) / 10000;
	*Green	= max(smallest, min(G, biggest)) / 10000;
	*Blue	= max(smallest, min(B, biggest)) / 10000;
}

void rgb_from_yuyv_array(unsigned char array[],int idx, int *Red, int *Green, int *Blue)
{
	int Y = 0;
	int Cb = 0;
	int Cr = 0;
	int block_idx = (idx / 2) * 4;
	if (idx%2==0)
	{
		Y = array[block_idx];
	}
	else
	{
		Y = array[block_idx+2];
	}
	Cb = array[block_idx + 1];
	Cr = array[block_idx + 3];
	YCbCr2rgb(Y, Cb, Cr, Red, Green, Blue);
	
}
