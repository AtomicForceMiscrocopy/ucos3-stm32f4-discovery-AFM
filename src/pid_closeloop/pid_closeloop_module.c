#include "pid_closeloop_modul.h"

CPU_INT32U enable_flag = 0;

void
pid_module_init()
{
	pid_init();

	struct module_stru temp;
	temp.module_head = MOD_PID_HEAD;
	temp.count_tasks = 1;
	temp.dispatch = pid_dispatch;
	temp.taks_init[0] = pid_task_init;

	module_addtolist(temp, MOD_PID_HEAD);
}

void
pid_task_init()
{
	OS_ERR err;
	OSQCreate(&SETQ, "setQ", 100, &err);
	OSQCreate(&REQUESTQ,'requestQ', 100, &err);

	OSTaskCreate(
				(OS_TCB	*)&Pid_Run_TCB,
				(CPU_CHAR	*)"pid Run",
				(OS_TASK_PTR)task_pid_run,
				(void	*)0,
				(OS_PRIO	)3,
				(CPU_STK	*)&Pid_Run_Stk[0],
				(CPU_STK_SIZE)Pid_Run_Stk[1024 / 10],
				(CPU_STK_SIZE)1024,
				(OS_MSG_QTY	)0,
				(OS_TICK	)0,
				(void	*)0,
				(OS_OPT)(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
				(OS_ERR *)&err
				);

	OSTaskCreate(
				(OS_TCB	*)&Pid_Set_TCB,
				(CPU_CHAR	*)"pid set",
				(OS_TASK_PTR)task_pid_set,
				(void	*)0,
				(OS_PRIO	)2,
				(CPU_STK	*)&Pid_Set_Stk[0],
				(CPU_STK_SIZE)Pid_Set_Stk[64 / 10],
				(CPU_STK_SIZE)64,
				(OS_MSG_QTY	)0,
				(OS_TICK	)0,
				(void	*)0,
				(OS_OPT)(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
				(OS_ERR *)&err
				);

	OSTaskCreate(
				(OS_TCB	*)&Pid_Request_TCB,
				(CPU_CHAR	*)"pid set",
				(OS_TASK_PTR)task_pid_request,
				(void	*)0,
				(OS_PRIO	)1,
				(CPU_STK	*)&Pid_Request_Stk[0],
				(CPU_STK_SIZE)Pid_Request_Stk[64 / 10],
				(CPU_STK_SIZE)64,
				(OS_MSG_QTY	)0,
				(OS_TICK	)0,
				(void	*)0,
				(OS_OPT)(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
				(OS_ERR *)&err
				);

}

void
pid_dispatch(void *msg)
{
	OS_ERR err;
	unsigned short temp_task = ((CMD_STRU*)msg)->cmd_word>>8;
	switch (temp_task)
	{
	case MOD_PID_TASK_SET:
		OSQPost(&SETQ,msg+1, sizeof(unsigned short)*3, OS_OPT_POST_FIFO, &err);
	}
}

void
task_pid_request(void *p_arg)
{
	OS_ERR err;
	OS_MSG_SIZE size;
	CPU_TS ts;

	CMD_STRU *msg;
	CMD_STRU *send_msg=(CMD_STRU*)malloc(sizeof(CMD_STRU));

	while (1)
	{
		msg = (CMD_STRU*)OSQPend(&REQUESTQ, 0, OS_OPT_PEND_BLOCKING, &size, &ts, &err);
		switch(msg->cmd_word & 0x00ff)
		{
		case MOD_PID_REQUEST_Z:
			// send messages to the model where the request came from.
			break;
		case MOD_PID_REQUEST_ERR:
			break;
		case MOD_PID_REQUEST_INT:
			break;
		}
	}
}

void
task_pid_set(void *p_arg)
{
	OS_ERR err;
	OS_MSG_SIZE size;
	CPU_TS ts;

	CMD_STRU *msg;
	CMD_STRU *send_msg = (CMD_STRU*)malloc(sizeof(CMD_STRU));

	while (1)
	{

		msg =(CMD_STRU*) OSQPend(&SETQ, 0, OS_OPT_PEND_BLOCKING, &size, &ts, &err);
		switch (msg->cmd_word & 0x00ff)
		{
		case MOD_PID_CMD_ENABLE:
			enable_flag  = 1;
			break;

		case MOD_PID_CMD_SETD:
			pid_setd(msg->para1/10+msg->para2/100.0);
			break;

		case MOD_PID_CMD_SETI:
			pid_seti(msg->para1/10+msg->para2/100.0);
			break;

		case MOD_PID_CMD_SETP:
			pid_setp(msg->para1/10+msg->para2/100.0);
			break;

		case MOD_PID_CMD_SETDELAY:
			pid_setdelay(msg->para1);
			break;

		case MOD_PID_CMD_SETPOINT:
			pid_setsetpoint(msg->para1);
			break;
		}
	}
}

void
task_pid_run(void *p_arg)
{
	unsigned short signal;
	while (1)
	{
		if (enable_flag)
		{
			pid_value_signal(&signal);
			pid_handler(signal);
		}
	}
}
