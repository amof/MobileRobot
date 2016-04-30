/************************************************************************
Title:    Controller for communication over Bluetooth
Author:   Julien Delvaux
Software: Atmel Studio 7
Hardware: AVR 8-Bits, tested with ATmega1284P
License:  GNU General Public License 3
Usage:    see Doxygen manual



LICENSE:
	Copyright (C) 2015 Julien Delvaux

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 3 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

    
************************************************************************/

/** 
 *  @defgroup Controller Library
 *  @code #include <Controller.h> @endcode
 * 
 *  @brief 
 *
 *  @author Julien Delvaux <delvaux.ju@gmail.com>
 */


#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

#include "Uart.h"
#include "CRC.h"

#if (__GNUC__ * 100 + __GNUC_MINOR__) < 304
#error "This library requires AVR-GCC 3.4 or later, update to newer AVR-GCC compiler !"
#endif

/************************************************************************/
/* Constants and macros                                                 */
/************************************************************************/

#define C_C_ACK		0x10
#define C_C_NACK	0x15
#define C_C_ALIVE   0x17
#define C_C_END_CON 0x19
#define C_M_STOP    0x20
#define C_M_FW      0x21
#define C_M_BW      0x22
#define C_M_RIGHT   0x23
#define C_M_LEFT    0x24

//#define DEBUG_ENABLED

/************************************************************************/
/* Functions prototype                                                  */
/************************************************************************/

/**
   @brief   Initialize the controller to work with UART @9600baud
   @param   none
   @return  none
*/
extern void controller_init(void);

/**
   @brief   Send data to bluetooth module
   @param   1 byte command (C_C_X) : x being the command
   @return  none
*/
extern void controller_sendCommand(uint8_t command);

/**
   @brief   Get data from the bluetooth module
   @param   none
   @return  byte with the command
*/
extern uint8_t controller_getData(void);

#endif /* CONTROLLER_H_ */