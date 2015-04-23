#include "scan_control.h"

short output_inc_x=0;
short output_inc_y=0;
short output_x=0;
short output_y=0;
short x_range = 0;
short y_range = 0;

void scan_init()
{
	scan_ic_init();
}

void set_originate(void)
{
	output_x = 0;
	output_y = 0;
	scan_ic_output(SCAN_X, output_x);
	scan_ic_output(SCAN_Y, output_y);
}

void inc_x()
{
	output_x+=output_inc_x;
	scan_ic_output(SCAN_X, output_x );
}

void dec_x()
{
	output_x -= output_inc_x;
	scan_ic_output(SCAN_X, output_x);
}

void inc_y()
{
	output_y+=output_inc_y;
	scan_ic_output(SCAN_Y,output_y);
}

void dec_y()
{
	output_y -= output_inc_y;
	scan_ic_output(SCAN_Y, output_y);
}

void set_inc(short inc_x,short  inc_y)
{
	output_inc_x=inc_x;
	output_inc_y=inc_y;
}

inline void set_x_range(short value )
{
	x_range = value;
}

inline void set_y_range(short value)
{
	y_range = value;
}
