#pragma once

#include <stdio.h>
#include <math.h>
#include <fenv.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//#define VIDEO_TYPE 'N'// P(AL) or N(TSC)

//Set image dimensions
#define Image_height 480 //NTSC
//#define Image_height 576 //PAL
#define Image_width 720

#define PIXEL_SIZE Image_width*Image_height//Number of pixels

#define RGB_height Image_height * 3
#define RGB_width Image_width * 3

#define Macro_pixel_size PIXEL_SIZE*2 

#define CbCr PIXEL_SIZE/2 //CbCr Resolution
#define UTV007_DIR "/dev/video0"






int Ipos2v(int x, int y)
{
	return y + Image_height*x;
}

void DataCapture(FILE *camera, unsigned char Y[], unsigned char Cb[], unsigned char Cr[])
{
	int i=0, j=0, k=0;//Set variables
	unsigned char data[Macro_pixel_size];
	
	
	fread(data, sizeof(data[0]), Macro_pixel_size, camera);
	
	while (i < Macro_pixel_size)
	{
		
		Y[j]	= data[i];
		Cb[k]	= data[i + 1];
		Y[j + 1] = data[i + 2];
		Cr[k]	= data[i + 3];				
			
		i += 4;
		j += 2;
		k++;
	}
}



void simple_down_sample(int* data, int data_size,int *new_data,int new_data_size)
{
	//Ratio between new and olda data size
	float ratio = (float)data_size / (float)new_data_size;
	//printf("\nDownsample Ratio: %.2f", ratio);
	
	//Find sample points
	int *data_points = (int*)malloc(sizeof(int)*new_data_size / 3);
	int i;
	
	//printf("\nData Points: ");
	for (i = 0; i < (new_data_size / 3); i++)
	{
		
		data_points[i] = (int)round(ratio*((float)i + 1.00 / 2.00));
		//printf("%i ", data_points[i]);
	}
	i = 0;
	int k = 0;
	while (i < new_data_size)
	{
		new_data[i]   = data[3 * data_points[k]];
		new_data[i+1] = data[3 * data_points[k] + 1];
		new_data[i+2] = data[3 * data_points[k] + 2];
		
		i += 3;
		k++;
	}
	
}
