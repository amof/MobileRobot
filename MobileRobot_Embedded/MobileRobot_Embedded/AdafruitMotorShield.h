/*
 * AdafruitMotorShield.h
 *
 * Created: 23/06/2015 08:57:59
 *  Author: amof
 * This library handle only DC Motors
 */ 


#ifndef ADAFRUITMOTORSHIELD_H_
#define ADAFRUITMOTORSHIELD_H_

/****************************************************************************
  I2C adresses used by PCA9685
****************************************************************************/

#define AdafruitMotorShield	0x60

#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD

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