

#ifndef SCAN_CONTROL
#define SCAN_CONTROL

#include "stm32f4xx.h"

#define	SCAN_OUTPUT_PORT					GPIOA
#define SCAN_OUTPUT_PORT_RCC			RCC_AHB1Periph_GPIOA
#define	SCAN_OUTPUT_DAC_RCC			RCC_APB1Periph_DAC

#define SCAN_OUTPUT_PIN							GPIO_Pin_4 | GPIO_Pin_5
#define SCAN_OUTPUT_X								DAC_Channel_1
#define SCAN_OUTPUT_Y								DAC_Channel_2

typedef enum
{
	SCAN_X=SCAN_OUTPUT_X,
	SCAN_Y=SCAN_OUTPUT_Y
}SCAN_DIR;

void scan_init(void);
void scan_ic_output(SCAN_DIR dir,uint16_t value);

void inc_x();
void inc_y();

void set_inc(uint32_t inc_x,uint32_t inc_y);

void set_x(uint16_t value);
void set_y(uint16_t value);

void scan_gpio_config(void);
void scan_dac_config(void);


#endif
