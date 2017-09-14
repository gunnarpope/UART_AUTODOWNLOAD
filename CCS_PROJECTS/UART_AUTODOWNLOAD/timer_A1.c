/*
 *timer_A2.c
 *
 * written by Gunnar Pope 02/05/2015
 * charles.g.pope.th@dartmouth.edu
 *
 * This timer is used by button1 for debouncing and for
 * generating button tap and hold events
 *
 * The associated interrupt vector is
 *
 */


#include "timer_A1.h"

#include <msp430.h>
#include <msp430fr5969.h>
#include <stdint.h>
#include <intrinsics.h>


void timerA1_init(){


	TA1CTL &= ~0x30;			//disable the timer
	TA1CTL |=  MC_0;			//set this equal to 0 to halt the timer when finished (continuous mode)

	 //source the SMCLK, divide clock by 8 (SMCLK = 1Mhz/8 at this point)/ enable interrupts, clear the timer
	TA1CTL |= (TASSEL_2 | ID_3 | TAIE | TACLR );
	TA1EX0   |= TAIDEX_3; //divide the clock by 4 (timer reflects 4Mhz / (8*4) = timer frequency)  //only used for SMCLK = 4MHz. Don't set if SMCLK = 1MHz

	//capture on rising edge, Synch capture, capture mode, enable the interrupt
	TA1CCTL0 |= (CM_1 | SCS | CAP | CCIE); //default CCI3A = 00 -> P3.4 (see p 463 on datasheet)


	TA1CTL |= MC_2;

	P3IE |= BIT4;

}


void timerA1_enable(){
	TA1IV = 0;
	TA1CTL |= (TACLR | MC_2);			//start counting up

}
void timerA1_disable(){
	TA1CTL &=  ~(MC_0 | MC_1 | MC_2); //disable timer
	TA1CTL |= TACLR;

}

