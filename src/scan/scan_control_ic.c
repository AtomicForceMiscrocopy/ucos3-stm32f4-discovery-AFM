
#include "scan_control.h"

void scan_ic_init(void)
{
	scan_gpio_config();
	scan_dac_config();
}
void scan_ic_output(SCAN_DIR dir,short value)
{
	if (dir==SCAN_X)
	{
		DAC_SetChannel1Data(DAC_Align_12b_R, value & 0x0fff );
	}
	else
	{
		DAC_SetChannel2Data(DAC_Align_12b_R, value & 0x0fff);
	}
}

void scan_gpio_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(SCAN_OUTPUT_PORT_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SCAN_OUTPUT_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SCAN_OUTPUT_PORT, &GPIO_InitStructure);
}
void scan_dac_config(void)
{
	DAC_InitTypeDef DAC_InitStructure;

	RCC_APB1PeriphClockCmd(SCAN_OUTPUT_DAC_RCC, ENABLE);

	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
//	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095;

	DAC_Init(SCAN_OUTPUT_X , &DAC_InitStructure);
	DAC_Init( SCAN_OUTPUT_Y, &DAC_InitStructure);
	DAC_Cmd(SCAN_OUTPUT_X , ENABLE);
	DAC_Cmd( SCAN_OUTPUT_Y, ENABLE);
//	DAC_SoftwareTriggerCmd(SCAN_OUTPUT_Y, ENABLE);
//	DAC_SoftwareTriggerCmd(SCAN_OUTPUT_X, ENABLE);
	DAC_SetChannel1Data(DAC_Align_12b_R, 1000);
	DAC_SetChannel2Data(DAC_Align_12b_R, 0);
}
