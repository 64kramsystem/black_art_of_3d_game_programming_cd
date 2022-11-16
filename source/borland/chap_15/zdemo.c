
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
#include "black15.h"

///////////////////////////////////////////////////////////////////////////////

void main(void)
{

int x=160,  // local position of object
    y=100,
    z=100;

// set graphics to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// point double buffer at video buffer, so we can see output without an
// animation loop

double_buffer = video_buffer;

// create a 200 line z buffer (128k)

Create_Z_Buffer(200);

// initialize the z buffer with a impossibly distant value

Fill_Z_Buffer(16000);

// draw two interesecting triangles

Draw_Tri_3D_Z(x,y,z,
              x-30,y+40,z,
              x+40,y+50,z,
              10);


Draw_Tri_3D_Z(x+10,y+10,z-5,
              x-50,y+60,z+5,
              x+30,y+20,z-2,
              1);

// wait for keyboard hit

while(!kbhit());

// restore graphics mode back to text

Set_Graphics_Mode(TEXT_MODE);

// release the z buffer memory

Delete_Z_Buffer();

} // end main

