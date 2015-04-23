

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
void scan_ic_init(void);
void scan_ic_output(SCAN_DIR dir,short value);

void inc_x(void);
void dec_x(void);
void inc_y(void);
void dec_y(void);

void set_inc(short inc_x, short inc_y);

void set_x_range(short value);
void set_y_range(short value);

void set_originate(void);

void scan_gpio_config(void);
void scan_dac_config(void);


#endif
