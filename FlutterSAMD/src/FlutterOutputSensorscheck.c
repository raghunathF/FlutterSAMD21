/*
 * outputSensorscheck.c
 *
 * Created: 10/15/2018 11:45:35 AM
 *  Author: Raghunath
 */ 
#include	<asf.h>
#include	"FlutterOutputSensorscheck.h"
#include	"FlutterGlobals.h"
#include    "FlutterADC.h"

extern uint8_t NoInDevCon;
extern uint8_t NoOutDevCon;
extern uint8_t outputsConnected[4];
extern uint8_t inputsConnected[3];
extern uint8_t sensorOutputs[40];

#define SENSOR1_DET_AI			14
#define SENSOR2_DET_AI			13
#define SENSOR3_DET_AI			18

#define OUTPUT1_DET_AI			4	
#define OUTPUT2_DET_AI			5
#define OUTPUT3_DET_AI			6
#define OUTPUT4_DET_AI			8


#define MASK_UPPER_BYTE         0xFF00
#define MASK_LOWER_BYTE         0x00FF

#define MIN_THRES_LIGHT				100
#define MAX_THRES_LIGHT				110

#define MIN_THRES_DIAL				130
#define MAX_THRES_DIAL				140

#define MIN_THRES_DISTANCE			100
#define MAX_THRES_DISTANCE			120

#define MIN_THRES_PLACEVALUE		180
#define MAX_THRES_PLACEVALUE		240


#define MIN_THRES_SOUND				110
#define MAX_THRES_SOUND				120

#define MIN_THRES_SERVO				70
#define MAX_THRES_SERVO				90


#define MIN_THRES_LEDMATRIX			20
#define MAX_THRES_LEDMATRIX			50



#define MIN_THRES_NOTHING			240 
#define MAX_THRES_NOTHING           255

#define COUNT_CONNECTING_THRESHOLD	5


//Sensors and Analog input channels
#define SENSORS_DET_AI {SENSOR1_DET_AI , SENSOR2_DET_AI, SENSOR3_DET_AI};
#define OUTPUTS_DET_AI {OUTPUT1_DET_AI , OUTPUT2_DET_AI, OUTPUT3_DET_AI, OUTPUT4_DET_AI};


static const  uint8_t sensorDetectAnalogChannles[NO_SENSORS] = SENSORS_DET_AI;
static const  uint8_t outputDetectAnalogChannles[NO_OUTPUTS] = OUTPUTS_DET_AI;


uint8_t  inputOutputConnection(uint8_t analogInput)
{
	uint8_t inputOutput = 100;
	
	if((analogInput > MIN_THRES_LIGHT) && (analogInput <= MAX_THRES_LIGHT))
	{
		inputOutput	=		ANALOG_LIGHT_SENSOR;
	}
	else if((analogInput > MIN_THRES_DIAL) && (analogInput <= MAX_THRES_DIAL))
	{
		inputOutput	=		ANALOG_DIAL;
	}
	else if((analogInput > MIN_THRES_DISTANCE) && (analogInput <= MAX_THRES_DISTANCE))
	{
		inputOutput	=	    ANALOG_DISTANCE_SENSOR;
	}
	else if((analogInput > MIN_THRES_SOUND) && (analogInput <= MAX_THRES_SOUND))
	{
		inputOutput	=		ANALOG_SOUND_SENSOR;
	}
	else if((analogInput > MIN_THRES_SERVO) && (analogInput <= MAX_THRES_SERVO))
	{
		inputOutput	=		OUTPUT_SERVO_POSITION;
	}
	else if((analogInput > MIN_THRES_LEDMATRIX) && (analogInput <= MAX_THRES_LEDMATRIX))
	{
		inputOutput	=		OUTPUT_LEDMATRIX;
	}
	else if((analogInput > MIN_THRES_PLACEVALUE) && (analogInput <= MAX_THRES_PLACEVALUE))
	{
		inputOutput	=		INPUT_PLACEVALUE;
	}
	else if((analogInput > MIN_THRES_NOTHING) && (analogInput <= MAX_THRES_NOTHING))
	{
		inputOutput	=		NOTHINGCONNECTED;
	}
	return inputOutput;
}


