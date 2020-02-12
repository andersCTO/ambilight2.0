#include <arpa/inet.h>
#include <sys/socket.h>
#include "general_types.h"


#define MANAGER_IP "192.168.0.150"
#define IP_PORT 1234
#define IM_IP_PORT 12345
#define NODE_UDP_PACKAGE_SIZE 100


#define LED_POS_PER_UDP_PACK 23


#include "com_com.h"

//Input LED count, Manager2Node data, Node2Manager data
int update_data_structures(int led_count, udp_package*data_in, udp_package*data_out);

int manager_udp_setup(int port);

int node_udp_setup(int port, udp_package*data_in);

int node_send_udp(udp_package*data_out);

int manager_recv_udp(udp_package*data_in, sockaddr_in *addr);

int node_recv_udp(udp_package*data_in);

int manager_send_udp(node_cell*node);