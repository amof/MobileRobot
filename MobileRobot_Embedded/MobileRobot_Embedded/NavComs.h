/*
 * Handle all the communication and frame between AVR and PC
 *
 *  Author: amof
 *  Specifically develop for ATMEGA-8bits family
 *  Based on the work of Atmel
 *  License: GNU General Public License V3
 *	Version : 1b0
 * 
 */


#ifndef NAVCOMS_H_
#define NAVCOMS_H_

#define USE_UART

// Public structure
typedef struct {
	uint8_t header = 0x10;
	char frameNumber[3];
	char numberDatasByte[3];
	uint8_t command;
	char datas[99]; //dynamic?
	uint8_t crc;
	uint8_t footer=0x08;
	uint8_t status; //to see the validy of the data
}NavComs;


/****************************************************************************
  Functions
****************************************************************************/

/**
   @brief   Initialize the NavComs library
   @param   Specify the baudrate
   @return  none
*/
extern void NavComs_init(unsigned int baud);
/**
   @brief   Check if new datas have been receive and send back
   @param   none
   @return  NavComs structure if valid
*/
extern NavComs NavComs_dataReceive();


#endif /* NAVCOMS_H_ */