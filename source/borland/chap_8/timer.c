
// TIMER.C - A demo of reprogramming the PC's internal timer

// I N C L U D E S ///////////////////////////////////////////////////////////

#include <io.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <bios.h>
#include <fcntl.h>
#include <memory.h>
#include <malloc.h>
#include <math.h>
#include <string.h>

#include "black8.h"

// M A I N ////////////////////////////////////////////////////////////////////

void main(void)
{

int done=0,     // exit flag
    selection;  // user input variable

// main event loop

while(!done)
     {

     // display menu

     printf("\nPC Timer Re-Programming Utility\n");
     printf("\n1 - Program timer to 120HZ.");
     printf("\n2 - Program timer to 100HZ.");
     printf("\n3 - Program timer to 60HZ.");
     printf("\n4 - Program timer to 50HZ.");
     printf("\n5 - Program timer to 40HZ.");
     printf("\n6 - Program timer to 30HZ.");
     printf("\n7 - Program timer to 20HZ.");
     printf("\n8 - Program timer to 18.2HZ (default).");
     printf("\n9 - Exit program.");
     printf("\n\nSelect one?");

     // get input

     scanf("%d",&selection);

     // what rate did user select?

     switch(selection)
           {
           case 1: // set timer to 120hz
                {
                Timer_Program(TIMER_COUNTER_0,TIMER_120HZ);
                } break;

           case 2: // set timer to 100hz
                {
                Timer_Program(TIMER_COUNTER_0,TIMER_100HZ);
                } break;

           case 3: // set timer to 60hz
                {
                Timer_Program(TIMER_COUNTER_0,TIMER_60HZ);
                } break;

           case 4: // set timer to 50hz
                {
                Timer_Program(TIMER_COUNTER_0,TIMER_50HZ);
                } break;

           case 5: // set timer to 40hz
                {
                Timer_Program(TIMER_COUNTER_0,TIMER_40HZ);
                } break;

           case 6: // set timer to 30hz
                {
                Timer_Program(TIMER_COUNTER_0,TIMER_30HZ);
                } break;

           case 7: // set timer to 20hz
                {
                Timer_Program(TIMER_COUNTER_0,TIMER_20HZ);
                } break;

           case 8: // set timer to 18hz
                {
                Timer_Program(TIMER_COUNTER_0,TIMER_18HZ);
                } break;

           case 9: // exit program
                {
                done=1;

                } break;

           default:
                  {
                  printf("\nInvalid Selection!\n");
                  } break;

           } // end switch

     } // end while

} // end main



