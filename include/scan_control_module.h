#include "module.h"
#include "scan_control.h"

#ifndef SCAN_MODULE
#define SCAN_MODULE


#define MOD_SCAN_HEAD						0x0010

#define MOD_SCAN_TASK_SET				0x00

#define MOD_SCAN_SETCMD_SET_X				0x00
#define MOD_SCAN_SETCMD_SET_Y				0x01
#define MOD_SCAN_SETCMD_STEPS				0x02
//#define MOD_SCAN_SETCMD_STEP_Y				0x03
#define MOD_SCAN_SETCMD_ORIGINATE	0x05

#define MOD_SCAN_TASK_SCAN			0x01

#define MOD_SCAN_SCANCMD_INC_X				0x00
#define MOD_SCAN_SCANCMD_INC_Y				0x01
#define MOD_SCAN_SCANCMD_DEC_X				0x02
#define MOD_SCAN_SCANCMD_DEC_Y				0x03

#define MOD_SCAN_REPORT_STEPS		0x00


static OS_TCB Scan_Set_TCB;
static CPU_STK Scan_Set_Stk[128];

static OS_TCB Scan_Scan_TCB;
static CPU_STK Scan_Scan_Stk[128];


extern void task_scan(void *p_arg);
extern void task_set(void* p_arg);

extern void scan_module_init();
extern void scan_task_init();
extern void scan_dispatch(void *msg);

#endif
