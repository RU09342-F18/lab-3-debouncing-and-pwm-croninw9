#include <msp430.h> 


/**
 * File: main.c
 * Author: William Cronin
 * Date Created: October 4th 2018
 * Date of Last Revision: October 5th 2018
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P1DIR |= BIT0;          //Set Pin 1.0 to output
	P1DIR &= ~BIT1;         //Set Pin 1.1 to input
	P1REN |= BIT1;          //Enable pull-up resistor
	P1OUT |= BIT1;

	P1IE |= BIT1;       //Enable interrupt on Pin 1.1
	P1IES |= BIT1;
	P1IFG &= ~BIT1;     //Clears Flag

	__bis_SR_register(LPM0_bits + GIE); //Low power and enable global interrupt
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1IE &= ~BIT1;              //Disable Interrupt
    WDTCTL = WDT_MDLY_32;       //Watchdog 32ms delay
    SFRIE1 |= WDTIE;            //enable WDT interrupt
    P1IFG &= ~BIT1;             //clear flag
}

#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
    SFRIE1 &= ~WDTIE;           //Disable Watchdog
    SFRIFG1 &= ~WDTIFG;           //clear flag
    WDTCTL = WDTPW | WDTHOLD;       //Stop watchdog timer

    if(!(P1IN & BIT1))
    {
        P1OUT ^= BIT0;          //Flip LED
    }
    P1IE |= BIT1;           //enable button interrupt

}
