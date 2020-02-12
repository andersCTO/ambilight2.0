//#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>

#include "lib/ambi_type_definitions.h"
#include "lib/ambi_simple_functions.h"

using namespace std;

void create_image_mask(char * I, vector<rgbPixel> & mask)
{
	int data_index = 0;
	for (int mask_index = 0; mask_index < mask.size(); mask_index++)
	{
		mask[mask_index].red	= &I[data_index++];
		mask[mask_index].green	= &I[data_index++];
		mask[mask_index].blue	= &I[data_index++];
	}
}


int load_grid_pos(vector<LED_POS> & grid)
{
	//load textfile
	FILE *fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	fp = fopen("grid_setup.txt", "r");
	LED_POS tmp;
	
	int i, grid_index = 0;;
	while ((read = getline(&line, &len, fp)) > 0)
	{
		//Find tab
		i = 0;
		while (line[i++] != '\t')
			;
		line[i - 1] = '\0';
		tmp.x = atoi(line);
		tmp.y = atoi(&line[i]);
		
		grid.push_back(tmp);
		//printf("\n(x,y) = (%d,%d) Total size:%d", tmp.x, tmp.y, grid.size());	
	}
	printf("\nFile loaded wih %u points",grid.size());
	
	
	
	return -1;
}

void find_closest_cell(const vector<LED_POS> & grid, const vector<LED_POS> & led_pos, const vector<rgbPixel> & mask, vector<rgbPixel> & led_data)
{
	int min_index;
	double min_dist, new_dist;
	for (int led_index = 0; led_index < led_pos.size(); led_index++)
	{
		min_dist = 10000;
		for (int grid_index = 0; grid_index < grid.size(); grid_index++)
		{
			new_dist = distance(led_pos[led_index].x, led_pos[led_index].y, grid[grid_index].x, grid[grid_index].y);
			if (new_dist <= min_dist)
			{
				min_dist = new_dist;
				min_index = grid_index;				
			}
			
		}
		//printf("\nLED(%d,%d) is closest to Cell(%d,%d)", led_pos[led_index].x, led_pos[led_index].y, grid[min_index].x, grid[min_index].y);
		led_data[led_index].red		= mask[min_index].red;
		led_data[led_index].green	= mask[min_index].green;
		led_data[led_index].blue	= mask[min_index].blue;
	}
	puts("\nLED masking done!");
}


int send_led_data(const vector<rgbPixel> & led_data, char*led_data_buffer, char*image_source, int sock)
{
	int nr_of_leds = led_data.size(), buffer_index = 0;
	for (int i = 0; i < nr_of_leds; i++)
	{
		led_data_buffer[buffer_index++] = *led_data[i].red;
		led_data_buffer[buffer_index++] = *led_data[i].green;
		led_data_buffer[buffer_index++] = *led_data[i].blue;
	}
	if ((send(sock, led_data_buffer, nr_of_leds * 3, NULL) < 0)) {
		return -1;
		puts("\nCould not send LED data");
	}
		else
		{
			return 0;
		}	
}
