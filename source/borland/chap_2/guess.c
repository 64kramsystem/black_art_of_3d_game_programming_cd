

// GUESS.C - An example of input driven event loops

// I N C L U D E S ////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// M A I N ////////////////////////////////////////////////////////////////////

void main(void)
{

int done=0,        // exit flag
	 number,        // the random nunber
	 num_tries=0,   // number of tries
	 guess;         // the players guess

unsigned int far *clock = (unsigned int far *)0x0000046CL; // pointer to clock

// SECTION 1 //////////////////////////////////////////////////////////////////

// print out introductory instructions

printf("\nI'm thinking of a number from 1-100.");
printf("\nTry and guess it!\n");

// seed the random number generator with the time

srand(*clock);

// choose a random number from 1-100

number = 1 + rand() % 100;

// SECTION 2 //////////////////////////////////////////////////////////////////

// main event loop

while(!done)
	  {

// SECTION 3 //////////////////////////////////////////////////////////////////

	  // query user for input (the event)

	  printf("\nWhat's your guess?");
	  scanf("%d",&guess);

// SECTION 4 //////////////////////////////////////////////////////////////////

	  // increment number of tries

	  num_tries++;

	  // process the event

	  if (guess > number)
		  printf("\nToo big!\n");
	  else
	  if (guess < number)
		  printf("\nToo small!\n");
	  else
		  {
		  // the user must have guessed the number

		  printf("\nYou guessed the number in %d tries!!!\n",num_tries);

		  // set the exit flag

		  done=1;

		  } // end else

	  } // end while

} // end main

