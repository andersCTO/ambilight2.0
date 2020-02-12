#pragma once
#pragma comment(linker, "/STACK:2000000")
#pragma comment(linker, "/HEAP:2000000")

#include <stdint.h>
#include <vector>
#include <math.h>
#include <stdlib.h>

#include "ambi_type_definitions.h"

using namespace std;


void bitconversion16_to_8(int16_t value, char*buffer)
{
	buffer[0] = ((value >> 8) & 0xff);//OK
	buffer[1] = ((value >> 0) & 0xff);
}


int16_t bitconversion8_to_16(char * buffer)
{
	return (buffer[0] << 8) | (buffer[1] & 0xff);
}

void array2vector(vector<LED_POS> & led_array, char*buffer)
{
	int buffer_idx = 0;
	char buff_tmp[2];
	for (int vector_idx = 0; vector_idx < led_array.size(); vector_idx++)
	{
		buff_tmp[0] = buffer[buffer_idx++];
		buff_tmp[1] = buffer[buffer_idx++];		
		led_array[vector_idx].x = bitconversion8_to_16(buff_tmp);
		
		buff_tmp[0] = buffer[buffer_idx++];
		buff_tmp[1] = buffer[buffer_idx++];
		led_array[vector_idx].y = bitconversion8_to_16(buff_tmp);
	}
}

double distance(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
	return sqrt(pow(double(x1 - x2), 2) + pow(double(y1 - y2), 2));
}