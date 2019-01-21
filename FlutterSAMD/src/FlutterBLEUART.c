/*
 * FlutterBLEUART.c
 *
 * Created: 10/18/2018 2:19:08 PM
 *  Author: raghu
 */ 
#include <asf.h>
#include "FlutterBLEUART.h"
#include "FlutterGlobals.h"
#include "FlutterUARTSendData.h"
#include "FlutterI2CMaster.h"

extern volatile uint8_t ringBuffer[MAX_LIMIT_RING_BUFFER];
extern volatile uint8_t tailPointer;
extern volatile uint8_t headPointer;
extern volatile struct outputPorts  outputPort[4];

uint8_t broadcastFlag	=	false;

#define	TEST					 1 

#define BROADCAST				'b'
#define START_BROADCAST			'g'
#define STOP_BROADCAST			's'

#define LEDMATRIX_SETALL		 'L'

#define COMMON_SETALL            'C' 

#define POSITION_SERVO			  2
#define RGB_LED					  1

#define LEN_BROADCAST_COMMAND	  1
#define LEN_LEDMATRIX_SETALL	  72

//#define LEN_COMMON_SETALL		

void UARTReceiveBytes(uint8_t* receiveData ,uint8_t countReceive)
{
	uint8_t i =0;
	while((headPointer - tailPointer) < countReceive )
	{
	}
	for(i=1;i<=countReceive;i++)
	{
		receiveData[i] = ringBuffer[tailPointer];
		tailPointer++;
	}
}

void transferLEDMatrixValues(uint8_t* tempTransfer , uint8_t port)
{
	uint8_t i =0;
	uint32_t err_code = 0;
	//static bool firstTime = true;
	static uint8_t LEDMatrixValues[LEN_LEDMATRIX_SETALL+1]; 
	for(i=0;i<(LEN_LEDMATRIX_SETALL+1);i++)
	{
		LEDMatrixValues[i] = tempTransfer[i] ;
	}
	struct i2c_master_packet wr_packet = {
		.address     = outputPort[port].address ,
		.data_length = outputPort[port].writeDataLength1,
		.data        = &LEDMatrixValues,
		.ten_bit_address = false,
		.high_speed      = false,
		.hs_master_code  = 0x0,
	};
	//while(outputPort[port].I2CStatus == BUSY_MODE);
	//outputPort[port].I2CStatus = BUSY_MODE;
	I2CWrite((port+1) , &wr_packet);
	delay_cycles_ms(1);
	I2CWrite((port+1) , &wr_packet);
	
	/*
	if(firstTime == false)
	{
		
	}
	firstTime = false;
	*/
	
}


void checkUART()
{
	uint8_t i =0;
	uint8_t port = 3;
	static uint8_t receivedValue[100];
	if(headPointer != tailPointer)
	{
		receivedValue[0]  = ringBuffer[tailPointer];
		tailPointer++;
		switch(receivedValue[0])
		{
			case TEST:
				break;
			case BROADCAST:
				UARTReceiveBytes(receivedValue , LEN_BROADCAST_COMMAND );
				if(receivedValue[1]  == START_BROADCAST)
				{
					broadcastFlag = true;
				}
				else if(receivedValue[1] == STOP_BROADCAST)
				{
					broadcastFlag = false;
				}
				break;
			case LEDMATRIX_SETALL:
				UARTReceiveBytes(receivedValue , LEN_LEDMATRIX_SETALL );
				for(port=0;port<4;port++)
				{
					if(outputPort[port].currentState == 	OUTPUT_LEDMATRIX )
					{
						transferLEDMatrixValues(receivedValue,port);
					}
				}
				
				//Get the port info
				//Get the packet info
				//update the flag to be sent to the 
				
				break;
			case COMMON_SETALL:
			/*
				UARTReceiveBytes(receivedValue, LEN_COMMON_SETALL) ;
				for(i=1;i<NO_OUTPUTS;i++)
				{
					switch(receivedValue[i*4 -3])
					{
						case POSITION_SERVO:
							checkSet(i,SERVO_CHECK);
							setPositionServo(i,receivedValue);
							break;
						case RGB_LED:
							//setRGBLED(i,receivedValue);
							break;
						default:
							break;
					}
				}
			*/
				break;
			case STOP_BROADCAST:
				break;
			default:
			    headPointer = 0;
			    tailPointer = 0;
				break;
		}
	}
	else if(broadcastFlag == true)
	{
		headPointer = 0;
		tailPointer = 0;
		sendDataBLE();
	}
	else
	{
		headPointer = 0;
		tailPointer = 0;
	}
}