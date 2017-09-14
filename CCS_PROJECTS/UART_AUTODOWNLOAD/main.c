/* A project to automate the download of EDA data from the MSP430 to a laptop using UART communications
 *
 * by: Gunnar Pope
 * Date: 8/10/17
 *
 * main.c
 *
 * Settings:
 * MCLK = 8MHz, subdivided by 8 so the actual clock is 1MHz
 * SMCLK = 1MHz
 * TimerA1 is used to trigger interrupts on P1.7 (rising edges) to halt the TA1 timer.
 * The final value is in the TA1CCR0 register.
 *
 *
 * UART-USB CONNECTIONS
 *
 * P2.6 -> Orange
 * P2.5 = Yellow
 * GND  = Black
 *
 *
 *Connect to the UART device in the terminal on your mac

~$ ls /dev/tty.usbserial-FTXOGKV5               //search for the connected uart thing
/dev/tty.usbserial-FTXOGKV5
~$ screen /dev/tty.usbserial-FTXOGKV5 115200	 	baud = 115200
 *
 */

#include <msp430.h>
#include <msp430fr5969.h>
#include <intrinsics.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "uart_driver.h"
#include "clocks_driver.h"
#include "interrupts.h"
#include "timer_A1.h"
#include "rtc.h"


extern void clocks_init(void);
extern void timerA1_init();
extern void rtc_init();


char print_status = 0;
char str[10] = {};






int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;



	clocks_init();
	UARTInit();
	rtc_init();

	P1DIR |= BIT0;

	P4DIR &= ~BIT5;
	P4OUT |=  BIT5;
	P4REN |=  BIT5;
	P4IE  |=  BIT5;
	P4IES |=  BIT5;

	P1DIR &= ~(BIT7);
	P1SEL0  |= BIT7; //select bit 4			//SEE DATASHEET P83
	P1SEL1  |= BIT7; //set port function to primary module to enable input on TB3 from P1.7 for TA1.CC10A (p83 on datasheet)

	//initialize the ACLK pin as low
	PJDIR |= (BIT2);
	PJOUT &= ~BIT2;



	__enable_interrupt();

	//test the UART screen connection
	PutChar('G');



    while(1){

//    		PutChar('G');  //test the uart functionality
//    		__delay_cycles(20000);

    		// if the UART is triggered
    		if (flag1 & UCA1RX_FLAG1){ //this bit is set in the interrupts.h file

    			flag1 &= ~UCA1RX_FLAG1;

    			PutChar(uart_byte); //print to uart screen

    			switch (uart_byte){

    				// CREATE FSM FOR COMMUNICATION WITH LAPTOP HERE
    				case 'D': 				//for download
    				{

    					sprintf(str, "This works!\r\n",timer_val);
    					PutData(str,strlen(str));

    				}
    				break;

//    				default:
//    				{
//    					sprintf(str, "Recieved: \r\n",timer_val);
//    					    					PutData(str,strlen(str));
//    				}

    			}
    		}

    }


	return 0;
}


