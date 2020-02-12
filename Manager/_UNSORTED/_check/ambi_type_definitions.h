#pragma once
#include <stdint.h>

//Type defineing the LED-strip
struct LED_POS
{
	int16_t x;
	int16_t y;
};
typedef struct LED_POS LED_POS;


//Type defining rgb pixels in RGB matrix
struct rgbPixel
{
	char* red;
	char* green;
	char* blue;
	
};
typedef struct rgbPixel rgbPixel;

//Master running modes
enum master_mode
{
	prolight,
	light,
	physical
};