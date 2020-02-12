#pragma once

#include <vector>
#include "ambi_type_definitions.h"

#define MASTER_NAME "ambipi2"
#define MASTER_IP_PORT 1111

using namespace std;

int hostname_to_ip(char * hostname, char* ip);
int host_ip_socket_setup(int port);//returns server socket
int client_ip_socket_setup(int port, char*name);//returns socket connected to server
int send_led_setup(vector<LED_POS> & arr, int socket);//Send LED setup to master
