
// MODEZ.C - A demo of mode Z (320x400x256)

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

// M A I N ////////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{
int index, // loop variables
    x,y,
    color; // holds the current color

// set the graphics mode to mode Z 320x400x256

Set_Mode_Z();

// fill the screen with dark grey

Fill_Screen_Z(8);

// plot 1000 pixels in each of the colors

for (color=1; color<256; color++)
    for (index=0; index<1000; index++)
        Write_Pixel_Z(rand()%320,rand()%400,color);

// wipe screen

for (index=320; index>=0; index--)
    for (y=0; y<400; y++)
        Write_Pixel_Z(index,y,0);

// restore the video system to text

Set_Graphics_Mode(TEXT_MODE);

} // end main

