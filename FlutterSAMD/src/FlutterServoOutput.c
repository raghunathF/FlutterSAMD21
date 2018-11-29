/*
 * FlutterServoOutput.c
 *
 * Created: 11/2/2018 12:37:33 AM
 *  Author: raghu
 */ 

/********************************************************************************************/
#include <asf.h>
#include "FlutterServoOutput.h"

/********************************************************************************************/
#define SERVO4_CH	3
#define SERVO3_CH	2

#define SERVO4_WO	3
#define SERVO3_WO	2

struct tcc_module tcc_ss_instance0;

/********************************************************************************************/
void enable_super_servo()
{
	tcc_enable(&tcc_ss_instance0); //Enable the TCC module
}

void disable_super_servo()
{
	tcc_disable(&tcc_ss_instance0); //Disable the TCC module
}

/********************************************************************************************
Period  ---  8Mhz / (4*40000) = 500Hz = 2msec
Single slope |\	  |\
			 | \  | \
			 |  \ |  \
			 |   \|   \
TCC0 module
Four channels are used
********************************************************************************************/
void PositionServoTCCInit(bool servo4_enable , bool servo3_enable)
{
	struct tcc_config config_tcc_ss;
	tcc_get_config_defaults(&config_tcc_ss, TCC0);
	
	
	config_tcc_ss.counter.clock_prescaler = TCC_CLOCK_PRESCALER_DIV4;
	config_tcc_ss.counter.period = 0x9C40; //40000
	config_tcc_ss.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;
	
	
	config_tcc_ss.compare.wave_polarity[SERVO4_CH] = TCC_WAVE_POLARITY_0;
	config_tcc_ss.compare.wave_polarity[SERVO3_CH] = TCC_WAVE_POLARITY_0;
	
	
	config_tcc_ss.compare.match[SERVO4_CH] = 0;
	config_tcc_ss.pins.enable_wave_out_pin[SERVO4_WO] = servo4_enable;
	config_tcc_ss.pins.wave_out_pin[SERVO4_WO]        = PIN_PA18F_TCC0_WO2; 
	config_tcc_ss.pins.wave_out_pin_mux[SERVO4_WO]    = PINMUX_PA18F_TCC0_WO2; 
	
	
	config_tcc_ss.compare.match[SERVO3_CH] = 0;
	config_tcc_ss.pins.enable_wave_out_pin[SERVO3_WO] = servo3_enable;
	config_tcc_ss.pins.wave_out_pin[SERVO3_WO]        = PIN_PA19F_TCC0_WO3; 
	config_tcc_ss.pins.wave_out_pin_mux[SERVO3_WO]    = PINMUX_PA19F_TCC0_WO3; 
	
	
	tcc_init(&tcc_ss_instance0, TCC0, &config_tcc_ss);

}

void positionServoInit()
{
	PositionServoTCCInit(false ,false );
}
