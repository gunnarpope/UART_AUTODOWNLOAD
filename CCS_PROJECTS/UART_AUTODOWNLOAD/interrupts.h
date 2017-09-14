

#ifndef interrupts_h
#define interupts_h

#include <stdio.h>
#include "timer_A1.h"

#define UCA1RX_FLAG1 0x0001

uint8_t sample_count = 0;
uint16_t timer_val = 0;

uint8_t record = 0;

extern void timerA1_disable();
extern char print_status;

extern uint16_t flag1 = 0;
extern uint16_t uart_byte = 0;


// this is the UART interrupt that triggers at each reception of a byte in the UCA1RXBUF register
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A1_VECTOR))) USCI_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{



	switch(__even_in_range(UCA1IV,8))
	  {

	PutChar(UCA1RXBUF);					//print the caracter

	  case 0:break;                             // Vector 0 - no interrupt
	  case 2:                                   // Vector 2 - RXIFG
	    while (!(UCA1IFG&UCTXIFG));             // USCI_A0 TX buffer ready?

	    uart_byte = UCA1RXBUF;				// READ THE CONTENTS OF THE UART REGISTER (1 byte)


		flag1 |= UCA1RX_FLAG1;				// set the flag to wake up from low power mode
		__low_power_mode_off_on_exit();
//		PutChar(uca1rxbuff);					//print the caracter


	    break;
	  case 4:break;                             // Vector 4 - TXIFG
	  default: break;
	  }
}




// trap isr assignation - put all unused ISR vector here
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) TIMER1_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
	switch(__even_in_range(TA1IV_TACCR1,TA1IV_TAIFG)){
	case TA1IV_TA1CCR1:{

		TA1CTL &= ~MC_3;  //stop the timer
		timer_val = TA1CCR0;
		TA1CTL |= TACLR;
		TA1CTL |= MC_2;  //start the timer

		if (record == 1){
			record = 0;
			print_status = 1;
		}
	}
		break;

	case TA1IV_TA1CCR2:
//		TA1CTL |= TACLR;
		break;
	case TA1IV_TA1IFG:
		TA1CCTL0 &= ~2; //clear the overflow flag
		break;
	default:
		break;
	}


}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=RTC_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(RTC_VECTOR)))
#endif
void RTC_B_ISR(void)
{
//    // Unlock backup system
//    while(BattBak_unlockBackupSubSystem(BAK_BATT_BASE))
//    {
//        ;
//    }

    switch(__even_in_range(RTCIV,16))
    {
    case 0: break;      //No interrupts
    case 2:             //RTCRDYIFG
//    	//insert code here to allow for RTC reading

//    	__low_power_mode_off_on_exit();
        break;
    case 6:  break;           //RTCAIFG
    case 8:

    	break;		//RT0PSIFG

    case 10:  {    //RT1PSIFG
    		record = 1;

    }
        	break;
    case 12: break;     //Reserved
    case 14: break;     //Reserved
    case 16: break;     //Reserved
    default: break;
    }
}



//ADDED BY GUNNAR 2/14/2015
//used to debounce button1
//ISR initialized in timer_A1.c
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TimerA1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) TimerA1 (void)
#else
#error Compiler not supported!
#endif
{
//	TA1CTL |= TACLR;
//	__low_power_mode_off_on_exit();
	TA1CTL &= ~MC_3;  //stop the timer
	TA1IV = 0;
	TA1CCTL0 &= ~2; //clear the overflow flag
	TA1CTL |= (TACLR | MC_2);
//	TA1CCTL0 &= ~0xE;
}//end interrupt



// trap isr assignation - put all unused ISR vector here
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER3_A0_VECTOR
__interrupt void TIMER3_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER3_A0_VECTOR))) TIMER3_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
	__low_power_mode_off_on_exit();
}




#pragma vector = TIMER3_A1_VECTOR
__interrupt void TimerA3 (void){
	TA3CCR0 =  0;
	if (TA3IV & TA3IV_TACCR1){


	}


}//end interrupt


// trap isr assignation - put all unused ISR vector here
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12_ISR (void)
#else
#error Compiler not supported!
#endif
{

	switch(_even_in_range(ADC12IFG0,ADC12IFG9)){
	case ADC12IFG0:{

		}
		break;
	case ADC12IFG1: break;
	case ADC12IFG2: break;
	case ADC12IFG3: break;
	case ADC12IFG4: break;
	case ADC12IFG5: break;
	case ADC12IFG6: break;
	case ADC12IFG7: break;
	case ADC12IFG8: break;
	case ADC12IFG9: break;
	default:
		break;
	}

  //wake up processor to handle flag
  __low_power_mode_off_on_exit();

}










#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT4_VECTOR
__interrupt void PORT4_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT4_VECTOR))) PORT4_ISR (void)
#else
#error Compiler not supported!
#endif
{


    switch (__even_in_range(P4IV, P4IV_P4IFG7)) {
    case P4IV_P4IFG0: {							// P4.0 Interrupt
			__low_power_mode_off_on_exit();
			}
    			break;
    case P4IV_P4IFG1: break;                    // P4.1 Interrupt
    case P4IV_P4IFG2: break;                    // P4.2 Interrupt
    case P4IV_P4IFG3: {						   // P4.3 Interrupt

    			__low_power_mode_off_on_exit();

    		}
    		break;
    case P4IV_P4IFG4: break;                    // P4.4 Interrupt
    case P4IV_P4IFG5: {
    		__low_power_mode_off_on_exit();
    		break;					   // P4.5 Interrupt
    }
    case P4IV_P4IFG6: break;                    // P4.6 Interrupt
    case P4IV_P4IFG7: break;                    // P4.7 Interrupt
    default: break;
    }

}



