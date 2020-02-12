#include <vector>
#include <iostream>
#include <fstream>
#include <string>
//#include <time.h>
#include <unistd.h>
#include <math.h>

#include "node_structure.h"
#include "general_types.h"
#include "UDP_package.h"

using namespace std;
char cfg_name[]  = "../../lib/left_sofa.txt";//192.168.0.200
//char cfg_name[]  = "../../lib/back_sofa.txt";//192.168.0.201
//char cfg_name[]  = "../../lib/right_sofa.txt";//192.168.0.202


int led_pos2udp(const vector<led_pos> & led_position, udp_package*data_out, udp_package*data_in)
{
	int data_idx,ret, 
		nr_of_packages = 0, 
		pos_count = led_position.size();
	char*tmp;
	*data_out->com = com_led_positions;
	cout << "LED position to UDP\tSending " << led_position.size() << " positions to Manager in " 
		<< ceil((float)led_position.size() / (float)LED_POS_PER_UDP_PACK) << " packages \r\n";
	
	cout << "Packaging positions accordingly\r\n";
	while (nr_of_packages < ceil((float)led_position.size() / (float)LED_POS_PER_UDP_PACK))
	{
			
		
		//Move data from pos to UDP package	

		data_idx = 1;
		data_out->data[0] = nr_of_packages;
		cout << "LED position to UDP\tPackage nr " << nr_of_packages << "\r\n";
		for (int i = nr_of_packages*LED_POS_PER_UDP_PACK; i < LED_POS_PER_UDP_PACK*(1 + nr_of_packages); i++)
		{
				

			data_out->data[data_idx++] = led_position[i].x & 0xFF;
			data_out->data[data_idx++] = (led_position[i].x >> 8) & 0xFF;
			data_out->data[data_idx++] = led_position[i].y & 0xFF; 
			data_out->data[data_idx++] = (led_position[i].y >> 8) & 0xFF;
			//cout <<"LED "<<i<<" with pos " << led_position[i].x << " "<<led_position[i].y << "\r\n";
			if (pos_count == 0)
			{
				break;				
			}
			else
			{
				pos_count--;
			}
		}
		//cout << "LED position to UDP\tPackage ID: " << nr_of_packages << "......";
		do
		{
			ret = node_send_udp(data_out);
		} while (ret < 0);
		

		//Wait on Ack
		do
		{
			ret = node_recv_udp(data_in);
		} while (ret < 0);
			
		if ((data_in->data[0] == nr_of_packages) && (*data_in->com == com_ack))
		{
			cout << "LED position to UDP\tACK on package: " << nr_of_packages << "\r\n";

		}
		else
			cout << "LED position to UDP\tACK on wrong package: " << (int)data_in->data[0] << "\r\n";
			
		nr_of_packages++;
	}
	cout << "LED position to UDP\tDone!\r\n";
	return 0;
}

int node_manager_cfg(vector<led_pos> & led_position, udp_package*data_in, udp_package*data_out, vector <unsigned char> & G)
{
	//Variables
	int ret;
	
	//Setup UDP Socket
	do
	{
		ret = node_udp_setup(IP_PORT, data_in);
	} while (ret < 0);
	
	while (1)//Sending LED Count
	{
		//Send LED data
		cout << "Node CFG\t\tSending...\r\n";
		data_out->data[0] = (data_in->data.size()-4) / 3;
		*data_out->com = com_led_count;		
		do
		{
			cout << "Node CFG\t\tSending UDP with ";
			print_command(*data_out->com);
			cout << "and datasize " << data_out->data.size()<<"\r\n";
			ret = node_send_udp(data_out); 
		} while (ret < 0);
		do
		{
			ret = node_recv_udp(data_in);
		} while (ret < 0);
		
		if (*data_in->com != com_ack)
		{
			
			sleep(1);	
		}
		else
		{
			cout << "Node CFG\t\tLED count ack received\r\n";
			break;
		}
		
	}
	
	while (1)//Send Positions
	{
		if (led_pos2udp(led_position, data_out, data_in) == 0)
			break;		
	}
		
	while (1)//Request Gamma Matrix
	{
		*data_out->com = com_gamma_matrix;
		ret = node_send_udp(data_out); 
		ret = node_recv_udp(data_in);
		
		if (*data_in->com == com_gamma_matrix)
			break;		
	}
	cout << "Gamma Matrix Recieved\r\n";
	//Move Gamma Matrix
	for (int i = 0; i < G.size(); i++)
	{
		G[i] = data_in->data[i];
	}
	cout << "Gamma Matrix Setup Done!\r\n";
	
	
	cout << "Node Setup Complete!\r\n";
	return 0;
	
	
}


int node_init(vector<led_pos> & led_position, udp_package*data_in, udp_package*data_out)
{
	
	//Load config_file
	load_led_pos_from_file(led_position, cfg_name);	
	
	//Update data structures
	update_data_structures(led_position.size(), data_in, data_out);

	return led_position.size();
}

int render_leds(NeoPixel*pixels, udp_package*data_in, vector<unsigned char> & G_matrix, vector<rgb_value> & filter,int filter_index)
{
	int data_idx = 0;
	for (int pixel_idx = 0; pixel_idx < pixels->numPixels(); pixel_idx++)
	{
		
		pixels->setPixelColor(pixel_idx,
			G_matrix[data_in->data[data_idx]], 
			G_matrix[data_in->data[data_idx + 1]], 
			G_matrix[data_in->data[data_idx + 2]]);
			
			/*(unsigned char)((int)G_matrix[data_in->data[data_idx]]		+ (int)filter[pixel_idx].red	+ (int)filter[pixel_idx + pixels->numPixels() - 1].red)/3, 
			(unsigned char)((int)G_matrix[data_in->data[data_idx + 1]]	+ (int)filter[pixel_idx].green	+ (int)filter[pixel_idx + pixels->numPixels()-1].green)/3, 
			(unsigned char)((int)G_matrix[data_in->data[data_idx + 2]]	+ (int)filter[pixel_idx].blue	+ (int)filter[pixel_idx + pixels->numPixels()-1].blue)/3);
			*/
		
		/*
		//Update filter values
		filter[pixel_idx + filter_index * (pixels->numPixels() - 1)].red	= G_matrix[data_in->data[data_idx]];
		filter[pixel_idx + filter_index * (pixels->numPixels() - 1)].green	= G_matrix[data_in->data[data_idx + 1]];
		filter[pixel_idx + filter_index * (pixels->numPixels() - 1)].blue	= G_matrix[data_in->data[data_idx + 2]];
		*/
		
		data_idx += 3;
	}
	
	
	
	return 0;
}