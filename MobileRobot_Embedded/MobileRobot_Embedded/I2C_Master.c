/*
 * Handle The I2C communication
 * DO NOT MODIFY THIS FILE for configuration purpose.
 *
 *  Author: amof
 *  Specifically develop for ATMEGA-8bits family
 *  Based on the work of Atmel
 *  License: GNU General Public License V3
 *	Version : 1
 *	
 * 
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include "I2C_Master.h"

/****************************************************************************
  Bit and byte definitions
****************************************************************************/
#define TWI_READ_BIT  0       // Bit position for R/W bit in "address byte".
#define TWI_ADR_BITS  1       // Bit position for LSB of the slave address bits in the init byte.

#define TRUE          1
#define FALSE         0

/****************************************************************************
  TWI State codes (TWSR)
****************************************************************************/
// General TWI Master status codes                      
#define TWI_START                  0x08  // START has been transmitted  
#define TWI_REP_START              0x10  // Repeated START has been transmitted
#define TWI_ARB_LOST               0x38  // Arbitration lost in SLA+W or data bytes
										 //						SLA+R or NACK bit

// TWI Master Transmitter status codes                      
#define TWI_MTX_ADR_ACK            0x18  // SLA+W has been tramsmitted and ACK received
#define TWI_MTX_ADR_NACK           0x20  // SLA+W has been tramsmitted and NACK received 
#define TWI_MTX_DATA_ACK           0x28  // Data byte has been tramsmitted and ACK received
#define TWI_MTX_DATA_NACK          0x30  // Data byte has been tramsmitted and NACK received 

// TWI Master Receiver status codes  
#define TWI_MRX_ADR_ACK            0x40  // SLA+R has been tramsmitted and ACK received
#define TWI_MRX_ADR_NACK           0x48  // SLA+R has been tramsmitted and NACK received
#define TWI_MRX_DATA_ACK           0x50  // Data byte has been received and ACK tramsmitted
#define TWI_MRX_DATA_NACK          0x58  // Data byte has been received and NACK tramsmitted

// TWI Miscellaneous status codes
#define TWI_NO_STATE               0xF8  // No relevant state information available; TWINT = “0”
#define TWI_BUS_ERROR              0x00  // Bus error due to an illegal START or STOP condition


// TWI TWCR shortcuts
#define TWSTART		TWCR =	(1<<TWEN)					|(1<<TWIE)|(1<<TWINT)										|(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)		|(0<<TWWC)
							//TWI Interface enabled		|Enable TWI Interupt and clear the flag to send byte		|Initiate a START condition
#define TWACK		TWCR =	(1<<TWEN)					|(1<<TWIE)|(1<<TWINT)										|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)		|(0<<TWWC)
							//TWI Interface enabled		|Enable TWI Interupt and clear the flag to send byte		|Send ACK after next reception
#define TWNACK		TWCR =	(1<<TWEN)					|(1<<TWIE)|(1<<TWINT)										|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)		|(0<<TWWC)
							//TWI Interface enabled		|Enable TWI Interupt and clear the flag to read next byte	|Send NACK after reception
#define TWSTOP		TWCR =	(1<<TWEN)					|(0<<TWIE)|(1<<TWINT)										|(0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)		|(0<<TWWC)
							//TWI Interface enabled		|Disable TWI Interrupt and clear the flag					|Initiate a STOP condition
#define TWRESTART	TWCR =	(1<<TWEN)					|(1<<TWIE)|(1<<TWINT)										|(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)		|(0<<TWWC)
							//TWI Interface enabled		|Enable TWI Interupt and clear the flag						|Initiate a (RE)START condition.		

/****************************************************************************
  Private definitions
****************************************************************************/							
static unsigned char TWI_buf[ TWI_BUFFER_SIZE ];    // Transceiver buffer
static uint8_t TWI_msgSize;							// Number of bytes to be transmitted.
static unsigned char TWI_state = TWI_NO_STATE;      // State byte. Default set to TWI_NO_STATE.
							
// ********** Functions ********** //

// ********** Get infos from twi ********** //

/****************************************************************************
Call this function to test if the TWI_ISR is busy transmitting.
****************************************************************************/
unsigned char TWI_busy( void )
{
	return ( TWCR & (1<<TWIE) );                  // IF TWI Interrupt is enabled then the Transceiver is busy
}

