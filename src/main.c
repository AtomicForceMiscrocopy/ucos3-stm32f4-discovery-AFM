#include "stm32f4xx.h"
#include "includes.h"
#include "motor.h"
#include "app_cfg.h"
#include "module.h"


int
main(void)
{
	OS_ERR err;
	OSInit(&err);
	module_init();
	OSStart(&err);
}


