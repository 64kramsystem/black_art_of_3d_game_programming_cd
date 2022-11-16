
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

int done=0,         // exit flag
    x=140,y=60,     // position of triangle

    intensity_1=15,
    intensity_2=50,
    intensity_3=5;

char buffer[80]; // output string buffer

// set graphics mode to 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// alter the palette and introduce some greys

Make_Grey_Palette();

// draw gouraud shaded traingle

Draw_Triangle_2D_Gouraud(x,y,x-50,y+60,x+30,y+80,video_buffer,
                        intensity_1,intensity_2,intensity_3);

// label vertices

Print_String(x,y-10,9,"1",1);
Print_String(x-60,y+60,9,"2",1);
Print_String(x+40,y+80,9,"3",1);

// main loop

while(!done)
     {
     // test for key

     if (kbhit())
        {

        // get the key

        switch(getch())
              {

              case 27: // escape key
                 {
                 // exit system
                 done=1;

                 } break;

              case '7': // increase vertex 1 intensity
                 {
                 if (++intensity_1>63)
                    intensity_1=63;
                 } break;

              case '4': // decrease vertex 1 intensity
                 {
                 if (--intensity_1<0)
                    intensity_1=0;
                 } break;

              case '8': // increase vertex 2 intensity
                 {
                 if (++intensity_2>63)
                    intensity_2=63;
                 } break;

              case '5': // decrease vertex 2 intensity
                 {
                 if (--intensity_2<0)
                    intensity_2=0;
                 } break;

              case '9': // increase vertex 31 intensity
                 {
                 if (++intensity_3>63)
                    intensity_3=63;
                 } break;

              case '6': // decrease vertex 3 intensity
                 {
                 if (--intensity_3<0)
                    intensity_3=0;
                 } break;

              default:break;

              } // end switch

        // draw gouraud shaded triangle

        Draw_Triangle_2D_Gouraud(x,y,x-50,y+60,x+30,y+80,video_buffer,
                                 intensity_1,intensity_2,intensity_3);

        } // end kbhit


     // print out vertex intensities

     sprintf(buffer,"Vertex 1 = %d  ",intensity_1);
     Print_String(0,0,12,buffer,0);

     sprintf(buffer,"Vertex 2 = %d  ",intensity_2);
     Print_String(0,10,12,buffer,0);

     sprintf(buffer,"Vertex 3 = %d  ",intensity_3);
     Print_String(0,20,12,buffer,0);

     } // end while

// restore text mode

Set_Graphics_Mode(TEXT_MODE);

} // end main

