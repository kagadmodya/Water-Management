/*
 * i2c.c
 *
 *  Created on: Jun 3, 2017
 *      Author: chigo
 */
#include "i2c.h"

void I2C_Init()
{
	TWSR = 0;
	TWBR = ((F_CPU/F_SCL)-16)/2;
	TWCR = 0x04;
}

int I2C_SendAddress(unsigned char addr)
{
	TWDR = addr;
	TWCR = TW_SEND;
	while(!TW_READY);
	return(TW_STATUS == 0x18);
}

int I2C_Start()
{
	TWCR = TW_START;
	while(!TW_READY);
	return(TW_STATUS == 0x08);
}

int I2C_Write(unsigned char addr)
{
	TWDR = addr;
	TWCR = TW_SEND;
	while(!TW_READY);
	return(TW_STATUS == 0x28);
}

void I2C_Stop()
{
	TWCR = TW_STOP;
}

unsigned char I2C_ReadNACK()
{
	TWCR = TW_NACK;
	while(!TW_READY);
	return TWDR;
}

void I2C_WriteRegister(unsigned char deviceRegister, unsigned char data)
{
	I2C_Start();
	I2C_SendAddress(DS1307);
	I2C_Write(deviceRegister);
	I2C_Write(data);
	I2C_Stop();
}

unsigned char I2C_ReadRegister(unsigned char deviceRegister)
{
	unsigned char data;
	I2C_Start();
	I2C_SendAddress(DS1307);
	I2C_Write(deviceRegister);
	I2C_Start();
	I2C_SendAddress(DS1307+READ);
	data = I2C_ReadNACK();
	I2C_Stop();
	return data;
}

