
// SPEED.C - A demo of 3-D palette animation

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

// D E F I N E S //////////////////////////////////////////////////////////////

// the color register ranges used by the road and side markers

#define START_ROAD_REG    16
#define END_ROAD_REG      23

#define START_MARKER_REG  32
#define END_MARKER_REG    34

// G L O B A L S  ////////////////////////////////////////////////////////////

pcx_picture image_pcx;  // general PCX image used to load background and imagery


// M A I N ///////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{

RGB_color color_1, color_2;  // used to perform the color rotation

int index;                   // looping variable

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// load the screen image

PCX_Init((pcx_picture_ptr)&image_pcx);

// load a PCX file (make sure it's there)

if (PCX_Load("speed.pcx", (pcx_picture_ptr)&image_pcx,1))
   {
   // copy the image to the display buffer

   PCX_Show_Buffer((pcx_picture_ptr)&image_pcx);

   // delete the PCX buffer

   PCX_Delete((pcx_picture_ptr)&image_pcx);

   // remap color registers to more appropriate values

   // remap the side marker colors to red, red, white

   color_1.red    = 63;
   color_1.green  = 0;
   color_1.blue   = 0;

   Write_Color_Reg(START_MARKER_REG,  (RGB_color_ptr)&color_1);
   Write_Color_Reg(START_MARKER_REG+1,(RGB_color_ptr)&color_1);

   color_1.green  = 63;
   color_1.blue   = 63;

   Write_Color_Reg(START_MARKER_REG+2,(RGB_color_ptr)&color_1);

   // now color the road all grey except for three slices

   color_1.red    = 20;
   color_1.green  = 20;
   color_1.blue   = 20;

   for (index=START_ROAD_REG; index<=END_ROAD_REG; index++)
       Write_Color_Reg(index,(RGB_color_ptr)&color_1);

   // now color three of the slices a slightly brighter grey

   color_1.red    = 30;
   color_1.green  = 30;
   color_1.blue   = 30;

   for (index=START_ROAD_REG; index<=END_ROAD_REG; index+=4)
       Write_Color_Reg(index,(RGB_color_ptr)&color_1);

   // wait for a keyboard press

   while(!kbhit())
        {

        // rotate road colors

        // temp = r1
        // r1 <--- r2 <---- r3 <---- ... ri-1 <---- ri
        // ri = temp


        Read_Color_Reg(END_ROAD_REG, (RGB_color_ptr)&color_1);

        for (index=END_ROAD_REG; index>START_ROAD_REG; index--)
            {

            // read the (i-1)th register

            Read_Color_Reg(index-1, (RGB_color_ptr)&color_2);

            // assign it to the ith

            Write_Color_Reg(index, (RGB_color_ptr)&color_2);

            } // end rotate loop

         // place the value of the first color register into the last to
         // complete the rotation

         Write_Color_Reg(START_ROAD_REG, (RGB_color_ptr)&color_1);

        // rotate side marker colors

        Read_Color_Reg(END_MARKER_REG, (RGB_color_ptr)&color_1);

        for (index=END_MARKER_REG; index>START_MARKER_REG; index--)
            {

            // read the (i-1)th register

            Read_Color_Reg(index-1, (RGB_color_ptr)&color_2);

            // assign it to the ith

            Write_Color_Reg(index, (RGB_color_ptr)&color_2);

            } // end rotate loop

         // place the value of the first color register into the last to
         // complete the rotation

         Write_Color_Reg(START_MARKER_REG, (RGB_color_ptr)&color_1);

        // synchronize to 2/18th of second or 9 FPS

        Time_Delay(1);

        } // end main loop

   // use a screen transition to exit

   Screen_Transition(SCREEN_WHITENESS);

   } // end if pcx file found

// reset graphics to text mode

Set_Graphics_Mode(TEXT_MODE);

} // end main

