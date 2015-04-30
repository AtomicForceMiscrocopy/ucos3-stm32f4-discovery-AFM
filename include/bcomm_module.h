#include "bcomm.h"
#include "module.h"

#ifndef BCOMM_MODULE
#define BCOMM_MODULE

#define MOD_BCOMM_HEAD       0x0014

OS_Q SendDataQ;
static OS_TCB Bcomm_Send_TCB;
static CPU_STK Bcomm_Send_Stk[256];

void bcomm_module_init();
void bcomm_task_init();
void bcomm_dispatch(void *msg);
void comm_render(unsigned short *data, unsigned short des_head, unsigned short des_word, unsigned short ori_task_interface, unsigned short *msg);

void task_bcomm_send(void *p_arg);

#endif
