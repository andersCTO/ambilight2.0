#include <stdio.h>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "lib/ambi_simple_functions.h"
#include "lib/ambi_network.h"
#include "lib/ambi_type_definitions.h"
#include "ws2812-rpi.h"
#include "led_functions.h"

using namespace std;

int main()
{
	//Initial variables
	vector<char>GAMMA_MATRIX, led_data_buffer;//Data for gamma correction and rgb data
	vector<LED_POS>led_array;//Positional data of led's
	
	//Initilize Node
	led_node_init(GAMMA_MATRIX, led_array, led_data_buffer);
	NeoPixel*neo_object = new NeoPixel(led_array.size());

	//Setup network with master	
	char name[] = "AMBIPI2";
	int sock;
	
	
	int led_index,data_index;
	int server_active;
	while (1)//Main Loop
	{
		
		while ((sock = client_ip_socket_setup(MASTER_IP_PORT, name)) < 0)//Connect to server loop
		{
			sleep(5);
			puts("\nRe-trying to connect to master");
			close(sock);		
		}
		server_active = 1;
		puts("\nConnected to master");
		
		send_led_setup(led_array, sock);//Sending LED Setup
		
		//Get gamma matrix
		recv(sock, &GAMMA_MATRIX[0], 256, MSG_WAITALL);
		puts("\nGamma Matrix Recieved");
		
		
	
		puts("\nSetup done, receiving data");
		while (server_active)//Communicate with server loop
		{
		
	
			if (recv(sock, &led_data_buffer[0], led_data_buffer.size(), MSG_WAITALL) < 1)
			{
				puts("\nServer disconnected");
				server_active = 0;
				break;
			}
				
		
			if (send(sock, "a", 1, NULL) < 1)
			{
				puts("\nServer disconnected");
				server_active = 0;
				break;
			}
			
		
			led_index = 0, data_index = 0;
			while (led_index < led_array.size())
			{
				neo_object->setPixelColor(led_index++, GAMMA_MATRIX[led_data_buffer[data_index]], GAMMA_MATRIX[led_data_buffer[data_index + 1]], GAMMA_MATRIX[led_data_buffer[data_index + 2]]);
				data_index += 3;
			}
			neo_object->show();
		
		
		}
	}	

}