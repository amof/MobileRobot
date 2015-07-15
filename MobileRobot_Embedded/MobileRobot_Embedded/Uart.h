/*
 * Handle The UART communication
 *
 *  Author: amof
 *  Specifically develop for ATMEGA-8bits family
 *  Based on the work of Atmel
 *  License: GNU General Public License V3
 *	Version : 1.1b1
 *	Warning : This version doesn't handle UART0 & UART1 at the same time
 *	!!! Problem to fix : buffer on reception  !!!
 * 
 */ 


#ifndef UART_H_
#define UART_H_

/****************************************************************************
  Macros and defines
****************************************************************************/

#define UART_BAUD_SELECT(baudRate,xtalCpu)  (xtalCpu / (8UL * baudRate)) -1UL

//Comment or uncomment which UART you want to use
#define UART0		//RX : PD0 and TX : PD1
//#define UART1		//RX : PD2 and TX : PD3
#define UART_RING_SIZE_RX	40
#define UART_RING_SIZE_TX	40
volatile uint8_t UART_datasReceived; //Automatically reset to zero when all the data are read

	/**--UART0--**/
//Define the length or a character after which the flag UART0_datasReceived is set to '1'
#define UART0_ENDCHAR		0x23	//Termination's Character : '#'
#define UART0_ENDSIZE		20		//Length : maximum 255

	/**--UART1--**/
//Define the length or a character after which the flag UART1_datasReceived is set to '1'
#define UART1_ENDCHAR		0x23	//Termination's Character : '#'
#define UART1_ENDSIZE		20		//Length : maximum 255

/****************************************************************************
  Functions
****************************************************************************/

/**
   @brief   Initialize USART and set baudrate 
   @param   baudrate : Specify baudrate using macro UART_BAUD_SELECT()
   @return  none
*/
extern void UART_init(unsigned int baud);
/**
   @brief   Launch the transmission of data with current buffer
   @param   uint8_t port : UART0 or UART1 in case of both defined else *void*
   @return  none
*/
extern void UART_tx_launch();

	/**-SEND DATAS-**/

/**
   @brief   Write Character in the buffer
   @param   Character
   @return  none
*/	
extern void UART_write_char(char data);
/**
   @brief   Write a text to the buffer
   @param   none
   @return  Character
*/
extern void UART_tx(const char *s );
/**
   @brief   Write a text from PROGMEM to the buffer
   @param   char
   @return  none
*/
extern void UART_txFromPROGMEM( const char *data);
/**
   @brief   Write an uint8 to the buffer
   @param   number : 8 bits number
   @return  none
*/
extern void UART_put8int( uint8_t number);

	/**-GET DATAS-**/
	
/**
   @brief   Read Character from the buffer
   @param   none
   @return  Character
*/
extern char UART_read_char(void);
/**
   @brief   Read all buffer
   @param   none
   @return  Character
*/
extern char* UART_read_buffer(void);

#endif /* UART_H_ */