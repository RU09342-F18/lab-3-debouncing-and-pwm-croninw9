# Software Debouncing
The following code creates a delay in the button presses. When a button is pressed, it is not a perfect on/off but oscillates for a brief moment in time. Without debouncing, there's a possibility that this ocsillation can cause a change in state depending on the program running. The debounce code creates a delay that lets the oscillation pass before making the button active again.

## How it works
The code runs by initiallizing the inputs and outputs, setting the clock to ACLK in stop configuration. The code then uses integers to check if the button is pressed. If the button is pressed, the LED flips and the timer starts in up-configuration. When the timer hits CCR0, the timer stops and the program waits for the button to be pushed again.

## MSP430G2553 Pinouts
*Pin 1.0 - Output (LED)
*Pin 1.3 - Input (Button)

## MSP430F5529 Pinouts
*Pin 1.0 - Output (LED)
*Pin 1.1 - Input (Button)
