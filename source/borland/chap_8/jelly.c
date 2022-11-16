
// JELLY.C - A demo of latching onto the timer interrupt 0x1C

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
#include "black8.h"


// G L O B A L S  ////////////////////////////////////////////////////////////

pcx_picture image_pcx;  // general PCX image used to load background and imagery

sprite jelly;           // the jelly fish

int ISR_jelly_x = -20,  // global variables that track the position of the
    ISR_jelly_y = 100;  // jelly fish and are updated by the ISR

// this holds the old timer keeper isr

void (_interrupt _far *Old_Timer_ISR)();

////////////////////////////////////////////////////////////////////////////

void _interrupt _far Jelly_ISR(void)
{
// this function will update the global jelly fish position variables and
// test if the jelly fish has moved off the screen, this function will be
// called once every timer tick

_asm sti  // re-enable interrupts

//////////////////////////////////////////////////

// move the jelly fish

if (++ISR_jelly_x > 320)
   ISR_jelly_x = -20;

ISR_jelly_y = ISR_jelly_y - 1 + rand()%3;

// test y bounds

if (ISR_jelly_y > 200)
	 ISR_jelly_y=-20;
else
if (ISR_jelly_y < -20)
    ISR_jelly_y=200;

/////////////////////////////////////////////////

// re-enable pic

outp(PIC_ICR,PIC_EOI);

} // end Jelly_ISR

// M A I N //////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{

int index;         // loop variable

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// create the double buffer

Create_Double_Buffer(200);

// load the imagery for the jelly fish

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("jelly.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the jelly fish

Sprite_Init((sprite_ptr)&jelly,-20,100,16,16,0,0,0,0,0,0);

// extract the bitmaps for the speeder, there are 4 animation cells

for (index=0; index<4; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&jelly,index,index,0);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// install the jelly fish motion interrupt

Old_Timer_ISR = _dos_getvect(TIME_KEEPER_INT);

_dos_setvect(TIME_KEEPER_INT,Jelly_ISR);

// scan background before entering event loop

Sprite_Under_Clip((sprite_ptr)&jelly,double_buffer);

// put up exit instructions

Print_String_DB(80,2,9,"Hit any key to exit",1);

// main event loop, process until keyboard hit

while(!kbhit())
     {

     // do animation cycle

     // erase the jelly fish

     Sprite_Erase_Clip((sprite_ptr)&jelly,double_buffer);

     // move the jelly fish, note that we only copy global variables here
     // the variables themselves are modified by the ISR

     jelly.x = ISR_jelly_x;
     jelly.y = ISR_jelly_y;

     // animate the jelly fish

     if (++jelly.curr_frame == 4)
        jelly.curr_frame = 0;

     // ready to draw speeder, but first scan background under it

     Sprite_Under_Clip((sprite_ptr)&jelly,double_buffer);

     Sprite_Draw_Clip((sprite_ptr)&jelly,double_buffer,1);

     // display double buffer

     Display_Double_Buffer(double_buffer,0);

     // lock onto 18 frames per second max

     Time_Delay(1);

     } // end while

// exit in a very cool way

Screen_Transition(SCREEN_DARKNESS);

// free up all resources

Sprite_Delete((sprite_ptr)&jelly);
Delete_Double_Buffer();

// restore the old isr

_dos_setvect(TIME_KEEPER_INT,Old_Timer_ISR);

Set_Graphics_Mode(TEXT_MODE);

} // end main

