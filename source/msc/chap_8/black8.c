
// BLACK8.C - Library Module

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

// G L O B A L S /////////////////////////////////////////////////////////////

long starting_time,  // these are used to compute the length of some event
     ending_time;

// F U N C T I O N S//////////////////////////////////////////////////////////

long Timer_Query(void)
{
// this function is used to record the current time

long far *clock = (long far *)0x0000046CL; // address of timer

return(*clock);

} // end Timer_Query

//////////////////////////////////////////////////////////////////////////////

void Timer_Program(int timer,unsigned int rate)
{

// this function re-programs the internal timer

// first program the timer to mode 2 - binary and data loading sequence of
// low byte then high byte

_outp(TIMER_CONTROL, TIMER_SET_BITS);

// write least significant byte of the new rate to the counter register

_outp(timer,LOW_BYTE(rate));

// and now the the most significant byte

_outp(timer,HI_BYTE(rate));

} // end Timer_Program

///////////////////////////////////////////////////////////////////////////////

