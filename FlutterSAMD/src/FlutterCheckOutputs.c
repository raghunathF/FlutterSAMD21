/*
 * FlutterCheckOutputs.c
 *
 * Created: 11/1/2018 10:40:40 PM
 *  Author: raghu
 */ 
#include <asf.h>
#include "FlutterGlobals.h"
#include "FlutterI2CMaster.h"


#define LEDMATRIX_I2C_SLAVE_ADDRESS				0x11
#define LEDMATRIX_READ_DATA_LENGTH				0x02
#define LEDMATRIX_WRITE_DATA_LENGTH_1			73
#define LEDMATRIX_WRITE_DATA_LENGTH_2			0x05

extern uint8_t writeDataI2C[60][3];
extern uint8_t outputsConnected[4];
extern volatile struct outputPorts  outputPort[4];


void  checkSetOutputs();




void resetOutput(uint8_t port)
{
	outputPort[port].address			=		0x00;
	outputPort[port].readDataLength     =		0x00;
	outputPort[port].writeDataLength1   =		0x00;
	outputPort[port].writeDataLength2   =		0x00;
	outputPort[port].initialization     =       false;
	outputPort[port].currentState		=		NOTHINGCONNECTED;
};



void  checkSetOutputs()
{
	//check if
	//struct i2c_master_packet wr_packet;
	uint8_t i					= 0;
	uint8_t outputConnected		= 0;
	static uint8_t testWriteDataI2C[73] = {0x4C,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
										   0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,
										   0xFF,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,
										   0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,
										   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
										   0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
										   0x00,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
										   0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
										   0x00
										   };
	/*
	outputPort[3].address 			  =		LEDMATRIX_I2C_SLAVE_ADDRESS;
	outputPort[3].readDataLength     =		LEDMATRIX_READ_DATA_LENGTH;
	outputPort[3].writeDataLength1   =		LEDMATRIX_WRITE_DATA_LENGTH_1;
	outputPort[3].writeDataLength2   =		LEDMATRIX_WRITE_DATA_LENGTH_2;
	enableI2CModule(3+1);
	//Send the Logo data
	//I2C write
	
	struct i2c_master_packet wr_packet = {
		.address     = outputPort[3].address ,
		.data_length = outputPort[3].writeDataLength1,
		.data        = &testWriteDataI2C,
		.ten_bit_address = false,
		.high_speed      = false,
		.hs_master_code  = 0x0,
	};
	*/
	/*
	wr_packet.address		= (uint16_t)outputPort[3].address;
	wr_packet.data_length	= (uint16_t)outputPort[3].writeDataLength1;
	wr_packet.data			= &writeDataI2C;
	*/
	//I2CWrite((3+1) , &wr_packet);
	//delay_cycles_ms(5);
	//outputPort[3].currentState		  =		OUTPUT_LEDMATRIX;
	
	for(i=0; i<NO_OUTPUTS;i++)
	{
		outputConnected	= outputsConnected[i];
		if(outputConnected != outputPort[i].currentState)
		{
			switch(outputConnected)
			{
				case OUTPUT_LEDMATRIX:
					if(i == 3) // Temporary
					{	
						if(outputPort[i].initialization	  == 		false)
						{
							//initialize the I2C Module
							outputPort[i].address 			  =		LEDMATRIX_I2C_SLAVE_ADDRESS;
							outputPort[i].readDataLength     =		LEDMATRIX_READ_DATA_LENGTH;
							outputPort[i].writeDataLength1   =		LEDMATRIX_WRITE_DATA_LENGTH_1;
							outputPort[i].writeDataLength2   =		LEDMATRIX_WRITE_DATA_LENGTH_2;
							enableI2CModule(i+1);
							//Send the Logo data
							//I2C write
							struct i2c_master_packet wr_packet = {
								.address     = outputPort[i].address ,
								.data_length = outputPort[i].writeDataLength1,
								.data        = &testWriteDataI2C,
								.ten_bit_address = false,
								.high_speed      = false,
								.hs_master_code  = 0x0,
							};
							
							I2CWrite((i+1) , &wr_packet);
							delay_cycles_ms(5);
							/*
							while(outputPort[i].)
							{
								
							}
							*/
							outputPort[i].initialization	  =		true;
							outputPort[i].currentState		  =		OUTPUT_LEDMATRIX;
						}
					}
					break;
					
				case NOTHINGCONNECTED:
					switch(outputPort[i].currentState)
					{
						case OUTPUT_LEDMATRIX:
							resetOutput(i);
							disableI2CModule(i+1);
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}
		
	}
	
	
}
