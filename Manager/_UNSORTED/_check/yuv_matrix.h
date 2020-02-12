#include "funkis.h"
#include <unistd.h>
#include <math.h>
#include "Header.h"

struct MacroPixel
{
	char * Y;
	char * Cb;
	char * Cr;
};

struct Pos
{
	int x;
	int y;
};
typedef struct Pos Pos;

typedef struct MacroPixel MacroPixel;

struct vector_dp
{
	Pos *start;
	Pos *end;
	int length;
};

typedef struct vector_dp vector_dp;



void setup_ycbcr_matrix(MacroPixel matrix[Image_width][Image_height], char*data)
{
	int idx = 0, row = 0, col = 0;
	
	while (idx < Macro_pixel_size)
	{
		matrix[col][row].Y  = &data[idx];
		matrix[col][row].Cb = &data[idx + 1];
		matrix[col][row].Cr = &data[idx + 3];
		
		col++;
		
		matrix[col][row].Y  = &data[idx + 2];
		matrix[col][row].Cb = &data[idx + 1];
		matrix[col][row].Cr = &data[idx + 3];
		
		if (col < Image_width-1)
		{
			col++;
			
		}
		else
		{
			row++;
			//printf("\n%i x %i", col, row);
			col = 0;
		}
		
		idx += 4;
	}
		
}

void save_matrix(MacroPixel matrix[Image_width][Image_height])
{
	FILE * Y = fopen("Y", "w");
	FILE * Cb = fopen("Cb", "w");
	FILE * Cr = fopen("Cr", "w");
	
	
	int i, j;
	
	for (i = 0; i < Image_height; i++)
	{
		for (j = 0; j < Image_width; j++)
		{
			fprintf(Y, "%i\t", *(matrix[j][i].Y));
			fprintf(Cb, "%i\t", *(matrix[j][i].Cb));
			fprintf(Cr, "%i\t", *(matrix[j][i].Cr));
		}
		fprintf(Y, "\n");
		fprintf(Cb, "\n");
		fprintf(Cr, "\n");
	}
	fclose(Y);
	fclose(Cb);
	fclose(Cr);
}

//Returns a vector containing the points of interest between positions A & B
int get_vector_points(Pos *PosA, Pos *PosB,int length,Pos **vector)
{
	int distance,i;
	double downsize_ratio;
	Pos ** full_vector;
	
	//Find orientation of vector
	if (PosA->y == PosB->y)
	{
		
		distance = abs(PosA->x - PosB->x);
		full_vector = malloc((1 + distance)*sizeof(Pos*));
		
		for (i = 0; i <= distance; i++)
		{
			full_vector[i] = malloc(sizeof(Pos));
		}
		
		if (PosA->x > PosB->x)
		{
			for (i = 0; i <= distance; i++)
			{
				full_vector[i]->x = PosA->x - i;
				full_vector[i]->y = PosA->y;
			
			}	
		}
		else
		{
			for (i = 0; i <= distance; i++)
			{
			
				full_vector[i]->x = PosA->x + i;
				full_vector[i]->y = PosA->y;
			
			}
		}
		
	}
	else if (PosA->x == PosB->x)
	{
		distance = abs(PosA->y - PosB->y);
		full_vector = malloc((1 + distance)*sizeof(Pos*));
		
		for (i = 0; i <= distance; i++)
		{
			full_vector[i] = malloc(sizeof(Pos));
		}
		
		
		if (PosA->y > PosB->y)
		{
			for (i = 0; i <= distance; i++)
			{
			
				full_vector[i]->x = PosA->x;
				full_vector[i]->y = PosA->y - i;
			
			}	
		}
		else
		{
			for (i = 0; i <= distance; i++)
			{
			
				full_vector[i]->x = PosA->x;
				full_vector[i]->y = PosA->y + i;
			
			}
		}
	}
	else
	{
		printf("\nDIAGONAL LINES NOT SUPPORTED YET!!!!!!!!!!");
	}
	/*
	printf("\nPrint vector points");
	for (i = 0; i <= distance; i++)
	{
		printf("\n(x,y) = (%i,%i)", full_vector[i]->x, full_vector[i]->y);
	}
	*/
	downsize_ratio = (double)distance / (double)length;
	
	for (i = 0; i < length; i++)
	{
		vector[i]->x = full_vector[(int)(i*downsize_ratio + downsize_ratio / 2)]->x;
		vector[i]->y = full_vector[(int)(i*downsize_ratio + downsize_ratio / 2)]->y;
	}
	
	
}

