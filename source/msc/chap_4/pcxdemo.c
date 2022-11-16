
// PCXDEMO.C - A pcx file demo that loads a pcx file and displays it

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

// G L O B A L S  ////////////////////////////////////////////////////////////

pcx_picture image_pcx;  // general PCX image used to load background and imagery


// M A I N ///////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// load the screen image

PCX_Init((pcx_picture_ptr)&image_pcx);

// load a PCX file (make sure it's there)

if (PCX_Load("andre.pcx", (pcx_picture_ptr)&image_pcx,1))
   {
   // copy the image to the display buffer

   PCX_Show_Buffer((pcx_picture_ptr)&image_pcx);

   // delete the PCX buffer

   PCX_Delete((pcx_picture_ptr)&image_pcx);

   // wait for a keyboard press

   while(!kbhit()){}

   // use a screen transition to exit

   Screen_Transition(SCREEN_DARKNESS);

   } // end if pcx file found

// reset graphics to text mode

Set_Graphics_Mode(TEXT_MODE);

} // end main

