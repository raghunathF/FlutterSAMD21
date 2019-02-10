/*
 * FlutterReadSensors.c
 *
 * Created: 10/17/2018 2:49:39 PM
 *  Author: raghu
 */ 

//Check if any sensors are attached to the board and if yes see how to read them appropriately.

#include <asf.h>
#include "FlutterReadSensors.h"
#include "FlutterGlobals.h"
#include "FlutterADC.h"

#define SENSOR1_INPUT_AI 0
#define SENSOR2_INPUT_AI 12
#define SENSOR3_INPUT_AI 19

#define SENSOR_INPUTS_AI {SENSOR1_INPUT_AI , SENSOR2_INPUT_AI, SENSOR3_INPUT_AI};
static const  uint8_t inputAnalogChannles[NO_SENSORS] = SENSOR_INPUTS_AI;


#define ANALOG_LIGHT_SENSOR			31
#define ANALOG_DIAL					30
#define ANALOG_DISTANCE_SENSOR		29
#define ANALOG_SOUND_SENSOR			28
#define ANALOG_TEMPERATURE			26


#define LENGTH_SENSOR_VALUE				5
#define PLACE_VALUE						10

extern uint8_t NoInDevCon ;
extern uint8_t inputsConnected[3];
extern uint8_t sensorOutputs[40];

void readSensors()
{
	//Check if any sensors are connected
	//Switch case
	uint8_t i,j =0;
	//if( NoInDevCon > 0)
	//{
		for(i=0;i<NO_SENSORS;i++)
		{
			switch(inputsConnected[i])
			{
				case ANALOG_LIGHT_SENSOR:
					sensorOutputs[5*i + 4]     = i+1;
					sensorOutputs[5*i + 4 +1] = adc_start_read_result(inputAnalogChannles[i]);
					break;
				case ANALOG_DISTANCE_SENSOR:
					sensorOutputs[5*i + 4]     = i+1;
					sensorOutputs[5*i + 4 +1] = adc_start_read_result(inputAnalogChannles[i]);
					break;
				case ANALOG_SOUND_SENSOR:
					sensorOutputs[5*i + 4]     = i+1;
					sensorOutputs[5*i + 4 +1] = adc_start_read_result(inputAnalogChannles[i]);
					break;
				case ANALOG_DIAL:
					sensorOutputs[5*i + 4]     = i+1;
					sensorOutputs[5*i + 4 +1] = adc_start_read_result(inputAnalogChannles[i]);
					break;
				case ANALOG_TEMPERATURE:
					sensorOutputs[5*i + 4]     = i+1;
					sensorOutputs[5*i + 4 +1] = adc_start_read_result(inputAnalogChannles[i]);
					break;
				case PLACE_VALUE:
					//readConnectedI2CSensors();
					 
				default :
					/*
					for(j=0;j<LENGTH_SENSOR_VALUE; j++)
					{
						sensorOutputs[5*i + 4 + j] = 0;
					}
					*/
					break;
			}
		}
	//}
}

