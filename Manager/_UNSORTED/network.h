#include <vector>
#include "general_types.h"




using namespace std;

int manager_network_setup(int port);

int recieve_node_setup(int sock, struct sockaddr_in *node_addr);

int node_setup_done(int sock, struct sockaddr_in node_addr);

int get_position_data(int sock, struct node_cell* node, vector<led_pos> & node_led_pos);

int recv_image(int sock, vector<rgb_value> & I);