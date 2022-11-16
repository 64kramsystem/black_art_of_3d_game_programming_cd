
// MODE13.C - A demo of all the mode 13h functions for this chapter

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

#include "black3.h"   // the header file for this module

// M A I N ////////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{

RGB_color color;  // used as temporary color variable
int index;        // used for looping

RGB_palette save_palette;  // used to save the palette

printf("\nHit any key to switch to mode 13h");
getch();

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// show some text

Print_String(0,0,15,"Hit a key to see text printing         ",0);
getch();

// print a few hundred strings on the screen

for (index=0; index<1000; index++)
    Print_String(rand()%(320-256),rand()%190,rand()%256,
                 "This is a demo of text printing",1);

Print_String(0,0,15,"Hit a key to see screen filling        ",0);
getch();

// fill the screen dark grey

Fill_Screen(8);

Print_String(0,0,15,"Hit a key to see pixel plotting        ",0);
getch();

// plot 10000 random pixels

for (index=0; index<10000; index++)
    Write_Pixel(rand()%320,rand()%200,12);

Print_String(0,0,15,"Hit a key to see lines                 ",0);
getch();

// draw 1000 randomly positioned horizontal and vertical lines

for (index=0; index<1000; index++)
    {
    Line_H(rand()%320,rand()%320,rand()%200,rand()%256);
    Line_V(rand()%200,rand()%200,rand()%320,rand()%256);
    } // end for index

Print_String(0,0,15,"Hit a key to change color registers    ",0);
getch();

// save the palette

Read_Palette(0,255,(RGB_palette_ptr)&save_palette);

// change the palette

for (index=0; index<256; index++)
    {
    // set the color to bright green

    color.red   = 0;
    color.green = 63;
    color.blue  = 0;

    // change the currently indexed color register

    Write_Color_Reg(index,(RGB_color_ptr)&color);

    // let user see it happen

    Time_Delay(1);

    } // end for index

// make color 15 visible so user can read text

color.red   = 63;
color.green = 63;
color.blue  = 63;

Write_Color_Reg(15,(RGB_color_ptr)&color);

Print_String(0,0,15,"Hit a key to restore palette           ",0);
getch();

// restore the palette

Write_Palette(0,255,(RGB_palette_ptr)&save_palette);

Print_String(0,0,15,"Hit a key to switch back to text mode  ",0);
getch();

// restore graphics mode to text

Set_Graphics_Mode(TEXT_MODE);

} // end main
