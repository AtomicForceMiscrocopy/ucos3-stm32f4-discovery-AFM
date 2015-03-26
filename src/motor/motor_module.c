#include "motor_module.h"

void
motor_module_init()
{
	motor_init();

	struct module_stru temp;
	temp.module_head = MOD_MOTOR_HEAD;
	temp.count_tasks = 1;
	temp.dispatch = motor_dispatch;
	temp.taks_init[0] = motor_task_init;
	module_addtolist(temp, MOD_MOTOR_HEAD);
}

void
motor_task_init()
{
	OS_ERR err;
	OSQCreate(&MoveQ, "MoveQ", 10, &err);
	OSQCreate(&StopQ, "StopQ", 10, &err);

	OSTaskCreate(
				(OS_TCB	*)&Motor_Move_TCB,
				(CPU_CHAR	*)"Led Blink",
				(OS_TASK_PTR)task_motor_move,
				(void	*)0,
				(OS_PRIO	)2,
				(CPU_STK	*)&Motor_Move_Stk[0],
				(CPU_STK_SIZE)Motor_Move_Stk[256 / 10],
				(CPU_STK_SIZE)256,
				(OS_MSG_QTY	)0,
				(OS_TICK	)0,
				(void	*)0,
				(OS_OPT)(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
				(OS_ERR *)&err
				);

	OSTaskCreate(
				(OS_TCB	*)&Motor_Stop_TCB,
				(CPU_CHAR	*)"Led Blink11",
				(OS_TASK_PTR)task_motor_stop,
				(void	*)0,
				(OS_PRIO	)1,
				(CPU_STK	*)&Motor_Stop_Stk[0],
				(CPU_STK_SIZE)Motor_Stop_Stk[256 / 10],
				(CPU_STK_SIZE)64,
				(OS_MSG_QTY	)0,
				(OS_TICK	)0,
				(void	*)0,
				(OS_OPT)(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
				(OS_ERR *)&err
				);

}

void
motor_dispatch(void *msg)
{
	OS_ERR err;

	switch ( (((CMD_STRU*)msg)->cmd_head) >> 8)
	{
	case MOD_MOTOR_TASK_MOVE:
		OSQPost(&MoveQ, msg, sizeof(unsigned short)*5, OS_OPT_POST_FIFO, &err);
		break;
	case MOD_MOTOR_TASK_STOP:
		OSQPost(&StopQ, msg, sizeof(unsigned short)*5, OS_OPT_POST_FIFO, &err);
	}
}

void
task_motor_stop(void *p_arg)
{
	OS_ERR err;

	OS_MSG_SIZE size;
	CPU_TS ts;

	CMD_STRU *msg;
	while (1)
	{
		msg = (CMD_STRU*)OSQPend(&StopQ, 0, OS_OPT_PEND_BLOCKING, &size, &ts, &err);
		motor_stop();
	}
}

void
task_motor_move(void *p_arg)
{
	OS_ERR err;
	OS_MSG_SIZE size;
	CPU_TS ts;
	CMD_STRU *msg;
	CMD_STRU *send_msg = (CMD_STRU*)malloc(sizeof(CMD_STRU));
	unsigned short msg_send[6];
	unsigned short data[2];

	while (1)
	{
		msg = (CMD_STRU*)OSQPend(&MoveQ, 0, OS_OPT_PEND_BLOCKING, &size, &ts, &err);
		switch (msg->cmd_word & 0x00ff)
		{
		case MOD_MOTOR_CMD_SET_ORIGIN:
			motor_origin_set();

			data[0]=0x0000;
			data[1]=0x0000;

////			motor_render(data, MOD_COMM_HEAD, (MOD_COMM_TASK_SEND<<8) + MOD_COMM_CMD_SEND_INT,
////						(MOD_MOTOR_TASK_MOVE<<8) + MOD_MOTOR_REPORT_ORIGINATE, msg_send);
//			module_msg_render(send_msg,
//					MOD_COMM_HEAD,
//					(MOD_COMM_TASK_SEND<<8)+MOD_COMM_CMD_SEND_INT,
//					data[0])
			break;

		case MOD_MOTOR_CMD_STEP_FORWARD:
			motor_step_forward(msg->para1);

//			data[0] = 0x0000;
//			data[1] = *(msg+1);

//			motor_render(data, MOD_COMM_HEAD, (MOD_COMM_TASK_SEND<<8) + MOD_COMM_CMD_SEND_INT,
//						(MOD_MOTOR_TASK_MOVE<<8) + MOD_MOTOR_REPORT_STEPS, msg_send);
			break;

		case MOD_MOTOR_CMD_STEP_BACKWARD:
			motor_step_backward(msg->para1);

//			data[0] = *(msg+1);
//			data[1] = 0x0000;

//			motor_render(data, MOD_COMM_HEAD, (MOD_COMM_TASK_SEND<<8) + MOD_COMM_CMD_SEND_INT,
//						(MOD_MOTOR_TASK_MOVE<<8) + MOD_MOTOR_REPORT_STEPS, msg_send);
			break;

		case MOD_MOTOR_CMD_AUTO_FORWARD:
			data[0] = 0x0000;
			data[1] = MOTOR_SINGLE_STEP;
			while (motor_continue_check() == MOTOR_GOON)
			{
				motor_step_forward(MOTOR_SINGLE_STEP);
//				moter_render(data, MOD_COMM_HEAD, (MOD_COMM_TASK_SEND<<8) + MOD_COMM_CMD_SEND_INT,
//							(MOD_MOTOR_TASK_MOVE<<8) + MOD_MOTOR_REPORT_STEPS, msg_send);
				module_msg_dispatch(msg_send);

//				motor_render(data, MOD_COMM_HEAD, (MOD_COMM_TASK_SEND<<8) + MOD_COMM_CMD_BOARD_SEND_INT,
//							(MOD_PID_TASK_SET<<8) + MOD_PID_CMD_MOTOR_STOP, msg_send);
				*(msg_send+4) = MOD_PID_HEAD;
				module_msg_dispatch(msg_send);

				if (motor_check_stop()==MOTOR_STOP)
				{
					motor_reset_stop();
					break;
				}
				for (int i=0;i<MOTOR_STEP_DELAY;i++);
			}
//			motor_render(data, MOD_COMM_HEAD, (MOD_COMM_TASK_SEND<<8) + MOD_COMM_CMD_SEND_INT,
//						(MOD_MOTOR_TASK_MOVE<<8) + MOD_MOTOR_REPORT_STOP, msg_send);
			break;

		case MOD_MOTOR_CMD_AUTO_BACKWARD:
			motor_auto_backward();
			data[0] = MOTOR_SINGLE_STEP;
			data[1] = 0x0000;
			while (motor_continue_check() == MOTOR_GOON)
			{
				motor_step_backward(MOTOR_SINGLE_STEP);
//				motor_render(data, MOD_COMM_HEAD, (MOD_COMM_TASK_SEND<<8) + MOD_COMM_CMD_SEND_INT,
//							(MOD_MOTOR_TASK_MOVE<<8) + MOD_MOTOR_REPORT_STEPS, msg_send);
				module_msg_dispatch(msg_send);

				if (motor_check_stop()==MOTOR_STOP)
				{
					motor_reset_stop();
					break;
				}
				for (int i=0;i<MOTOR_STEP_DELAY;i++);
			}
//			motor_render(data, MOD_COMM_HEAD, (MOD_COMM_TASK_SEND<<8) + MOD_COMM_CMD_SEND_INT,
//						(MOD_MOTOR_TASK_MOVE<<8) + MOD_MOTOR_REPORT_STOP, msg_send);
			break;

		case MOD_MOTOR_CMD_ORIGINATE:
			motor_originate();
			int origin_point = motor_getorigin();
			data[0] = origin_point;
			if (origin_point>0)
			{
//				motor_render(data, MOD_MOTOR_HEAD, (MOD_MOTOR_TASK_MOVE<<8) + MOD_MOTOR_CMD_STEP_BACKWARD,
//							(MOD_MOTOR_TASK_MOVE<<8) + MOD_MOTOR_REPORT_ORIGINATE, msg_send);
			}
			else
			{
//				motor_render(data, MOD_MOTOR_HEAD, (MOD_MOTOR_TASK_MOVE<<8) + MOD_MOTOR_CMD_STEP_FORWARD,
//							(MOD_MOTOR_TASK_MOVE<<8) + MOD_MOTOR_REPORT_ORIGINATE, msg_send);
			}

			break;
		}
//		module_msg_dispatch(msg_send);
	}

}
