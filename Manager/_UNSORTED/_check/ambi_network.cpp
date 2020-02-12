#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <vector>
#include <stdlib.h>

#include "ambi_type_definitions.h"
#include "ambi_simple_functions.h"

#define NETWORK_MSG_SIZE 20

using namespace std;

int hostname_to_ip(char * hostname, char* ip)
{
	struct hostent *he;
	struct in_addr **addr_list;
	int i;
         
	if ((he = gethostbyname(hostname)) == NULL) 
	{
	    // get the host info
		herror("gethostbyname");
		return 1;
	}
 
	addr_list = (struct in_addr **) he->h_addr_list;
     
	for (i = 0; addr_list[i] != NULL; i++) 
	{
	    //Return the first one;
		strcpy(ip, inet_ntoa(*addr_list[i]));
		return 0;
	}
     
	return 1;
}

int host_ip_socket_setup(int port)
{
	int socket_desc = socket(AF_INET,SOCK_STREAM,0);
	if (socket_desc == -1)
	{
		puts("\nCould not create host socket!");
		return -1;
	}
	
	//Prepare sockaddr_in structure
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);
	
	//Bind
	if (bind(socket_desc, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		puts("\nHost bind failed");
		return -1;
	}
	puts("\nHost socket created");
	return socket_desc;
}

int client_ip_socket_setup(int port, char*name)
{
	
	int socket_desc;
	char master_ip[]="192.168.0.112";
	struct sockaddr_in master;
	
	//hostname_to_ip(name, master_ip);
	printf("\nConnecting to Master %s (%s)", name, master_ip);
	
	puts("\nCreating Socket");
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1)
	{
		puts("Could not create socket");
		return -1;
	}
	
	master.sin_addr.s_addr = inet_addr(master_ip);
	master.sin_family = AF_INET;
	master.sin_port = htons(port);
	
	puts("\nConnecting to socket");
	
	if (connect(socket_desc, (struct sockaddr*)&master, sizeof(master)) < 0)
	{
		puts("\nCould not connect to Master");
		return -1;
	}
		
	puts("\nConnected to Master");
	return socket_desc;
}

int send_led_setup(vector<LED_POS> & arr,int socket)
{
	
	//Send LED quantity
	int16_t led_quantity = arr.size();
	char led_q[2];
	bitconversion16_to_8(led_quantity, led_q);
	send(socket, led_q, 2, NULL);
	
	//Send LED positions
	char *led_buffer = (char*)malloc(led_quantity * 4);
	
	int data_index = 0;
	char bit_tmp[2];
	
	for (int led_index = 0; led_index < led_quantity; led_index++)
	{
		bitconversion16_to_8(arr[led_index].x, bit_tmp);
		led_buffer[data_index++] = bit_tmp[0];
		led_buffer[data_index++] = bit_tmp[1];
		
		bitconversion16_to_8(arr[led_index].y, bit_tmp);
		led_buffer[data_index++] = bit_tmp[0];
		led_buffer[data_index++] = bit_tmp[1];
	}	
	send(socket, led_buffer, led_quantity * 4, NULL);
	//char recv_buffer[NETWORK_MSG_SIZE];
	//recv(socket, recv_buffer, NETWORK_MSG_SIZE, MSG_WAITALL);
	//printf("\n%s", recv_buffer);
}
