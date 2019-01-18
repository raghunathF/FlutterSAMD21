/*
 * FlutterI2CMaster.c
 *
 * Created: 11/19/2018 12:51:30 PM
 *  Author: Raghunath Reddy 
 */ 
#include <asf.h>
#include "FlutterI2CMaster.h"
#include "FlutterGlobals.h"

#define MAX_DATA_LENGTH		20
extern volatile struct outputPorts  outputPort[4];

#define DATA_LENGTH 10
static uint8_t write_buffer[DATA_LENGTH] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
};

#define TIMEOUT 1000

#define SLAVE_ADDRESS		10
//struct i2c_master_packet wr_packet;

struct i2c_master_packet rd_packet_1;
struct i2c_master_packet rd_packet_2;
struct i2c_master_packet rd_packet_3;
struct i2c_master_packet rd_packet_4;

struct i2c_master_module i2c_master_instance_1;
struct i2c_master_module i2c_master_instance_2;
struct i2c_master_module i2c_master_instance_3;
struct i2c_master_module i2c_master_instance_4;

//static uint8_t wr_buffer[DATA_LENGTH] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
//static uint8_t wr_buffer_reversed[DATA_LENGTH] = {0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};

static uint8_t rd_buffer_1[MAX_DATA_LENGTH];
static uint8_t rd_buffer_2[MAX_DATA_LENGTH];
static uint8_t rd_buffer_3[MAX_DATA_LENGTH];
static uint8_t rd_buffer_4[MAX_DATA_LENGTH];

void I2CRead(uint8_t instanceNo , struct i2c_master_packet *const rd_packet)
{
	uint16_t timeout = 0;
	switch(instanceNo)
	{
		case OUTPUT1:
			i2c_master_read_packet_job(&i2c_master_instance_1, rd_packet);
			break;
		case OUTPUT2:
			i2c_master_read_packet_job(&i2c_master_instance_2, rd_packet);
			break;
		case OUTPUT3:
			i2c_master_read_packet_job(&i2c_master_instance_3, rd_packet);
			break;
		case OUTPUT4:
			//i2c_master_read_packet_wait_no_nack(&i2c_master_instance_4, rd_packet);
			//i2c_master_write_packet_job(&i2c_master_instance_4, rd_packet);
			//i2c_master_read_packet_job(&i2c_master_instance_4, rd_packet);
			while (i2c_master_read_packet_wait(&i2c_master_instance_4, rd_packet) !=STATUS_OK) {
				/* Increment timeout counter and check if timed out. */
				if (timeout++ == TIMEOUT) {
					i2c_master_disable(&i2c_master_instance_4);
					i2c_master_enable(&i2c_master_instance_4);
					break;
				}
			}
			break;
		default:
			break;
	}
}

void  I2CWrite(uint8_t instanceNo , struct i2c_master_packet *const wr_packet)
{
	volatile uint32_t err_code = 0;
	switch(instanceNo)
	{
		case OUTPUT1:
			 i2c_master_write_packet_job(&i2c_master_instance_1, wr_packet);
			break;
		case OUTPUT2:
			i2c_master_write_packet_job(&i2c_master_instance_2, wr_packet);
			break;
		case OUTPUT3:
			i2c_master_write_packet_job(&i2c_master_instance_3, wr_packet);
			break;
		case OUTPUT4:
			err_code = i2c_master_write_packet_job(&i2c_master_instance_4, wr_packet);
			break;
		default:
			break;
	}
}

void i2c_read_complete_callback_1(struct i2c_master_module *const module)
{
	outputPort[0].I2CStatus = IDLE_MODE;
}

void i2c_read_complete_callback_2(struct i2c_master_module *const module)
{
	outputPort[1].I2CStatus = IDLE_MODE;
}

void i2c_read_complete_callback_3(struct i2c_master_module *const module)
{
	outputPort[2].I2CStatus = IDLE_MODE;
}

void i2c_read_complete_callback_4(struct i2c_master_module *const module)
{
	outputPort[3].I2CStatus = IDLE_MODE;
}


//! [callback_func]
void i2c_write_complete_callback_1(struct i2c_master_module *const module)
{
	/*
	rd_packet_1.address     = outputPort[0].address;
	rd_packet_1.data_length = outputPort[0].readDataLength;
	rd_packet_1.data        = rd_buffer_1;
	i2c_master_read_packet_job(&i2c_master_instance_1,&rd_packet_1);
	*/
	outputPort[0].I2CStatus = IDLE_MODE;
}


