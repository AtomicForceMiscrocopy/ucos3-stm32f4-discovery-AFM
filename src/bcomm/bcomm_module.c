#include "bcomm.h"
#include "bcomm_module.h"


void
bcomm_module_init()
{
	bcomm_init();

	struct module_stru temp;
	temp.module_head = MOD_BCOMM_HEAD ;
	temp.count_tasks = 1;
	temp.dispatch = bcomm_dispatch;
	temp.taks_init[0] = bcomm_task_init;
	module_addtolist(temp,MOD_BCOMM_HEAD);
}

void
bcomm_task_init()
{
	OS_ERR err;
	OSQCreate(&SendDataQ,"SendDataQ",30,&err);

	OSTaskCreate(
					(OS_TCB	*)&Bcomm_Send_TCB,
					(CPU_CHAR	*)"BCOMM",
					(OS_TASK_PTR)task_bcomm_send,
					(void	*)0,
					(OS_PRIO	)1,
					(CPU_STK	*)&Bcomm_Send_Stk[0],
					(CPU_STK_SIZE)256/10,
					(CPU_STK_SIZE)256,
					(OS_MSG_QTY	)0,
					(OS_TICK	)0,
					(void	*)0,
					(OS_OPT)(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
					(OS_ERR *)&err
					);
}

void
bcomm_dispatch(void *msg)
{
	OS_ERR err;
	OSQPost(&SendDataQ,msg,sizeof(CMD_STRU),OS_OPT_POST_FIFO,&err);
}


void
task_bcomm_send(void *p_arg)
{
	OS_ERR err;
	OS_MSG_SIZE size;
	CPU_TS ts;
	CMD_STRU *msg;

	while(1)
	{
		msg=(CMD_STRU *)OSQPend(&SendDataQ,0,OS_OPT_PEND_BLOCKING,&size,&ts,&err);
		module_msg_render((MSG_STRU *)msg,
				msg->cmd_head,
				msg->cmd_word,
				msg->para1,
				msg->para2,
				msg->para3);
		bcomm_ic_array_send(msg,10);
	}
}





















