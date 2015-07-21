/*
 * Handle The UART communication
 * DO NOT MODIFY THIS FILE for configuration purpose.
 *
 *  Author: amof
 *  Specifically developp for Atmega-8bits family
 *  Based on the work of Atmel
 *  License: GNU General Public License V3
 *	Version : 1.1b1
 * 
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <stdint.h>

#include "Uart.h"

//Structure
typedef struct {
	uint8_t tx_ring_head;
	uint8_t tx_ring_tail;
	char	tx_ring_data[UART_RING_SIZE_TX];

	uint8_t rx_ring_head;
	uint8_t rx_ring_tail;
	char	rx_ring_data[UART_RING_SIZE_RX];
}UART_buffer_struct;

volatile uint8_t UART0_countDatasReceive=0;
volatile uint8_t UART1_countDatasReceive=0;

//DEFINES
#if defined( UART0 )
#define UART0_A					UCSR0A
#define UART0_B					UCSR0B
#define UART0_C					UCSR0C
#define UART0_DATA				UDR0
#define UART0_UDRIE				UDRIE0
#define UART0_U2X0				U2X0

#elif defined( UART1 )
#define UART1_A					UCSR1A
#define UART1_B					UCSR1B
#define UART1_C					UCSR1C
#define UART1_DATA				UDR1
#define UART1_UDRIE				UDRIE1
#define UART1_U2X1				U2X1
#endif

//BUFFER
#if defined( UART0 )
static volatile UART_buffer_struct UART_buffer;

#elif defined( UART1 )
static volatile UART_buffer_struct UART_buffer;
#endif

//Intern Functions
static int UART_tx_ring_add(char c);
static int UART_rx_ring_remove(void);


	// ********** Functions ********** //

void UART_init(unsigned int baud){
	#if defined( UART0 )
	//Asynchronous Transmission | Must be set to 0 if used synchronous
	UART0_A = 1<<UART0_U2X0 ;
	
	//Set baud rate
	UBRR0 = baud ;
	
	//Configuration
	UART0_B = 1<<RXEN0 | 1<<TXEN0 | 1<<RXCIE0 ;
	UART0_C = 3<<UCSZ00 ;
	
	//Pull_Up
		#if defined(UART0_RX_PULL_UP)
		DDRD &= ~(1<<PIND0);
		PORTD |= 1<<PIND0; // Protection to avoid problem to receive datas
		#endif
	
	#elif defined( UART1 )
	//Asynchronous Transmission | Must be set to 0 if used synchronous
	UART1_A = 1<<UART1_U2X1 ;
		
	//Set baud rate
	UBRR1 = baud ;
		
	//Configuration
	UART1_B = 1<<RXEN1 | 1<<TXEN1 | 1<<RXCIE1 ;
	UART1_C = 3<<UCSZ10 ;
		
	//Pull_Up
		#if defined(UART1_RX_PULL_UP)
		DDRD &= ~(1<<PIND2);
		PORTD = 1<<PIND2; // Protection to avoid problem to receive datas
		#endif
	#endif
}
void UART_tx_launch(){
	#if defined( UART0 )
	UART0_B |= (1 << UART0_UDRIE); 
	#elif defined( UART1 )
	UART1_B |= (1 << UART1_UDRIE); 
	#endif
	
}

/****************************************************************************
  Send datas
****************************************************************************/

void UART_write_char(char data) {
	// Add the data to the ring buffer now that there's room
	//if return -1 launch the transmission and after put the char in buffer
	if(UART_tx_ring_add(data)==-1){
	UART_tx_launch();
	UART_tx_ring_add(data);
	}
}
void UART_tx(const char *s ){
	while (*s){
		UART_write_char(*s++);
	}
}
void UART_txFromPROGMEM ( const char *data)
{
	while ( pgm_read_byte (data) != 0x00)
	UART_write_char(pgm_read_byte (data ++));
}
void UART_put8int( uint8_t number )
{
	UART_write_char('0'+((number / 100) % 10));
	UART_write_char('0'+((number / 10) % 10));
	UART_write_char('0'+(number % 10));
}
		
/****************************************************************************
  Get datas
****************************************************************************/
		
