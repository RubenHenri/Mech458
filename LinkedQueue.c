/* Solution Set for the LinkedQueue.c */
/* 	
	Course		: UVic Mechatronics 458
	Milestone	: 3
	Title		: Data structures for MCUs and the Linked Queue Library

	Name 1:	Dmitri Karaman				Student ID:V00853615
	Name 2:	Ruben Henri					Student ID:V00988496
	
	Description: This will implement a FIFO linked list, and take in switch values when a button is pressed.
	These values are stored in the linked list and after the first is discarded, they will be displayed on the PORTC leds
*/

/* include libraries */
#include <stdlib.h>
#include <avr/io.h>
#include "LinkedQueue.h" 	/* This is the attached header file, which cleans things up */
							/* Make sure you read it!!! */
/* global variables */
/* Avoid using these */

int main(){	
	
	CLKPR = 0x00;		/* Required for timer functionality */
	CLKPR = 0x01;
	TCCR1B |=_BV(CS11);

	link *head;			/* The ptr to the head of the queue */
	link *tail;			/* The ptr to the tail of the queue */
	link *newLink;		/* A ptr to a link aggregate data type (struct) */
	link *rtnLink;		/* same as the above */
	element eTest;		/* A variable to hold the aggregate data type known as element */
	char readInput;

	DDRC = 0xFF; 		/* Used for debugging purposes only LEDs on PORTC */
	DDRA = 0x00;		/* Set PORT A to input */
	//DDRD = 0xFF;
	
	rtnLink = NULL;
	newLink = NULL;

	setup(&head, &tail);
	
	int button = 0;
	uint8_t display = 0b00000000;

	// Reads from the PINA input			
	while(1){
		readInput = PINA;
		button = buttonPress(readInput);
		
		
		// Inserts a new element to the queue if the button is being pressed
		if(button < 5){		
			//Creates a new link and adds to the queue
			initLink(&newLink);
			newLink->e.value = button;
			enqueue(&head, &tail, &newLink);
			PORTC = head->e.value;
			//After reading a value, display it and wait 1 second before another value can be read
			mTimer(1000);

			//If the queue is the required size, display the values with the required delays
			if(size(&head, &tail) => 4){
				
				//Remove the first item and do not use it
				dequeue(&head, &rtnLink);
				
				//Assign the first value to the display
				display = head->e.value;
				PORTC = display;
				mTimer(2000);
				dequeue(&head, &rtnLink);
				
				//Takes the new head and adds the value of it to the bit-shifted previous value
				display << 2;
				display = display + head->e.value
				PORTC = display;
				mTimer(2000);
				dequeue(&head, &rtnLink);
				
				//This is the final display value, so the queue should be empty
				display << 2;
				display = display + head->e.value
				PORTC = display;
				mTimer(2000);
				dequeue(&head, &rtnLink);							
				
			}						
		}
	}

	return(0);
}/* main */


/**************************************************************************************/
/***************************** SUBROUTINES ********************************************/
/**************************************************************************************/

//There are 4 options for the rocker switch positions while the button is pushed, if you have anything else pass the value 5
int buttonPress(char input){
	switch(input){
		case(0x01):
			PORTC = 0b00000001;
			return 1;
		case(0x02):
			PORTC = 0b00000010;
			return 2;
		case(0x03):
			PORTC = 0b00000011;
			return 3;
		case(0x04):
			PORTC = 0b00000100;
			return 5;
		case(0x05):
			PORTC = 0b00000101;
			return 5;
		case(0x06):
			PORTC = 0b00000110;
			return 5;
		case(0x07):
			PORTC = 0b00000111;
			return 5;
		default:
			PORTC = 0b00000000;
			return 0;
	}
}


//Hardware based timer
void mTimer(int count){
	int i ;
	i = 0;
	
	TCCR1B |= _BV(WGM12);
	OCR1A = 0x03E8;
	TCNT1 = 0x0000;
	TIMSK1 = TIMSK1 | 0b00000010;
	TIFR1 |= _BV(OCF1A);
	
	while(i<count){
		if((TIFR1 & 0x02) == 0x02){
			TIFR1 |= _BV(OCF1A);
			i++;
		}
	}
	
	return;
	
}


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

