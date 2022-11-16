
// LOSTNSPC.C - A demo of random motion

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

sprite ship;            // the alien ship

// M A I N //////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{

int index,         // loop variable
    velocity_x=0,  // used to control velocity of ship
    velocity_y=0;

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// create the double buffer

Create_Double_Buffer(200);

// load the imagery for the ship

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("lostship.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the ship

Sprite_Init((sprite_ptr)&ship,160,100,18,16,0,0,0,0,0,0);

// make ship sit for the first 1.5 seconds

ship.counter_1   = 0;
ship.threshold_1 = 25;

// extract the bitmaps for the ship, there are 2 animation cells

for (index=0; index<2; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&ship,index,index,0);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// now load the background image

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("lostback.pcx",(pcx_picture_ptr)&image_pcx,1);

// copy PCX image to double buffer

PCX_Copy_To_Buffer((pcx_picture_ptr)&image_pcx,double_buffer);
PCX_Delete((pcx_picture_ptr)&image_pcx);

// scan background beforee entering event loop

Sprite_Under_Clip((sprite_ptr)&ship,double_buffer);

// put up exit instructions

Print_String_DB(80,2,9,"Hit any key to exit",1);

// main event loop, process until keyboard hit

while(!kbhit())
     {

     // do animation cycle, erase, move draw...

     // erase all objects by replacing what was under them

     Sprite_Erase_Clip((sprite_ptr)&ship,double_buffer);


// BEGIN RANDOM MOTION LOGIC //////////////////////////////////////////////////

     // test if ship is complete with current trajectory and
     // needs a new one

     if (++ship.counter_1 > ship.threshold_1)
        {

        // select new direction vector

        velocity_x = -5 + rand()%10;
        velocity_y = -5 + rand()%10;

        // select a random number of frames to stay on new heading

        ship.threshold_1 = 5 + rand()%50;

        // reset counter

        ship.counter_1   = 0;

        } // end if

     // move ship

     ship.x+=velocity_x;
     ship.y+=velocity_y;

     // test if ship went beyond screen edges

     if (ship.x > 320)
         ship.x = -18;
     else
     if (ship.x < -18)
         ship.x = 320;

     if (ship.y > 200)
         ship.y = -16;
     else
     if (ship.y < -16)
         ship.y = 200;


// END RANDOM MOTION LOGIC /////////////////////////////////////////////////////

     // animate ship

     if (++ship.curr_frame == 2)
        ship.curr_frame = 0;

     // add some special effects via a vapor trail

     if (rand()%10==1)
        Write_Pixel_DB(ship.x+rand()%20,ship.y+12+rand()%4,24+rand()%4);

     // ready to draw ship, but first scan background under it

     Sprite_Under_Clip((sprite_ptr)&ship,double_buffer);

     Sprite_Draw_Clip((sprite_ptr)&ship,double_buffer,1);

     // display double buffer

     Display_Double_Buffer(double_buffer,0);

     // lock onto 18 frames per second max

     Time_Delay(1);

     } // end while

// exit in a very cool way

Screen_Transition(SCREEN_DARKNESS);

// free up all resources

Sprite_Delete((sprite_ptr)&ship);
Delete_Double_Buffer();

Set_Graphics_Mode(TEXT_MODE);

} // end main

