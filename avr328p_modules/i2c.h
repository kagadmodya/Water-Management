/*
 * i2c.h
 *
 *  Created on: Jun 3, 2017
 *      Author: chigo
 */

#ifndef I2C_H_
#define I2C_H_

#define F_CPU 16000000L //CPU clock speed 16 MHz
#define F_SCL 100000L  // I2C clock speed 100 kHz

#define TW_START 	0xA4
#define TW_READY 	(TWCR & 0x80)
#define TW_STATUS 	(TWSR & 0xF8)
#define DS1307 		0xD0
#define TW_SEND 	0X84
#define TW_STOP 	0x94
#define TW_NACK		0x84
#define READ 		1

#include <avr/io.h>

void I2C_Init();
int I2C_SendAddress(unsigned char addr);
int I2C_Start();
int I2C_Write(unsigned char addr);
void I2C_Stop();
unsigned char I2C_ReadNACK();
void I2C_WriteRegister(unsigned char deviceRegister, unsigned char data);
unsigned char I2C_ReadRegister(unsigned char deviceRegister);


#endif /* I2C_H_ */
