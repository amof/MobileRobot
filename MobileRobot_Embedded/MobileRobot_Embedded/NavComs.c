/*
 * Handle all the communication and frame between AVR and PC
 * DO NOT MODIFY THIS FILE for configuration purpose.
 *
 *  Author: amof
 *  Specifically developp for Atmega-8bits family
 *  Based on the work of Atmel
 *  License: GNU General Public License V3
 *	Version : 1b0
 * 
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "NavComs.h"
#include "Uart.h"

//List of commands
#define ACK
#define NACK
#define CCS
#define MM
#define CR
#define AM
#define TC
#define DIA
#define DIU
#define DIC

/****************************************************************************
  Private functions
****************************************************************************/
//Automatic ACK
void ack(void){
	
}
//Automatic NACK
void nack(void){
	
}
#if defined( USE_UART )
void sendToUART(void){
	
}
#endif

/****************************************************************************
  Public functions
****************************************************************************/

void NavComs_init(unsigned int baud){
	#if defined( USE_UART )
	UART_init(UART_BAUD_SELECT(baud,F_CPU));
	#endif
}

NavComs NavComs_dataReceive(){
	
}


