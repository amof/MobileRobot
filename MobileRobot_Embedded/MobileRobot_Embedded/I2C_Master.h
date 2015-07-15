/*
 * Handle The I2C communication
 *
 *  Author: amof
 *  Specifically develop for ATMEGA-8bits family
 *  Based on the work of Atmel
 *  License: GNU General Public License V3
 *	Version : 1
 *	
 * 
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
extern void TWI_Master_sendDatas(uint8_t slaveAdress, const unsigned char *msg, uint8_t msgSize);
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

#endif /* I2C_MASTER_H_ */