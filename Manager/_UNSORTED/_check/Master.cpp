#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <linux/in.h>
#include <stdlib.h> 

#include <iostream>
#include <fstream>
#include <string>

#include "lib/ambi_simple_functions.h"
#include "master_specific_setup.h"
#include "lib/ambi_network.h"
#include "master_functions.h"



int active_clients = 0;
int GRID_WIDTH = 178;
int GRID_HEIGHT = 100;

vector <char> image_source;
vector <rgbPixel> image_mask;
vector <LED_POS> grid;


void *image_manager(void*ptr)
{
	puts("\nImage Manager\tRunning");
	
	int im_sock = host_ip_socket_setup(2800), new_ip_socket, c = sizeof(struct sockaddr_in);
	
	struct sockaddr_in image_client;
	
	listen(im_sock, 10);
	puts("\nImage Manager\tWaiting for Incoming connections...");
	new_ip_socket = accept(im_sock, (struct sockaddr*)&image_client, (socklen_t*)&c);
	puts("\nImage Manager\tImage client connected...");
	char ack;
	while (1)
	{
		puts("\nWaiting for image data");
		if (recv(new_ip_socket, &image_source[0], GRID_WIDTH*GRID_HEIGHT * 3, MSG_WAITALL) < 0) {
			//Wait for ack
			puts("\nKUKEN");
			break;
		}
	}
	return NULL;
}

void *node_client(void*ip_socket)
{
	puts("\nNode Client\tRunning");
	
	int *socket_p = (int*)ip_socket, new_socket = *socket_p;
	
	//Recieve setup
	char init_leds[2];
	// = (char*)malloc(2);
	if (recv(new_socket, &init_leds, 2, MSG_WAITALL) >= 0)
	{
		int led_q = bitconversion8_to_16(init_leds);
		printf("\nNode Client\tSystem reports %i LEDS's", led_q);
		vector<LED_POS>node_led_pos(led_q);
	
		vector<char>setup_buffer(node_led_pos.size() * 4);
	
		
		if (recv(new_socket, &setup_buffer[0], setup_buffer.size(), MSG_WAITALL) >= 0)
		{
			printf("\nNode Client\tRecieved all LED positions(%d)", node_led_pos.size());
	
			//Move data from buffer to real dataarray
			array2vector(node_led_pos, &setup_buffer[0]);
	
			//Send Gamma matrix
			send(new_socket, GAMMA_MATRIX, 256, NULL);
			puts("\nNode Client\tGamma Matrix sent");
	
	
			//Find closest grid cell for every LED position
			vector<rgbPixel>led_data(node_led_pos.size());	
	
			find_closest_cell(grid, node_led_pos, image_mask, led_data);
			node_led_pos.clear();
	
			//Send LED data
			vector<char>led_data_buffer(led_data.size() * 3);
			char ack;
			puts("\nNode Client\tRunning");
			while (1)
			{
		
				if ((send_led_data(led_data, &led_data_buffer[0], &image_source[0], new_socket)) < 0)
					break;
				if (recv(new_socket, &ack, 1, MSG_WAITALL) < 0)
					break;
				usleep(500);//Wait for ack
			}
			
		}
	

		
	}	
	puts("\nNode Client\tClient left, shutting down thread");
	//Clear and shutdown
	shutdown(new_socket, SHUT_RDWR);
	close(new_socket);
//	led_data.clear();
	//led_data_buffer.clear();	

	pthread_exit(NULL);
	
	
	
		
		
}

void *connection_handler(void*ptr)
{
	puts("\nConn Handler\tConnection handler running");
	active_clients++;
	
	int *new_ip_sock, new_ip_socket, ip_socket = host_ip_socket_setup(MASTER_IP_PORT);//Start server
	
	struct sockaddr_in client;
	
	listen(ip_socket, 10);
	
	puts("\nConn Handler\tWaiting for Incoming connections...");
	int c = sizeof(struct sockaddr_in);
	while ((new_ip_socket = accept(ip_socket, (struct sockaddr*)&client, (socklen_t*)&c)))
	{
		puts("\nConn Handler\tClient connected to IP-Socket");
		pthread_t client_thread;
		new_ip_sock = (int*)malloc(sizeof(int));
		*new_ip_sock = new_ip_socket;
		
		if (pthread_create(&client_thread, NULL, node_client, (void*)&new_ip_socket) < 0)
		{
			perror("could not create thread");
		}
		
	}
	
}

int main()
{
	master_mode mode = prolight;
	
	switch (mode)
	{
	case prolight:
		image_source.resize(GRID_WIDTH*GRID_HEIGHT * 3);	
		image_mask.resize(GRID_WIDTH*GRID_HEIGHT);
		load_grid_pos(grid);
	
		create_image_mask(&image_source[0], image_mask);
	
		pthread_t connection_handler_t, image_manager_t;
	
		pthread_create(&connection_handler_t, NULL, connection_handler, NULL);
		pthread_create(&image_manager_t, NULL, image_manager, NULL);
		sleep(1);
		break;
		
	case light:
		puts("\nRunning light");
		break;
		
	case physical:
		puts("\nRunning physical");
		break;
		
	}
	

	
	while (1)
	{
		sleep(10);
	}
	
	
}
