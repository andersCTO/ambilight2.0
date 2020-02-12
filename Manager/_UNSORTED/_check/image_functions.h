#pragma once
#include "type_setup.h"
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <vector>

using namespace std;



void save_matrix(vector<MacroPixel> & I, int idx, int I_width, int I_height)
{
	//adaf
	char Y_filename[]	= "Y000"; 
	char Cb_filename[]	= "Cb000";
	char Cr_filename[]	= "Cr000";
	
	sprintf(Y_filename, "Y%03d", idx);
	sprintf(Cb_filename, "Cb%03d", idx);
	sprintf(Cr_filename, "Cr%03d", idx);
	
	FILE * Y = fopen(Y_filename, "w");
	FILE * Cb = fopen(Cb_filename, "w");
	FILE * Cr = fopen(Cr_filename, "w");
	
	
	for (int i = 0; i < I_height; i++)
	{
		
		for (int j = 0; j < I_width; j++)
		{
			fprintf(Y, "%i\t", *I[j*I_height + i].Y);
			fprintf(Cb, "%i\t", *I[j*I_height + i].Cb);//[col*Image_height + row]
			fprintf(Cr, "%i\t", *I[j*I_height + i].Cr);
			
			
			//fprintf(Y, "%i\t", *I[j*I_height + i].Y);
			//fprintf(Cb, "%i\t", *(I[j*I_height + i].Cb));//[col*Image_height + row]
			//fprintf(Cr, "%i\t", *(I[j*I_height + i].Cr));
		}
		fprintf(Y, "\n");
		fprintf(Cb, "\n");
		fprintf(Cr, "\n");
		
	}
	fclose(Y);
	fclose(Cb);
	fclose(Cr);
}

void crop_image(int *p1, int *p2, const vector<MacroPixel> & I, vector<MacroPixel> & crop, int new_width, int new_height)
{
	crop.resize(new_width*new_height);
	int crop_idx = 0;
	for (int row = p1[1]; row < p2[1]; row++)
	{
		for (int col = p1[0]; col < p2[0]; col++)
		{
			crop[crop_idx].Y = I[col*Image_height + row].Y;
			crop[crop_idx].Cb = I[col*Image_height + row].Cb;
			crop[crop_idx].Cr = I[col*Image_height + row].Cr;
			crop_idx++;
		}
	}
	
}