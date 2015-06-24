/*
 * MobileRobot_Embedded.c
 *
 * Created: 22/06/2015 13:10:29
 *  Author: Julien
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "AdafruitMotorShield.h"

DCMotor M1={8,10,9};
DCMotor M2={13,11,12};
DCMotor M3={2,4,3};
DCMotor M4={7,5,6};

int main(void)
{

	sei();
	
	AFMS_init();
	AFMS_DCMotor_setSpeed(M1,150);
	AFMS_DCMotor_run(M1,FORWARD);
	AFMS_DCMotor_setSpeed(M2,150);
	AFMS_DCMotor_run(M2,FORWARD);
	AFMS_DCMotor_setSpeed(M3,150);
	AFMS_DCMotor_run(M3,FORWARD);
	AFMS_DCMotor_setSpeed(M4,150);
	AFMS_DCMotor_run(M4,FORWARD);
    while(1)
    {


    }
}