#include "module.h"
#include "scan_control.h"

#ifndef SCAN_MODULE
#define SCAN_MODULE


#define MOD_SCAN_HEAD						0x0010

#define MOD_SCAN_TASK_SCAN				0x00

#define MOD_SCAN_CMD_SET_INC			0x00
#define MOD_SCAN_CMD_SET_X				0x01
#define MOD_SCAN_CMD_SET_Y				0x02
#define MOD_SCAN_CMD_INC_X				0x03
#define MOD_SCAN_CMD_INC_Y				0x04
#define MOD_SCAN_CMD_DEC_X				0x05
#define MOD_SCAN_CMD_DEC_Y				0x06

#define MOD_SCAN_REPORT_STEPS		0x00

OS_Q ScanQ;

static OS_TCB Scan_TCB;
static CPU_STK Scan_Stk[128];


extern void task_scan(void *p_arg);

extern void scan_module_init();
extern void scan_task_init();
extern void scan_dispatch(void *msg);



#endif
