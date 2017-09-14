/*
 * clocks_driver.c
 * written by: Gunnar Pope
 * June 26th, 2015
 * charles.g.pope.th@dartmouth.edu
 *
 * The clocks driver uses the following settings
 *
 * MCLK 	= 1  Mhz (default setting)	//used for MPU processing
 * SMCLK 	= 1 MHz					    //used for peripherals that require precise timing (SPI, UART, etc)
 * ACLK  	= 32.768 kHz				//used to keep time during low-power mode
 */

#include "clocks_driver.h"
#include "msp430fr5969.h"


void clocks_init(void){

	//set the CSKey password to access CS registers
	CSCTL0 = CSKEY;

	//clear the defaults
	CSCTL1 &= ~(0x8 | 0x4 | 0x2);

	//select lower DCORSEL range, set mclk to 8MHz (to be subdivided by 8 later on)
	CSCTL1 |= DCORSEL;
	CSCTL1 |= (DCOFSEL0 | DCOFSEL1);


	//***********************************************set the sources for mclk, smclk, aclk**********************************************

	//set SELA bits to 000, sourcing ACLK on LFXTCLK (32kHz oscillator)
	CSCTL2 &= ~(0x0700);

	//use the dco to for usart debug to start (011 = DCO for SMCLK)
	CSCTL2 |=   0x0030;

	//set the SELM bits to 011 to source the mclk on the DCO
	CSCTL2 |= 0x0003;

	//set the SELS bits to 011 to source the SMCLK on the DCO
//	CSCTL2 &= ~(0x0070);
//	CSCTL2 |=   SELS_3;

	//***********************************************set the clock dividers for mclk, smclk, aclk**********************************************

	//set DIVA bits to 000 to divide aclk by 1
	CSCTL3 &= ~0x0700;

	//set DIVS bits to 000 to divide smclk by 8
	CSCTL3 |= 0x0030; //divide by 8; this works to generate a 1Mhz MCLK
//	CSCTL3 |= 0x00; //generate a 4Mhz MCLK						<<---------CHECK THIS (SHOULDN'T IT BE ZERO)??!!!!!!!!

	//set DIVM bits to 011 to divide mclk by 8
	CSCTL3 |= 0x0003;

//	CSCTL3 &= ~0x0003; //generate a 4Mhz MCLK (8MHZ/2 = 4MHZ)
//	CSCTL3 |= 1;
	//***********************************************set the clock dividers for mclk, smclk, aclk**********************************************

	//turn off the VLO and HFXT
	CSCTL4 &= ~( HFXTDRIVE_3 | SMCLKOFF | LFXTOFF);
	CSCTL4 |= (HFXTOFF | VLOOFF);
	
	//enable the PJ.4 and PJ.5 registers to enable the LFXIN/LFXOUT (this is right)
	PJSEL0 |=  (BIT5 | BIT4);
	PJSEL1 &= ~(BIT5 | BIT4);
	PJDIR  |= BIT5; //SET PJ.5OUT AS OUTPUT
	PJREN  &= ~BIT4;
	PJDIR  &= ~BIT4;

	//LFXT is sourced from external crystal
	CSCTL4 &= ~(LFXTBYPASS | LFXTOFF);

	//increase the drive strength of the crystal
	CSCTL4 |= LFXTDRIVE_0;

	//clear the fault bit. this should work at all times
	CSCTL5 &= ~0x0001;
	while(CSCTL5 & 0x0001 > 0){
		CSCTL5 &= ~0x0001;
	}

	//clear the cskey
	CSCTL0_H = 0;

}

