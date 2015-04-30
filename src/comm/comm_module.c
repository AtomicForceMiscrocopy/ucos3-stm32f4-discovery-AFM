#include "comm_module.h"
#include "comm.h"

void
comm_module_init()
{
	comm_init();

	struct module_stru temp;
	temp.module_head = MOD_COMM_HEAD;
	temp.count_tasks = 1;
	temp.dispatch = comm_dispatch;
	temp.taks_init[0] = comm_task_init;
	module_addtolist(temp, MOD_COMM_HEAD);
}

void
comm_task_init()
{
	OS_ERR err;
	OSQCreate(&SendDataQ, "SendDataQ", 30, &err);

	OSTaskCreate(
				(OS_TCB	*)&Comm_Send_TCB,
				(CPU_CHAR	*)"COMM",
				(OS_TASK_PTR)task_comm_send,
				(void	*)0,
				(OS_PRIO	)1,
				(CPU_STK	*)&Comm_Send_Stk[0],
				(CPU_STK_SIZE)Comm_Send_Stk[256 / 10],
				(CPU_STK_SIZE)256,
				(OS_MSG_QTY	)0,
				(OS_TICK	)0,
				(void	*)0,
				(OS_OPT)(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
				(OS_ERR *)&err
				);
}

void
comm_dispatch(void *msg)
{
	OS_ERR err;
	OSQPost(&SendDataQ,msg,sizeof(CMD_STRU),OS_OPT_POST_FIFO,&err);
}

void
task_comm_send(void *p_arg)
{
	OS_ERR err;
	OS_MSG_SIZE size;
	CPU_TS ts;
	CMD_STRU *msg;
	while (1)
	{
		msg = (CMD_STRU*)OSQPend(&SendDataQ,0,OS_OPT_PEND_BLOCKING,&size,&ts,&err);  //msg = OSQPend(...) ??
		module_msg_render((MSG_STRU*)msg,
				msg->cmd_head,
				msg->cmd_word,
				msg->para1,
				msg->para2,
				msg->para3);
		comm_IC_array_send((unsigned char*)msg,10);

	}
}
