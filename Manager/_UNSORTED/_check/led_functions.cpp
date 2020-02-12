#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdlib.h>

#include "ws2812-rpi.h"
#include "ws2812-rpi-defines.h"

#include "lib/ambi_type_definitions.h"
#include "node_information.h"

using namespace std;


void save_node_cfg(vector <LED_POS> & led_positions)
{
	char cfg_filename[] = "cfg.txt";
	ofstream myfile;
	myfile.open(cfg_filename);
	myfile << "#LED QUANTITY\t" << led_positions.size() << "\n";
	
	for (int i = 0; i < led_positions.size(); i++)
	{
		myfile << led_positions[i].x << "\t" << led_positions[i].y << "\n";
	}
	
	myfile.close();
	
}

int load_node_cfg(vector <LED_POS> & led_positions)
{
	char cfg_filename[] = "cfg.txt";
	
	FILE*fp = fopen(cfg_filename, "r");
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	if (fp == NULL)
	{
		puts("\nERROR, no config file");
		return -1;
	}
		
	//GET LED quantity
	read = getline(&line, &len, fp);
	int index = 0;
	while (line[index++] != '\t')
		;
	uint16_t led_quantity = atoi(&(line[index]));
	
	led_positions.resize(led_quantity);

	int led_index = 0;
	while ((read = getline(&line, &len, fp)) != -1)
	{
		index = 0;
		while (line[index++] != '\t')
			;
		line[index - 1] = '\n';
		led_positions[led_index].x = atoi(line);
		led_positions[led_index++].y = atoi(&(line[index]));		
	}
	puts("\nClosing file!");
	fclose(fp);
	
	for (int i = 0; i < led_positions.size(); i++)
	{
		printf("\nID %u : %u %u", i, led_positions[i].x, led_positions[i].y);
	}
	return 0;
}



void set_led_color(class NeoPixel N,char*GAMMA, unsigned int index, char *red, char* green, char *blue)
{

	N.setPixelColor(index, GAMMA[*red], GAMMA[*green], GAMMA[*blue]);
	
}


void led_node_init(vector <char> & gamma,  vector <LED_POS> & led_positions,vector <char> & led_data_buffer)
{
	load_node_cfg(led_positions);
	
	
	//Create linear gamma mask
	gamma.resize(256);
	for (int i = 0; i < gamma.size(); i++)
	{
		gamma[i] = i;
	}
	
	//Allocate memory for led color data
	led_data_buffer.resize(led_positions.size() * 3);
	
	
	
	
	
}


