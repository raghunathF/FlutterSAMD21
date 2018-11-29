/*
 * FlutterADC.h
 *
 * Created: 10/18/2018 11:50:22 AM
 *  Author: raghu
 */ 


#ifndef FLUTTERADC_H_
#define FLUTTERADC_H_
#include <asf.h>

struct adc_module adc_instance;

void configureADC();
uint16_t adc_start_read_result(const enum adc_positive_input analogPin);



#endif /* FLUTTERADC_H_ */