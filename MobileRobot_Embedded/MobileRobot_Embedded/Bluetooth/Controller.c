/*************************************************************************
	
	Controller library by Julien Delvaux

*************************************************************************/

#include "Controller.h"


/************************************************************************/
/* Constants and macros                                                 */
/************************************************************************/

#define controller_length_command	7
#define controller_BUFFER_SIZE		15

//States for the getData function
#define stateDetection_S_STX		0 // waiting for the STX
#define stateDetection_S_FLAG		1 // waiting for the flag
#define stateDetection_S_VERSION	2 // waiting for the version
#define stateDetection_S_LENGTH		3 // waiting for the version
#define stateDetection_S_COMMAND	4 // waiting for the version
#define stateDetection_S_ETX		5 // waiting for the ETX
#define stateDetection_C_SIZE		6 // check if the minimal size if reached


/************************************************************************/
/* Global variable                                                      */
/************************************************************************/
static const uint8_t f_STX=0x02;
static const uint8_t f_ETX=0x03;
static const uint8_t f_FLAG=0xAA;
static const uint8_t f_version=0x01;

volatile uint8_t stateDetection=stateDetection_S_STX;
volatile uint8_t buffer_rx[controller_BUFFER_SIZE];
volatile uint8_t buffer_rx_head = 0;

/*************************************************************************
Function: controller_init()
Purpose:  Initialize controller in UART @9600 baud
Input:    none
Returns:  none
**************************************************************************/
void controller_init(void){
	uart_init(UART_BAUD_SELECT(9600, F_CPU));
	
	//Bluetooth module need his TX to be HIGH -> RX in 1284P Pull-UP enabled
	DDRD &= ~(1<<PIND0);
	PORTD |= 1<<PIND0; // Protection to avoid problem to receive datas
	
	#ifdef DEBUG_ENABLED
		uart1_init(UART_BAUD_SELECT(57600, F_CPU));
		uart1_puts("[Controller] DEBUG ENABLED\r\n");
	#endif
}

/*************************************************************************
Function: controller_sendCommand()
Purpose:  Send a command to the bluetooth
Input:    1 byte command (C_C_X) : x being the command
Returns:  none
**************************************************************************/
void controller_sendCommand(uint8_t command){
	
	uint8_t framed_data[controller_length_command];
	
	//Clear the array
	for(uint8_t i=0; i<controller_length_command;i++){
		framed_data[i]=0;
	}
	
    //1. Append start + flag
    framed_data[0]=f_STX;
    framed_data[1]=f_FLAG;
    framed_data[2]=f_FLAG;

    //2. Append version + length
    framed_data[3]=f_version;
    framed_data[4]=1;

    //3. Append command
    framed_data[5]=command;

    //4. Append end
    framed_data[6]=f_ETX;
	
	// Send framed_data
	for(uint8_t i=0; i<controller_length_command;i++){
		uart_putc(framed_data[i]);
	}
}

/*************************************************************************
Function: controller_getData()
Purpose:  Retrieve command from Bluetooth
Input:    none
Returns:  none
**************************************************************************/
uint8_t controller_getData(void){
	
	uint8_t command=0;
	uint8_t length=0;
	
	while(uart_available()>0){

		buffer_rx[buffer_rx_head++] = uart_getc();
		
		switch(stateDetection){
		
			case stateDetection_S_STX:
			if(buffer_rx[buffer_rx_head - 1]==f_STX){
				#ifdef DEBUG_ENABLED
				uart1_puts("[Controller] Start STX detected\r\n");
				#endif
				stateDetection=stateDetection_S_FLAG;
			}
			else{
				if(buffer_rx_head>0)buffer_rx_head--;
			}
			break;
		
			case stateDetection_S_FLAG:
			if(buffer_rx[buffer_rx_head - 2]==f_FLAG && buffer_rx[buffer_rx_head - 1]==f_FLAG){
				#ifdef DEBUG_ENABLED
				uart1_puts("[Controller] FLAG detected\r\n");
				#endif
				stateDetection=stateDetection_S_VERSION;
			}
			break;
		
			case stateDetection_S_VERSION:
			if(buffer_rx[buffer_rx_head - 1]==f_version){
				#ifdef DEBUG_ENABLED
				uart1_puts("[Controller] Version detected\r\n");
				#endif
				stateDetection=stateDetection_S_LENGTH;
			}
			break;
		
			case stateDetection_S_LENGTH:
			#ifdef DEBUG_ENABLED
			uart1_puts("[Controller] Length detected\r\n");
			#endif
			length=buffer_rx[buffer_rx_head - 1];
			stateDetection=stateDetection_S_COMMAND;
			break;
		
			case stateDetection_S_COMMAND:
			#ifdef DEBUG_ENABLED
			uart1_puts("[Controller] Command detected\r\n");
			#endif
			stateDetection=stateDetection_S_ETX;
			break;
		
			case stateDetection_S_ETX:
			if(buffer_rx[buffer_rx_head - 1]==f_ETX){
				#ifdef DEBUG_ENABLED
				uint8_t conv[10];
				uart1_puts("[Controller] End ETX detected\r\n");
				itoa(command, conv, 10);
				uart1_puts("[Controller] Command: ");
				uart1_puts(conv);
				uart1_puts("\r\n");
				#endif
				command=buffer_rx[buffer_rx_head - 2];
				stateDetection=stateDetection_S_STX;
			}else{
				command=0;
			}
			break;
		
			default:
			break;
		}
	}
	
	return command;
	
}