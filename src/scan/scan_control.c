#include "scan_control.h"

uint32_t output_inc_x=0;
uint32_t output_inc_y=0;
uint32_t output_x=0;
uint32_t output_y=0;

void scan_init()
{
	scan_IC_init();
}

void inc_x()
{
	output_x+=output_inc_x;
	scan_ic_output(SCAN_X, output_x>>20);
}
void inc_y()
{
	output_y+=output_inc_y;
	scan_ic_output(SCAN_Y,output_y>>20);
}
void set_inc(uint32_t  inc_x,uint32_t inc_y)
{
	output_inc_x=inc_x;
	output_inc_y=inc_y;
}

inline void set_x(uint16_t value )
{
	scan_ic_output(SCAN_X, value);
}
inline void set_y(uint16_t value)
{
	scan_ic_output(SCAN_Y, value);
}
