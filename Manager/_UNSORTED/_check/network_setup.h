#pragma once
#include <stdio.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <netdb.h> //hostent

int client_setup_socket(char name[], int port)
{
	
	int socket_desc,i;
	struct sockaddr_in server;
	struct hostent *he;
	struct in_addr **addr_list;
	char ip[100];
	
	char ip2[] = "192.168.0.10";
	
	printf("\nConnecting to %s (%s)",name, ip2);
	
	printf("\nCreating Socket");
	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
		return 0;
	}
        
	
	server.sin_addr.s_addr = inet_addr(ip2);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
 
	printf("\nConnecting to socket");
    //Connect to remote server
	if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("\n Connect error");
		return -1;
	}
     
	printf("\n Socket Connected\n");
	
	
	
	
	return socket_desc;
}




