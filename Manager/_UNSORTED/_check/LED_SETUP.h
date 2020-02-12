#pragma once
#include <stdint.h>
#include "ws2812-rpi-defines.h"
#include <iostream>
#include <vector>

using namespace std;

class LED_BUFFER
{
public:
	LED_BUFFER(int n);
	vector<Color_t>LED_ARRAY;
	
	void dispalyLED(LED_BUFFER *L);
	int char2LED(char*buffer, int buffersize, LED_BUFFER *L);
	
private:
};



