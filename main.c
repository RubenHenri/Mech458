
/* ##################################################################
# MILESTONE: 3
# PROGRAM: 3
# PROJECT: Lab3 Demo
# GROUP: 10
# NAME 1: Dmitri Karaman, V00853615
# NAME 2: Ruben Henri, V00988496
# DESC: This program 
# DATA
# REVISED ############################################################### */

#include <stdlib.h>			// the header of the general-purpose standard library of C
#include <avr/io.h>			// the header of I/O port
#include <avr/interrupt.h>	// Needed for interrupt functionality
#include <util/delay_basic.h>
#include "LinkedQueue.h"		

void mTimer(int count);
int debug(char input);

/* ################## MAIN ROUTINE ################## */
int main(int argc, char *argv[]){
	
	CLKPR = 0x80;			//
	CLKPR = 0x01;			// Sets CPU clk to 8MHz
	
	char readInput;			// variable for reading value from PORTA

	DDRL = 0xF0;			// set PORT L pin 7:5 as output
	DDRA = 0x00;			// Set all pins on PORTA to input
	DDRC = 0xFF;			// Set all pins on PORTC to output
	
	PORTL = 0x00;			// Set PORTL to zero (all LEDS off)
	
	link *head;				// The ptr to the head of the queue 
	link *tail;				// The ptr to the tail of the queue 
	link *newLink;			// A ptr to a link aggregate data type (struct) 
	link *rtnLink;			// same as the above 
	element eTest;			// A variable to hold the aggregate data type known as element 
	
	head = NULL;			// Initialize pointers to NULL
	tail = NULL;
	rtnLink = NULL;
	newLink = NULL;
	
	clearQueue(&head, &tail);	// Clear any items in queue
	
	while(1){
		setup(&head, &tail);				// Initialize linked queue
		
		while((size(&head, &tail)) < 4){
			if (isEmpty(&head) == 1){		// this tells us if the queue is empty
				PORTL = PORTL | 0x80;		// if so, turn on leftmost yellow LED
				}	// end for
			} // end while
		while ((PINA & 0x04) != 0x00);		// wait for Left button (PA2 active low) to be pressed
		mTimer(20);							// de bounce on press (20ms)
		readInput = (PINA & 0x03);			// Read bit 0 and bit 1 from register PINA (Not PORTA)
 		PORTL = (readInput << 4);			// display value of input on PORTL LEDS (for debugging, optional)
 		initLink(&newLink);					// init a new link 
		newLink->e.itemCode = readInput;	// store 2 bit value in new link
		enqueue(&head, &tail, &newLink);	// send the new link to the queue
		while((PINA & 0x04) == 0x00);		// wait for button release 
		mTimer(20);							// de bounce on release
		
		dequeue(&head, &rtnLink);			// remove the item at the head of the list
		free(rtnLink);						// free the memory allocated for the item we just removed
		PORTL = 0x40;											// turn on the leftmost green LED, clear the rest of the PORTL LEDs
		
		for (int i = 0; i < 3; i++){
			dequeue(&head, &rtnLink);							// remove the item at the head of the list
			PORTC = (PORTC | ((rtnLink->e.itemCode) << i*2));	// send the value of the first item in the queue to PORTC (red LEDs)
			mTimer(2000);										// wait 2000ms = 2s
			free(rtnLink);										// 
			} // end for
		mTimer(5000);
		PORTL = 0x00;		// clear PORTL
		PORTC = 0x00;		// clear PORTC
		clearQueue(&head, &tail);	// Clear any items in queue
		
	} // end while(1)
	return (0);
}	// end main


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

/**************************************************************************************/
/***************************** SUBROUTINES ********************************************/
/**************************************************************************************/


/**************************************************************************************
* DESC: initializes the linked queue to 'NULL' status
* INPUT: the head and tail pointers by reference
*/

void setup(link **h,link **t){
	*h = NULL;		/* Point the head to NOTHING (NULL) */
	*t = NULL;		/* Point the tail to NOTHING (NULL) */
	return;
}/*setup*/




/**************************************************************************************
* DESC: This initializes a link and returns the pointer to the new link or NULL if error 
* INPUT: the head and tail pointers by reference
*/
void initLink(link **newLink){
	//link *l;
	*newLink = malloc(sizeof(link));
	(*newLink)->next = NULL;
	return;
}/*initLink*/




/****************************************************************************************
*  DESC: Accepts as input a new link by reference, and assigns the head and tail		
*  of the queue accordingly				
*  INPUT: the head and tail pointers, and a pointer to the new link that was created 
*/
/* will put an item at the tail of the queue */
void enqueue(link **h, link **t, link **nL){

	if (*t != NULL){
		/* Not an empty queue */
		(*t)->next = *nL;
		*t = *nL; //(*t)->next;
	}/*if*/
	else{
		/* It's an empty Queue */
		//(*h)->next = *nL;
		//should be this
		*h = *nL;
		*t = *nL;
	}/* else */
	return;
}/*enqueue*/




/**************************************************************************************
* DESC : Removes the link from the head of the list and assigns it to deQueuedLink
* INPUT: The head and tail pointers, and a ptr 'deQueuedLink' 
* 		 which the removed link will be assigned to
*/
/* This will remove the link and element within the link from the head of the queue */
void dequeue(link **h, link **deQueuedLink){
	/* ENTER YOUR CODE HERE */
	*deQueuedLink = *h;	// Will set to NULL if Head points to NULL
	/* Ensure it is not an empty queue */
	if (*h != NULL){
		*h = (*h)->next;
	}/*if*/
		
	return;
	}/*dequeue*/




/**************************************************************************************
* DESC: Peeks at the first element in the list
* INPUT: The head pointer
* RETURNS: The element contained within the queue
*/
/* This simply allows you to peek at the head element of the queue and returns a NULL pointer if empty */
element firstValue(link **h){
	return((*h)->e);
}/*firstValue*/





/**************************************************************************************
* DESC: deallocates (frees) all the memory consumed by the Queue
* INPUT: the pointers to the head and the tail
*/
/* This clears the queue */
void clearQueue(link **h, link **t){

	link *temp;

	while (*h != NULL){
		temp = *h;
		*h=(*h)->next;
		free(temp);
	}/*while*/
	
	/* Last but not least set the tail to NULL */
	*t = NULL;		

	return;
}/*clearQueue*/





/**************************************************************************************
* DESC: Checks to see whether the queue is empty or not
* INPUT: The head pointer
* RETURNS: 1:if the queue is empty, and 0:if the queue is NOT empty
*/
/* Check to see if the queue is empty */
char isEmpty(link **h){
	/* ENTER YOUR CODE HERE */
	return(*h == NULL);
}/*isEmpty*/





/**************************************************************************************
* DESC: Obtains the number of links in the queue
* INPUT: The head and tail pointer
* RETURNS: An integer with the number of links in the queue
*/
/* returns the size of the queue*/
int size(link **h, link **t){

	link 	*temp;			/* will store the link while traversing the queue */
	int 	numElements;

	numElements = 0;

	temp = *h;			/* point to the first item in the list */

	while(temp != NULL){
		numElements++;
		temp = temp->next;
	}/*while*/
	
	return(numElements);
}/*size*/