/****************************************************************************
Call this function to set up the TWI master to its initial standby state.
Remember to enable interrupts from the main application after initializing the TWI.
****************************************************************************/
void TWI_Master_init(uint8_t twbr)
{
	//reset state
	TWI_statusReg.currentState=TWI_STATE_WAIT;
	
	TWBR = twbr;								// Set bit rate register (Baudrate). Defined in header file.
	// TWSR = TWI_TWPS;							// Not used. Driver presumes prescaler to be 00.
	TWDR = 0xFF;								// Default content = SDA released.
	TWCR = (1<<TWEN)|							// Enable TWI-interface and release TWI pins.
	(0<<TWIE)|(0<<TWINT)|						// Disable Interupt.
	(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|			// No Signal requests.
	(0<<TWWC);									//

}

/****************************************************************************
Call this function to send a prepared message. The first byte must contain the slave address and the
read/write bit. Consecutive bytes contain the data to be sent, or empty locations for data to be read
from the slave. Also include how many bytes that should be sent/read including the address byte.
The function will hold execution (loop) until the TWI_ISR has completed with the previous operation,
then initialize the next operation and return.
****************************************************************************/
void TWI_Master_sendDatas(uint8_t slaveAdress, const unsigned char *msg, uint8_t msgSize){
	//Reset State twi status
	
	TWI_statusReg.lastTransOK=FALSE;

	while ( TWI_busy() );						// Wait until TWI is ready for next transmission.

	TWI_msgSize = msgSize;						// Number of data to transmit.
	
	TWI_buf[0]= (slaveAdress<<TWI_ADR_BITS) | (FALSE<<TWI_READ_BIT); // The first byte must always consit of General Call code or the TWI slave address.
	
	for(int i=0;i<msgSize;i++){					//Copy data to twi_buffer
		TWI_buf[i+1] = msg[i];
	}

	TWSTART;

}

/***********************************************************************
Initialise transmission to get data from slave                                                                     
************************************************************************/
void TWI_Master_getDatas(uint8_t slaveAdress, uint8_t msgSize){
	//Reset State twi status
	TWI_statusReg.lastTransOK=FALSE;
	
	while ( TWI_busy() );						// Wait until TWI is ready for next transmission.

	TWI_msgSize = msgSize;						// Number of data to transmit.
	
	TWI_buf[0]= (slaveAdress<<TWI_ADR_BITS) | (TRUE<<TWI_READ_BIT); // The first byte must always consit of General Call code or the TWI slave address.
	
	TWSTART;
}
/***********************************************************************
Get datas from the buffer
************************************************************************/
void TWI_Master_getBuffer( unsigned char *buf ){
	
	for(int i=0;i<TWI_BUFFER_SIZE;i++){
		buf[i]=TWI_buf[i];
	}
}
// ********** Interrupt Handlers ********** //

/****************************************************************************
This function is the Interrupt Service Routine (ISR), and called when the TWI interrupt is triggered;
that is whenever a TWI event has occurred. This function should not be called directly from the main
application.
****************************************************************************/
ISR(TWI_vect){
	static unsigned char TWI_bufPtr;

	switch (TWSR)
	{
		// START has been transmitted
		case TWI_START:

		// Repeated START has been transmitted
		case TWI_REP_START:
		TWI_bufPtr = 0;									// Set buffer pointer to the TWI Address location
		
		/*WRITE PROCESS*/
		//1. SLA+W has been tramsmitted and ACK received
		case TWI_MTX_ADR_ACK:
		
		//2. Data byte has been tramsmitted and ACK received
		case TWI_MTX_DATA_ACK:
		if (TWI_bufPtr < TWI_msgSize+1)
		{
			TWDR = TWI_buf[TWI_bufPtr++];
			TWCR = (1<<TWEN)|							// TWI Interface enabled
			(1<<TWIE)|(1<<TWINT)|						// Enable TWI Interupt and clear the flag to send byte
			(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|			//
			(0<<TWWC);									//
		}else // Send STOP after last byte
		{
			TWI_statusReg.lastTransOK = TRUE;			// Set status bits to completed successfully.
			TWCR = (1<<TWEN)|							// TWI Interface enabled
			(0<<TWIE)|(1<<TWINT)|						// Disable TWI Interrupt and clear the flag
			(0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|			// Initiate a STOP condition.
			(0<<TWWC);									//
		}
		break;
		
		/*READ PROCESS*/
		//1. SLA+R has been tramsmitted and ACK received
		case TWI_MRX_ADR_ACK:
		TWACK;
		break;
		
		//2. Data byte has been received and send ACK to continue or NACK to stop
		case TWI_MRX_DATA_ACK:
		if (TWI_bufPtr < (TWI_msgSize-1) )		// Detect the last byte to ACK it.
		{
			TWI_buf[TWI_bufPtr++] = TWDR;	
			TWACK;
		}else									// Send NACK after next reception
		{
			TWNACK;								
		}
		break;
		
		//3. Data byte has been received and NACK tramsmitted
		case TWI_MRX_DATA_NACK:
		TWI_buf[TWI_bufPtr] = TWDR;
		TWI_statusReg.lastTransOK = TRUE;				// Set status bits to completed successfully.
		TWSTOP;
		break;
		
		
		/*Error occurs*/
		
		// Arbitration lost
		case TWI_ARB_LOST:
		TWRESTART;
		break;
		
		// SLA+W has been tramsmitted and NACK received
		case TWI_MTX_ADR_NACK:
		
		// SLA+R has been tramsmitted and NACK received
		case TWI_MRX_ADR_NACK:
		
		// Data byte has been tramsmitted and NACK received
		case TWI_MTX_DATA_NACK:
		
		// Bus error due to an illegal START or STOP condition
		case TWI_BUS_ERROR:
		
		default:
		TWI_state = TWSR;							// Store TWSR and automatically sets clears noErrors bit.
		// Reset TWI Interface
		TWCR = (1<<TWEN)|							// Enable TWI-interface and release TWI pins
		(0<<TWIE)|(0<<TWINT)|						// Disable Interupt
		(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|			// No Signal requests
		(0<<TWWC);									//
	}
}