char UART_read_char(void) {
	// Then return the character
	return (char)UART_rx_ring_remove();
}

char* UART_read_buffer(void){
	char* tampon=malloc(UART_RING_SIZE_RX+1*(sizeof(char)));
	for(int i=0;i<UART_RING_SIZE_RX;i++){
		tampon[i]=UART_read_char();
	}
	tampon[UART_RING_SIZE_RX]='\0';
	
	return tampon;
}

/****************************************************************************
  Intern Functions
****************************************************************************/

static int UART_tx_ring_add(char c){
	int8_t statut=0;
	
	if (UART_buffer.tx_ring_head + 1 != UART_buffer.tx_ring_tail) {
		/* there is room */
		UART_buffer.tx_ring_data[UART_buffer.tx_ring_head++] = c;
	}else {
		/* no room left in the buffer */
		statut=-1;
	}
	
	return statut;
}
static int UART_rx_ring_remove(void){
	uint8_t c= 0x00;
	
	if (UART_buffer.rx_ring_head != UART_buffer.rx_ring_tail) {
		c = UART_buffer.rx_ring_data[UART_buffer.rx_ring_tail];	//load the current char from buffer
		UART_buffer.rx_ring_data[UART_buffer.rx_ring_tail++]=0x00; //reset char after reading
	}else {	
		UART_buffer.rx_ring_tail=0;		
		UART_buffer.rx_ring_head=0;
		UART_datasReceived=0;	//datas received and read ! -> reset
	}
	
	return c;
}

/************************************************************************/
/* Interruptions                                                        */
/************************************************************************/
	/**--UART0--**/
#if defined( UART0 )
ISR(USART0_RX_vect) {
	//Check if there is room left in buffer if not got back to position 0
	if(UART_buffer.rx_ring_head == UART_RING_SIZE_RX){
		UART_buffer.rx_ring_head = 0;					//Reset value of sender 
	}
	UART0_countDatasReceive++;
	char data=UART0_DATA;
	
	UART_buffer.rx_ring_data[UART_buffer.rx_ring_head++]=data;
	if(data==UART0_ENDCHAR|| UART0_countDatasReceive==UART0_ENDSIZE ) UART_datasReceived=1;
	
}

ISR(USART0_UDRE_vect) {
	
	//The pointer has arrived at the end of message we want to transmit
	uint8_t tx_data = UART_buffer.tx_ring_data[UART_buffer.tx_ring_tail]; //Get the data from tx_buffer
	if(UART_buffer.tx_ring_head != UART_buffer.tx_ring_tail ){ //while head != tail
		UART_buffer.tx_ring_data[UART_buffer.tx_ring_tail++]=0x00; //reset the data
		UART0_DATA = tx_data ;
	}
	else{
		UART0_B &= ~(1 << UART0_UDRIE);
		UART_buffer.tx_ring_tail=0;
		UART_buffer.tx_ring_head=0;
	}
}
	/**--UART1--**/
#elif defined( UART1 )
ISR(USART1_RX_vect) {
	//Check if there is room left in buffer if not got back to position 0
	if(UART_buffer.rx_ring_head == UART_RING_SIZE_RX){
		UART_buffer.rx_ring_head = 0;					//Reset value of sender
	}
	UART0_countDatasReceive++;
	char data=UART1_DATA;
	
	UART_buffer.rx_ring_data[UART_buffer.rx_ring_head++]=data;
	if(data==UART1_ENDCHAR || UART0_countDatasReceive==UART1_ENDSIZE) UART_datasReceived=1;
	
}

ISR(USART1_UDRE_vect) {
	
	//The pointer has arrived at the end of message we want to transmit
	uint8_t tx_data = UART_buffer.tx_ring_data[UART_buffer.tx_ring_tail]; //Get the data from tx_buffer
	if(UART_buffer.tx_ring_head != UART_buffer.tx_ring_tail ){ //while head != tail
		UART_buffer.tx_ring_data[UART_buffer.tx_ring_tail++]=0x00; //reset the data
		UART1_DATA = tx_data ;
	}
	else{
		UART1_B &= ~(1 << UART1_UDRIE);
		UART_buffer.tx_ring_tail=0;
		UART_buffer.tx_ring_head=0;
	}
}
#endif