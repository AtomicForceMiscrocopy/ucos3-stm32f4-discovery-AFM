#include "bcomm.h"
#include "bcomm_module.h"

unsigned char brxbuff[BCOMM_RX_BUFF_SIZE];
unsigned char brxbuff_count;

void bcomm_IC_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB1PeriphClockCmd(BCOMM_USART_APB1,ENABLE);
	RCC_AHB1PeriphClockCmd(BCOMM_USART_PORT,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = BCOMM_USART_TX;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);

	GPIO_InitStructure.GPIO_Pin = BCOMM_USART_RX;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(BCOMM_USART,&USART_InitStructure);
	USART_Cmd(BCOMM_USART,ENABLE);

	USART_ITConfig(BCOMM_USART,USART_IT_RXNE,ENABLE);
	NVIC_EnableIRQ(USART3_IRQn);

	for (int i=0;i<BCOMM_RX_BUFF_SIZE;i++)
		brxbuff[i] = 0;


}


void
bcomm_IC_send(unsigned char data)
{
	while(USART_GetFlagStatus(BCOMM_USART,USART_FLAG_TXE)==RESET);
	USART_SendData(BCOMM_USART,data);
}

void
bcomm_IC_array_send(unsigned char *data,int len)
{
	int i=0;
	while(i<len)
	{
		bcomm_ic_send(*(data+i));
		i++;
	}
}

void
USART3_IRQHandler(void)
{
	if (USART_GetITStatus(BCOMM_USART, USART_IT_RXNE) != RESET)
	{
		unsigned char c = USART_ReceiveData(BCOMM_USART);
		brxbuff[brxbuff_count]=c;
		brxbuff_count++;
		if ((brxbuff_count%2)==0)
		{
			unsigned char temp;
			temp = brxbuff[brxbuff_count-1];
			brxbuff[brxbuff_count-1]=brxbuff[brxbuff_count-2];
			brxbuff[brxbuff_count-2]=temp;
		}
		if (brxbuff_count == BCOMM_RX_BUFF_SIZE)
		{
			USART_ITConfig(BCOMM_USART, USART_IT_RXNE, DISABLE);
			module_msg_dispatch((CMD_STRU*)brxbuff);
			brxbuff_count = 0;
			USART_ITConfig(BCOMM_USART, USART_IT_RXNE, ENABLE);
		}
	}

}