void i2c_write_complete_callback_2(struct i2c_master_module *const module)
{
	/*
	rd_packet_2.address     = outputPort[1].address;
	rd_packet_2.data_length = outputPort[1].readDataLength;
	rd_packet_2.data        = rd_buffer_2;
	i2c_master_read_packet_job(&i2c_master_instance_2,&rd_packet_2);
	*/
	outputPort[1].I2CStatus = IDLE_MODE;
}


void i2c_write_complete_callback_3(struct i2c_master_module *const module)
{
	/*
	rd_packet_3.address     = outputPort[2].address;
	rd_packet_3.data_length = outputPort[2].readDataLength;
	rd_packet_3.data        = rd_buffer_3;
	i2c_master_read_packet_job(&i2c_master_instance_3,&rd_packet_3);
	*/
	outputPort[2].I2CStatus = IDLE_MODE;
}


void i2c_write_complete_callback_4(struct i2c_master_module *const module)
{
	/*
	rd_packet_4.address     = outputPort[3].address;
	rd_packet_4.data_length = outputPort[3].readDataLength;
	rd_packet_4.data        = rd_buffer_4;
	i2c_master_read_packet_job(&i2c_master_instance_4,&rd_packet_4);
	*/
	
	outputPort[3].I2CStatus = IDLE_MODE;
}

//uint8_t 
/*
void i2c_write_complete_callback_4(struct i2c_master_module *const module)
{
	rewrite = true;
}
*/
void initializeI2CModules()
{
	uint16_t timeout = 0;
	struct i2c_master_config config_i2c_master;
	i2c_master_get_config_defaults(&config_i2c_master);
	
	config_i2c_master.generator_source = GCLK_GENERATOR_0;
	//Output 1 
	config_i2c_master.pinmux_pad0    = OUTPUT1_I2C_SDA;
	config_i2c_master.pinmux_pad1    = OUTPUT1_I2C_SCL;
	while(i2c_master_init(&i2c_master_instance_1,  CONF_I2C_MASTER_MODULE_OUTPUT_1, &config_i2c_master)   != STATUS_OK);
	i2c_master_register_callback(&i2c_master_instance_1, i2c_write_complete_callback_1,I2C_MASTER_CALLBACK_WRITE_COMPLETE);
	i2c_master_register_callback(&i2c_master_instance_1, i2c_read_complete_callback_1,I2C_MASTER_CALLBACK_READ_COMPLETE);
	
	//Output 2
	config_i2c_master.pinmux_pad0    = OUTPUT2_I2C_SDA;
	config_i2c_master.pinmux_pad1    = OUTPUT2_I2C_SCL;
	while(i2c_master_init(&i2c_master_instance_2,  CONF_I2C_MASTER_MODULE_OUTPUT_2, &config_i2c_master)   != STATUS_OK);
	i2c_master_register_callback(&i2c_master_instance_2, i2c_write_complete_callback_2, I2C_MASTER_CALLBACK_WRITE_COMPLETE);
	i2c_master_register_callback(&i2c_master_instance_2, i2c_read_complete_callback_2,  I2C_MASTER_CALLBACK_READ_COMPLETE);
	
	//Output 3
	config_i2c_master.pinmux_pad0    = OUTPUT3_I2C_SDA;
	config_i2c_master.pinmux_pad1    = OUTPUT3_I2C_SCL;
	while(i2c_master_init(&i2c_master_instance_3,  CONF_I2C_MASTER_MODULE_OUTPUT_3, &config_i2c_master)   != STATUS_OK);
	i2c_master_register_callback(&i2c_master_instance_3, i2c_write_complete_callback_3,I2C_MASTER_CALLBACK_WRITE_COMPLETE);
	i2c_master_register_callback(&i2c_master_instance_3, i2c_read_complete_callback_3,  I2C_MASTER_CALLBACK_READ_COMPLETE);
	
	//Output 4
	config_i2c_master.pinmux_pad0    = OUTPUT4_I2C_SDA;
	config_i2c_master.pinmux_pad1    = OUTPUT4_I2C_SCL;
	config_i2c_master.buffer_timeout = 1000;
	
	while(i2c_master_init(&i2c_master_instance_4,  CONF_I2C_MASTER_MODULE_OUTPUT_4, &config_i2c_master)   != STATUS_OK);
	i2c_master_register_callback(&i2c_master_instance_4, i2c_write_complete_callback_4, I2C_MASTER_CALLBACK_WRITE_COMPLETE);
	//i2c_master_register_callback(&i2c_master_instance_4, i2c_error_callback_4, I2C_MASTER_CALLBACK_ERROR);
	//i2c_master_register_callback(&i2c_master_instance_4, i2c_read_complete_callback_4,  I2C_MASTER_CALLBACK_READ_COMPLETE);
	
	
	//i2c_master_enable(&i2c_master_instance_4);
	//i2c_master_enable_callback(&i2c_master_instance_1,I2C_MASTER_CALLBACK_WRITE_COMPLETE);
	/*
		struct i2c_master_packet packet = {
			.address     = SLAVE_ADDRESS,
			.data_length = DATA_LENGTH,
			.data        = write_buffer,
			.ten_bit_address = false,
			.high_speed      = false,
			.hs_master_code  = 0x0,
		};
		//! [packet]
		//! [init]

		//! [main]
		/* Write buffer to slave until success. */
		//! [write_packet]
		
		//while (i2c_master_write_packet_wait(&i2c_master_instance_4, &packet) !=STATUS_OK) {
			/* Increment timeout counter and check if timed out. */
	
			//if (timeout++ == TIMEOUT) {
			//	break;
			//}
		//}
	
	
}

