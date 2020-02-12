#pragma once
#include <vector>

#include "lib/ambi_type_definitions.h"

using namespace std;

void create_image_mask(char * I,vector<rgbPixel> & mask);//Creates datastructure that points out rgb in data stream
int load_grid_pos(vector<LED_POS> & grid);//Loads grid position from textfile
void find_closest_cell(const vector<LED_POS> & grid, const vector<LED_POS> & led_pos, const vector<rgbPixel> & mask, vector<rgbPixel> & led_data);
int send_led_data(const vector<rgbPixel> & led_data,char*led_data_buffer,char*image_source,int socket);
