#include <vector>
#include <iostream>
#include <fstream>
#include <string>


#include "general_types.h"
using namespace std;

int load_led_pos_from_file(vector<led_pos> & data, char* filename)
{
	led_pos tmp;
	string line;
	//ifstream s_grid("../../lib/grid_setup.txt");
	ifstream s_grid(filename);
	int y_idx = 0;
	if (s_grid.is_open())
	{
		while (getline(s_grid, line))
		{
			//cout << line << '\t';
			y_idx = line.find('\t');
			line[y_idx] = '\n';
			
			tmp.x = stoi(line, NULL, 10);
			tmp.y = stoi(&line[y_idx + 1], NULL, 10);
			//cout << tmp.x << '|' << tmp.y << '\n';
			data.push_back(tmp);
		}
		cout << "Load LED position\tFile loaded with " << data.size() << " positions\r\n";
	}
	s_grid.close();
	return 0;
}