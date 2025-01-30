
/* ##################################################################
# MILESTONE: 3
# PROGRAM: 3
# PROJECT: Lab3 Demo
# GROUP: 10
# NAME 1: Dmitri Karaman, Student ID
# NAME 2: Ruben Henri, V00988496
# DESC: This program reads input from two leftmost switch positions and the left momentary switch. It displays the values on the yellow and green LEDS as well.
# DATA
# REVISED ############################################################### */

#include <stdlib.h>			// the header of the general-purpose standard library of C
#include <avr/io.h>			// the header of I/O port
#include <avr/interrupt.h>	// Needed for interrupt functionality
#include <util/delay_basic.h>

void mTimer(int count);
int debug(char input);

/* ################## MAIN ROUTINE ################## */
int main(int argc, char *argv[]){
	
	char readInput;
	DDRL = 0xF0;			// set PORT L pin 7:5 as output
	DDRA = 0x00;			// Set all pins on PORTA to input
	DDRC = 0xFF;			// Set all pins on PORTC to output
	uint8_t d = 200;		// variable for timer delay in ms
	uint8_t pattern;		// Variable for PORTC bit pattern
	
	CLKPR = 0x80;		//
	CLKPR = 0x01;		// Sets CPU clk to 8MHz
	
	while(1){
		readInput = PINA;		// Read value from register PINA (Not PORTA)
// 		PORTC = pattern;		//
// 		mTimer(d);		// wait for d ms
		PORTL = (readInput << 4);
		debug(readInput);
		
	}
	return (0);
}

int debug(char input){
	switch (input){
		case (0x01):
			PORTC = 0b00000001;
			break;
		case (0x02):
			PORTC = 0b00000010;
			break;
		case (0x04):
			PORTC = 0b00000100;
			break;
		case (0x08):
			PORTC = 0b00001000;
			break;
		default:
			PORTC = 0b00000000;
			break;				
	}	// end switch
	return(input);
}	// end debug

void mTimer(int count){
	int i = 0; // init loop counter, set to zero
	
	// set the waveform generation mode bit description to Clear Timer on Compare Math Mode (CTC) only
	//TCCR1B |= _BV(WGM12);		// set WGM bits to 0100, see page 145 NOTE WGM bit is spread over two registers
	
	// Put TCCR1B into "CTC mode" + "clk/8 prescaler"
	TCCR1B = (1 << WGM12) | (1 << CS11);
	
	OCR1A = 0x03E8;		// Set output compare register for 1000 cycles = 1ms
	
	TCNT1 = 0x0000;		// sets initial value of timer counter to 0x0000
	
	TIMSK1 = TIMSK1 | 0b00000010;	// Enable output compare interrupt enable
	
	TIFR1 |= _BV(OCF1A);		// Clear the timer interrupt flag and start new timing
	
	// Poll the timer to determine when the timer has reached 0x03E8
	
	while(i<count){
		if((TIFR1 & 0x02) == 0x02){
			
			TIFR1 |= _BV(OCF1A);	// clear interrupt flag by writing a ONE to the bit
			
			i++;
			
		} // end if
	} // end while
	return;
}
