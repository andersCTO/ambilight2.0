#include<arpa/inet.h>
#include<sys/socket.h>
#include<stdint.h>



#include <vector>
#include "general_types.h"
using namespace std;





double distance(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

int locate_corresponding_grid_pos(const vector<led_pos> & grid, const vector<led_pos> & node_led_pos, vector<int> & image_idx);

int distribute_image_data(vector <node_cell> & nodes, vector<rgb_value> & I);

int check_for_node(vector<node_cell> & N, node_cell* tmp_node);

int add_node(vector<node_cell> & N, node_cell tmp_node);

int udp_to_led_pos(udp_package *data, vector<led_pos> & node_led_pos);

int repack_gamma(udp_package *data2node);

int clear_node(node_cell* Node, node_cell* tmp_node);





static char GAMMA_MATRIX[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	1,	1,	1,	1,	1,	1,
	1,	1,	1,	1,	1,	2,	2,	2,	2,	2,	2,	2,	2,	3,	3,	3,	3,	3,	3,	3,	4,	4,	4,	4,	4,	5,	5,	5,	5,	6,	6,	6,	6,	7,	7,	7,	7,	8,	8,	8,
	9,	9,	9,	10,	10,	10,	11,	11,	11,	12,	12,	13,	13,	13,	14,	14,	15,	15,	16,	16,	17,	17,	18,	18,	19,	19,	20,	20,	21,	21,	22,	22,	23,	24,	24,	25,	25,	26,	27,	27,
	28,	29,	29,	30,	31,	32,	32,	33,	34,	35,	35,	36,	37,	38,	39,	39,	40,	41,	42,	43,	44,	45,	46,	47,	48,	49,	50,	50,	51,	52,	54,	55,	56,	57,	58,	59,	60,	61,	62,	63,
	64,	66,	67,	68,	69,	70,	72,	73,	74,	75,	77,	78,	79,	81,	82,	83,	85,	86,	87,	89,	90,	92,	93,	95,	96,	98,	99,	101, 102, 104, 105, 107, 109, 110, 112, 114, 115,
	117, 119, 120, 122,	124, 126, 127, 129,	131, 133, 135, 137, 138, 140,142,144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,177,180,182,184,186,189,
	191,193,196,198,200,203,205,208,210,213,215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255}
	;