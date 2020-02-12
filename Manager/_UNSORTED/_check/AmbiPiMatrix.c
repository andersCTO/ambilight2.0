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

//RGB datatypes
rgb_macro_pixel rgb_data_array[PIXEL_SIZE];

rgb_macro_pixel *rgb_matrix[Image_height][Image_width];


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
	char domain_socket[MAIN_COM_SIZE + 1];
};
typedef struct com_arg com_arg;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;



//Handles connected clients
//void *ledpi_com(void*socket_name)
void *ledpi_com(void*argu)
{
	com_arg *local_argu = argu;
	
	
	
	//Setup config variables
	led_config configuration[MAX_CONFIGS_PER_CLIENT];
	init_led_config(configuration);
	char config_buffer[SERIAL_CONFIG_SIZE + 1];
	
	char * data_stream;
	int i;
	
	//Message that client has started
	printf("\nLED Pi\t\tStarted with IPS |%i|", local_argu->ip_socket);

	//Create
	printf("\nLED PI\t\tTask setup done");

	//Get Config from IP-socket
	
	read(local_argu->ip_socket, config_buffer, sizeof(config_buffer));
	printf("\nLED Pi\t\tRecieved Config:");
	config_demod(configuration, config_buffer);
	printf("\nLED Pi\t\t LED config:");
	for (i = 0; i < MAX_CONFIGS_PER_CLIENT; i++)
	{
		print_led_config(&configuration[i]);
	}
	int nr_of_leds = count_leds(configuration);
	int buffer_size = nr_of_leds * 3;
	data_stream = malloc(nr_of_leds*3);
	
	//Make place for biggest datatype width>height
	char *vector_stream = malloc(RGB_width);
	printf("\nLED Pi\t\t Client has %i LEDs", nr_of_leds);
		
	printf("\nLED Pi\t\t|%s| ready to run", local_argu->domain_socket);
	memset(data_stream, 100, nr_of_leds * 3);
	
	
	char A;
	//Ready for the real work!
	while (1)
	{
		//Create stream for IP-client
		
		for (i = 0; i < MAX_CONFIGS_PER_CLIENT; i++)
		{
			if (configuration[i].pos != null)
			{
				
				pthread_mutex_lock(&mutex1);
				rgb_matrix_to_vector(rgb_matrix, vector_stream, configuration[i]);
				pthread_mutex_unlock(&mutex1);
				printf("\nLED Pi\t\t Read from RGB Matrix");
				
				simple_down_sample((int*)vector_stream, (configuration[i].pos == top || configuration[i].pos == bottom) ? RGB_width : RGB_height, (int*) data_stream, configuration[i].nr_of_leds * 3);
				
				write(local_argu->ip_socket, data_stream, configuration[i].nr_of_leds*3);	
			}
		}
		
		//usleep(1000 * 50);
		//printf("\nLED Pi\t\t Data sent to IP-socket");
		read(local_argu->ip_socket, A, 1);		
		
		//Send stream to IP-client
	}
}

void capture(FILE * utv007,rgb_macro_pixel *rgb)
{
	int i, R, G, B, idx = 0;
	//Create YCbCr structure
	unsigned char *Y  = (unsigned char*)malloc(PIXEL_SIZE);
	unsigned char *Cb = (unsigned char*)malloc(CbCr);
	unsigned char *Cr = (unsigned char*)malloc(CbCr);
	
	//Run Frame grab
	DataCapture(utv007, Y, Cb, Cr);
	
	
	YCbcr_array2rgb_macropixel(rgb, Y, Cb, Cr);

	//printf("\nGrab Task\t\t Capture Done!");
	//free(Y);
	//free(Cb);
	//free(Cr);
}

//Task that extract data from UTV007
void *grab_task()
{
	
	//UTV007 Descriptor
	FILE * UTV007 = fopen(UTV007_DIR, "rb");
	
	//Main Loop
	printf("\nGrabbing started!");
	while (1)
	{
		
		//Grabbing
		printf("\nGrab Task\t Grabbing......");
		pthread_mutex_lock(&mutex1);
		capture(UTV007, rgb_data_array);
		pthread_mutex_unlock(&mutex1);
	}
	
	
}


//Handles incoming client requests
void *conn_handler()
{
	
	printf("\nConn Handler\tStarted...");
	sleep(1);
	
	
	int c, active_clients = 0;
	
	//Create Sockets
	int main_socket = domain_socket_client_setup(MAIN_COM, "Conn Handler");
	int *new_ip_sock, new_ip_socket, ip_socket = ip_socket_setup(IP_PORT, "Conn Handler");
	
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
	char config_txt[SERIAL_CONFIG_SIZE + 1];
	while ((new_ip_socket = accept(ip_socket, (struct sockaddr *)&client, (socklen_t*)&c)))
	{
		argu = malloc(sizeof(com_arg));
		printf("\nConn Handler\tClient connected to IP-Socket");
		
		//add_sock_name(&socket_array, active_clients);
		//write(main_socket, socket_array[active_clients], strlen(socket_array[active_clients]));
		
		//printf("\nConn Handler\tDomain Sockname |%s|", socket_array[active_clients]);
		
		pthread_t client_thread;
		new_ip_sock = malloc(1);
		*new_ip_sock = new_ip_socket;
		
		
		argu->ip_socket = *new_ip_sock;
		//memcpy(argu->domain_socket, socket_array[active_clients], MAIN_COM_SIZE + 1);
		
		//printf("\nConn Handler\tTesting argu:domain_socket|%s|\t IP_socket|%i|", argu->domain_socket, argu->ip_socket);
		//if (pthread_create(&client_thread, NULL, ledpi_com, (void*)socket_array[active_clients]) < 0)
		if (pthread_create(&client_thread, NULL, ledpi_com, (void*)argu) < 0)
		{
			perror("could not create thread");
			return 1;
		}
		printf("\nConn Handler\tClient Thread Created");
		active_clients++;
		printf("\nConn Handler\tActive Clients: %i", active_clients);
		sleep(10);
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
	system("rm UD*");
	signal(SIGINT, signal_catcher);
	
	selector = " ";

	
	//Setup RGB matrix as mask to data
	setup_rgb_matrix(rgb_data_array, rgb_matrix);

	//Create Threads
	pthread_t grab_thread, handler_thread;
	
	//Start Threads
	pthread_create(&grab_thread, NULL, grab_task, NULL);
	sleep(1);
	pthread_create(&handler_thread, NULL, conn_handler, NULL);
	
	
	while (1)
	{
		scanf("%c", &selector);
		
		if (selector == 'q')
		{
			break;	
		}
		sleep(10);
	}

	
}