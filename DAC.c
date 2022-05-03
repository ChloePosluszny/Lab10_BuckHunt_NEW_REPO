// DAC.c
// This software configures DAC output
// Lab 6 requires 6 bits for the DAC
// Runs on LM4F120 or TM4C123
// Program written by: Chloe Posluszny & Erik Halldorson
// Date Created: 3/6/17 
// Last Modified: 1/11/22 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data



// **************DAC_Init*********************
// Initialize 6-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
  SYSCTL_RCGCGPIO_R |= 0x02; //enable clock for port B
	volatile uint8_t  Z; //waste 2 instruction cycles for clock stability
	Z=1; //waste 2 instruction cycles for the clock
	
	GPIO_PORTB_DR8R_R |= 0x3F; //allow TM4C to give more current: 8mA //for pins PB5-PB0
	
	GPIO_PORTB_DEN_R |= 0x3F; //Enable pins PB5-PB0
	GPIO_PORTB_DIR_R |= 0x3F; //Set pins PB5-PB0 as output (1 = out;0 = in)
}

// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 63 
// Input=n is converted to n*3.3V/63
// Output: none
void DAC_Out(uint32_t data){
	GPIO_PORTB_DATA_R = data	; //	 6 bit data to n*3.3/63
}
