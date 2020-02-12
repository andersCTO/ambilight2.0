#include <stdio.h>
#include <string.h>    //strlen
#include <stdlib.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h> //for threading , link with lpthread
#include "config_structures.h"

void *connection_handler(void *);
 
int main99(int argc, char *argv[])
{
	int socket_desc, new_socket, c, *new_sock;
	struct sockaddr_in server, client;
	char *message;
     
	
	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
     
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);
     
	//Bind
	if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("bind failed");
		return 1;
	}
	puts("bind done");
     
	//Listen
	listen(socket_desc, 3);
     
	//Accept and incoming connection
	puts("Waiting for Incoming connections...");
	c = sizeof(struct sockaddr_in);
	while ((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)))
	{
		puts("Connection accepted");
         
		//Reply to the client
		message = "Hello Client , I have received your connection. And now I will assign a handler for you\n";
		write(new_socket, message, strlen(message));
		sleep(1);
		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = new_socket;
         
		if (pthread_create(&sniffer_thread, NULL, connection_handler, (void*) new_sock) < 0)
		{
			perror("could not create thread");
			return 1;
		}
         
		//Now join the thread , so that we dont terminate before the thread
		//pthread_join( sniffer_thread , NULL);
		puts("Handler assigned");
	}
     
	if (new_socket < 0)
	{
		perror("accept failed");
		return 1;
	}
     
	return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
	led_config configuration[4];
    //Get the socket descriptor
	int i, sock = *(int*)socket_desc;
     
	char name[SERIAL_CONFIG_SIZE];
	
	init_led_config(configuration);
	
	recv(sock, name, SERIAL_CONFIG_SIZE,0);
	
	printf("\nConfig Recieved:\n");
	for (i = 0; i < SERIAL_CONFIG_SIZE;i++)
	{
		printf("%i ", name[i]);
	}
	config_demod(configuration, &name);
	
	for (i = 0; i < 4; i++)
	{
	
		print_led_config(&configuration[i]);
		
	}
	
	printf("Done!");
	
	//Free the socket pointer		
	free(socket_desc);
	
	return 0;
}