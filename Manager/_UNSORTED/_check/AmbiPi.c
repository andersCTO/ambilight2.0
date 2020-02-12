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
#include "comm_setup.h"
#include <string.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <semaphore.h>
#include "config_structures.h"
#include "funkis.h"
#include "Header.h"
#include <stdio.h>


//used for serving grab task with socket adresses to clients
#define MAIN_COM "main_socket"
#define MAIN_COM_SIZE 8

#define MAX_CLIENTS 5
#define IP_PORT 6666

char selector;

//Argument to be sent to initiliazed client tasks
struct com_arg
{
	int ip_socket;
	char domain_socket[MAIN_COM_SIZE+1];
};
typedef struct com_arg com_arg;


//Monitor Alignment
int TOP_Alignment = 35;
int BOTTOM_Alignment = 35;
int RIGHT_Alingment = 20;
int LEFT_Alignment =20;

//Pixels of intrerest
int top_POI[Image_width];
int bottom_POI[Image_width];
int left_POI[Image_height];
int right_POI[Image_height];

//Create image vertices for LED setups
int generate_vertices()
{
	int i;
	
	for (i = 0; i < Image_width; i++)
	{
		top_POI[i] = Ipos2v(i, TOP_Alignment);
		bottom_POI[i] = Ipos2v(i,(Image_height-1)-BOTTOM_Alignment);
	}
	
	//printf("\n Image Height = %i", Image_height);
	for (i = 0; i < Image_height; i++)
	{
		//printf(" %i",i);
		left_POI[i] = Ipos2v(LEFT_Alignment,i);
		right_POI[i] = Ipos2v((Image_width-1)-RIGHT_Alingment,i);
	}
}


//Handles connected clients
void *ledpi_com(void*argu)
{
	com_arg *local_argu = argu;
	
	
	
	//Setup config variables
	led_config configuration[MAX_CONFIGS_PER_CLIENT];
	init_led_config(configuration);
	char config_buffer[SERIAL_CONFIG_SIZE+1];
	
	
	int i;
	
	//Message that client has started
	printf("\nLED Pi\t\tStarted with UDS |%s| and IPS |%i|", local_argu->domain_socket,local_argu->ip_socket);

	//Create
	int grab_socket = domain_socket_server_setup(local_argu->domain_socket,"LED Pi\t");
	printf("\nLED PI\t\tTask setup done");


	
	//Get Config
	
	read(local_argu->ip_socket, config_buffer, sizeof(config_buffer));
	printf("\nLED Pi\t\tRecieved Config:");
	config_demod(configuration, config_buffer);
	printf("\nLED Pi\t\t LED config:");
	for (i = 0; i < MAX_CONFIGS_PER_CLIENT; i++)
	{
		print_led_config(&configuration[i]);
	}
	
	printf("\nLED Pi\t\t Sending config to Grab Task");
	
	write(grab_socket, configuration, sizeof(configuration));
	
	
	printf("\nLED Pi\t\t|%s| ready to run", local_argu->domain_socket);
	//Ready for the real work!
	
	
	//Read data from Grab Task
		
	char *color_arr = malloc(171);
	char *temp_arr = malloc(RGB_width);
	char *color_recv = malloc(Image_width * 3);
	char A;
	int nBytes, j, sent_count = 0;
	while (1)
	{
		for (j = 0; j < MAX_CONFIGS_PER_CLIENT; j++)
		{
				
			if (configuration[j].pos == null)
				break;
			printf("\nGrab Task\twriting image slice to client");
				
			switch (configuration[j].pos)
			{

			case top:
				nBytes = read(grab_socket, color_recv, RGB_width);
				simple_down_sample((int*)color_recv, RGB_width, (int*)temp_arr, configuration[j].nr_of_leds*3);
				write(local_argu->ip_socket, temp_arr, configuration[j].nr_of_leds * 3);
				break;
			case bottom:
				nBytes = read(grab_socket, color_recv, RGB_width);
				simple_down_sample((int*)color_recv, RGB_width, (int*)temp_arr, configuration[j].nr_of_leds * 3);
				write(local_argu->ip_socket, temp_arr, configuration[j].nr_of_leds * 3);
				break;
			case left:
				nBytes = read(grab_socket, color_recv,  RGB_height);
				simple_down_sample((int*)color_recv, RGB_height, (int*)temp_arr, configuration[j].nr_of_leds * 3);
				write(local_argu->ip_socket, temp_arr, configuration[j].nr_of_leds * 3);
				break;
			case right:
				nBytes = read(grab_socket, color_recv, RGB_height);
				simple_down_sample((int*)color_recv, RGB_height, (int*)temp_arr, configuration[j].nr_of_leds * 3);
				write(local_argu->ip_socket, temp_arr, configuration[j].nr_of_leds * 3);
				break;	
			case null:
				break;
			}
				
			printf("\nLED Pi Task\t\t Image written to Client task");	
		}
		read(local_argu->ip_socket, &A, 1);
		
	}
}

