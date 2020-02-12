#include<arpa/inet.h>
#include<sys/socket.h>

#include <vector>

#include "com_com.h"

#ifndef GT
#define GT

#define IMAGE_WIDTH 320
#define IMAGE_HEIGHT 240

#define GRID_WIDTH 320
#define GRID_HEIGHT 240

using namespace std;
	
struct rgb_value
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

struct ycbcr_value
{
	unsigned char *y;
	unsigned char *cb;
	unsigned char *cr;
};
//Indicates spatial position of LED in room in mm (-16 384....16 384)
struct led_pos
{
	short int x;
	short int y;	
};

struct node_data_incoming
{
	command com;
	vector<rgb_value> LEDs;
};
struct node_data_outgoing
{
	command com=com_error;
	unsigned char idx=0;
	vector<unsigned char>data;
};

struct udp_package
{
	command *com;
	vector<char>data;
};

struct node_cell
{
	uint16_t led_count;//Nr of LEDs
	struct sockaddr_in addr;//UDP addr and port to node
	vector<int>image_indecies;//Inidacting which rgb value
	int nUDP_size = 0;
	int time_out_counter = 0;
	int active = 0;
	udp_package udp_stream;
	vector<led_pos>led_positions;
};

int load_led_pos_from_file(vector<led_pos> & data, char* filename);






















#endif // !rgb_value


