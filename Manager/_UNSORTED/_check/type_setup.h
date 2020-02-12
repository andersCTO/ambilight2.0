#include <stdint.h>

#pragma once

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

struct MacroPixel
{
	char * Y;
	char * Cb;
	char * Cr;
};
typedef struct MacroPixel MacroPixel;

struct LED_POS
{
	uint16_t x;
	uint16_t y;
};
typedef struct LED_POS LED_POS;

struct rgbPixel
{
	char* red;
	char* green;
	char* blue;
	
};
typedef struct rgbPixel rgbPixel;
