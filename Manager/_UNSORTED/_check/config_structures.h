#pragma once

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <string.h>
#include "Header.h"
#include "funkis.h"

#define MAX_CONFIGS_PER_CLIENT 4


//Monitor Alignment vector TOP, BOTTOM, RIGHT, LEFT
int ALIGNMENT[4] = { 35, Image_height-20,Image_width-20, 20 };


//LED setup structure
enum orientation { top, bottom, left, right, null };
typedef enum orientation orientation;
static const char *orientation_string[] = { "Top   ", "Bottom", "Left  ", "Right ","Empty " };

//Serial Text size
static const unsigned int SERIAL_CONFIG_SIZE =  MAX_CONFIGS_PER_CLIENT * 6;

//RGB MATRIX datatype

struct rgb_macro_pixel
{
	int Red;
	int Green;
	int Blue;
};

typedef struct rgb_macro_pixel rgb_macro_pixel;

//rgb_macro_pixel rgb_matrix[Image_width][Image_height];



//Led Configuration Structure
struct led_config
{
	orientation pos; //Position of LED strip
	orientation start; //Where does the LED strip start
	uint16_t nr_of_leds; 
	uint16_t offset;	//From center of image
};

typedef struct led_config led_config;

struct grab_list
{
	led_config config[MAX_CONFIGS_PER_CLIENT];
	int socket_id;
};

typedef struct  grab_list grab_list;


//Set the LED configuration for one strip
void set_led_config(led_config *config, orientation pos, uint16_t nr_of_leds, orientation start, uint16_t offset)
{
	config->nr_of_leds = nr_of_leds;
	config->offset = offset;
	config->pos = pos;
	config->start = start;
}

//Create initial LED config structure with initial values
void init_led_config(led_config config[])
{
	int i;
	for (i = 0; i < MAX_CONFIGS_PER_CLIENT; i++)
	{
		set_led_config(&config[i], null, 0, null, 0);
	}
}

//Print clients entire configuration
void print_led_config(led_config *config)
{
	
	printf("\nPosition (%s)   Orientation(%s)   Nr of LEDs(%i)   Offset(%i)", orientation_string[(int)config->pos], orientation_string[(int)config->start],config->nr_of_leds,config->offset);
	
}

//Pack 16bit uint in 8bit chars
void uint16_pack(uint16_t val, char word[])
{
	word[0] = val & 0xFF;
	word[1] = val >> 8;
}

//Un-pack 16bit uint in 8bit chars
uint16_t uint16_unpack(char word1, char word2)
{
	uint16_t val = ((uint16_t)word2 << 8) | word1;
	return val;
}

//Serialize configuration
void config_mod(struct led_config config[], char serial_txt[])
{
	int i=0,j = 0;
	char temp_8bit[2];
	memset(serial_txt, ' ', SERIAL_CONFIG_SIZE);//Clean up buffer
	
	while ( i < MAX_CONFIGS_PER_CLIENT)//Until undefined config
	{
		serial_txt[j + 0] = (int)config[i].pos;
		serial_txt[j + 1] = (int)config[i].start;
		
		uint16_pack(config[i].nr_of_leds, temp_8bit);//Pack 16bit and save as char i buffer		
		serial_txt[j + 2] = temp_8bit[0];
		serial_txt[j + 3] = temp_8bit[1];
		
		uint16_pack(config[i].offset, temp_8bit);//Pack 16bit and save as char i buffer
		serial_txt[j + 4] = temp_8bit[0];
		serial_txt[j + 5] = temp_8bit[1];
		
		j += 6;
		i++;
	}
}

//De-Serialize configuration
void config_demod(struct led_config config[], char serial_txt[])
{
	int j = 0,i=0;
	char temp_8bit[2];
	
	while (j < SERIAL_CONFIG_SIZE)
	{
		if (serial_txt[j] == ' ')
			break;
		config[i].pos = (orientation)serial_txt[j + 0];
		config[i].start = (orientation)serial_txt[j + 1];
		config[i].nr_of_leds = uint16_unpack(serial_txt[j + 2], serial_txt[j + 3]);
		config[i].offset = uint16_unpack(serial_txt[j + 4], serial_txt[j + 5]);
			
		j += 6;
		i++;
	}
}

int YCbcr_array2rgb_macropixel(rgb_macro_pixel *rgb, unsigned char Y[], unsigned char Cb[], unsigned char Cr[])
{
	int i,nr_of_pixels=strlen((char*)Y);
	for (i = 0; i < nr_of_pixels; i++)
	{
		YCbCr2rgb(Y[i], Cb[i / 2], Cr[i / 2], &rgb[i].Red, &rgb[i].Green, &rgb[i].Blue);
	}
	
	return 0;
}

int setup_rgb_matrix(rgb_macro_pixel *rgb_data_array, rgb_macro_pixel *rgb_matrix[Image_height][Image_width])
{
	int x, y;
	
	for (x = 0; x < Image_height; x++)
	{
		for (y = 0; y < Image_width; y++)
		{
			rgb_matrix[x][y] = &rgb_data_array[Ipos2v(x,y)];
		}
	}
	return 0;
}

