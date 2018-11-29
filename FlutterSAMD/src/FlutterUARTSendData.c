/*
 * UARTSendData.c
 *
 * Created: 10/18/2018 2:53:09 PM
 *  Author: raghu
 */ 
#include <asf.h>
#include "FlutterUARTSendData.h"
#include "FlutterUART.h"

#define   SEND_TIMER   TC7

extern bool	readySendData ; 
extern uint8_t sensorOutputs[20];

#define LENGTH_SENSORS_DATA  20


void configureTimer()
{
	struct tc_config config_tc;
	tc_get_config_defaults(&config_tc);
	struct tc_events config_events;
	config_tc.enable_capture_on_channel[0]		=		true;
	config_tc.counter_size						=		TC_COUNTER_SIZE_8BIT; //8
	config_tc.clock_source						=		GCLK_GENERATOR_0;
	config_tc.clock_prescaler					=		TC_CLOCK_PRESCALER_DIV1024; //1024
	config_tc.counter_8_bit.period				=		0xFF;
	tc_init(&tc_encoder_capture, SEND_TIMER , &config_tc);
	tc_enable(&tc_encoder_capture);
}

void sendDataBLE()
{
	if(readySendData)
	{
		readySendData = false;
		usart_write_buffer_wait(&usart_instance, sensorOutputs , LENGTH_SENSORS_DATA );
	}
}

void sendInfoCallback()
{
	readySendData = true;
	//usart_write_buffer_wait(&usart_instance, transmit_value ,VERSION_SET_LEN);
}


void configureTimerCallbacks()
{
	tc_register_callback(&tc_encoder_capture, sendInfoCallback,TC_CALLBACK_OVERFLOW);
	tc_enable_callback(&tc_encoder_capture, TC_CALLBACK_OVERFLOW);
}


//Send data to BLE every 30msec so that notifications can be sent in a timely manner
void configureSendInfo()
{
	configureTimer();
	configureTimerCallbacks();
}