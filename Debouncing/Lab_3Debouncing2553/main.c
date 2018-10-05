#include <msp430.h> 


/**
 * File: main.c
 * Author: William Cronin
 * Date Created: October 1st 2018
 * Date of Last Revision: October 3rd 2018
 */

int timerdelay;

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P1SEL = 0;                  //Sets Pin 1 as a GPIO

	P1DIR &= ~BIT3;             //Sets P1.3 as an input
	P1IN &= ~BIT3;
	P1REN |= BIT3;              //Sets a pull-up resistor on P1.3

	P1DIR |= BIT0;              //Sets P1.0 as an output
	P1OUT &= ~BIT0;             //Sets the LED on P1.0 to off

	P1IE |= BIT3;               //Enables Interrupt on P1.3
	P1IES |= BIT3;              //
	P1IFG &= ~BIT3;             //Reset flag

	_BIS_SR(GIE);               //Enable global interrupt

	TA0CTL = TASSEL_1 + MC_0 + TAIE;        //Sets Timer in AClock configured in stop mode with interrupt enabled
	TA0CCR0 = 40000;

	timerdelay = 1;
	while(1)
	{

	}
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A(void)
{
    switch(TA0IV)
    {
    case 2:
        break;
    case 4:
        break;
    case 10:
        timerdelay = 1;
        TA0CTL = TACLR;               //Clear timer
        TA0CTL = TASSEL_1 + MC_0;     //Sets Timer in AClock configured in stop mode with interrupt enabled
        break;
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port1(void)
{
    if(timerdelay)
    {
        P1OUT ^= BIT0;          //Flip LED
        TA0CTL = TACLR;         //Clear timer
        TA0CTL = TASSEL_1 + MC_1 + TAIE;    //Sets Timer in AClock configured in up mode with interrupt enabled
        timerdelay = 0;
    }
    P1IFG &= ~BIT3;             //Clear interrupt flag on Pin 1.3
}
