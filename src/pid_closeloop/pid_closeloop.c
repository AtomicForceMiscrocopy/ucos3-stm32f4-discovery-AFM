#include "pid_clooseloop.h"

int auto_report_flag;
int pid_point_delay;

double pid_p;
double pid_i;
double pid_d;

double pid_z;
double pid_err;
double pid_sum;
double pid_derr;
double pid_setpoint;

void
pid_init()
{
	pid_IC_init();
	pid_p = PID_PARA_p;
	pid_i = PID_PARA_i;
	pid_d = PID_PARA_d;
	pid_setpoint = PID_PARA_setpoint;
	pid_sum = PID_PARA_sum;
	pid_derr = PID_PARA_derr;
	auto_report_flag = 0;
	pid_point_delay = PID_PARA_delay;
}

inline void
pid_handler(unsigned short signal)
{
	pid_err = pid_setpoint - signal;
	pid_sum += pid_err;
	if (pid_sum > PID_INT_LIMIT) pid_sum=PID_INT_LIMIT;
	if (pid_sum < -PID_INT_LIMIT) pid_sum=-PID_INT_LIMIT;

	pid_z = pid_p*pid_err + pid_i*pid_sum + pid_d*(pid_err-pid_derr);

	if (pid_z>0xfff) pid_z = 0xfff;
	if (pid_z<0) pid_z = 0;

	pid_IC_outputz(PID_OUTPUT_PORT_CHANNEL, ((unsigned short)pid_z) & 0x0FFF);
}

void
pid_setp(double p)
{
	pid_p = p;
}

void
pid_seti(double i)
{
	pid_i = i;
}

void
pid_setd(double d)
{
	pid_d = d;
}

void
pid_setsetpoint(double setpoint)
{
	pid_setpoint = setpoint;
}

void
pid_setdelay(int delay)
{
	pid_point_delay = delay;
}

void
pid_auto_report_enable()
{
	auto_report_flag = 1;
}

void
pid_auto_report_disable()
{
	auto_report_flag = 0;
}

void
pid_report_z(unsigned short *result)
{
	*result = (unsigned short)pid_z;
}

void
pid_report_err(unsigned short *result)
{
	*result = (unsigned short)pid_err;
}