int capture(FILE * utv007, char *left_vertical, char* right_vertical, char* top_horizontal, char* bottom_horizontal)
{
	int i, R, G, B, idx = 0;
	//Create YCbCr structure
	unsigned char *Y  = malloc(PIXEL_SIZE);
	unsigned char *Cb = malloc(CbCr);
	unsigned char *Cr = malloc(CbCr);
	
	//Run Frame grab
	DataCapture(utv007, Y, Cb, Cr);

	int k = 0;
	//use POI to set values in vertices
	while (idx < Image_height)
	{
		
		YCbCr2rgb(Y[left_POI[idx]], Cb[left_POI[idx] / 2], Cr[left_POI[idx] / 2], &R, &G, &B);
		
		left_vertical[k]	=R;
		left_vertical[k+1]	=G;
		left_vertical[k+2]	=B;
		
		YCbCr2rgb(Y[right_POI[idx]], Cb[right_POI[idx] / 2], Cr[right_POI[idx] / 2], &R, &G, &B);
		
		right_vertical[k]   =R;
		right_vertical[k+1] =G;
		right_vertical[k+2] =B;

		idx++;
		k += 3;
	}
	idx = 0;
	k = 0;
	while (idx < Image_width)
	{
		YCbCr2rgb(Y[top_POI[idx]], Cb[top_POI[idx] / 2], Cr[top_POI[idx] / 2], &R, &G, &B);
		
		top_horizontal[k]	= R;
		top_horizontal[k + 1] = G;
		top_horizontal[k + 2] = B;
		
		YCbCr2rgb(Y[bottom_POI[idx]], Cb[bottom_POI[idx] / 2], Cr[bottom_POI[idx] / 2], &R, &G, &B);
		//printf("\nR(%i) G(%i) B(%i)", R, G, B);
		bottom_horizontal[k]		= R;
		bottom_horizontal[k + 1]	= G;
		bottom_horizontal[k + 2]	= B;
		
		idx++;
		k += 3;
	}
	
	printf("\nGrab Task\t\t Capture Done!");
	free(Y);
	free(Cb);
	free(Cr);
	
	return 0;
}

