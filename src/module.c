#include "module.h"

void (*init_function[10])()={comm_module_init,scan_module_init,bcomm_module_init};
unsigned short head_list[10] = {MOD_COMM_HEAD,MOD_SCAN_HEAD,MOD_BCOMM_HEAD};
int count_modules = 2;

void module_init()
{
	for (int i=0 ;i<count_modules; i++)
	{
		(*init_function[i])();
	}
	module_task_init();
}

void module_task_init()
{
	for (int i =0;i<count_modules;i++)
	{
		struct module_stru temp = modules_list[head_list[i]];
		for (int j = 0; j<temp.count_tasks; j++)
		{
			(*temp.taks_init[j])();
		}
	}
}

void module_msg_dispatch(CMD_STRU *msg)
{
	if (*modules_list[msg->cmd_head].dispatch)
	{
		(*modules_list[msg->cmd_head].dispatch)(msg);
	}
}

void module_addtolist(struct module_stru temp, unsigned short head)
{
	modules_list[head]=temp;
}

void module_msg_render(MSG_STRU *msg,
		unsigned short p1,
		unsigned short p2,
		unsigned short p3,
		unsigned short p4,
		unsigned short p5)
{
	MSG_STRU temp;
	temp.para1 = p1;
	temp.para2 = p2;
	temp.para3 = p3;
	temp.para4 = p4;
	temp.para5 = p5;
	*msg = temp;
}


