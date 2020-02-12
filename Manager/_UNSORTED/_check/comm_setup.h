#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>    //strlen
#include <stdlib.h>    //strlen
#include <pthread.h> //for threading , link with lpthread
#include <string.h>
#include <arpa/inet.h> //inet_addr

const char RANDOM_TEXT_ARRAY[52] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};


//Creates the socket(grabside) comm between grab and conn handler
int domain_socket_server_setup(char adress_input[],char name[])
{
	char adress[strlen(adress_input)+10];
	sprintf(adress, "UD_socket_%s", adress_input);
	printf("\n%s\tServer Socket Adress|%s|",name, adress);
	//\UD_sockets
	int fromlen;
	int timer, s, ns, len;
	struct sockaddr_un saun, fsaun;
	printf("\n%s\tDomain Socket Server Started!\n",name);
	
	if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("server: socket");
	
	}
	
	saun.sun_family = AF_UNIX;
	strcpy(saun.sun_path, adress);
	
	unlink(adress);
	len = sizeof(saun.sun_family) + strlen(saun.sun_path);

	if (bind((int)s, (__CONST_SOCKADDR_ARG)&saun, (socklen_t)len) < 0) {
		perror("Domain server: bind");
		return 0;
	
	}
	
	if (listen(s, 100) < 0) {
		perror("Domain server: listen");
		return 0;
	
	}
	printf("\n%s\tDomain Socket Server waiting for Connection\n",name);
	
	ns = accept((int)s, (__SOCKADDR_ARG)&fsaun, (socklen_t*)&fromlen);
	
	return ns;
	
}

//Creates the socket(conn side) comm between grab and conn handler
int domain_socket_client_setup(char adress_input[],char name[])
{
	char adress[strlen(adress_input) + 10];
	sprintf(adress, "UD_socket_%s", adress_input);
	printf("\n%s\tClient Socket Adress|%s|",name, adress);
	char c;
	int i, s, len, count = 0;
	struct sockaddr_un saun;
	
	if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("Domain client: socket");
		return 0;
	}
	
	//Connect To server
	saun.sun_family = AF_UNIX;
	strcpy(saun.sun_path, adress);
	
	
	len = sizeof(saun.sun_family) + strlen(saun.sun_path);

	if (connect(s, (__SOCKADDR_ARG)&saun, len) < 0) {
		perror("Domain client: connect");
		return 0;
	}
	printf("\n%s\tConnected to Server Domain Socket!",name);
	return s;
}

//Generate Client Socket name

void sock_name(char buffer[])
{
	
	int i;
	
	for (i = 0; i < 8; i++)
	{
		
		buffer[i] = RANDOM_TEXT_ARRAY[rand()%52];
	}

}
/*
void add_sock_name(char ***socket_array,int active_clients)
{
	char *buffer = malloc(sizeof(char)*9);
	buffer[8] = '\0';
	int i,done=0;
	

	while (!done)
	{
		sock_name(buffer);
		done = 1;
		for (i = 0; i < active_clients; i++)
		{
			if (!strcmp((*socket_array)[i], buffer))
			{
				done = 0;
				break;
			}
				
		}	
	}
	printf("\nSock name\t\t Socket Name:|%s|",buffer);
	*socket_array = (char**)realloc(*socket_array, sizeof(char*) * (active_clients + 1));	
	
	(*socket_array)[active_clients] = malloc(9*sizeof(char));
	
	
	memcpy((*socket_array)[active_clients], buffer, 9);
	free(buffer);
	//Loop through Socket Array
	
	
}
*/
int ip_socket_setup(int port,char name[])
{
	int socket_desc, new_socket, c, *new_sock;
	struct sockaddr_in server, client;
     
	
	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);
     
	//Bind
	if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("bind failed");
		return 1;
	}
	printf("\n%s\tBind Complete",name);
	return socket_desc;
     
}
				
