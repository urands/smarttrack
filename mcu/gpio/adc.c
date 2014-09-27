#include "adc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"



void adc_init (void){
		ADC_InitTypeDef ADC_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
			/* ADCCLK = PCLK2/8 */
		RCC_ADCCLKConfig(RCC_PCLK2_Div8);

		/* ADC1 configuration ------------------------------------------------------*/
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_InitStructure.ADC_ScanConvMode = ENABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfChannel = 1;
		ADC_Init(ADC1, &ADC_InitStructure);
		/* ADC1 regular channel0 configuration */
		ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
		/* Enable ADC1 */
		ADC_Cmd(ADC1, ENABLE);
		/* Enable ADC1 reset calibration register */
		ADC_ResetCalibration(ADC1);
		/* Check the end of ADC1 reset calibration register */
		while(ADC_GetResetCalibrationStatus(ADC1));
		/* Start ADC1 calibration */
		ADC_StartCalibration(ADC1);
		/* Check the end of ADC1 calibration */
		while(ADC_GetCalibrationStatus(ADC1));
}
unsigned int adc_get_value (void){
		ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
		return ADC_GetConversionValue(ADC1);
}