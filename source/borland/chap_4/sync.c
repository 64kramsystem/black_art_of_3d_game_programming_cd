
// SYNC.C - A demo of the vertical retrace

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

#include "black3.h"
#include "black4.h"

// M A I N ///////////////////////////////////////////////////////////////////

void main(void)
{
int x=0,y=100;  // position of the dot

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// loop until keyboard is hit

while(!kbhit())
     {

     // wait for retrace period

     Wait_For_Vertical_Retrace();

     // at this point the electron gun is retracing, so updates to the
     // video buffer won't be visible

     // update our little dot

     Write_Pixel(x,y,0);

     // move the dot

     if (++x>319) // the dot will move at approx. 70 pixels/second
        x=0;

     Write_Pixel(x,y,15);

     } // end while

// reset to text mode

Set_Graphics_Mode(TEXT_MODE);

} // end main

