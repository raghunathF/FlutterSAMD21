/*
 * FlutterI2CMaster.h
 *
 * Created: 11/19/2018 12:52:27 PM
 *  Author: raghu
 */ 


#ifndef FLUTTERI2CMASTER_H_
#define FLUTTERI2CMASTER_H_

#define OUTPUT1			1
#define OUTPUT2			2
#define OUTPUT3			3
#define OUTPUT4			4
#define SENSORS			5


#define CONF_I2C_MASTER_MODULE_OUTPUT_1		SERCOM0
#define CONF_I2C_MASTER_MODULE_OUTPUT_2		SERCOM2
#define CONF_I2C_MASTER_MODULE_OUTPUT_3		SERCOM1
#define CONF_I2C_MASTER_MODULE_OUTPUT_4		SERCOM3
#define CONF_I2C_MASTER_MODULE_SENSORS		SERCOM5


#define OUTPUT1_I2C_SDA					PINMUX_PA08C_SERCOM0_PAD0
#define OUTPUT1_I2C_SCL					PINMUX_PA09C_SERCOM0_PAD1


#define OUTPUT2_I2C_SDA					PINMUX_PA12C_SERCOM2_PAD0
#define OUTPUT2_I2C_SCL					PINMUX_PA13C_SERCOM2_PAD1


#define OUTPUT3_I2C_SDA					PINMUX_PA16C_SERCOM1_PAD0
#define OUTPUT3_I2C_SCL					PINMUX_PA17C_SERCOM1_PAD1


#define OUTPUT4_I2C_SDA					PINMUX_PA22C_SERCOM3_PAD0
#define OUTPUT4_I2C_SCL					PINMUX_PA23C_SERCOM3_PAD1


#define SENSORS_I2C_SDA					PINMUX_PB02D_SERCOM5_PAD0
#define SENSORS_I2C_SCL					PINMUX_PB03D_SERCOM5_PAD1



void initializeI2CModules();
void I2CWrite(uint8_t instanceNo , struct i2c_master_packet *const wr_packet);
void I2CRead(uint8_t instanceNo , struct i2c_master_packet *const rd_packet);
void enableI2CModule(uint8_t port);
void disableI2CModule(uint8_t port);


#endif /* FLUTTERI2CMASTER_H_ */