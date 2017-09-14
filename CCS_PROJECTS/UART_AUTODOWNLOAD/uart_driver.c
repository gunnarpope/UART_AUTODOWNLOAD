// created by Gunnar Pope
// charles.g.pope.th@dartmouth.edu
// June 15th, 2015
//modified from amulet code

/*connections
TTL-232RG-VIP-WE UART CABLE
Vcc = RED
GND = BLK
RxD = YLW
TxD = ORG
RTS = GRN
CTS = BRN

To connect:
Orange UART cable (TxD) connect to Rx Bracelet Port 2.6
Yellow UART cable (RxD) connect to Tx Bracelet Port 2.5
Black  UART cable (GND) connect to GND Bracelet Port
Red    UART cable (VCC) connect to GPIO Bracelet Port 4.1


  //this works with a 115200 Baud rate, 8N1, Xon/Xoff
   *
   *
*/
#include <stdint.h>
#include <stdlib.h>
#include "msp430fr5969.h"


#define UART_PxSEL1	P2SEL1
#define UART_PxSEL0 P2SEL0
#define UART_PIN0	BIT5
#define UART_PIN1	BIT6




void UARTInit(void){

	// Disable the GPIO power-on default high-impedance mode to activate
	// previously configured port settings
	PM5CTL0 &= ~LOCKLPM5;

	//initialize P4.5 as the input CS bit to start the upload stream
  	P4DIR 	&= ~BIT0;		//0 = direction is input
  	P4REN	&= ~BIT0;		//pull up resistor disabled
  	//P4OUT   				//don't care
  	P4IES	&= ~BIT0;		//Interrupt generated on low to high transition
  	P4IE 	|=  BIT0;		//Enable the interrupt on P4.5


	//initialize uart pins


	UART_PxSEL1 |=   UART_PIN0 | UART_PIN1;                    // USCI_A0 UART operation
	UART_PxSEL0 &= ~(UART_PIN0 | UART_PIN1);					//p88 device specific UG


	//see p559 of user manual for setting the baud rate


	//this works with a 115200 Baud rate, 8N1, Xon/Xoff
	//1MHz / 115200 = 8.68
	UCA1CTLW0   |= UCSWRST;                      // Put eUSCI in reset
	UCA1CTLW0  	|= (UCSSEL__SMCLK);
	UCA1BRW		 = 8;					//this works for SMCLK = 1Mhz
//	UCA1BRW		 = 34;					//SMCLK = 4Mhz (see clocks driver DIVS bits) --->> 4MHz / 115200 = 34.77
	UCA1MCTLW	&= ~0xFF00;
	UCA1MCTLW	|= 0xB600;				//UCBRSx = 0xC6, empirically found, instead of 0xD6 modulator to account for 0.68 remainder

	UCA1CTL1 	&= ~UCSWRST;                     // release from reset
	UCA1IE 		|= UCRXIE;                         // Enable USCI_A0 RX interrupt


  //****************************************************Works to output 1.0000 MHz output at Aclk********************************
//  //this works with a 115200 Baud rate, 8N1, Xon/Xoff
//  //1MHz / 115200 = 8.68
//  	UCA1CTLW0   |= UCSWRST;                      // Put eUSCI in reset
//  	UCA1CTLW0  	|= (UCSSEL__SMCLK);
//    UCA1BR0    	|= 0;
//    UCA1BR1 	|= 0;
//    UCA1BRW 	&= ~0xFF00;
//    //UCA1BRW |= 0xD600; //set UCBRSx = 0xD6 to modulator to account for 0.68 remainder
//    //UCA1MCTLW = (0xF700 | 0x0000 | 0x0001);						//set UCBRS = 3;
//    UCA1CTL1 	&= ~UCSWRST;                     // release from reset
//    UCA1IE 		|= UCRXIE;                         // Enable USCI_A0 RX interrupt
    //****************************************************Works to output 1.0000 MHz output at Aclk********************************

//**********************************************************************origional UART settings below that work on Amulet board,dev board**************************
//    //this works with a 115200 Baud rate, 8N1, Xon/Xoff
//    //16Mhz sclk (this had 131Hz kHz baud rate)
//      UCA1CTLW0   = UCSWRST;                      // Put eUSCI in reset
//      UCA1CTLW0  |= (UCSSEL__SMCLK);
//      UCA1BR0 = 2;
//      UCA1BR1 = 0;
//      //UCBRSx = 0xF7, UCBRFx = 00, UCOS = 1
//      UCA1MCTLW = (0xF700 | 0x0000 | 0x0001);						//set UCBRS = 3;
//      UCA1CTL1 &= ~UCSWRST;                     // release from reset
//      UCA1IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
//************************************************************************************************************************************************************************
}

void PutChar(char data){
	while (!(UCA1IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
	UCA1TXBUF = data;                  // TX -> RXed character

}

void PutData(char *data, uint8_t len){
	uint8_t i;
	for(i=0; i<len; i++) {
		PutChar(data[i]);
	}
}
//
//void PutFloat(long data){
//
//	long x = (long) data;
//	long nums[7];
//
//	x = x * 1000;
//	nums[6] = x % 10;
//	x = x/10;
//	nums[5] = x % 10;
//	x = x/10;
//	nums[4] = x % 10;
//	x = x/10;
//	nums[3] = x % 10;
//	x = x/10;
//	nums[2] = x % 10;
//	x = x/10;
//	nums[1] = x % 10;
//	x = x/10;
//	nums[0] = x % 10;
//	x = x/10;
//
//	sprintf(str, "%l%l%l.%l%l%l%l",nums[6],nums[5],nums[4],nums[3],nums[2],nums[1],nums[0]);
//	PutData(str,strlen(str));
//
//}
////Used to print interger in UART
//void print_number(uint16_t value){
//	char result[6];
//	itoa(value, result, 10);
//	PutData(result,5);
//}


