/*****************************************************************************
 *                                                                           *
 *  uCUnit - A unit testing framework for microcontrollers                   *
 *                                                                           *
 *  (C) 2007 - 2008 Sven Stefan Krauss                                       *
 *                  https://www.ucunit.org                                   *
 *                                                                           *
 *  File        : System.c                                                   *
 *  Description : System dependent functions used by uCUnit.                 *
 *                This file has to be customized for your hardware.         *
 *  Author      : Sven Stefan Krauss                                         *
 *  Contact     : www.ucunit.org                                             *
 *                                                                           *
 *****************************************************************************/

/*
 * This file is part of ucUnit.
 *
 * You can redistribute and/or modify it under the terms of the
 * Common Public License as published by IBM Corporation; either
 * version 1.0 of the License, or (at your option) any later version.
 *
 * uCUnit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Common Public License for more details.
 *
 * You should have received a copy of the Common Public License
 * along with uCUnit.
 *
 * It may also be available at the following URL:
 *       http://www.opensource.org/licenses/cpl1.0.txt
 *
 * If you cannot obtain a copy of the License, please contact the
 * author.
 */

#define F_CPU 20000000UL

#include <avr/io.h>

#include <stdio.h>
#include <stdlib.h>
#include "../System.h"



#define USART_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5)

/* Stub: Initialize your hardware here */
void System_Init(void)
{
	//Disable Clk Prescaler (F_CPU = 20MHz)
	CCP = 0xd8;
	CLKCTRL.MCLKCTRLB = 0x00;
	
	//Initialize UART
	/*//for discovery kit
	PORTB.DIR |= PIN0_bm;
	USART3.BAUD = (uint16_t)USART_BAUD_RATE(9600);
	USART3.CTRLB |= USART_TXEN_bm;
	*/
	
	//for custom pcb
	PORTA.DIR |= PIN0_bm;
	USART0.BAUD = (uint16_t)USART_BAUD_RATE(115200);
	USART0.CTRLB |= USART_TXEN_bm;
	
	
	printf("Init of hardware finished.\n");
}

/* Stub: Shutdown your hardware here */
void System_Shutdown(void)
{

	/* asm("\tSTOP"); */
	printf("System shutdown.\n");
	exit(0);
}

/* Stub: Resets the system */
void System_Reset(void)
{
	/* Stub: Reset the hardware */
	/* asm("\tRESET"); */
	printf("System reset.\n");
	exit(0);
}

/* Stub: Put system in a safe state */
void System_Safestate(void)
{
	/* Disable all port pins */
	/* PORTA = 0x0000; */
	/* PORTB = 0x0000; */
	/* PORTC = 0x0000; */

	/* Disable interrupts */
	/* DIE(); */

	/* Put processor into idle state */
	/* asm("\tIDLE"); */
	printf("System safe state.\n");
	exit(0);
}

void uart_putc(char c)
{
// for discovery kit
// 	while (!(USART3.STATUS & USART_DREIF_bm))
// 	{
// 		;
// 	}
// 	USART3.TXDATAL = c;

// for custom pcb
	while (!(USART0.STATUS & USART_DREIF_bm))
	{
		;
	}
	USART0.TXDATAL = c;
}


/* Stub: Transmit a string to the host/debugger/simulator */
void System_WriteString(char * s)
{
	while(*s)
	{
		uart_putc(*s);
		s++;
	}
}

void System_WriteInt(int n)
{
	uint8_t buffer[10];
	itoa(n, buffer, 10);
	System_WriteString(buffer);
}

