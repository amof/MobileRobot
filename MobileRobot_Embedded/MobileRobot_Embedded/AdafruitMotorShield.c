/*
 * AdafruitMotorShield.c
 *
 * Created: 23/06/2015 08:57:29
 *  Author: amof
 */ 

#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "AdafruitMotorShield.h"
#include "I2C_Master.h"

/****************************************************************************
  Private definitions
****************************************************************************/
#define LOW		0
#define HIGH	1
/****************************************************************************
  Private functions
****************************************************************************/
//LOW LEVEL
uint8_t readConf(uint8_t addr){
	TWI_Master_getDatas(AdafruitMotorShield,1);
	
	uint8_t oldmode;
	
	//wait until we have the response
	while (1){
		if(! TWI_busy() && TWI_statusReg.lastTransOK==1){
			unsigned char *dataBuffer=malloc(20*sizeof(char));
			TWI_Master_getBuffer(dataBuffer);
			oldmode=dataBuffer[0];
			free(dataBuffer);
			break;
		}
		
	}
	
	return oldmode;

}
void writeConf(uint8_t addr, uint8_t d){
	uint8_t msg[2]={addr,d};
	TWI_Master_sendDatas(AdafruitMotorShield, msg,2);
}
void reset(void){
	uint8_t msg[1]={PCA9685_MODE1};
	TWI_Master_sendDatas(AdafruitMotorShield, msg,1);
}
void setPWM(uint8_t num, uint16_t on, uint16_t off){
	uint8_t msg[5];
	msg[0]=LED0_ON_L+4*num;
	msg[1]=on;
	msg[2]=on>>8;
	msg[3]=off;
	msg[4]=off>>8;
	TWI_Master_sendDatas(AdafruitMotorShield, msg,5);

}
void setPWMFreq(float freq){
	float prescaleval = 25000000;
	prescaleval /= 4096;
	prescaleval /= freq;
	prescaleval -= 1;
	uint8_t prescale = floor(prescaleval + 0.5);
	
	uint8_t oldmode = readConf(PCA9685_MODE1);
	oldmode = 0x00;
	uint8_t newmode = (oldmode&0x7F) | 0x10; // sleep
	
	writeConf(PCA9685_MODE1, newmode); // go to sleep
	writeConf(PCA9685_PRESCALE, prescale); // set the prescaler
	writeConf(PCA9685_MODE1, oldmode);
	_delay_ms(5);
	writeConf(PCA9685_MODE1, oldmode | 0xa1);  //  This sets the MODE1 register to turn on auto increment.
	// This is why the beginTransmission below was not working.
}
//HIGH LEVEL
void AFMS_setPWM(uint8_t pin, uint16_t value){
	if (value > 4095) {
		setPWM(pin, 4096, 0);
	} else
		setPWM(pin, 0, value);
}
void AFMS_setPIN(uint8_t pin, uint8_t value){
	  if (value == LOW)
		setPWM(pin, 0, 0);
	  else if(value == HIGH)
		setPWM(pin, 4096, 0);
}
/****************************************************************************
  Public functions
****************************************************************************/

void AFMS_init( void ){
	
	sei();
	
	TWI_Master_init(TWI_FREQ_SELECT(10000,1000000UL)); //initialize TWI interface in master mode
	
	reset();
	
	setPWMFreq(1600);
	
	for (uint8_t i=0; i<16; i++) setPWM(i, 0, 0);
	
	cli();
	
}

void AFMS_DCMotor_run(DCMotor M, uint8_t cmd ){
	switch (cmd) {
		case FORWARD:
		AFMS_setPIN(M.IN2pin, LOW); // take low first to avoid 'break'
		AFMS_setPIN(M.IN1pin, HIGH);
		break;
		case BACKWARD:
		AFMS_setPIN(M.IN1pin, LOW); // take low first to avoid 'break'
		AFMS_setPIN(M.IN2pin, HIGH);
		break;
		case RELEASE:
		AFMS_setPIN(M.IN1pin, LOW); 
		AFMS_setPIN(M.IN2pin, LOW);
		break;
	}
}

void AFMS_DCMotor_setSpeed(DCMotor M, uint8_t speed ){
	AFMS_setPWM(M.PWMpin, speed*16);
}