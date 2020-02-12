#include <vector>
#include "Node_Specific/ws2812-rpi.h"



#include "general_types.h"


using namespace std;

int led_pos2udp(const vector<led_pos> & led_position, udp_package*data_out, udp_package*data_in);
int node_manager_cfg(vector<led_pos> & led_position, udp_package*data_in, udp_package*data_out, vector <unsigned char> & G);
int node_init(vector<led_pos> & led_position, udp_package*data_in, udp_package*data_out);
int render_leds(NeoPixel*pixels, udp_package*data_in, vector<unsigned char> & G_matrix, vector<rgb_value> & filter, int filter_index);