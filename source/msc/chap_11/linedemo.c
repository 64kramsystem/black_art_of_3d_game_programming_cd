
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
#include <search.h>             // this one is needed for qsort()

// include all of our stuff

#include "black3.h"
#include "black4.h"
#include "black5.h"
#include "black6.h"
#include "black8.h"
#include "black9.h"
#include "black11.h"

// M A I N ////////////////////////////////////////////////////////////////////

void main(void)
{

// set graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// draw randomly positioned lines until user hits a key

while(!kbhit())
     {
     // draw the line with random (xo,yo) - (x1,y1) with a random color
     Draw_Line(rand()%320,rand()%200,
               rand()%320,rand()%200,
               rand()%256,video_buffer);

     } // end while

// restore graphics mode back to text

Set_Graphics_Mode(TEXT_MODE);

} // end main

