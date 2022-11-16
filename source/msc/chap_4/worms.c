
// WORMS.C - A demo of sprites, clipping and double buffering

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

sprite worm,ant;        // the worm and ant

// M A I N //////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{

int index;  // loop variable

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// create the double buffer

Create_Double_Buffer(200);

// load the imagery for worm

PCX_Init((pcx_picture_ptr)&image_pcx);

PCX_Load("wormimg.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the worm sprite

Sprite_Init((sprite_ptr)&worm,160,100,38,20,0,0,0,0,0,0);

// extract the bitmaps for the worm, there are 4 animation cells

for (index=0; index<4; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&worm,index,index,0);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// load the imagery for ant

PCX_Init((pcx_picture_ptr)&image_pcx);

PCX_Load("antimg.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the ant sprite

Sprite_Init((sprite_ptr)&ant,160,180,12,6,0,0,0,0,0,0);

// extract the bitmaps for the ant, there are 3 animation cells

for (index=0; index<3; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&ant,index,index,0);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// now load the background that the worm and ant will run around in

PCX_Init((pcx_picture_ptr)&image_pcx);

PCX_Load("mushroom.pcx",(pcx_picture_ptr)&image_pcx,1);

// copy PCX image to double buffer

PCX_Copy_To_Buffer((pcx_picture_ptr)&image_pcx,double_buffer);

PCX_Delete((pcx_picture_ptr)&image_pcx);

// scan under ant and worm before entering the event loop, this must be
// done or else on the first cycle the "erase" function will draw garbage

Sprite_Under((sprite_ptr)&ant,double_buffer);
Sprite_Under_Clip((sprite_ptr)&worm,double_buffer);

// main event loop, process until keyboard hit

while(!kbhit())
     {

     // do animation cycle, erase, move draw...


     // erase all objects by replacing what was under them

     Sprite_Erase((sprite_ptr)&ant,double_buffer);

     Sprite_Erase_Clip((sprite_ptr)&worm,double_buffer);

     // move objects, test if they have ran off right edge of screen

     if ((ant.x+=1) > 320-12)
        ant.x = 0;

     if ((worm.x+=6) > 320)
        worm.x = -40;   // start worm back one length beyond the
                        // left edge of screen

     // do animation for objects

     if (++ant.curr_frame==3)   // if all frames have been displayed then
        ant.curr_frame = 0;     // reset back to frame 0

     if (++worm.curr_frame==4)  // if all frames have been displayed then
        worm.curr_frame = 0;    // reset back to frame 0


     // ready to draw objects, but first scan background under them

     Sprite_Under((sprite_ptr)&ant,double_buffer);
     Sprite_Under_Clip((sprite_ptr)&worm,double_buffer);

     Sprite_Draw((sprite_ptr)&ant,double_buffer,1);
     Sprite_Draw_Clip((sprite_ptr)&worm,double_buffer,1);

     // display double buffer

     Display_Double_Buffer(double_buffer,0);

     // lock onto 18 frames per second max

     Time_Delay(1);

     } // end while

// exit in a very cool way

Screen_Transition(SCREEN_SWIPE_X);

// free up all resources

Sprite_Delete((sprite_ptr)&worm);

Sprite_Delete((sprite_ptr)&ant);

Delete_Double_Buffer();

Set_Graphics_Mode(TEXT_MODE);

} // end main

