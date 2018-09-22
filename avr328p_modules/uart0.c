/*
 * uart0.c
 *
 *  Created on: May 27, 2017
 *      Author: chigo
 */


#include"uart0.h"

void uart0_init(int baudrate) //enabled the double speed (to minimize the frequency error).
{
	 UCSR0B = (1<<TXEN0) | (1<<RXEN0); //enable transmission and reception
	 UCSR0B |= 1 << RXCIE0; //enable receive interrupt
	 UCSR0C = (1<<UCSZ00) | (1<<UCSZ01); // Set frame format to 8 data bits, no parity, 1 stop bit
	 UBRR0H = 0x00;

	 sei();

	 switch(baudrate) // Set baud rate
	 {
	 case 9600:
		 UBRR0L  = 0xCF;
		 break;
	 case 19200:
		 UBRR0L  = 0x67;
		 break;
	 case 115200:
		 UBRR0L  = 0x10;
		 break;
	 }
}

void send_char(char ch)
{
	 // wait until the port is ready to be written to
	  while( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 ){}
	  // write the byte to the serial port
	  UDR0 = ch;
}

char rx_char(void)
{
	char ch;
	while( ( UCSR0A & ( 1 << RXC0 ) ) == 0 ){}
	// grab the byte from the serial port
	ch = UDR0;
	return ch;
}

void send_string(char* stringPtr)
{
	while(*stringPtr != 0x00)
	{
		send_char(*stringPtr);
		stringPtr++;
	}
}