int count_leds(struct led_config config[])
{
	int i;
	int count = 0;
	for (i = 0; i < MAX_CONFIGS_PER_CLIENT; i++)
	{
		if (config[i].pos != null)
		{
			count += config[i].nr_of_leds;
		}	
		
	}
	return count;
	
}


int rgb_matrix_to_vector(rgb_macro_pixel *rgb_matrix[Image_height][Image_width],char *data_stream,led_config config)
{
	int idx, i;
	switch (config.pos)
	{
	case null:
		return 0;
		
	case right:
		//Standard orientation
		if (config.start == top)
		{
			idx = 0;
			i = 0;
			while (i < Image_height)
			{
				data_stream[idx] =		rgb_matrix[i][ALIGNMENT[2]]->Red;
				data_stream[idx + 1] =	rgb_matrix[i][ALIGNMENT[2]]->Green;
				data_stream[idx + 2] =	rgb_matrix[i][ALIGNMENT[2]]->Blue;
				idx += 3;
				i++;
			}	
		}
		//Inverted orientation
		else if (config.start == bottom)
		{
			i = Image_height - 1;
			idx = 0;
			while (i >= 0)
			{
				data_stream[idx] =		rgb_matrix[i][ALIGNMENT[2]]->Red;
				data_stream[idx + 1] =	rgb_matrix[i][ALIGNMENT[2]]->Green;
				data_stream[idx + 2] =	rgb_matrix[i][ALIGNMENT[2]]->Blue;
				idx += 3;
				i--;
			}	
			
		}
		
	case left:
		//Standard orientation
		if (config.start == top)
		{
			idx = 0;
			i = 0;
			while (i < Image_height)
			{
				data_stream[idx] =		rgb_matrix[i][ALIGNMENT[3]]->Red;
				data_stream[idx + 1] =	rgb_matrix[i][ALIGNMENT[3]]->Green;
				data_stream[idx + 2] =	rgb_matrix[i][ALIGNMENT[3]]->Blue;
				idx += 3;
				i++;
			}	
		}
		//Inverted orientation
		else if (config.start == bottom)
		{
			i = Image_height - 1;
			idx = 0;
			while (i >= 0)
			{
				data_stream[idx] =		rgb_matrix[i][ALIGNMENT[3]]->Red;
				data_stream[idx + 1] =	rgb_matrix[i][ALIGNMENT[3]]->Green;
				data_stream[idx + 2] =	rgb_matrix[i][ALIGNMENT[3]]->Blue;
				idx += 3;
				i--;
			}	
			
		}
		break;
		
	case top:
		//Standard orientation
		if (config.start == left)
		{
			idx = 0;
			i = 0;
			while (i < Image_width)
			{
				data_stream[idx] =		rgb_matrix[ALIGNMENT[0]][i]->Red;
				data_stream[idx + 1] =	rgb_matrix[ALIGNMENT[0]][i]->Green;
				data_stream[idx + 2] =	rgb_matrix[ALIGNMENT[0]][i]->Blue;
				idx += 3;
				i++;
			}	
		}
		//Inverted orientation
		else if (config.start == right)
		{
			i = Image_width - 1;
			idx = 0;
			while (i >= 0)
			{
				data_stream[idx] =		rgb_matrix[ALIGNMENT[0]][i]->Red;
				data_stream[idx + 1] =	rgb_matrix[ALIGNMENT[0]][i]->Green;
				data_stream[idx + 2] =	rgb_matrix[ALIGNMENT[0]][i]->Blue;
				idx += 3;
				i--;
			}	
			
		}
		break;
		
	case bottom:
		//Standard orientation
		if (config.start == left)
		{
			idx = 0;
			i = 0;
			while (i < Image_width)
			{
				data_stream[idx] =		rgb_matrix[ALIGNMENT[1]][i]->Red;
				data_stream[idx + 1] =	rgb_matrix[ALIGNMENT[1]][i]->Green;
				data_stream[idx + 2] =	rgb_matrix[ALIGNMENT[1]][i]->Blue;
				idx += 3;
				i++;
			}	
		}
		//Inverted orientation
		else if (config.start == right)
		{
			i = Image_width - 1;
			idx = 0;
			while (i >= 0)
			{
				data_stream[idx] =		rgb_matrix[ALIGNMENT[1]][i]->Red;
				data_stream[idx + 1] =	rgb_matrix[ALIGNMENT[1]][i]->Green;
				data_stream[idx + 2] =	rgb_matrix[ALIGNMENT[1]][i]->Blue;
				idx += 3;
				i--;
			}	
			
		}
		break;
		
	}
	
	
}
//Monitor Alignment vector TOP, BOTTOM, RIGHT, LEFT
//int ALIGNMENT[4] = { 10, 10, 25, 100 };
