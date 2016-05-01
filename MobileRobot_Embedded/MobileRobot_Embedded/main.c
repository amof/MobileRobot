/*
 * MobileRobot_Embedded.c
 *
 * Created: 22/06/2015 13:10:29
 *  Author: Julien
 */ 

#define F_CPU 8000000UL //!!Please adjust to your speed to get a correct configuration

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Bluetooth/Controller.h"
#include "Motors/AdafruitMotorShield.h"
#include "Bluetooth/Uart.h"

volatile uint8_t waitResponse = 0;
volatile uint8_t sendRespondAlive=0;
volatile uint8_t active=1;

void timer_init();
void moveRobot(uint8_t command);

int main(void)
{	
	controller_init();
	timer_init();
	AFMS_init();
	
	sei();
	
    while(1){
		uint8_t message = controller_getData();
		if(message!=0 && message!=C_C_ALOHA && message!=C_C_ALIVE && message!=C_C_END_CON && active==1){
			controller_sendCommand(C_C_ACK);
			moveRobot(message);
		}
		else if(message==C_C_ALOHA){
			waitResponse=0;
			active=1;
			controller_sendCommand(C_C_ACK);
			TCNT1=15535; // 50 ms
			TCCR1B = (0<<CS12)|(1<<CS11)|(0<<CS10); // start timer
			//moveRobot(C_M_STOP);
			
		}else if(message==C_C_END_CON){
			controller_sendCommand(C_C_ACK);
			moveRobot(C_M_STOP);
			active=0;
			uart_flush();
			TCCR1B = (0<<CS12)|(0<<CS11)|(0<<CS10); // stop timer
			
		}else if(message==C_C_ALIVE){
			waitResponse=0;
		}
		if(waitResponse>5){
			TCCR1B = (0<<CS12)|(0<<CS11)|(0<<CS10); // stop timer
			uart_flush();
			waitResponse=0;
			active=0;
			AFMS_DCMotor_run(M1,RELEASE);
			AFMS_DCMotor_run(M2,RELEASE);
			AFMS_DCMotor_run(M3,RELEASE);
			AFMS_DCMotor_run(M4,RELEASE);
		}
		if(sendRespondAlive==1){
			controller_sendCommand(C_C_ALIVE);
			sendRespondAlive=0;
		}
			
    }
}

void timer_init(){
	TCCR1A = (0<<COM1A1)|(0<<COM1A0)|(0<<COM1B0)|(0<<COM1B1)|(0<<WGM10)|(0<<WGM11)|(0<<WGM12);
	TCCR1B = (0<<CS12)|(0<<CS11)|(0<<CS10);
	TIMSK1 |= (1 << TOIE1);
}
ISR(TIMER1_OVF_vect){
	TCNT1=15535; // 50 ms
	waitResponse++;
	sendRespondAlive=1;
}

void moveRobot(uint8_t command){
	
	switch(command){
		case C_M_FW:
		AFMS_DCMotor_setSpeed(M1,150);
		AFMS_DCMotor_run(M1,FORWARD);
		AFMS_DCMotor_setSpeed(M2,150);
		AFMS_DCMotor_run(M2,FORWARD);
		AFMS_DCMotor_setSpeed(M3,150);
		AFMS_DCMotor_run(M3,FORWARD);
		AFMS_DCMotor_setSpeed(M4,150);
		AFMS_DCMotor_run(M4,FORWARD);
		break;
		case C_M_BW:
		AFMS_DCMotor_setSpeed(M1,150);
		AFMS_DCMotor_run(M1,BACKWARD);
		AFMS_DCMotor_setSpeed(M2,150);
		AFMS_DCMotor_run(M2,BACKWARD);
		AFMS_DCMotor_setSpeed(M3,150);
		AFMS_DCMotor_run(M3,BACKWARD);
		AFMS_DCMotor_setSpeed(M4,150);
		AFMS_DCMotor_run(M4,BACKWARD);
		break;
		case C_M_RIGHT:
		AFMS_DCMotor_setSpeed(M2,150);
		AFMS_DCMotor_run(M2,FORWARD);
		AFMS_DCMotor_setSpeed(M3,150);
		AFMS_DCMotor_run(M3,FORWARD);
		break;
		case C_M_LEFT:
		AFMS_DCMotor_setSpeed(M1,150);
		AFMS_DCMotor_run(M1,FORWARD);
		AFMS_DCMotor_setSpeed(M4,150);
		AFMS_DCMotor_run(M4,FORWARD);
		break;
		case C_M_STOP:
		AFMS_DCMotor_run(M1,RELEASE);
		AFMS_DCMotor_run(M2,RELEASE);
		AFMS_DCMotor_run(M3,RELEASE);
		AFMS_DCMotor_run(M4,RELEASE);
		break;
	}
}