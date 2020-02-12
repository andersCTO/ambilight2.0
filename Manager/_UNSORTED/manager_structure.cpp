#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <assert.h>

#include <stdlib.h>


#include "general_types.h"
#include "manager_structure.h"
#include "UDP_package.h"

using namespace std;

double distance(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
	double p1[2], p2[2];
	p1[0] = x1;
	p1[1] = y1;
	p2[0] = x2;
	p2[1] = y2;
	
	return sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2));
}

int locate_corresponding_grid_pos(const vector<led_pos> & grid, const vector<led_pos> & node_led_pos, vector<int> & image_idx)
{
	int min_index;
	double min_dist, new_dist;
	cout << "Grid search\t\tStart LED position estimation " << node_led_pos.size() << " LEDs\r\n";
	for (int node_led_idx = 0; node_led_idx < node_led_pos.size(); node_led_idx++)//loop through all node led's
	{
		min_dist = 2000000;
		//For every led, loop through entire grid
		for (int grid_idx = 0; grid_idx < grid.size(); grid_idx++)
		{
			new_dist = distance(node_led_pos[node_led_idx].x, node_led_pos[node_led_idx].y, grid[grid_idx].x, grid[grid_idx].y);
			if (new_dist < min_dist)
			{
				min_dist = new_dist;
				min_index = grid_idx;				
			}
			
		}
		image_idx[node_led_idx] = min_index;		
		
		/*
		cout << "LED " << node_led_idx << " (" << node_led_pos[node_led_idx].x << ',' << node_led_pos[node_led_idx].y << ") ~= (" 
			<< grid[min_index].x << ',' << grid[min_index].y << ") GRID "<<min_index<<"\r\n";
		*/
			
	}
	cout << "Grid search\t\tLED position search for " << node_led_pos.size() << " LEDs done!\r\n";
	return 0;
}

int distribute_image_data(vector <node_cell> & nodes, vector<rgb_value> & I)
{
	timespec t;
	t.tv_sec = 0;
	t.tv_nsec = 30000;
	
	node_cell tmp = node_cell();
	int data_idx,sock_f;
	char buff_test[10];
	
	//Loop through all Nodes
	for (int node_idx = 0; node_idx < nodes.size(); node_idx++)
	{
		if (nodes[node_idx].active == 1)
		{
			
			tmp.addr = nodes[node_idx].addr;
			tmp.nUDP_size = nodes[node_idx].nUDP_size;
			tmp.udp_stream.data.resize(nodes[node_idx].nUDP_size);
			tmp.udp_stream.com = (command*)&tmp.udp_stream.data[tmp.nUDP_size - 4];
			*tmp.udp_stream.com = com_led_data;
			//Move data from Image source to udp package
			
			data_idx = 0;				
			for (int led_idx = 0; led_idx < nodes[node_idx].led_count; led_idx++)
			{
				
				nodes[node_idx].udp_stream.data[data_idx++] = I[nodes[node_idx].image_indecies[led_idx]].red;
				nodes[node_idx].udp_stream.data[data_idx++] = I[nodes[node_idx].image_indecies[led_idx]].green;
				nodes[node_idx].udp_stream.data[data_idx++] = I[nodes[node_idx].image_indecies[led_idx]].blue;

			}				
		
			*nodes[node_idx].udp_stream.com = com_led_data;
			manager_send_udp(&nodes[node_idx]);
			nodes[node_idx].time_out_counter++;
		}
			
		
	}
	
	return 0;
}

int check_for_node(vector<node_cell> & N, node_cell* tmp_node)
{

	for (int i = 0; i < N.size(); i++)
	{
		
		//Check IP
		
		if (N[i].addr.sin_addr.s_addr == tmp_node->addr.sin_addr.s_addr)
		{
			return i;
		}
	}
	return -1;//Node not present
	
}

