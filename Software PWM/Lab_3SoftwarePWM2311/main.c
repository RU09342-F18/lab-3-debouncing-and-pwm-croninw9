#include <msp430.h> 


/**
 * File: main.c
 * Author: William Cronin
 * Date Created: October 5th 2018
 * Date of Last Revision: October 5th 2018
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;       //power to channel

	LEDSetup();
	buttonSetup();
	timerSetup();

	__bis_SR_register(GIE); //Global interrupt enable
	while(1)
	{

	}
}

void LEDSetup(void)
{
    P1DIR |= BIT0;          //Pin 1.0 set to output
    P2DIR |= BIT0;          //Pin 2.0 set to output
    P1OUT |= BIT0;          //Set LED on
    P2OUT |= BIT0;          //Set LED on
}
void buttonSetup(void)
{
    P1DIR &= ~BIT1;         //Set Pin 1.1 to input
    P1IE |=  BIT1;          // P1.1 interrupt enabled
    P1REN |= BIT1;          // Enable pull-up resistor
    P1IES |= BIT1;          //falling edge
    P1OUT |= BIT1;          //Pull up resistor on Pin 1.1
    P1IFG &= ~BIT1;         // P1.1 Interrupt Flag cleared
}
void timerSetup(void)
{
    TB0CTL |= TBIE;         //Enable timer interrupt
    TB0CCTL0 |= CCIE;       //Enable CCR0 interrupt
    TB0CCTL0 &= ~TBIFG;     //clear flag
    TB0CCTL1 |= CCIE;       //enable CCR1 interrupt
    TB0CCTL1 &= ~TBIFG;     //clear flag

    TB0CTL = TBSSEL_2 + MC_1;    //SMCLK in up-mode configuration
    TB0CCR0 = 1000;         //1 KHz signal
    TB0CCR1 = 499;          //50% duty cycle
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer0_B0(void)
{
    P1OUT |= BIT0;          //Turn on LED 1.0
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void Timer0_B1(void)
{
    switch(TB0IV)
    {
    case 2: //CCR1
        P1OUT &= ~BIT0; //Turn LED 1.0 off
    }
}

#pragma vector_PORT1_VECTOR
__interrupt void Port_1(void)
{
    P2OUT ^= BIT0;          //Flip Led 2.0
    if(TB0CCR1 >= 1000)
    {
        TB0CCR1 = 0; //reset to zero
    }
    else
    {
        TB0CCR1 += 100;     //Increase by 100 Hz
    }
    P1IFG &= ~BIT1;         //clear flag
}
