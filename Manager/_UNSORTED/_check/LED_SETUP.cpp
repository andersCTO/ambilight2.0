#include "LED_SETUP.h"
#include <stdio.h>
#include <iostream>
#include "ws2812-rpi-defines.h"

using namespace std;

LED_BUFFER::LED_BUFFER(int n)
{
	LED_ARRAY.resize(n);
}
void LED_BUFFER::dispalyLED(LED_BUFFER *L)
{
	
	
	for (int i = 0; i < L->LED_ARRAY.size(); i++)
	{
		printf("Red(%d) Green(%d) Blue(%d)\n", L->LED_ARRAY[i].r, L->LED_ARRAY[i].g, L->LED_ARRAY[i].b);
	}
}

int LED_BUFFER::char2LED(char*buffer, int buffersize, LED_BUFFER *L) {
	if (buffersize % 3 != 0)
		return -1;
	
	
	int i=0 , j = 0;
	
	while (i < buffersize)
	{
		L->LED_ARRAY[j].r = buffer[i];
		L->LED_ARRAY[j].g = buffer[i+1];
		L->LED_ARRAY[j].b = buffer[i+2];
		i = i + 3;
		j++;
	}
	return 0;
	
	
}