//Task that extract data from UTV007
void *grab_task()
{
	
	//UTV007 Descriptor
	FILE * UTV007 = fopen(UTV007_DIR, "rb");
	
	
	int timer = 0, nr_of_active_clients = 0;
	
	//Initialize generic communication domain socket
	int socket = domain_socket_server_setup(MAIN_COM,"Grab Task");
	
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 1000 * 10;
	
	setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
	
	//Create Buffer
	char *buffer=malloc(MAIN_COM_SIZE+1);
	
	//Init client domain sockets array
	int *client_sockets = malloc(sizeof(int));
	grab_list *list_of_clients = malloc(sizeof(grab_list));
	
	int nBytes,i,j,capture_count=0;
	char config_txt[SERIAL_CONFIG_SIZE+1];
	//led_config configuration[MAX_CONFIGS_PER_CLIENT];
	
	//Dataset for framegrabbing
	
	char *left_vertical=malloc(Image_height*3);
	char *right_vertical=malloc(Image_height * 3);
	char *top_horizontal=malloc(Image_width*3);
	char *bottom_horizontal = malloc(Image_width * 3);
	
	//Main Loop
	while (1)
	{
		
		//Grabbing
		printf("\nGrab Task\t Grabbing......");
		capture(UTV007, left_vertical, right_vertical, top_horizontal, bottom_horizontal);

		
		//Updating Clients with data
		for (i = 0; i < nr_of_active_clients; i++)
		{
			for (j = 0; j < MAX_CONFIGS_PER_CLIENT; j++)
			{
				
				if (list_of_clients[i].config[j].pos == null)
					break;
				printf("\nGrab Task\twriting image slice to client");
				
				switch (list_of_clients[i].config[j].pos)
				{
				case null:
					break;
				case top:
					write(list_of_clients[i].socket_id, top_horizontal, sizeof(right_vertical[0])*Image_height * 3);					
					break;
				case bottom:
					write(list_of_clients[i].socket_id, bottom_horizontal, sizeof(right_vertical[0])*Image_height * 3);
					break;
				case left:
					write(list_of_clients[i].socket_id, left_vertical, sizeof(right_vertical[0])*Image_height * 3);
					break;
				case right:
					write(list_of_clients[i].socket_id, right_vertical, sizeof(right_vertical[0])*Image_height * 3);
					break;					
				}
				
				printf("\nGrab Task\t Image written to Client task");	
			}
			
		}
		
		
		//Check for new client socket
		if (timer >= 100)
		{
			//Wait for Connectionhandler to send new descriptor
			
			nBytes = read(socket, buffer, MAIN_COM_SIZE+1);
			printf("\nGrab Task\tChecking for new client descriptor Bytes Availabe(%i)",nBytes);
			if (nBytes > 0)
			{
				printf("\nGrab Task\tNew Client descriptor recieved");
				nr_of_active_clients++;
				
				printf("\nGrab Task\tConnecting to client socket |%s|", buffer);
				list_of_clients = realloc(list_of_clients, sizeof(grab_list)*nr_of_active_clients);		
							
				while(!(list_of_clients[nr_of_active_clients-1].socket_id = domain_socket_client_setup(buffer,"Grab Task")))
				{
					usleep(1000*100);
				}
				
				//Add Setup for client
				printf("\nGrab task\tReading config from new client");
				read(list_of_clients[nr_of_active_clients - 1].socket_id, list_of_clients[nr_of_active_clients - 1].config, sizeof(list_of_clients[nr_of_active_clients - 1].config));
				
				printf("\nGrab Task\t\t Configuration Recv:");
				for (i = 0; i < MAX_CONFIGS_PER_CLIENT; i++)
				{
					
					print_led_config(&list_of_clients[nr_of_active_clients - 1].config[i]);	
					//print_led_config(&configuration[i]);
				}
	

			}
			//}
			timer = 0;
		}
		timer++;
		usleep(1000 * 500);
		printf("\nGrab Task\t\t Frame Number: %i", capture_count++);
	}
	
	
}


//Handles incoming client requests
void *conn_handler()
{
	
	printf("\nConn Handler\tStarted...");
	sleep(1);
	
	
	int c, active_clients = 0;
	
	//Create Sockets
	int main_socket = domain_socket_client_setup(MAIN_COM,"Conn Handler");
	int *new_ip_sock,new_ip_socket, ip_socket = ip_socket_setup(IP_PORT,"Conn Handler");
	
	struct sockaddr_in client;
	
	
	char buffer[MAIN_COM_SIZE];
	
	
	//UNIX DOMAIN SETUP
	char **socket_array = malloc(sizeof(char**));
	
	srand(time(NULL));
	
	//Listen
	listen(ip_socket, 10);
     
	//Accept and incoming connection
	printf("\nConn Handler\tWaiting for Incoming connections...");
	c = sizeof(struct sockaddr_in);
	
	
	
	
	com_arg *argu;
	char acc;
	char config_txt[SERIAL_CONFIG_SIZE+1];
	while ((new_ip_socket = accept(ip_socket, (struct sockaddr *)&client, (socklen_t*)&c)))
	{
		argu = malloc(sizeof(com_arg));
		printf("\nConn Handler\tClient connected to IP-Socket");
		
		add_sock_name(&socket_array, active_clients);
		write(main_socket, socket_array[active_clients], strlen(socket_array[active_clients]));
		
		printf("\nConn Handler\tDomain Sockname |%s|", socket_array[active_clients]);
		
		pthread_t client_thread;
		new_ip_sock = malloc(1);
		*new_ip_sock = new_ip_socket;
		
		
		argu->ip_socket = *new_ip_sock;
		memcpy(argu->domain_socket, socket_array[active_clients], MAIN_COM_SIZE+1);
		
		printf("\nConn Handler\tTesting argu:domain_socket|%s|\t IP_socket|%i|", argu->domain_socket, argu->ip_socket);
		//if (pthread_create(&client_thread, NULL, ledpi_com, (void*)socket_array[active_clients]) < 0)
		if (pthread_create(&client_thread, NULL, ledpi_com, (void*)argu) < 0)
		{
			perror("could not create thread");
			return 1;
		}
		printf("\nConn Handler\tClient Thread Created");
		active_clients++;
		printf("\nConn Handler\tActive Clients: %i", active_clients);
		sleep(1);
	}
}

