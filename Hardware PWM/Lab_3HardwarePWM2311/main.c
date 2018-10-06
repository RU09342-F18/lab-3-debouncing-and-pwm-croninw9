#include <msp430.h> 


/**
 * main.c
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	LEDSetup();
    buttonSetup();
    timerSetup();

    PM5CTL0 &= ~LOCKLPM5;
    __bis_SR_register(GIE + LPM0_bits);     //Enables global interrupt and low power mode
}

void LEDSetup(void)
{
    //P2SEL |= BIT0;
    P2DIR |= BIT0;      //Sets Pin 2 to GPIO
    P1DIR |= BIT0;      //Sets Pins 1.0 as output
    P2DIR |= BIT0;      //Sets Pin 2.0 as an output
}
void buttonSetup(void)
{
    P1REN |= BIT1;      //enable pull-up resistor
    P1OUT |= BIT1;      //Sets Pin 1.1 on
    P1DIR |= 0x01;      //
    P1IE |= BIT1;       //enable interrupt
    P1IES |= BIT1;      //negative edge
}
void timerSetup(void)
{
    TB1CCR0 = 999;      //1KHz
    TB1CCTL1 = OUTMOD_7 ;    //CCR1 reset/set
    TB1CCR1 = 499;       //50% duty cycle
    TB1CTL = TBSSEL_2 | MC_2 | TBCLR;   //SMCLK, UP, Clear timer
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
    P2OUT ^= BIT0; //toggle led
    if(TB1CCR1 >= 1000) { //check if timer has passed 1 khz
        TB1CCR1 = 0; //reset to 0
    }
    else {
        TB1CCR1 = TB1CCR1 + 100; //increment by 100 hz
    }
    P1IFG &=~BIT1; //reset
}

