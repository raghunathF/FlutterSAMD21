#include <asf.h>

#include "FlutterOutputSensorscheck.h"
#include "FlutterReadSensors.h"
#include "FlutterUART.h"
#include "FlutterGlobals.h"
#include "FlutterUARTSendData.h"
#include "FlutterBLEUART.h"
#include "FlutterI2CMaster.h"
#include "FlutterCheckOutputs.h"
#include "FlutterCheckInputs.h"
#include "FlutterOutputsControl.h"
#include "FlutterInputControl.h"



uint8_t sensorOutputs[40];
uint8_t outputsConnected[4];
uint8_t inputsConnected[3] = {0x00 , 0x00 , 0x00} ;
uint8_t NoOutDevCon = 0;
uint8_t NoInDevCon = 0;
uint8_t writeDataI2C[60][5];
bool readySendData		= false;
bool readySendI2CRead	= false;
bool	readySendI2CReadInput = false;

volatile uint8_t ringBuffer[MAX_LIMIT_RING_BUFFER];
volatile bool recDataStatus;
volatile uint8_t tailPointer;
volatile uint8_t headPointer;


volatile struct outputPorts  outputPort[4];
volatile struct inputPorts   inputPort[4];

/*
struct outputPortGen outputPort;
struct inputPortGen  inputPort;
struct outputPortGen
{
	uint8_t connectionStatus[4];
	uint8_t NoConnectedDevices;
};

struct inputPortGen
{
	uint8_t connectionStatus[3];
	uint8_t NoConnectedDevices;
};
*/ 

void testDetectPin()
{
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(PIN_PB06, &config_port_pin);
}

void toggleDetectPin()
{
	port_pin_toggle_output_level(PIN_PB06);
}


void setDetectPin()
{
	port_pin_set_output_level(PIN_PB06,true);
}

void clearDetectPin()
{
	port_pin_set_output_level(PIN_PB06,false);
}


int main (void)
{
	system_init();
	delay_init();
	configureSendInfo();
	initializeI2CModules();
	delay_cycles_ms(500);
	//enableServoTCC();
	//USART -- 115200
	serialInit();  
	//testDetectPin();
	configureInputOutputDetection();
	while(1)
	{
		
		checkOutputsInputs();
		checkSetOutputs();
		checkSetInputs();
		readSensors();
		//setOutputs();
		checkUART();
		delay_cycles_ms(5);
		checkSendOutputs();
		checkSendInputs();
		
		
	}
}
