# Hardware PWM
The following code creates a software version of a PWM. A PWM stands for a pulse with modulation and is a modulation technique used to encode a message into a pulsed signal.


## How it works
The code runs by initiallizing the inputs and outputs, setting the clock to SMCLK in up configuration. The CCR0 is set to 999 and CCR1 is set to 499. This creates a 50% duty cycle. The timer setup method also enables outmod_7 which enables reset/set. The only interrupt in the code is the button interrupt. The button interrupt  increaes the CCR1 value by 100, which increases the duty cycle by 10%. If the CCR1 is at 1000, it is set back to zero. 

## MSP430G2553 Pinouts
* Pin 1.0 - Output (LED)
* Pin 1.3 - Input (Button)
* Pin 1.6 - Output (LED)

## MSP430FR2311 Pinouts
* Pin 1.0 - Output (LED)
* Pin 1.1 - Input (Button)
* Pin 2.0 - Output (LED)

