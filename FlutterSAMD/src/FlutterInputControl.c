/*
 * FlutterInputControl.c
 *
 * Created: 2/7/2019 7:02:30 AM
 *  Author: raghu
 */ 

#include <asf.h>
#include "FlutterInputControl.h"
#include "FlutterI2CMaster.h"
#include "FlutterGlobals.h"
#include "FlutterCheckInputs.h"


extern bool	readySendI2CReadInput ;
extern uint8_t sensorOutputs[40];
extern volatile struct inputPorts  inputPort[4];



void checkSendInputs()
{
	uint8_t i =0;
	uint8_t temp =0;
	static uint8_t rd_buffer[20] ;
	
	if(readySendI2CReadInput == true)
	{
		readySendI2CReadInput = false;
		for(i=0;i<NO_SENSORS;i++)
		{
			if(inputPort[i].currentState != NOTHINGCONNECTED)
			{
				switch(inputPort[i].currentState)
				{
					case INPUT_PLACEVALUE:
					
						sensorOutputs[4 + i*5]		=	 i + 1;
						struct i2c_master_packet rd_packet = {
							.address		 = inputPort[i].address,
							.data_length	 = inputPort[i].readDataLength,
							.data			 = rd_buffer,
							.ten_bit_address = false,
							.high_speed      = false,
							.hs_master_code  = 0x0,
						};
					    selectLineLow(i+1);
						I2CRead(SENSORS , &rd_packet);
						selectLineDisable(i+1);
						sensorOutputs[4 + i*5]		=	 i + 1;
						sensorOutputs[4 + i*5 + 1] =     rd_buffer[0] ;
						sensorOutputs[4 + i*5 + 2] = 	 rd_buffer[1] ;
						sensorOutputs[4 + i*5 + 3] =     rd_buffer[2] ;
						sensorOutputs[4 + i*5 + 4] = 	 rd_buffer[3] ;
					
						break;
					default:
					break;
				}
			}
		}
	}
}