//Catch Ctrl+c
void signal_catcher(int sign)
{
	
	system("rm UD*");
	printf("\nRemoving UNIX domain Sockets\n And Exiting!");
	exit(EXIT_SUCCESS);
}



int main()
{

	
	
	generate_vertices();
	
	
	system("rm UD*");
	signal(SIGINT, signal_catcher);
	
	selector = " ";
	/*
	//TEST--------------------------------------------------------
	char *left_vertical = malloc(Image_height * 3);
	char *right_vertical = malloc(Image_height * 3);
	char *top_horizontal = malloc(Image_width * 3);
	char *bottom_horizontal = malloc(Image_width * 3);
	
	//UTV007 Descriptor
	FILE * UTV007 = fopen(UTV007_DIR, "rb");
	FILE *data = fopen("data", "w");
	FILE *vectors = fopen("vectors", "wb");
	
	//Grabbing
	capture(UTV007, left_vertical, right_vertical, top_horizontal, bottom_horizontal);
	int i=0;
	
	printf("\nRight Vertical");
	while (i < Image_height * 3)
	{
		printf("\nR G B = %i %i %i", right_vertical[i], right_vertical[i+1], right_vertical[i + 2]);
			
			i += 3;
	}
	
	int R, G, B, idx = 0,k;
	//Create YCbCr structure
	unsigned char *Y  = malloc(PIXEL_SIZE);
	unsigned char *Cb = malloc(CbCr);
	unsigned char *Cr = malloc(CbCr);
	
	unsigned char * RGB = malloc(PIXEL_SIZE * 3);
	
	//Run Frame grab
	printf("\nCapturing");
	DataCapture(UTV007, Y, Cb, Cr);
	i = 0;
	k = 0;
	printf("\nCreate RGB datatype");
	while (k < PIXEL_SIZE)
	{
		YCbCr2rgb(Y[k], Cb[k / 2], Cr[k / 2], (int*)&RGB[i], (int*)&RGB[i + 1], (int*)&RGB[i + 2]);
		k++;
		i += 3;
	}
	i = 0;
	k = 0;
	
	
	printf("\nWriting data to file");
	while (i < PIXEL_SIZE)
	{
		fprintf(vectors, "\n %i\t%i\t%i", RGB[k], RGB[k + 1], RGB[k + 2]);
		i++;
		k += 3;
	}
	//OUTPUT
	
	
	
	
	
	return 0;
	//TEST-----------------------------------------------------------------------------------

	*/
	
	
	

	//Create Threads
	pthread_t grab_thread, handler_thread;
	
	//Start Threads
	pthread_create(&grab_thread, NULL, grab_task,NULL);
	sleep(1);
	pthread_create(&handler_thread, NULL, conn_handler,NULL);
	
	
	while (1)
	{
		scanf("%c",&selector);
		
		if (selector == 'q')
		{
			break;	
		}
		else if (selector == 'g')
		{
			generate_vertices();
		}
		
	}

	
}