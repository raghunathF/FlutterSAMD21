/*
 * FlutterGlobals.h
 *
 * Created: 10/15/2018 1:13:00 PM
 *  Author: raghu
 */ 


#ifndef FLUTTERGLOBALS_H_
#define FLUTTERGLOBALS_H_

#define NO_SENSORS		3
#define NO_OUTPUTS		4


//UART
#define BLE_UART_MODULE							SERCOM4
#define USART_SERCOM_MUX_SETTING				USART_RX_2_TX_0_XCK_1
#define USART_SERCOM_PINMUX_PAD0_TX				PINMUX_PB12C_SERCOM4_PAD0
#define USART_SERCOM_PINMUX_PAD1_UNUSED			PINMUX_UNUSED
#define USART_SERCOM_PINMUX_PAD2_RX				PINMUX_PB14C_SERCOM4_PAD2
#define USART_SERCOM_PINMUX_PAD3_UNUSED         PINMUX_UNUSED

#define BAUDRATE_BLE							115200

//RINGBUFFER
#define MAX_LIMIT_RING_BUFFER					255

//Port Identification
#define ANALOG_LIGHT_SENSOR						31
#define ANALOG_DIAL								30
#define ANALOG_DISTANCE_SENSOR					29
#define ANALOG_SOUND_SENSOR						28

//Outputs
#define OUTPUT_SERVO_POSITION					2
#define OUTPUT_LEDMATRIX						3

#define NOTHINGCONNECTED						0

#define IDLE_MODE								1
#define BUSY_MODE								2

struct outputPorts
{
	uint8_t address;
	uint8_t currentState;
	uint8_t nextState;
	uint8_t readDataLength;
	uint8_t writeDataLength1;
	uint8_t writeDataLength2;
	uint8_t initialization;
	uint8_t I2CStatus;
};

#endif /* FLUTTERGLOBALS_H_ */