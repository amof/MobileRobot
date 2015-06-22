/*
 * Handle The UART communication
 *
 * Created: 15/11/2014 17:20:58
 *  Author: amof
 *  Specifically developp for @tmega1284P-PU
 * DO NOT MODIFY THIS FILE ; IF YOU WANT ADAPT THE SIZE OF BUFFER IN UART.H
 */ 

/*INCLUDES*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <stdint.h>

#include "Uart.h"

/*DEFINES*/
#define USART0_A					UCSR0A
#define USART0_B					UCSR0B
#define USART0_C					UCSR0C
#define USART0_DATA					UDR0
#define USART0_UDRIE				UDRIE0
#define U2X							U2X0

/*Circular BUFFER*/
static volatile uint8_t UART_tx_ring_head;
static volatile uint8_t UART_tx_ring_tail;
static volatile char	UART_tx_ring_data[UART_RING_SIZE_TX];

static volatile uint8_t UART_rx_ring_head;
static volatile uint8_t UART_rx_ring_tail;
static volatile char	UART_rx_ring_data[UART_RING_SIZE_RX];

/*Intern Functions*/
static int UART_tx_ring_add(char c);
static int UART_rx_ring_remove(void);

/************************************************************************/
/* PUBLIC FUNCTIONS                                                     */
/************************************************************************/
void UART_init(unsigned int baud){
	
	//Asynchronous Transmission | Must be set to 0 if used synchronous
	USART0_A = 1<<U2X ;
	
	//Set baud rate
	UBRR0 = baud ;
	
	//Configuration
	USART0_B = 1<<RXEN0 | 1<<TXEN0 | 1<<RXCIE0 ;
	USART0_C = 3<<UCSZ00 ;
	
	//Pull_Up
	PORTD = 1<<PIND0; // Protection to avoid problem to receive datas
}
void UART_tx_launch(){
	USART0_B |= (1 << USART0_UDRIE); 
}

	/**-SEND DATAS-**/

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
void UART_put8int( uint8_t i )
{
	UART_write_char('0'+((i / 100) % 10));
	UART_write_char('0'+((i / 10) % 10));
	UART_write_char('0'+(i % 10));
}
		
		/**-GET DATAS-**/
		
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

/************************************************************************/
/* Intern Functions                                                     */
/************************************************************************/

static int UART_tx_ring_add(char c){
	int8_t statut=0;
	
	if (UART_tx_ring_head + 1 != UART_tx_ring_tail) {
		/* there is room */
		UART_tx_ring_data[UART_tx_ring_head++] = c;
	}else {
		/* no room left in the buffer */
		statut=-1;
	}
	
	return statut;
}
static int UART_rx_ring_remove(void){
	uint8_t c= 0x00;
	
	if (UART_rx_ring_head != UART_rx_ring_tail) {
		c = UART_rx_ring_data[UART_rx_ring_tail];	//load the current char from buffer
		UART_rx_ring_data[UART_rx_ring_tail++]=0x00; //reset char after reading
	}else {	
		UART_rx_ring_tail=0;		
		UART_rx_ring_head=0;
		UART_datasReceived=0;	//datas received and read ! -> reset
	}
	
	return c;
}

/************************************************************************/
/* Interruptions                                                        */
/************************************************************************/

ISR(USART0_RX_vect) {
	//Check if there is room left in buffer if not got back to position 0
	if(UART_rx_ring_head == UART_RING_SIZE_RX){
		UART_rx_ring_head = 0;					//Reset value of sender 
	}
	char data=USART0_DATA;
	
	UART_rx_ring_data[UART_rx_ring_head++]=data;
	if(data==UART_ENDCHAR) UART_datasReceived=1;
	
}

ISR(USART0_UDRE_vect) {
	
	//The pointer has arrived at the end of message we want to transmit
	uint8_t tx_data = UART_tx_ring_data[UART_tx_ring_tail]; //Get the data from tx_buffer
	if(UART_tx_ring_head != UART_tx_ring_tail ){ //while head != tail
		UART_tx_ring_data[UART_tx_ring_tail++]=0x00; //reset the data
		USART0_DATA = tx_data ;
	}
	else{
		USART0_B &= ~(1 << USART0_UDRIE);
		UART_tx_ring_tail=0;
		UART_tx_ring_head=0;
	}
}