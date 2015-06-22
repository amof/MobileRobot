/*
 * Handle The UART communication
 *
 * Created: 15/11/2014 17:20:49
 *  Author: amof
 */ 


#ifndef UART_H_
#define UART_H_

/*MACROS & Define*/

#define UART_BAUD_SELECT(baudRate,xtalCpu)  (xtalCpu / (8UL * baudRate)) -1UL
#define UART_RING_SIZE_RX   40
#define UART_RING_SIZE_TX   40

#define UART_ENDCHAR		0x23 //Terminaison Character : '#'

volatile uint8_t UART_datasReceived;


/*Functions*/

/**
   @brief   Initialize USART and set baudrate 
   @param   baudrate Specify baudrate using macro USART_BAUD_SELECT()
   @return  none
*/
extern void UART_init(unsigned int baud);
/**
   @brief   Launch the transmission of data with current buffer
   @param   none
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
   @brief   Write an int to the buffer
   @param   none
   @return  none
*/
extern void UART_put8int( uint8_t i );

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