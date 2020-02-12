#include <iostream>


#include "com_com.h"

using namespace std;
void print_command(command com)
{
	cout << "command(";
	switch (com)
	{
	case com_error:
		cout << "Error";
		break;
	case com_led_data:
		cout << "LED data";
		break;
	case com_led_count:
		cout << "LED Count";
		break;
	case com_ack:
		cout << "Ack";
		break;
	case com_keep_alive:
		cout << "Keep Alive";
		break;
	case com_dead:
		cout << "Dead";
		break;
	case com_led_positions:
		cout << "LED positions";
		break;
	case com_gamma_matrix:
		cout << "Gamma Matrix";
		break;
	case com_spatial_filter:
		cout << "Spatial Filter";
		break;
	case com_linear_filter:
		cout << "Linear Filter";
			break;
	}
	cout << ") ";
}