#pragma once
#include <stdint.h>
#include <vector>
#include "ambi_type_definitions.h"

using namespace std;

//Converts signed 16 bit to two 8 bit unsigned
void bitconversion16_to_8( int16_t value, char * buffer);

//Converts two 8 bit unsigned to signed 16 bit. 
int16_t bitconversion8_to_16( char * buffer);

//Moves LED position data from array to vector(8bit to 16bit)
void array2vector(vector<LED_POS> & led_array,char*buffer);

//Returns Eucledean distance in R2 between two points
double distance(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

