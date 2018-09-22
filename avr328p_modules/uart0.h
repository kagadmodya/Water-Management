/*
 * uart0.h
 *
 *  Created on: May 27, 2017
 *      Author: chigo
 */

#ifndef UART0_H_
#define UART0_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define FOSC 16000000	// Clock Speed



void uart0_init(int baudrate);
void send_char(char ch);
void send_string(char* stringPtr);
char rx_char(void);


#endif /* UART0_H_ */
