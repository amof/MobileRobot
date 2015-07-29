/*
 * This library can handle the Adafruit Motor Shield v2 by I2C
 * **REQUIRED: I2C library
 *
 *  Author: amof
 *  Specifically develop for ATMEGA-8bits family
 *  Based on the work of Adafruit
 *  License: GNU General Public License V3
 *	Version : 1.0
 * 
 */ 
 


#ifndef ADAFRUITMOTORSHIELD_H_
#define ADAFRUITMOTORSHIELD_H_

/****************************************************************************
  Public definitions
****************************************************************************/

#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

typedef struct {
	uint8_t PWMpin;
	uint8_t IN1pin;
	uint8_t IN2pin;
}DCMotor;

static DCMotor M1={8,10,9};
static DCMotor M2={13,11,12};
static DCMotor M3={2,4,3};
static DCMotor M4={7,5,6};

/****************************************************************************
  Function definitions
****************************************************************************/
/**
   @brief   Initialize the library
   @param   none
   @return  none
*/
extern void AFMS_init( void );
/**
   @brief   Define the direction of the motor
   @param   M: Motor (M1 .. M4)
			cmd : FORWARD/BACKWARD/BRAKE/RELEASE
   @return  none
*/
extern void AFMS_DCMotor_run(DCMotor M, uint8_t cmd );
/**
   @brief   Set the speed of a motor
   @param   M: Motor (M1 .. M4)
			speed : value between [0-255]
   @return  none
*/
extern void AFMS_DCMotor_setSpeed(DCMotor M, uint8_t speed );

#endif /* ADAFRUITMOTORSHIELD_H_ */