void enableI2CModule(uint8_t port)
{
	switch(port)
	{
		case OUTPUT1:
			i2c_master_enable(&i2c_master_instance_1);
			i2c_master_enable_callback(&i2c_master_instance_1,I2C_MASTER_CALLBACK_WRITE_COMPLETE);
			i2c_master_enable_callback(&i2c_master_instance_1, I2C_MASTER_CALLBACK_READ_COMPLETE);
			break;

		case OUTPUT2:
			i2c_master_enable(&i2c_master_instance_2);
			i2c_master_enable_callback(&i2c_master_instance_2, I2C_MASTER_CALLBACK_WRITE_COMPLETE);
			i2c_master_enable_callback(&i2c_master_instance_2, I2C_MASTER_CALLBACK_READ_COMPLETE);
			break;
		
		case OUTPUT3:
			i2c_master_enable(&i2c_master_instance_3);
			i2c_master_enable_callback(&i2c_master_instance_3,I2C_MASTER_CALLBACK_WRITE_COMPLETE);
			i2c_master_enable_callback(&i2c_master_instance_3, I2C_MASTER_CALLBACK_READ_COMPLETE);
			break;
		
		case OUTPUT4:
			i2c_master_enable(&i2c_master_instance_4);
			i2c_master_enable_callback(&i2c_master_instance_4,I2C_MASTER_CALLBACK_WRITE_COMPLETE);
			//i2c_master_enable_callback(&i2c_master_instance_4,I2C_MASTER_CALLBACK_READ_COMPLETE);
			break;
			
		case SENSORS:
			break;
			
		default:
			break;
	}
	
}

void disableI2CModule(uint8_t port)
{

	switch(port)
	{
		case OUTPUT1:
			i2c_master_disable(&i2c_master_instance_1);
			i2c_master_disable_callback(&i2c_master_instance_1,I2C_MASTER_CALLBACK_WRITE_COMPLETE);
			break;

		case OUTPUT2:
			i2c_master_disable(&i2c_master_instance_2);
			i2c_master_disable_callback(&i2c_master_instance_2, I2C_MASTER_CALLBACK_WRITE_COMPLETE);
			break;
		
		case OUTPUT3:
			i2c_master_disable(&i2c_master_instance_3);
			i2c_master_disable_callback(&i2c_master_instance_3,I2C_MASTER_CALLBACK_WRITE_COMPLETE);
			break;
		
		case OUTPUT4:
			i2c_master_disable(&i2c_master_instance_4);
			i2c_master_disable_callback(&i2c_master_instance_4,I2C_MASTER_CALLBACK_WRITE_COMPLETE);
			break;
		
		case SENSORS:
			break;
		
		default:
			break;
	}
	
}
