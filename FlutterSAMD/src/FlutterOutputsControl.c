/*
 * FlutterOutputsControl.c
 *
 * Created: 11/20/2018 1:16:43 PM
 *  Author: Raghu
 */ 
#include <asf.h>
#include "FlutterOutputsControl.h"
#include "FlutterI2CMaster.h"
#include "FlutterGlobals.h"

extern bool	readySendI2CRead ;
extern uint8_t sensorOutputs[40];





static bool timerOutput = false;

extern volatile struct outputPorts  outputPort[4];

void checkSendOutputs()
{
	uint8_t i =0;
	uint8_t temp =0;
	static uint8_t rd_buffer[20] ;
	
	if(readySendI2CRead == true)
	{
		readySendI2CRead = false;
		for(i=0;i<NO_OUTPUTS;i++)
		{
			if(outputPort[i].currentState != NOTHINGCONNECTED)
			{
				switch(outputPort[i].currentState)
				{
					case OUTPUT_LEDMATRIX:
						
						//Read the dial values regularly 
						//if(i == 3)
						//{
						sensorOutputs[20 + i*5]		=	 i + 4;
						struct i2c_master_packet rd_packet = {
							.address		 = outputPort[i].address, 
							.data_length	 = outputPort[i].readDataLength,
							.data			 = rd_buffer,
							.ten_bit_address = false,
							.high_speed      = false,
							.hs_master_code  = 0x0,
						};
							
						//outputPort[i].I2CStatus =	BUSY_MODE; 
						I2CRead((i+1) , &rd_packet);
						sensorOutputs[20 + i*5]		=	 i + 4;
						sensorOutputs[20 + i*5 + 1] =    rd_buffer[0] ;
						sensorOutputs[20 + i*5 + 2] = 	 rd_buffer[1] ;
						//while(outputPort[i].I2CStatus == BUSY_MODE);
						//delay_cycles_ms(3);
						//sensor
						//read the sensor values 
						//}
						
						break;
					default:
						break;
				}
			}
		}
	}
}

