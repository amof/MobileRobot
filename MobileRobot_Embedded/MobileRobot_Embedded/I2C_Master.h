/*
 * I2C_Master.h
 *
 * Created: 30/01/2015 09:35:28
 *  Author: amof
 */ 


#ifndef I2C_MASTER_H_
#define I2C_MASTER_H_

/****************************************************************************
  TWI Status/Control register definitions + Slaves Adress
****************************************************************************/
#define TWI_BUFFER_SIZE		20	// Set this to the largest message size that will be sent including address byte.
#define TWI_FREQ_SELECT(freq,xtalCpu)  (xtalCpu / (2UL * freq)) -8UL //Refer to Datasheet to see calculus

/****************************************************************************
  Function definitions
****************************************************************************/
/**
   @brief   Function to test if the TWI_ISR is busy transmitting.
   @param   none
   @return  0 or 1 : TWI Interrupt is enabled then the Transceiver is busy
*/
extern unsigned char TWI_busy( void );
/**
   @brief   Initialization of interface TWI in Master
   @param   none
   @return  none
*/
extern void TWI_Master_init(uint8_t twbr);
/**
   @brief   Send a message via TWI
   @param   slaveAdress : adress of the slave we want to talk to
			msg : array with datas
			msgSize : size of array of datas
   @return  none
*/
extern void TWI_Master_sendDatas(uint8_t slaveAdress, unsigned char msg[], uint8_t msgSize);
/**
   @brief   Send a byte via TWI
   @param   slaveAdress : adress of the slave we want to talk to
			msg : 1 byte
   @return  none
*/
extern void TWI_Master_getDatas(uint8_t slaveAdress, uint8_t msgSize);
/**
   @brief   Request one data byte from slave via TWI
   @param   slaveAdress : adress of the slave we want to talk to
   @return  none
*/
extern void TWI_Master_getDatasByte(uint8_t slaveAdress);
/**
   @brief   Transfer the twi_buffer into the buffer we want to use 
   @param   buf : buffer
   @return  none
*/
extern void TWI_Master_getBuffer( unsigned char *buf );

/****************************************************************************
  TWI struct to know status of transmission
****************************************************************************/
struct TWI_statusReg
{
	uint8_t currentState;
	uint8_t lastTransOK;
};

volatile struct TWI_statusReg TWI_statusReg;

#define TWI_STATE_WAIT		0
#define TWI_STATE_SEND		1
#define TWI_STATE_REQUEST	2

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
		


#endif /* I2C_MASTER_H_ */