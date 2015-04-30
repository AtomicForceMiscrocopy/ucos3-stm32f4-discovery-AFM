#include "bcomm.h"


void
bcomm_init()
{
	bcomm_IC_init();
}




void
bcomm_cmd_send(short data, int cmd_word, int cmd_head)
{
	short cmd[BCOMM_CMD_WORD_SIZE];
}