int add_node(vector<node_cell> & N, node_cell tmp_node)
{
	
	/*
	tmp_node->nUDP_size = sizeof(rgb_value) * tmp_node->led_count + 4;
	
	cout << "Add Node\t\tCreating new Node with:\r\n";
	cout << "Add Node\t\tAdress: " << inet_ntoa(tmp_node->addr.sin_addr) << "@" << ntohs(tmp_node->addr.sin_port) << "\r\n";
	cout << "Add Node\t\t" << tmp_node->led_count << " LEDs\r\n";
	cout << "Add Node\t\tUDP size: " << tmp_node->nUDP_size << "\r\n";
	*/
	
	tmp_node.nUDP_size = sizeof(rgb_value) * tmp_node.led_count + 4;
	
	cout << "Add Node\t\tCreating new Node with:\r\n";
	cout << "Add Node\t\tAdress: " << inet_ntoa(tmp_node.addr.sin_addr) << "@" << ntohs(tmp_node.addr.sin_port) << "\r\n";
	cout << "Add Node\t\t" << tmp_node.led_count << " LEDs\r\n";
	cout << "Add Node\t\tUDP size: " << tmp_node.nUDP_size << "\r\n";
	cout << "Add Node\t\tImage index size: " << tmp_node.image_indecies.size() << "\r\n";
	cout << "Add Node\t\tLED positions " << tmp_node.led_positions.size() << "\r\n";
	cout << "Add Node\t\tUDP-data size: " << tmp_node.udp_stream.data.size() << "\r\n";
	
	
	N.push_back(tmp_node);
	
	N.back().time_out_counter = 0;
	N.back().nUDP_size = tmp_node.nUDP_size;
	N.back().image_indecies.resize(tmp_node.led_count);
	N.back().active = 0;
	N.back().udp_stream.data.resize(tmp_node.nUDP_size);
	N.back().udp_stream.com = (command*)&N.back().udp_stream.data[N.back().nUDP_size-4];

	
	
	
	return 0;
}

int udp_to_led_pos(udp_package *data, vector<led_pos> & node_led_pos)
{
	int led_start_idx = data->data[0]*LED_POS_PER_UDP_PACK,
		data_idx = 1;
	cout << "Package " << (int)data->data[0] << " \r\n";
	for (int i = led_start_idx;i < (LED_POS_PER_UDP_PACK + led_start_idx); i++)
	{

		node_led_pos[i].x = ((short)data->data[data_idx] & 0xFF) | (((short)data->data[data_idx + 1] & 0xFF) << 8);
		node_led_pos[i].y = ((short)data->data[data_idx + 2] & 0xFF) | (((short)data->data[data_idx + 3] & 0xFF) << 8);
		//cout <<"LED: "<< i << " " << node_led_pos[i].x << " | " << node_led_pos[i].y << "\r\n"; 
		data_idx += 4;
		if (i == node_led_pos.size() - 1)
			break;
	}
	
	return 0;
}

int repack_gamma(udp_package *data2node)
{
	if (data2node->data.size()-4 < 256)
	{
		cout << "Send Gamma\t\tCan't send Gamma, to small UDP package\r\n";
		return -1;
	}
	for (int i = 0; i < 256; i++)
	{
		data2node->data[i] = GAMMA_MATRIX[i];		
	}
	cout << "Gamma Repack\t\tDone!\r\n";
	*data2node->com = com_gamma_matrix;
	return 0;
}

int clear_node(node_cell* Node, node_cell* tmp_node)
{
	//Clear old data
	Node->active = 0;
	Node->time_out_counter = 0;
	Node->nUDP_size = sizeof(rgb_value)*tmp_node->led_count + 4;
	Node->image_indecies.resize(tmp_node->led_count);
	Node->udp_stream.data.resize(Node->nUDP_size);
	Node->udp_stream.com = (command*)&Node->udp_stream.data[Node->nUDP_size - 4];
	Node->addr = tmp_node->addr;	
	
	cout << "Add Node\t\tAdded Node with:\r\n";
	cout << "Add Node\t\tAdress: " << inet_ntoa(Node->addr.sin_addr) << "@" << ntohs(Node->addr.sin_port) << "\r\n";
	cout << "Add Node\t\t" << Node->led_count << " LEDs\r\n";
	cout << "Add Node\t\tUDP size: " << Node->nUDP_size << "\r\n";
	
	
	return 0;
}

