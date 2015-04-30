#ifndef BCOMM
#define BCOMM

#include "stm32f4xx.h"

#define BCOMM_USART_APB1				RCC_APB1Periph_USART3
#define BCOMM_USART				        USART3
#define BCOMM_USART_PORT				RCC_AHB1Periph_GPIOB
#define BCOMM_USART_TX				    GPIO_Pin_10
#define BCOMM_USART_RX				    GPIO_Pin_11

#define BCOMM_RX_BUFF_SIZE				10
#define BCOMM_CMD_WORD_SIZE             4

void bcomm_IC_init();
void bcomm_IC_send(unsigned char data);
void bcomm_IC_array_send(unsigned char *data,int len);
void USART3_IRQHandler(void);

void bcomm_init();


#endif
