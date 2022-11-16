
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

// include all of our stuff

#include "black3.h"
#include "black4.h"
#include "black5.h"
#include "black6.h"
#include "black8.h"
#include "black9.h"
#include "black11.h"

// M A I N /////////////////////////////////////////////////////////////////////

void main(void)
{

int done=0;         // exit flag

// set graphics mode to 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// point double buffer to video buffer since the triangle function
// only writes to the double buffer and we want to see it doing the
// writing

double_buffer = video_buffer;

// main loop

while(!kbhit())
     {

     // draw a triangle somewhere on the screen

     Draw_Triangle_2D(rand()%320,rand()%200,
                      rand()%320,rand()%200,
                      rand()%320,rand()%200,
                      rand()%256);

     } // end while

// restore text mode

Set_Graphics_Mode(TEXT_MODE);

} // end main

