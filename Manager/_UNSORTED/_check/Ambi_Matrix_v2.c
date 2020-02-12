#include "funkis.h"
#include "yuv_matrix.h"
#include "comm_setup.h"
#include "Header.h"
#include <unistd.h>

#define IP_PORT 8888


MacroPixel Matrix[Image_width][Image_height];
char *data;
int active_clients;




void ledpi(void*ip_socket)
{
	int exit_func = 0;
	//Initial
	printf("\nLED Pi\t\tStarted!");
	int *socket_p = ip_socket, new_socket = *socket_p;
	
	///////////////Start config handshake
	
	//Get number of vectors
	char* tmp = malloc(3);	
	int nBytes = read(new_socket,tmp, 3);
	int nr_of_vectors = atoi(tmp);
	
	printf("\nLED Pi\t\tSystem reports %i vector(s)", nr_of_vectors);
	//free(tmp);
	
	
	//Create vector array
	vector_dp*pos_vector=malloc(sizeof(vector_dp)*nr_of_vectors);
	int i;
	
	for (i = 0; i < nr_of_vectors; i++){
		pos_vector[i].start = malloc(sizeof(Pos));
		pos_vector[i].start->x = 0;
		pos_vector[i].start->y = 0;
		pos_vector[i].end = malloc(sizeof(Pos));
		pos_vector[i].end->x = 0;
		pos_vector[i].end->y = 0;
		pos_vector[i].length = 0;
	}
	
	
	//Read pos vector from Client
	int vector_data_size = 5 * 4 * nr_of_vectors;//5 types & 4 digits per type
	char *vector_data = malloc(vector_data_size + 1);
	vector_data[vector_data_size] = '\0';
	read(new_socket, vector_data, vector_data_size);
	
	printf("\nLED Pi\t\tData stream |%s|", vector_data);
	
	//deserialize data vector
	de_serialize(pos_vector, vector_data, nr_of_vectors);
	
	print_pos_vector(pos_vector,nr_of_vectors);//prints the position vector read from serial data

	//Create Point of interest vector
	int number_of_leds = count_leds(pos_vector, nr_of_vectors);
	int vector_idx=0;
	Pos **vector = malloc(sizeof(Pos*)*number_of_leds);
	Pos **p_temp = malloc(sizeof(Pos*)*number_of_leds);
	for (i = 0; i < number_of_leds; i++)
	{
		vector[i] = malloc(sizeof(Pos));
		p_temp[i] = malloc(sizeof(Pos));
	}
	
	int j,k;
	for (i = 0; i < nr_of_vectors; i++)
	{
		get_vector_points(pos_vector[i].start,pos_vector[i].end, pos_vector[i].length, p_temp);
	
		for (j = 0; j < pos_vector[i].length; j++)
		{
			vector[vector_idx]->x = p_temp[j]->x;
			vector[vector_idx]->y = p_temp[j]->y;
			vector_idx++;
		}	
	}

	
	int frame=0;
	
	int buffer_size = number_of_leds * 3;
	char serial_data[buffer_size];
	memset(serial_data, 0, buffer_size);
	char A;
	int delay = 1;
	

	//->>>>>>>>>>>MAIN LOOP<<<<<<<<<<<<<<<<<<<<<<-
	while (1)
	{
		serialize_vector_gaussian(vector, number_of_leds, serial_data, Matrix,5);
		//serialize_vector(vector, number_of_leds, serial_data, Matrix);
		//nBytes = write(new_socket, serial_data, buffer_size);
		
		if ((nBytes = write(new_socket, serial_data, buffer_size)) <= 0)
		{
			printf("\nLED Pi\t\tClient Disconnected");
			break;
		}
		
		if ((nBytes = read(new_socket, &A, 1)) <= 0)
		{
			printf("\nLED Pi\t\tClient Disconnected");
			break;
		}
		usleep(1000*delay);
	}
	printf("\nLED Pi\t\tClosing Client thread");
	close(new_socket);
	free(vector);
	free(p_temp);
	free(pos_vector);
	active_clients--;
	
}

void conn_handler()
{
	printf("\nConn Handler\t\t Started!");
	int c;
	active_clients = 0;
	
	//Create Sockets
	int *new_ip_sock, new_ip_socket, ip_socket = ip_socket_setup(IP_PORT, "Conn Handler");
	
	struct sockaddr_in client;
	
	srand(time(NULL));
	
	//Listen
	listen(ip_socket, 10);
     
	//Accept and incoming connection
	printf("\nConn Handler\tWaiting for Incoming connections...");
	c = sizeof(struct sockaddr_in);
	
	char acc;
	while ((new_ip_socket = accept(ip_socket, (struct sockaddr *)&client, (socklen_t*)&c)))
	{
		printf("\nConn Handler\tClient connected to IP-Socket");
			
		pthread_t client_thread;
		new_ip_sock = malloc(1);
		*new_ip_sock = new_ip_socket;
		
		
		if (pthread_create(&client_thread, NULL, (void*)ledpi, (void*)&new_ip_socket) < 0)
		{
			perror("could not create thread");
		}
		printf("\nConn Handler\tClient Thread Created");
		active_clients++;
		printf("\nConn Handler\tActive Clients: %i", active_clients);
		
		printf("\nConn Handler\tWaiting for Incoming connections...");
	}
	
}

void grab_task()
{
	printf("\nGrab Task\t\t Started!");
	
	printf("\nGrab Task\t\t Opening Grabber");
	FILE * UTV007 = fopen(UTV007_DIR, "rb");
	int sizer = sizeof(data[0]),delay = 5;
	sleep(1);
	
	printf("\nGrab Task\t\t Start Grabbing");
	//Main loop
	while (1)
	{
		fread(data, sizer, Macro_pixel_size, UTV007);	
		usleep(1000*delay);
	}
	//save_matrix(Matrix);
	//printf("\nMatrix Saved!");
	fclose(UTV007);
	
	sleep(3);
}

int main()
{
	//Create datatypes
	data = malloc(Macro_pixel_size);
	setup_ycbcr_matrix(Matrix, data);
	
	
	pthread_t grab_thread, handler_thread;
	
	pthread_create(&grab_thread, NULL, (void*)grab_task, NULL);
	sleep(1);
	pthread_create(&handler_thread, NULL, (void*)conn_handler, NULL);
	
	while (1)
	{
		sleep(1);
	}
	
	return 0;
}