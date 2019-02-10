/*
 * FlutterADC.c
 *
 * Created: 10/18/2018 11:50:11 AM
 *  Author: raghu
 */ 
#include	<asf.h>
#include    "FlutterADC.h"
#define OUTPUT1_DET_AI			4	
uint16_t* ADCResult = NULL;

void configureADC()
{
	struct adc_config conf_adc;
	ADCResult = malloc(sizeof(uint16_t));
	
	adc_get_config_defaults(&conf_adc);

	conf_adc.reference			=  ADC_REFCTRL_REFSEL_INTVCC0_Val;
	conf_adc.clock_prescaler	=  ADC_CLOCK_PRESCALER_DIV32;
	conf_adc.positive_input		=  OUTPUT1_DET_AI;
	conf_adc.negative_input		=  ADC_NEGATIVE_INPUT_GND;
	conf_adc.resolution			=  ADC_RESOLUTION_8BIT;
	conf_adc.clock_source       =  GCLK_GENERATOR_3;
	//conf_adc.left_adjust		=  true;

	adc_init(&adc_instance, ADC, &conf_adc);
	adc_enable(&adc_instance);
}




uint16_t adc_start_read_result(const enum adc_positive_input analogPin)
{
	uint16_t temp = 0;
	adc_set_positive_input(&adc_instance, analogPin );
	adc_start_conversion(&adc_instance);
	while((adc_get_status(&adc_instance) & ADC_STATUS_RESULT_READY) != 1);
	adc_read(&adc_instance, ADCResult);
	temp = *ADCResult;
	return temp;
}