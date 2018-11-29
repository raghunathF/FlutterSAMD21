/*
 * FlutterUART.h
 *
 * Created: 10/12/2018 9:50:29 AM
 *  Author: raghu
 */ 
#include <asf.h>
struct usart_module usart_instance;

void serialInit();

static void configureUSARTCallbacks();
static void usart_read_callback(struct usart_module *const usart_module);
static void configureUSART();