#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCI_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{


	switch(__even_in_range(UCB0IV,4))
	  {
	//	000h = No interrupt pending
	//	002h =

	  case 0:	break;                          // Vector 0 - no interrupt pending

	  case 2:									// 002h = Interrupt Source: Data received; Interrupt Flag: UCRXIFG; Interrupt


		  //wake up processor to handle flag
		    __low_power_mode_off_on_exit();
		  break;

	  case 4:                           		//	004h = Interrupt Source: Transmit buffer empty; Interrupt Flag: UCTXIFG;

	  	break;
	  default: break;
	  }

}





/**************************************************************begin interrupts copy*************
 */


/*..........................................................................*/
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void timerA_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) timerA_ISR (void)
#else
#error Compiler not supported!
#endif
{

}

/*..........................................................................*/
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=UNMI_VECTOR
__interrupt void unmi_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(UNMI_VECTOR))) unmi_ISR (void)
#else
#error Compiler not supported!
#endif
{
  WDTCTL = (WDTPW | WDTHOLD); /* Stop WDT */
}


// trap isr assignation - put all unused ISR vector here
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = PORT2_VECTOR
__interrupt void    PORT2_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) PORT2_ISR (void)
#else
#error Compiler not supported!
#endif
{

  _bic_SR_register_on_exit(LPM3_bits);

    switch (__even_in_range(P2IV, P2IV_P2IFG7)) {
    case P2IV_P2IFG0: break;                    // P2.0 Interrupt
    case P2IV_P2IFG1: break;                          // Option Select Interrupt
    case P2IV_P2IFG2: break;                    // P2.2 Interrupt
    case P2IV_P2IFG3: break;                    // P2.3 Interrupt
    case P2IV_P2IFG4: break;                    // P2.4 Interrupt
    case P2IV_P2IFG5: break;                    // P2.5 Interrupt
    case P2IV_P2IFG6: break;                    // P2.6 Interrupt
    case P2IV_P2IFG7: break;                    // P2.7 Interrupt
    default: break;
    }

}

// trap isr assignation - put all unused ISR vector here
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A1_VECTOR
__interrupt void    TIMER0_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) TIMER0_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
	//update the realtime clock
	//http://www.ti.com/lit/an/slaa290a/slaa290a.pdf
	//incrementSeconds();
}



// trap isr assignation - put all unused ISR vector here
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = WDT_VECTOR
__interrupt void    WDT_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) WDT_ISR (void)
#else
#error Compiler not supported!
#endif
{

}

// trap isr assignation - put all unused ISR vector here
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = DMA_VECTOR
__interrupt void    DMA_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(DMA_VECTOR))) DMA_ISR (void)
#else
#error Compiler not supported!
#endif
{

}




#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = SYSNMI_VECTOR
__interrupt void   SYSNMI_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(SYSNMI_VECTOR))) SYSNMI_ISR (void)
#else
#error Compiler not supported!
#endif
{
	SFRIFG1 = 0;
}

// trap isr assignation - put all unused ISR vector here

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_B0_VECTOR
__interrupt void  TIMER0_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B0_VECTOR)))TIMER0_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{

}

// trap isr assignation - put all unused ISR vector here

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_B1_VECTOR
__interrupt void  TIMER0_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B1_VECTOR))) TIMER0_B1_ISR (void)
#else
#error Compiler not supported!
#endif
{

}




// trap isr assignation - put all unused ISR vector here
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = AES256_VECTOR
__interrupt void AES256_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(AES256_VECTOR))) AES256_ISR (void)
#else
#error Compiler not supported!
#endif
{

}




// trap isr assignation - put all unused ISR vector here

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER2_A0_VECTOR
__interrupt void TIMER2_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER2_A0_VECTOR))) TIMER2_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{

}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = COMP_E_VECTOR
__interrupt void COMP_E_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(COMP_E_VECTOR))) COMP_E_ISR (void)
#else
#error Compiler not supported!
#endif
{

}




#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) PORT1_ISR (void)
#else
#error Compiler not supported!
#endif
{

    switch (__even_in_range(P1IV, P1IV_P1IFG7)) {
    case P1IV_P1IFG0: break;                    // P1.0 Interrupt
    case P1IV_P1IFG1: break;                          // Option Select Interrupt
    case P1IV_P1IFG2: break;                    // P1.2 Interrupt
    case P1IV_P1IFG3: break;                    // P1.3 Interrupt
    case P1IV_P1IFG4: break;                    // P1.4 Interrupt
    case P1IV_P1IFG5: break;                    // P1.5 Interrupt
    case P1IV_P1IFG6: break;                    // P1.6 Interrupt
    case P1IV_P1IFG7: break;                    // P1.7 Interrupt
    default: break;
    }
}







//ADDED BY GUNNAR 2/14/2015
//used to debounce button1
//ISR initialized in timer_A2.c
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER2_A1_VECTOR
__interrupt void TimerA2_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER2_A1_VECTOR))) TimerA2_ISR (void)
#else
#error Compiler not supported!
#endif
{

}//end interrupt

#endif
