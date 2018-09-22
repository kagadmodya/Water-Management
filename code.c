/*
 * testfile.c
 *
 *  Created on: May 25, 2017
 *      Author: chigo
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "uart0.h"
#include "i2c.h"
#include "rtc.h"
#include <math.h>
#include <avr/eeprom.h>
#include <stdlib.h>

#define BUFFSIZE 20

unsigned char hours, minutes, seconds;
unsigned char rx_buffer[BUFFSIZE] = {0}, num[2] = {0};
unsigned char
ONhour = 0,
ONmin = 0,
OFFhour = 0,
OFFmin = 0,
rx_pointer = 0;

void System_Init();
void uart_send_BCD(unsigned char);
void Check_Cmd();
unsigned int hex_to_dec(unsigned int);
unsigned int power(unsigned int base, unsigned int exp);
void clr_rx_buff(void);

int main(void)
{
	System_Init();

	while(1)
	{
		getTime(&hours, &minutes, &seconds);

//		itoa(hours, num, 16);
//		send_string(&num);
//		itoa(minutes, num, 16);
//		send_string(&num);
//		send_char('\n*');
//		itoa(ONhour, num, 10);
//		send_string(&num);
//		itoa(ONmin, num, 10);
//		send_string(&num);
//		send_char('\n#');


		if((hours == ONhour) && (minutes == ONmin))
		{
			PORTB = 1 << DDB4 | 1 << DDB5;
		}
		else if((hours == OFFhour) && (minutes == OFFmin))
		{
			PORTB = 0 << DDB4 | 0 << DDB5;
		}

		_delay_ms(1000);
	}

	return 0;
}

void System_Init()
{
	DDRB = 1 << DDB4 | 1 << DDB5 ;

	uart0_init(9600);

	rtc_init();

	send_string("\n**Chinmay's Embedded Solutions**\n");
	send_string("Water Management System\n");

	_EEGET(ONhour, 10);
	_EEGET(ONmin, 11);
	_EEGET(OFFhour, 12);
	_EEGET(OFFmin, 13);

	send_string("\nON Time: ");
	uart_send_BCD(ONhour);
	send_char(':');
	uart_send_BCD(ONmin);

	send_string("\nOFF Time: ");
	uart_send_BCD(OFFhour);
	send_char(':');
	uart_send_BCD(OFFmin);

	send_string("\nRTC: ");
	getTime(&hours, &minutes, &seconds);
	uart_send_BCD(hours);
	send_char(':');
	uart_send_BCD(minutes);
	send_char(':');
	uart_send_BCD(seconds);
	send_char('\n');


}

void uart_send_BCD(unsigned char ch)
{
	send_char('0' + (ch >> 4));
	send_char('0' + (ch & 0x0F));
}

void Check_Cmd()
{
	uint8_t hr = 0, min = 0, sec = 0;

	if((rx_buffer[1] == 'N') && (rx_buffer[2] == ':'))
	{
		ONhour = (rx_buffer[rx_pointer-5] - '0')*10 + (rx_buffer[rx_pointer-4] - '0');
//		_EEGET(ONhour, 10);
//		itoa(ONhour, num1, 10);
//		send_string(&num1);
		ONhour = hex_to_dec(ONhour);
		_EEPUT(10, ONhour);		//Save in EEPROM


		ONmin = (rx_buffer[rx_pointer-3] - '0')*10 + (rx_buffer[rx_pointer-2] - '0');
//		_EEGET(ONmin, 11);
//		itoa(ONmin, &num2, 10);
//		send_string(&num2);
		ONmin = hex_to_dec(ONmin);
		_EEPUT(11, ONmin);		//Save in EEPROM

		send_string("\nON Time Set.\n");

		clr_rx_buff();
	}

	else if((rx_buffer[1] == 'F') && (rx_buffer[3] == ':'))
	{
		OFFhour = (rx_buffer[rx_pointer-5] - '0')*10 + (rx_buffer[rx_pointer-4] - '0');
//		_EEGET(OFFhour, 12);
//		itoa(OFFhour, &num1, 10);
//		send_string(&num1);
		OFFhour = hex_to_dec(OFFhour);
		_EEPUT(12, OFFhour);		//Save in EEPROM

		OFFmin = (rx_buffer[rx_pointer-3] - '0')*10 + (rx_buffer[rx_pointer-2] - '0');
//		_EEGET(OFFmin, 13);
//		itoa(OFFmin, &num2, 10);
//		send_string(&num2);
		OFFmin = hex_to_dec(OFFmin);
		_EEPUT(13, OFFmin);		//Save in EEPROM

		send_string("\nOFF Time Set.\n");

		clr_rx_buff();
	}

	else if(rx_buffer[1] == 'T' && (rx_buffer[3] == ':'))
	{
		hr = (rx_buffer[rx_pointer-7] - '0')*10 + (rx_buffer[rx_pointer-6] - '0');
		min = (rx_buffer[rx_pointer-5] - '0')*10 + (rx_buffer[rx_pointer-4] - '0');
		sec = (rx_buffer[rx_pointer-3] - '0')*10 + (rx_buffer[rx_pointer-2] - '0');


		hr = hex_to_dec(hr);
		min = hex_to_dec(min);
		sec = hex_to_dec(sec);

		setTime(hr, min, sec);

		send_string("\nRTC Set.\n");
		clr_rx_buff();
	}

	else
	{
		send_string("\nWRONG COMMAND.\n");
		clr_rx_buff();
	}
}

unsigned int hex_to_dec(unsigned int x)
{
      unsigned int count = 0, decimal_number = 0, remainder = 0;
      while(x > 0)
      {
            remainder = x % 10;
            decimal_number = decimal_number + remainder * power(16,count);
            x = x / 10;
            count++;
      }

      return decimal_number;
}

unsigned int power(unsigned int base, unsigned int exp)
{
	unsigned int result = 1;

	while(exp != 0)
	{
		result = result* base;
		exp--;
	}
	return result;
}

void clr_rx_buff()
{
	uint8_t i = 0;
	for(i = 0; i< BUFFSIZE; i++)
	{
		rx_buffer[i] = 0;
	}
	rx_pointer = 0;
}

ISR(USART_RX_vect)
{
	unsigned char rx_ch;
	rx_ch = UDR0;

	rx_buffer[rx_pointer++] = rx_ch;

	if(rx_ch == ' ')
	{
		Check_Cmd();
	}

}
