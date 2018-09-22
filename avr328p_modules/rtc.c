/*
 * rtc.c
 *
 *  Created on: Jun 4, 2017
 *      Author: chigo
 */
#include "rtc.h"

void rtc_init()
{
	I2C_Init();
	I2C_Start();
	I2C_SendAddress(DS1307);
	I2C_SendAddress(CONTROL_REGISTER);
	I2C_SendAddress(SECONDS_REGISTER);
	I2C_Stop();
}

void setTime(unsigned char hr, unsigned char min, unsigned char sec)
{
	I2C_WriteRegister(SECONDS_REGISTER, sec);
	I2C_WriteRegister(MINUTES_REGISTER, min);
	I2C_WriteRegister(HOURS_REGISTER, hr);
	I2C_Stop();
}

void getTime(unsigned char *hours, unsigned char *minutes, unsigned char *seconds)
{
	*seconds = I2C_ReadRegister(SECONDS_REGISTER);
	*minutes = I2C_ReadRegister(MINUTES_REGISTER);
	*hours = I2C_ReadRegister(HOURS_REGISTER);

}
