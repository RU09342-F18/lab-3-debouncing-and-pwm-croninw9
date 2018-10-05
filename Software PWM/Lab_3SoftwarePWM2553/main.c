#include <msp430.h> 


/**
 * File Name: main.c
 * Author: Will Cronin
 * Date Created: October 3rd 2018
 * Date of Last Revision: October 4th 2018
 */
int buttonpressed = 0;
int count = 0;
int x = 500;

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;       //stop watchdog timer

	LEDSetup();
	buttonSetup();
	timerSetup();
	
	_BIS_SR(GIE);   //global interrupts enabled
	while(1)
	{

	}
}

void LEDSetup(void)
{
    P1SEL |= 0;             //Sets P1 to GPIO
    P1DIR |= BIT0;          //Sets Pin 1.0 as an output
    P1DIR |= BIT6;          //Sets Pin 1.6 as an output
    P1OUT &= ~BIT6;         //Pin 1.6 LED set on
}

void buttonSetup(void)
{
    P1DIR &= ~BIT3;         //Sets Pin 1.3 as an input
    P1REN |= BIT3;          //Enables a pull-up resistor
    P1OUT |= BIT3;          //Sets Pin 1.3 to pull up
    P1IE = BIT3;            //enable interrupt on Pin 1.3
    P1IES &= ~BIT3;         //Interrupt flag on negative
    P1IFG &= ~BIT3;        //clear interrupt flag
}

void timerSetup(void)
{
    TA0CTL |= TAIE;         //Enables timer interrupt
    TA0CTL = MC_1 + TASSEL_2;        //Sets timer to up-mode on SMCLK
    TA0CCR0 = 999;          //Sets Timer to 1 KHz
    TA0CCTL0 |= CCIE;        //Enables interrupt

    TA0CCR1 = 499;          //Starts with 50% duty cycle
    TA0CCTL1 |= CCIE;        //Enables Interrupt

    TA0CCTL0 &= ~TAIFG;        //Clears flag
    TA0CCTL1 &= ~TAIFG;        //Clears flag

    TA1CCR0 = 4000;
    TA1CCTL0 |= CCIE;
    TA1CTL |= TASSEL_2 + MC_1;
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void timer_debounce(void)
{
    if (buttonpressed == 1)
    {
        if (count == 100)
        {
            buttonpressed = 0;
            count = 0;
        }

        else
        {
            count++;
        }
    }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
    if (x != 1)
    {
        P1OUT |= BIT0; // Sets LED on
    }
    TA0CCTL0 &= ~BIT0;
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1 (void)
{
    switch(TA0IV)
    {
        case 2: //CCR1
        P1OUT &= ~BIT0;     //LED off
            break;
    }
}

#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1OUT ^= BIT6;
    if (!(P1IN & BIT3))
    {
        if (buttonpressed == 0)
        {
            buttonpressed = 1;
            if (x >= 1000)
            {
                x = 1;
            }

            else
            {
                x += 100;
            }
            TA0CCR1 = x - 1;
        }
    }
    P1IES ^= BIT3;
    P1IFG &= ~BIT3;         //Clears flag
}

