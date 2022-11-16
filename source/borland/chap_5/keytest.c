
// KEYTEST.C - A demo of the keyboard driver

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

// include our graphics library

#include "black3.h"
#include "black4.h"
#include "black5.h"

// D E F I N E S /////////////////////////////////////////////////////////////

#define START_NUMERIC_COLOR 6*16    // start of color register bank that
                                    // keys are drawn with

// G L O B A L S  ////////////////////////////////////////////////////////////

pcx_picture image_pcx;  // general PCX image used to load background and imagery

// M A I N ///////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{

RGB_color on  = {0,63,0},   // the color values for the on and off buttons
          off = {0,20,0};

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// load the screen image

PCX_Init((pcx_picture_ptr)&image_pcx);

// load a PCX file (make sure it's there)

if (PCX_Load("keypad.pcx", (pcx_picture_ptr)&image_pcx,1))
   {
   // copy the image to the display buffer

   PCX_Show_Buffer((pcx_picture_ptr)&image_pcx);

   // delete the PCX buffer

   PCX_Delete((pcx_picture_ptr)&image_pcx);

   // install the keyboard driver

   Keyboard_Install_Driver();

   // enter main evet loop

   while(!keyboard_state[MAKE_ESC])
        {

        // to avoid video snow wait for vertical retrace

        Wait_For_Vertical_Retrace();

        // now test all the keys to see if they are pressed or released
        // based on this turn the virtual light that illuminates each
        // button on or off

        if (keyboard_state[MAKE_1])
           Write_Color_Reg(START_NUMERIC_COLOR+0, (RGB_color_ptr)&on);
        else
           Write_Color_Reg(START_NUMERIC_COLOR+0, (RGB_color_ptr)&off);

        if (keyboard_state[MAKE_2])
           Write_Color_Reg(START_NUMERIC_COLOR+1, (RGB_color_ptr)&on);
        else
           Write_Color_Reg(START_NUMERIC_COLOR+1, (RGB_color_ptr)&off);

        if (keyboard_state[MAKE_3])
           Write_Color_Reg(START_NUMERIC_COLOR+2, (RGB_color_ptr)&on);
        else
           Write_Color_Reg(START_NUMERIC_COLOR+2, (RGB_color_ptr)&off);

        if (keyboard_state[MAKE_4])
           Write_Color_Reg(START_NUMERIC_COLOR+3, (RGB_color_ptr)&on);
        else
           Write_Color_Reg(START_NUMERIC_COLOR+3, (RGB_color_ptr)&off);

        if (keyboard_state[MAKE_5])
           Write_Color_Reg(START_NUMERIC_COLOR+4, (RGB_color_ptr)&on);
        else
           Write_Color_Reg(START_NUMERIC_COLOR+4, (RGB_color_ptr)&off);

        if (keyboard_state[MAKE_6])
           Write_Color_Reg(START_NUMERIC_COLOR+5, (RGB_color_ptr)&on);
        else
           Write_Color_Reg(START_NUMERIC_COLOR+5, (RGB_color_ptr)&off);

        if (keyboard_state[MAKE_7])
           Write_Color_Reg(START_NUMERIC_COLOR+6, (RGB_color_ptr)&on);
        else
           Write_Color_Reg(START_NUMERIC_COLOR+6, (RGB_color_ptr)&off);

        if (keyboard_state[MAKE_8])
           Write_Color_Reg(START_NUMERIC_COLOR+7, (RGB_color_ptr)&on);
        else
           Write_Color_Reg(START_NUMERIC_COLOR+7, (RGB_color_ptr)&off);

        if (keyboard_state[MAKE_9])
           Write_Color_Reg(START_NUMERIC_COLOR+8, (RGB_color_ptr)&on);
        else
           Write_Color_Reg(START_NUMERIC_COLOR+8, (RGB_color_ptr)&off);

        } // end main event loop

   // remove the keyboard driver

   Keyboard_Remove_Driver();

   // use a screen transition to exit

   Screen_Transition(SCREEN_WHITENESS);

   } // end if pcx file found

// reset graphics to text mode

Set_Graphics_Mode(TEXT_MODE);

} // end main


