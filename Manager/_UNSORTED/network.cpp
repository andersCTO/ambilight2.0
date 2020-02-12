#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include <iostream>

#include "manager_structure.h"
using namespace std;

int manager_network_setup(int port)
{
	struct sockaddr_in local_addr, si_other;
	int sock;
	
	//create a UDP socket
	do
	{
		printf("Creating Socket\r\n");
		sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	} while (sock < 0);
	printf("Socket Created\r\n");
	memset((char *) &local_addr, 0, sizeof(local_addr));
	
	//Setup addr
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(port);
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//bind socket to port
	do
	{
		printf("Binding socket to port\r\n");
	} while (bind(sock, (struct sockaddr*)&local_addr, sizeof(local_addr)) < 0);
	printf("Binding done!\r\n");
	return sock;
}

int recieve_node_setup(int sock, struct sockaddr_in *node_addr )
{
	
	int ret, led_count=0,slen = sizeof(*node_addr);	
	recvfrom(sock, (char*)&led_count, 2, MSG_WAITALL, (struct sockaddr*)node_addr, (socklen_t*)&slen);	
	if (ret < 0)
	{
		return -1;
	}
	else
	{
		cout << "Connection Handler\tGot setup from: " << inet_ntoa(node_addr->sin_addr) << '@' << ntohs(node_addr->sin_port) << "\r\n";
		return led_count;
	}
		
}

int node_setup_done(int sock, struct sockaddr_in node_addr)
{
	struct sockaddr_in outgoing;
	
	
	return 0;
	
	
}

int get_position_data(int sock, struct node_cell* node, vector<led_pos> & node_led_pos)
{
	int slen = 0, ret = 0;;
	printf("Connection Handler\tWaiting for %u positions(%u byte)\r\n", node_led_pos.size()*sizeof(led_pos) / 4, node_led_pos.size()*sizeof(led_pos));
	if((ret = recvfrom(sock, (char*)&node_led_pos.data()->x, node_led_pos.size()*sizeof(led_pos), MSG_WAITALL, (struct sockaddr*)&node->addr, (socklen_t*)&slen)) < 0)
		return -1;	
	printf("Connection Handler\tReceived %u bytes\r\n", ret);
	return 0;
}

int recv_image(int sock, vector<rgb_value> & I)
{
	sockaddr_in incoming;
	int slen = sizeof(incoming);
	recvfrom(sock, (char*)&I[0].red, 1200, MSG_WAITALL, (struct sockaddr*)&incoming, (socklen_t*)&slen);	
	
	return 0;
}