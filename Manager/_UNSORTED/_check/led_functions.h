#include <vector>

#include "ws2812-rpi.h"
#include "ws2812-rpi-defines.h"
#include "lib/ambi_type_definitions.h"


using namespace std;



void set_led_color(char*GAMMA,int index,char *red,char* green, char blue);

void led_node_init(vector <char> & gamma, vector <LED_POS> & led_positions, vector <char> & led_data_buffer);

void save_node_cfg(vector <LED_POS> & led_positions);

int load_node_cfg(vector <LED_POS> & led_positions);