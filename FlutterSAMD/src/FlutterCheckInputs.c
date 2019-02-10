/*
 * FlutterCheckInputs.c
 *
 * Created: 1/21/2019 2:07:24 PM
 *  Author: raghu
 */ 
#include <asf.h>
#include "FlutterGlobals.h"
#include "FlutterI2CMaster.h"

extern volatile struct inputPorts  inputPort[4];
extern uint8_t inputsConnected[4];
extern uint8_t sensorOutputs[40];

#define PLACEVALUE_I2C_SLAVE_ADDRESS				0x12
#define PLACEVALUE_READ_DATA_LENGTH					0x04
#define PLACEVALUE_WRITE_DATA_LENGTH_1				73
#define PLACEVALUE_WRITE_DATA_LENGTH_2				0x05

#define SENSORS_I2C                                  5

void resetInput(uint8_t port)
{
	inputPort[port].address				=		0x00;
	inputPort[port].readDataLength     =		0x00;
    inputPort[port].writeDataLength1   =		0x00;
	inputPort[port].writeDataLength2   =		0x00;
	inputPort[port].initialization     =       false;
	inputPort[port].currentState		=		NOTHINGCONNECTED;
}


void selectLineLow(uint8_t port)
{
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction  = PORT_PIN_DIR_OUTPUT;
	
	switch(port)
	{
		case PORT1:
			port_pin_set_config(SEN1_DET_PIN, &config_port_pin);
			port_pin_set_output_level(SEN1_DET_PIN,false);
			break;
		case PORT2:
			port_pin_set_config(SEN2_DET_PIN, &config_port_pin);
			port_pin_set_output_level(SEN2_DET_PIN,false);
			break;
		case PORT3:
			port_pin_set_config(SEN3_DET_PIN, &config_port_pin);
			port_pin_set_output_level(SEN3_DET_PIN,false);
			break;
		default:
			break;
	}	
	
}

void selectLineDisable(uint8_t port)
{
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction  = PORT_PIN_DIR_INPUT;
	config_port_pin.input_pull  = PORT_PIN_PULL_NONE;
	
	switch(port)
	{
		case PORT1:
		port_pin_set_config(SEN1_DET_PIN, &config_port_pin);
		//port_pin_set_output_level(SEN1_DET_PIN,false);
		break;
		case PORT2:
		port_pin_set_config(SEN2_DET_PIN, &config_port_pin);
		//port_pin_set_output_level(SEN2_DET_PIN,false);
		break;
		case PORT3:
		port_pin_set_config(SEN3_DET_PIN, &config_port_pin);
		//port_pin_set_output_level(SEN2_DET_PIN,false);
		break;
		default:
		break;
	}
	
}

void  checkSetInputs()
{
	//check if
	//struct i2c_master_packet wr_packet;
	uint8_t i					= 0;
	uint8_t inputConnected		= 0;
	static uint8_t readBufferSensors[20] ;
	
	for(i=0; i<NO_SENSORS;i++)
	{
		inputConnected	= inputsConnected[i];
		if(inputConnected != inputPort[i].currentState)
		{
			switch(inputConnected)
			{
				case INPUT_PLACEVALUE:
					if(inputPort[i].initialization	  == 		false)
					{
						//initialize the I2C Module
						inputPort[i].address 			  =		PLACEVALUE_I2C_SLAVE_ADDRESS;
						inputPort[i].readDataLength      =		PLACEVALUE_READ_DATA_LENGTH;
						inputPort[i].writeDataLength1    =		PLACEVALUE_WRITE_DATA_LENGTH_1;
						inputPort[i].writeDataLength2    =		PLACEVALUE_WRITE_DATA_LENGTH_2;
						enableI2CModule(SENSORS_I2C);
						
						/*
						//Pull the select pin high
						//I2C write
						struct i2c_master_packet wr_packet = {
							.address		 = inputPort[i].address ,
							.data_length	 = inputPort[i].writeDataLength1,
							.data			 = &initWriteDataI2C,
							.ten_bit_address = false,
							.high_speed      = false,
							.hs_master_code  = 0x0,
						};
						I2CWrite((i+1) , &wr_packet);
						
						//Pull the select pin low
						delay_cycles_ms(5);
						*/
						/*
						while(outputPort[i].)
						{
						}
						*/
						struct i2c_master_packet rd_packet = {
							.address		 = inputPort[i].address,
							.data_length	 = inputPort[i].readDataLength,
							.data			 = readBufferSensors,
							.ten_bit_address = false,
							.high_speed      = false,
							.hs_master_code  = 0x0,
						};
						
						//Select line low
						selectLineLow(i+1);
						I2CRead(SENSORS_I2C , &rd_packet);
						//Select line high
						selectLineDisable(i+1);
						/*
						sensorOutputs[5 + i*5]		=	 i + 4;
						sensorOutputs[5 + i*5 + 1] =    readBufferSensors[0] ;
						sensorOutputs[5 + i*5 + 2] = 	 readBufferSensors[1] ;
						*/
						
						
						inputPort[i].initialization	      =		true;
						inputPort[i].currentState		  =		INPUT_PLACEVALUE;
					}
				//}
					break;
					
				case NOTHINGCONNECTED:
					switch(inputPort[i].currentState)
					{
						case INPUT_PLACEVALUE:
							resetInput(i);
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
