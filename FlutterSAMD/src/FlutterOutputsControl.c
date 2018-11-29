/*
 * FlutterOutputsControl.c
 *
 * Created: 11/20/2018 1:16:43 PM
 *  Author: Raghu
 */ 
#include <asf.h>
#include "FlutterOutputsControl.h"

extern bool readySendData;
static bool timerOutput = false;

/*
void checkSendOutputs()
{
	uint8_t i =0;
	if(readySendData == false)
	{
		for(i=0;i<NO_OUTPUTS;i++)
		{
			if(outputPort[i].currentState != NOTHINGCONNECTED)
			{
				switch(outputPort[i].currentState)
				{
					case OUTPUT_LEDMATRIX:
						//Send set all if set all value is changed 
						//Read the dial values
						break;
					default:
						break;
				}
			}
		}
	}
}
*/
