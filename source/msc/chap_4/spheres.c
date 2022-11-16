
// SPHERES.C - A demo of mode Z page flipping

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

// F U N C T I O N S /////////////////////////////////////////////////////////

void Draw_Sphere(int x0,int y0,int radius, int color)
{
// this function will draw a sphere in

int x1,y1,y2, // location coordinates
    angle;   // used to track current angle

// interate thru 90 degrees and use symetry to draw circle

for (angle=0; angle<90; angle++)
    {
    // draw the sphere as a collection of vertical strips

    x1 = radius*cos(3.14159*(float)angle/(float)180);
    y1 = -radius*sin(3.14159*(float)angle/(float)180)*1.66;

    // draw the next vertical strip

    for (y2=y0+y1; y2<y0-y1; y2++)
        {
        Write_Pixel_Z(x0+x1,y2,color);
        Write_Pixel_Z(x0-x1,y2,color);
        } // end for y2

    } // end for angle

} // end Draw_Sphere

// M A I N ////////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{
int index; // loop variables

// set the graphics mode to mode Z 320x400x256

Set_Mode_Z();

// clear out all of display memory, only page 1 was cleared during set_mode_z

Set_Working_Page_Mode_Z(PAGE_1);
Fill_Screen_Z(0);

// set visual and working page to page 0

Set_Visual_Page_Mode_Z(PAGE_0);
Set_Working_Page_Mode_Z(PAGE_0);

// draw some colored spheres on this page

for (index=0; index<50; index++)
    {
    Draw_Sphere(20+rand()%280,20+rand()%360,rand()%15, 34 + rand()%6);
    } // end for index

// now draw grey spheres on page 1

Set_Working_Page_Mode_Z(PAGE_1);

for (index=0; index<50; index++)
    {
    Draw_Sphere(20+rand()%280,20+rand()%360,rand()%15, 16 + rand()%16);
    } // end for index

// now toggle between pages

while(!kbhit())
     {

     Set_Visual_Page_Mode_Z(PAGE_0);
     Time_Delay(5);

     Set_Visual_Page_Mode_Z(PAGE_1);
     Time_Delay(5);

     } // end while

// restore the video system to text

Set_Graphics_Mode(TEXT_MODE);

} // end main

