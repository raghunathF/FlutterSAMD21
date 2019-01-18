/*
 * CFile1.c
 *
 * Created: 10/12/2018 9:41:55 AM
 *  Author: Raghunath
 */ 
#include <asf.h>
#include "FlutterUART.h"
#include "FlutterGlobals.h"

#define MAX_RX_BUFFER_LENGTH   1

extern volatile uint8_t ringBuffer[MAX_LIMIT_RING_BUFFER];
extern volatile bool recDataStatus;
extern volatile uint8_t tailPointer;
extern volatile uint8_t headPointer;

//Configure UART with 115200 baudrate RX-- PB14 & TX -- PB12
void configureUSART()
{
	struct usart_config config_usart;
	usart_get_config_defaults(&config_usart);
	
	config_usart.baudrate    = BAUDRATE_BLE;
	config_usart.mux_setting = USART_SERCOM_MUX_SETTING;
	config_usart.pinmux_pad0 = USART_SERCOM_PINMUX_PAD0_TX;
	config_usart.pinmux_pad1 = USART_SERCOM_PINMUX_PAD1_UNUSED;
	config_usart.pinmux_pad2 = USART_SERCOM_PINMUX_PAD2_RX;
	config_usart.pinmux_pad3 = USART_SERCOM_PINMUX_PAD3_UNUSED;
	config_usart.generator_source = GCLK_GENERATOR_3;
	while (usart_init(&usart_instance,BLE_UART_MODULE, &config_usart) != STATUS_OK){}
	usart_enable(&usart_instance);
}

//USART callback with a ring buffer 
void usart_read_callback(struct usart_module *const usart_module)
{
	volatile uint16_t recData = 0;
	recData = (usart_instance.hw->USART.DATA.reg & SERCOM_USART_DATA_MASK);
	*(ringBuffer + headPointer) = (uint8_t)recData;
	headPointer++;
	recDataStatus = true;
}

//Configure and Enable the callbacks and also initiate the first read
void configureUSARTCallbacks()
{
	usart_register_callback(&usart_instance,usart_read_callback, USART_CALLBACK_BUFFER_RECEIVED);
	usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_RECEIVED);
}

//Main function 
void serialInit()
{
	uint8_t temp_buffer[MAX_RX_BUFFER_LENGTH];
	configureUSART();
	configureUSARTCallbacks();
	usart_read_buffer_job(&usart_instance,(uint8_t *)temp_buffer, MAX_RX_BUFFER_LENGTH);
}