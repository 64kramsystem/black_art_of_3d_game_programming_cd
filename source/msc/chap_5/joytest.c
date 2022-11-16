
// JOYTEST.C - A demo of the joystick driver

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
#include "black5.h"

// D E F I N E S /////////////////////////////////////////////////////////////

#define SHIP_FRAMES 16  // number of animaton frames of ship

// G L O B A L S  ////////////////////////////////////////////////////////////

pcx_picture image_pcx;  // general PCX image used to load background and imagery

sprite ship;            // the players ship

// these are the velocity lookup tables, they have pre-computed velocities
// for each of the 16 directions the ship can be pointing in

int x_velocity[SHIP_FRAMES] = {0,2,4,4,6,4,4,2,0,-2,-4,-4,-6,-4,-4,-2};
int y_velocity[SHIP_FRAMES] = {-6,-4,-4,-2,0,2,4,4,6,4,4,2,0,-2,-4,-4};

// M A I N //////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{

int index,     // loop variable
    dx,        // use to hold roughly 15% of the range of each
    dy,        // joystick axis
    joy_x,     // the final normalized joystick position values
    joy_y;

// test if there is a joystick

if (!Joystick_Available(JOYSTICK_1))
   {
   printf("\nJoystick 1 not connected. Exiting.");
   return;
   } // end if

printf("\nJoystick 1 detected...");

// calibrate the stick

Joystick_Calibrate(JOYSTICK_1,USE_LOW_LEVEL);

// compute 15% of range

dx = (int)( (float).5 + (float).15 * (float)(joystick_1_max_x-joystick_1_min_x));
dy = (int)( (float).5 + (float).15 * (float)(joystick_1_max_y-joystick_1_min_y));

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// create the double buffer

Create_Double_Buffer(200);

// load the imagery for ship

PCX_Init((pcx_picture_ptr)&image_pcx);

PCX_Load("falcon.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the ship sprite

Sprite_Init((sprite_ptr)&ship,160,100,24,20,0,0,0,0,0,0);

// extract the bitmaps for the ship, there are 16 of them, one for each
// pre-rotated angle

// get images off first row of template 0-11

for (index=0; index<12; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&ship,index,index,0);

// get images off second row of template 12-15

for (index=12; index<16; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&ship,index,index-12,1);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// now load the background startfield

PCX_Init((pcx_picture_ptr)&image_pcx);

PCX_Load("frontier.pcx",(pcx_picture_ptr)&image_pcx,1);

// copy PCX image to double buffer

PCX_Copy_To_Buffer((pcx_picture_ptr)&image_pcx,double_buffer);

// delete the pcx image

PCX_Delete((pcx_picture_ptr)&image_pcx);

// scan under the ship, so the first time through the event loop
// whas something to replace

Sprite_Under_Clip((sprite_ptr)&ship,double_buffer);

// main event loop, process until keyboard hit

while(!kbhit())
     {

     // do animation cycle: 1. erase, 2.game logic, 3. scan, 4. draw

     // erase the ship

     Sprite_Erase_Clip((sprite_ptr)&ship,double_buffer);

// PLAYERS SHIP LOGIC

     // get joystick position and subtract away center to
     // compute delta from center

     joy_x = Joystick(JOYSTICK_1_X) - joystick_1_neutral_x;
     joy_y = Joystick(JOYSTICK_1_Y) - joystick_1_neutral_y;

     // test if player has moved stick past the 10% mark, if so transform
     // ship

     if (joy_x > dx)
        {
        // rotate ship right

        if (++ship.curr_frame==SHIP_FRAMES)
           ship.curr_frame = 0;
        }
     else
     if (joy_x < -dx)
        {
        // rotate ship left

        if (--ship.curr_frame == -1)
           ship.curr_frame = SHIP_FRAMES-1;

        } // end if rotating left

     // test if player is movinh ship foward or backward

     if (joy_y > dy)
        {
        // move ship backward

        // index into velocity table and translate ship with values

        ship.x -= x_velocity[ship.curr_frame];
        ship.y -= y_velocity[ship.curr_frame];

        }
     else
     if (joy_y < -dy)
        {
        // move ship foward

        ship.x += x_velocity[ship.curr_frame];
        ship.y += y_velocity[ship.curr_frame];

        } // end if foward

     // clip ship to screen universe

     if (ship.x > 319)
         ship.x = -24;
     else
     if (ship.x < -24)
         ship.x = 319;

     if (ship.y > 199)
         ship.y = -20;
     else
     if (ship.y < -20)
         ship.y = 199;

     // ready to draw ship, but first scan background under them

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