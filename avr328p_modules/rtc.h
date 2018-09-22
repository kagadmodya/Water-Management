/*
 * rtc.h
 *
 *  Created on: Jun 4, 2017
 *      Author: chigo
 */

#ifndef RTC_H_
#define RTC_H_

#include <avr/io.h>
#include "i2c.h"

#define SECONDS_REGISTER	0X00
#define MINUTES_REGISTER	0X01
#define HOURS_REGISTER		0X02
#define DAY_REGISTER		0X03
#define DATE_REGISTER		0X04
#define MONTH_REGISTER		0X05
#define YEAR_REGISTER		0X06
#define CONTROL_REGISTER	0X07

void rtc_init();
void setTime(unsigned char hr, unsigned char min, unsigned char sec);
void getTime(unsigned char *hours, unsigned char *minutes, unsigned char *seconds);

#endif /* RTC_H_ */