void convertAnalogOutputs(uint8_t* analogOutput)
{
	static volatile uint8_t temp_test_outputs_connected_1[500];
	static volatile uint8_t temp_test_outputs_connected_2[500];
	static volatile uint8_t temp_test_outputs_connected_3[500];
	static uint8_t previousOutputsConnected[4] = {0,0,0,0};
	static uint8_t connecting[4];
	static uint8_t tempOutputsConnected[4];
	static uint8_t countConnecting[4];
	static uint16_t temp_count = 0;
	static uint8_t countDisconnection[4]= {0,0,0,0};
	uint8_t tempOutput = 0;
	uint8_t i =0;
	uint8_t tempOutputCount = 0;
	for(i=0;i<NO_OUTPUTS;i++)
	{
		tempOutput	=   inputOutputConnection(analogOutput[i]);
		if(tempOutput != NOTHINGCONNECTED)
		{
			countDisconnection[i] = 0;
			if((previousOutputsConnected[i] == NOTHINGCONNECTED) && (connecting[i] == false))
			{
				connecting[i] = true;
				tempOutputsConnected[i] = tempOutput;
			}
			else
			{
				if((connecting[i] == true) && (tempOutputsConnected[i] == tempOutput ))
				{
					countConnecting[i]++;
					if(countConnecting[i] > COUNT_CONNECTING_THRESHOLD)
					{
						outputsConnected[i] = tempOutput;
						connecting[i] = false;
						countConnecting[i] = 0;
					}
				}
				else
				{
					connecting[i] = false;
					countConnecting[i] = 0;
					tempOutputsConnected[i] = NOTHINGCONNECTED;
				}
			}
		}
		else
		{
			countDisconnection[i]++;
			if(countDisconnection[i] > COUNT_CONNECTING_THRESHOLD)
			{
				if(i==3)
				{
					countDisconnection[3] = 0;
				    outputsConnected[3]	= NOTHINGCONNECTED;
				}
				countDisconnection[i] = 0;
				outputsConnected[i]	= NOTHINGCONNECTED;
			}
		}
		
		previousOutputsConnected[i] = outputsConnected[i];
	}
	temp_test_outputs_connected_1[temp_count] = analogOutput[0] ;
	temp_test_outputs_connected_2[temp_count] = analogOutput[1] ;
	temp_test_outputs_connected_3[temp_count] = analogOutput[3] ;
	
	temp_count++;
	if(temp_count == 500)
	{
		temp_count = 0;
	}
	
}




void convertAnalogInputs(uint8_t* analogInputs)
{
	static volatile uint8_t temp_test_inputs_connected_1[500];
	static volatile uint8_t temp_test_inputs_connected_2[500];
	static volatile uint8_t temp_test_inputs_connected_3[500];
	static uint8_t previousInputsConnected[4] = {0,0,0,0};
	static uint8_t connecting[4];
	static uint8_t tempInputsConnected[4];
	static uint8_t countConnecting[4];
	static uint16_t temp_count = 0;
	static uint8_t countDisconnection[4]= {0,0,0,0};
	uint8_t tempInput = 0;
	uint8_t i =0;
	uint8_t tempInputCount = 0;
	for(i=0;i<NO_SENSORS;i++)
	{
		tempInput			=		inputOutputConnection(analogInputs[i]);
		if(tempInput != NOTHINGCONNECTED)
		{
			countDisconnection[i] = 0;
			if((previousInputsConnected[i] == NOTHINGCONNECTED) && (connecting[i] == false))
			{
				connecting[i] = true;
				tempInputsConnected[i] = tempInput;
			}
			else 
			{
				if((connecting[i] == true) && (tempInputsConnected[i] == tempInput ))
				{
					countConnecting[i]++;
					if(countConnecting[i] > COUNT_CONNECTING_THRESHOLD)
					{
						inputsConnected[i] = tempInput;
						connecting[i] = false;
						countConnecting[i] = 0;
					}
				}
				else
				{
					connecting[i] = false;
					countConnecting[i] = 0;
					tempInputsConnected[i] = NOTHINGCONNECTED;
				}
			}
		}
		else
		{
			countDisconnection[i]++;
			if(countDisconnection[i] > COUNT_CONNECTING_THRESHOLD)
			{
				countDisconnection[i]	= 0;
				inputsConnected[i]		= NOTHINGCONNECTED;
			}
		}
		
		previousInputsConnected[i] = inputsConnected[i];
	}
	
	/*
	if(tempInputCount != NoInDevCon)
	{
		NoInDevCon = tempInputCount;
	}
	*/
	
	temp_test_inputs_connected_1[temp_count] = analogInputs[0] ;
	temp_test_inputs_connected_2[temp_count] = analogInputs[1];
	temp_test_inputs_connected_3[temp_count] = analogInputs[2] ;
	
	temp_count++;
	if(temp_count == 500)
	{
		temp_count = 0;
	}
}



void checkOutputs()
{
	uint8_t i =0;
	static uint8_t analogOutput[4];
	for(i=0;i<4;i++)
	{
		analogOutput[i] = adc_start_read_result(outputDetectAnalogChannles[i]);
	}
	convertAnalogOutputs(analogOutput);
}


void checkInputs()
{
	uint8_t i =0;
	static uint8_t analogInput[4];
	for(i=0;i<3;i++)
	{
		analogInput[i]			= adc_start_read_result(sensorDetectAnalogChannles[i]);
	}
	convertAnalogInputs(analogInput);
}


void loadIdentifier()
{
	static uint32_t previous_identifier	= 0 ;
	uint32_t current_identifier	= 0 ;
	
	uint32_t temp				= 0;
	uint8_t  i					= 0;
	
	for(i=0;i<NO_OUTPUTS;i++)
	{
		temp		         =  outputsConnected[i];
		current_identifier  |=  temp << (28 - 4*i);
	}
	
	for(i=0;i<NO_SENSORS;i++)
	{
		temp		  =  inputsConnected[i];
		current_identifier  |=  temp << (11-(5*i));
	}

	if(current_identifier != previous_identifier)
	{
		for(i=0;i<4;i++)
		{
			sensorOutputs[i] = (current_identifier >> 8* (3-i)) & 0x000000FF ;
		}
		previous_identifier = current_identifier;
	}
}

void checkOutputsInputs()
{
	checkOutputs();
	checkInputs();
	loadIdentifier(); //Set the identifier which will be used in the notifications by the Bluetooth
}


void configureInputOutputDetection()
{
	configureADC();
}