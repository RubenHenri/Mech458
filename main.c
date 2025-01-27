/* ##################################################################
# MILESTONE: 2
# PROGRAM: 2
# PROJECT: Lab2 Demo
# GROUP: 10
# NAME 1: Dmitri Karaman, Student ID
# NAME 2: Ruben Henri, V00988496
# DESC: This program does a knight rider effect using bit shifting and the built in timer to delays
# DATA
# REVISED ############################################################### */

#include <stdlib.h>			// the header of the general-purpose standard library of C
#include <avr/io.h>			// the header of I/O port
#include <avr/interrupt.h>	// Needed for interrupt functionality 
#include <util/delay_basic.h>
void mTimer(int count);

/* ################## MAIN ROUTINE ################## */
int main(int argc, char *argv[]){
	DDRL = 0xFF;; // Sets all pins on PORTL to output
	DDRC = 0xFF; // Sets all pins on PORTC to output
	PORTL = 0xC0; // initialize pins to high to turn on LEDs (1 Yel & 1 Grn)
	uint8_t d = 200;
	uint8_t dir = 1;	// direction 1 = shifting right, 0 means shifting left
	uint8_t pattern = 0xC0; // start with two leftmost bits on
	
	CLKPR = 0x80;		//
	CLKPR = 0x01;		// Sets CPU clk to 8MHz
	
	//TCCR1B |= _BV(CS11);	// BV sets the bit to logic 1
						// Register is TCCR1B1
						// RCCR1 is the timer/counter control register 1
						// B is the 'B' register and 1 is bit 1
						// CS means clock select and has a pre-scaler set to 8
	 	
	while(1){
		
		PORTC = pattern; // initialize LED 7 and 6 as on (leftmost two LEDS)
		mTimer(d);		// wait for d ms
		
		if(dir){
			pattern >>= 1;	// bitshift pattern right
			if(pattern == 0x03) {	// 0x03 = 0b00000011
				dir = 0;			// time to go left
			}
		} else {
			pattern <<= 1;	// bitshift pattern left
			if(pattern == 0xC0){	//0xC0 = 0b11000000
				dir = 1;
			}
		}
	
//Knight Rider with for loops and bitshifting		
// 		PORTC = 0xC0; // initialize LED 7 and 6 as on (leftmost two LEDS)
// 		mTimer(d);		// wait for d ms		
// 		for (int j = 0; j < 2; j++){
// 			PORTC = (PORTC >> 1) | PORTC;	
// 			mTimer(d);		// wait for d ms	
// 		}
// 		for (int k = 0; k < 6; k++){
// 			PORTC = (PORTC >> 1);
// 			mTimer(d);		// wait for d ms
// 		}
// 		for (int j = 0; j < 2; j++){
// 			PORTC = (PORTC << 1) | PORTC;
// 			mTimer(d);		// wait for d ms
// 		}
// 		for (int k = 0; k < 6; k++){
// 			PORTC = (PORTC << 1);
// 			mTimer(d);		// wait for d ms
// 		}		

// Knight Rider the long way
// 		PORTC = 0b11100000;
// 		delaynms(d);
// 		PORTC = 0b11110000;
// 		delaynms(d);
// 		PORTC = 0b01111000;
// 		delaynms(d);
// 		PORTC = 0b00111100;
// 		delaynms(d);
// 		PORTC = 0b00011110;
// 		delaynms(d);
// 		PORTC = 0b00001111;
// 		delaynms(d);
// 		PORTC = 0b00000111;
// 		delaynms(d);
// 		PORTC = 0b00000011;
// 		delaynms(d);
// 		PORTC = 0b00000111;
// 		delaynms(d);
// 		PORTC = 0b00001111;
// 		delaynms(d);
// 		PORTC = 0b00011110;
// 		delaynms(d);
// 		PORTC = 0b00111100;
// 		delaynms(d);
// 		PORTC = 0b01111000;
// 		delaynms(d);
// 		PORTC = 0b11110000;
// 		delaynms(d);
// 		PORTC = 0b11100000;
// 		delaynms(d);
	
	}
	return (0);
}

void delaynus(int n) // delay microsecond
{
	int k;
	for(k=0;k<n;k++)
	_delay_loop_1(1);
}
void delaynms(int n) // delay millisecond
{
	int k;
	for(k=0;k<n;k++)
	delaynus(1000);
}

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
