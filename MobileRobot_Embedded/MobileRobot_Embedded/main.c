/*
 * MobileRobot_Embedded.c
 *
 * Created: 22/06/2015 13:10:29
 *  Author: Julien
 */ 

#define F_CPU 1000000UL //!!Please adjust to your speed to get a correct configuration

#include <avr/io.h>
#include <avr/interrupt.h>
#include "AdafruitMotorShield.h"
#include "NavComs.h"


int main(void)
{
	//AFMS_init();
	

	sei();
	UART_tx("Initialize!");
	UART_tx_launch();

    while(1){

		if(UART_datasReceived==1){ //Datas have been received from UART
			UART_datasReceived=0;
			char* tampon=UART_read_buffer();
			UART_tx("R:");
			UART_tx(tampon);
			UART_tx_launch();
			/*switch (tampon[0]){
			case 'U':
				AFMS_DCMotor_setSpeed(M1,150);
				AFMS_DCMotor_run(M1,FORWARD);
				AFMS_DCMotor_setSpeed(M2,150);
				AFMS_DCMotor_run(M2,FORWARD);
				AFMS_DCMotor_setSpeed(M3,150);
				AFMS_DCMotor_run(M3,FORWARD);
				AFMS_DCMotor_setSpeed(M4,150);
				AFMS_DCMotor_run(M4,FORWARD);
				UART_tx("UP !");
				break;
			case 'D':
				AFMS_DCMotor_setSpeed(M1,150);
				AFMS_DCMotor_run(M1,BACKWARD);
				AFMS_DCMotor_setSpeed(M2,150);
				AFMS_DCMotor_run(M2,BACKWARD);
				AFMS_DCMotor_setSpeed(M3,150);
				AFMS_DCMotor_run(M3,BACKWARD);
				AFMS_DCMotor_setSpeed(M4,150);
				AFMS_DCMotor_run(M4,BACKWARD);
				UART_tx("DOWN !");
				break;
			case 'R':
				AFMS_DCMotor_setSpeed(M2,150);
				AFMS_DCMotor_run(M2,FORWARD);
				AFMS_DCMotor_setSpeed(M3,150);
				AFMS_DCMotor_run(M3,FORWARD);
				UART_tx("RIGHT !");
				break;
			case 'L':
				AFMS_DCMotor_setSpeed(M1,150);
				AFMS_DCMotor_run(M1,FORWARD);
				AFMS_DCMotor_setSpeed(M4,150);
				AFMS_DCMotor_run(M4,FORWARD);
				UART_tx("LEFT !");
				break;
			case 'S':
				AFMS_DCMotor_run(M1,RELEASE);
				AFMS_DCMotor_run(M2,RELEASE);
				AFMS_DCMotor_run(M3,RELEASE);
				AFMS_DCMotor_run(M4,RELEASE);
				UART_tx("STOP !");
				break;
			}		
			UART_tx_launch();*/
		}
    }
}