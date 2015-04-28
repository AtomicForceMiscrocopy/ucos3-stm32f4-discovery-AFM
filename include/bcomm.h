#ifndef BCOMM
#define BCOMM

#include "stm32f4xx.h"

#define BCOMM_USART				RCC_APB1Periph_USART3
#define BCOMM_USART				USART3
#define BCOMM_USART_PORT				RCC_AHB1Periph_GPIOB
#define BCOMM_USART_TX				GPIO_Pin_10
#define BCOMM_USART_RX				GPIO_Pin_11

#define BCOMM_RX_BUFF_SIZE				10



#endif
