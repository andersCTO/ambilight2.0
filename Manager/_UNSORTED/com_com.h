#ifndef comcom
#define comcom
enum command
{
	com_error,
	com_led_data,
	com_led_count,
	com_ack,
	com_keep_alive,
	com_dead,
	com_led_positions,
	com_gamma_matrix,
	com_spatial_filter,
	com_linear_filter,
	com_kill
};

void print_command(command com);
#endif // !comcom

