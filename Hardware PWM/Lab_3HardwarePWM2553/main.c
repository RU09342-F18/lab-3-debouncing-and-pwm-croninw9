#include <msp430.h> 


/**
 * File Name: main.c
 * Author: Will Cronin
 * Date Created: October 3rd 2018
 * Date of Last Revision: October 5th 2018
 */
int buttonpressed = 0;
int count = 0;
int x = 500;
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	LEDSetup();
    buttonSetup();
    timerSetup();

    __bis_SR_register(GIE + LPM0_bits);     //Enables global interrupt and low power mode
}

void LEDSetup(void)
{
    P1DIR |= (BIT0 + BIT6);     //Sets Pins 1.0 and 1.6 as outputs
    P1SEL |= BIT6;              //

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
    TA0CCR0 = 999;          //PWM Period
    TA0CCR1 = 499;          //Duty Cycle (starts @ 50%)
    TA0CCTL1 = OUTMOD_7;    //reset/set
    TA0CTL = TASSEL_2 + MC_1;  //SMCLK in up-mode
}

#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(TA0CCR1 >= 1000)
    {
        TA0CCR1 = 0;    //reset to zero
    }
    else
    {
        TA0CCR1 += 100; //increment by 100Hz
    }
    P1IFG &=~BIT3; //reset flag
}