void serialize_vector(Pos**vector, int vector_length, char*rgb_stream, MacroPixel matrix[Image_width][Image_height])
{
	int i = 0, k = 0;
	while (i < vector_length)
	{
		YCbCr2rgb(	*(matrix[vector[i]->x][vector[i]->y].Y), 
					*(matrix[vector[i]->x][vector[i]->y].Cb), 
					*(matrix[vector[i]->x][vector[i]->y].Cr), 
					(int*)&(rgb_stream[k]), (int*)&(rgb_stream[k + 1]), (int*)&(rgb_stream[k + 2]));
					
		i++;
		k += 3;
	}
}

void serialize_vector_gaussian(Pos**vector, int vector_length, char*rgb_stream, MacroPixel matrix[Image_width][Image_height],int size)
{
	int i = 0, k = 0;
	int x, y, half_size = size / 2,Y=0,Cb=0,Cr=0,square_size = size*size;
	while (i < vector_length)
	{
		for (x = vector[i]->x - half_size; x <= vector[i]->x + half_size; x++)
		{
		
			for (y = vector[i]->y - half_size; y <= vector[i]->y + half_size; y++)
			{
				Y += *(matrix[x][y].Y);
				Cb += *(matrix[x][y].Cb);
				Cr += *(matrix[x][y].Cr);
			}
		}
		
		
		YCbCr2rgb(	Y/square_size, 
			Cb/square_size, 
			Cr/square_size, 
			(int*)&(rgb_stream[k]),
			(int*)&(rgb_stream[k + 1]),
			(int*)&(rgb_stream[k + 2]));
					
		i++;
		k += 3;
		Y = 0;
		Cb = 0;
		Cr = 0;
	}
}

void de_serialize(vector_dp*vectors,char*data_stream,int quantity)
{
	char*stream_p = &data_stream[0];
	int i;
	char tmp[4];
	
	for (i = 0; i < quantity; i++)
	{
		memcpy(tmp, stream_p, 4);		
		vectors[i].length = atoi(tmp);
		//printf("\nTMP %s", tmp);
		
		stream_p += 4;
		memcpy(tmp, stream_p, 4);				
		vectors[i].start->x = atoi(tmp);
		//printf("\nTMP %s", tmp);
		
		stream_p += 4;
		memcpy(tmp, stream_p, 4);				
		vectors[i].start->y = atoi(tmp);
		//printf("\nTMP %s", tmp);
		
		stream_p += 4;
		memcpy(tmp, stream_p, 4);				
		vectors[i].end->x = atoi(tmp);
		//printf("\nTMP %s", tmp);
		
		stream_p += 4;
		memcpy(tmp, stream_p, 4);				
		vectors[i].end->y = atoi(tmp);
		//printf("\nTMP %s", tmp);
		
		stream_p += 4;
	}
	
}

void print_pos_vector(vector_dp*pos_vector,int quantity)
{
	int i;
	for (i = 0; i < quantity; i++)
	{
		printf("\nLED Pi\t\t (L,Psx,Psy,Pex,Pey) = (%i, %i, %i, %i, %i)", 
			pos_vector[i].length, 
			pos_vector[i].start->x,
			pos_vector[i].start->y,
			pos_vector[i].end->x,
			pos_vector[i].end->y);
	}
}



int count_leds(vector_dp*pos_vector,int quantity)
{
	int i, count = 0;
	
	for (i = 0; i < quantity; i++)
	{
		count = count + pos_vector[i].length;
	}
	
	
	return count;
}
