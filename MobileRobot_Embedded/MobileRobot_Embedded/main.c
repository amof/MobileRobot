/*
 * MobileRobot_Embedded.c
 *
 * Created: 22/06/2015 13:10:29
 *  Author: Julien
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "Uart.h"

int main(void)
{
	UART_init(UART_BAUD_SELECT(9600,1000000UL));
	
	sei();
	
    while(1)
    {
        if(UART_datasReceived==1){ //Datas have been received from UART
	        UART_datasReceived=0;
	        char* tampon=UART_read_buffer();
	        UART_tx(tampon);
			UART_tx_launch();
        }

    }
}