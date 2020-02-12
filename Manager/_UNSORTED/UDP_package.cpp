#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <string.h>



//#include "network.h"
#include "UDP_package.h"
#include "com_com.h"
//#include "general_types.h"


using namespace std;

//Node and Manager adresses
static struct sockaddr_in Node_addr, Manager;
int slen = sizeof(Node_addr);
static int sock = -1,im_sock = -1;
static int dynamic_udp_package_size = -1;//UDP package size for Node(incoming).
static char receive_buffer[NODE_UDP_PACKAGE_SIZE];

int update_data_structures(int led_count, udp_package*data_in, udp_package*data_out)
{
	udp_package tmp;
	if (data_in != NULL)
	{
		data_in->data.resize(led_count * 3 + 4);
	}
	else
	{
		data_in = &tmp;
		data_in->data.resize(4);
	}
	
	data_in->com = (command*)&data_in->data[data_in->data.size()-4];
	*data_in->com = com_error;
	
	
	data_out->data.resize(NODE_UDP_PACKAGE_SIZE);
	
	data_out->com = (command*)&data_out->data[data_out->data.size() - 4];
	*data_out->com = com_error;
	cout << "Update data structers\tUpdated with size(M2N,N2M): (" << data_in->data.size() << "," << data_out->data.size()<<") and ";
	print_command(*data_in->com);
	cout << "& ";
	print_command(*data_out->com);
	cout << "\r\n";
	return 0;
}

int manager_udp_setup(int port)
{
	int *sock_p;
	if (port == IP_PORT)
	{
		sock_p = &sock;
	}
	else if (port == IM_IP_PORT)
	{
		sock_p = &im_sock;		
	}
	
	//create a UDP socket
	do
	{
		cout<<"Manager UDP Setup\tCreating Socket\r\n";
		*sock_p = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	} while (*sock_p < 0);
	cout << "Manager UDP Setup\tSocket Created\r\n";
	
	//Setup addr
	memset((char *) &Manager, 0, sizeof(slen));
	Manager.sin_family = AF_INET;
	Manager.sin_port = htons(port);
	Manager.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//bind socket to port
	do
	{
		cout << "Manager UDP Setup\tBinding socket to port "<<port<< " \r\n";
	} while (bind(*sock_p, (struct sockaddr*)&Manager, slen) < 0);
	printf("Manager UDP Setup\tBinding done!\r\n");
	return *sock_p;
}

int node_udp_setup(int port, udp_package*data_in)
{
	do
	{
		cout << "Node UDP Setup\tCreating Socket\r\n";
		sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	
	} while (sock < 0);
	cout << "Node UDP Setup\tSocket Created\r\n";
	
	//Setup addr
	memset((char *) &Node_addr, 0, slen);
	Node_addr.sin_family = AF_INET;
	Node_addr.sin_port = htons(IP_PORT);
	
	if (inet_aton(MANAGER_IP, &Node_addr.sin_addr) == 0) 
	{
		cout << "Node UDP Setup\tinet_aton() Failed!\r\n";
		return -1;
	}
	cout << "Node UDP Setup\tSetting Node incoming UDP package size to " << data_in->data.size() << "\r\n";
	
	cout << "Node UDP Setup\tDone!\r\n";
	
	return 0;	
}

int node_send_udp(udp_package*data_out)
{
	if (sock < 0)
	{
		cout << "Node send UDP\tNo socket present\r\n";
		return -1;
	}
	if (sendto(sock,&data_out->data[0], NODE_UDP_PACKAGE_SIZE, 0, (struct sockaddr *)&Node_addr, slen) < 0)
	{
		cout << "Node send UDP\tFailed to send\r\n";
		return -1;
	}
	return 0;
}

int manager_recv_udp(udp_package*data_in, sockaddr_in *addr)
{
	
	ssize_t recv_len;
	socklen_t len = sizeof(struct sockaddr);
	if (sock < 0)
	{
		cout << "Manager recv UDP\tNo socket present\r\n";
		return -1;
	}
	if (data_in->data.size() != NODE_UDP_PACKAGE_SIZE)
		cout << "Manager recv UDP\tERROR ERROR \r\n";
	
	
	do
	{
		recv_len = recvfrom(sock, &data_in->data[0], NODE_UDP_PACKAGE_SIZE, 0, (struct sockaddr*)addr, (socklen_t*)&len);	
	} while (recv_len < NODE_UDP_PACKAGE_SIZE);

	/*
	cout << "Manager recv UDP\tReceived ";
	print_command(*data_in->com);
	cout << "with data[0] "<< (int)data_in->data[0] <<  " from " << inet_ntoa(addr->sin_addr) << " @ " << ntohs(addr->sin_port) << "\r\n";
*/
	return recv_len;
}

int node_recv_udp(udp_package*data_in)
{
	ssize_t recv_len;
	
	if (sock < 0)
	{
		cout << "Node recv UDP\tNo socket present\r\n";
		return -1;
	}
	do
	{
		recv_len = recvfrom(sock, &data_in->data[0], data_in->data.size(), 0, (struct sockaddr*)&Node_addr, (socklen_t*)&slen);	
	} while (recv_len < data_in->data.size());
	/*
	cout << "Node recv UDP\tReceived ";
	print_command(*data_in->com);
	cout << "from " << inet_ntoa(Node_addr.sin_addr) << ntohs(Node_addr.sin_port) << "\r\n";
	*/
	return recv_len;
}

int manager_send_udp(node_cell*node)
{
	int l_sock;
	if (*node->udp_stream.com == com_led_data)
	{
		l_sock = im_sock;
	}
	else
	{
		l_sock = sock;
	}
		
	
	
	//unsigned char tmp[node->udp_stream.data.size()];
	if (l_sock < 0)
	{
		cout << "Manager send UDP\tNo socket present\r\n";
		return -1;
	}
	if (node->nUDP_size <= 0)
	{
		cout << "Manager send UDP\tnUDP size 0\r\n";
		return -1;
	}

	
	if (sendto(l_sock, &node->udp_stream.data[0], node->udp_stream.data.size(), 0, (struct sockaddr*)&node->addr, sizeof(struct sockaddr)) < 0)
	{
		cout << "Manager send UDP\tFailed to send\r\n";
		return -1;
	}
	return 0;
}