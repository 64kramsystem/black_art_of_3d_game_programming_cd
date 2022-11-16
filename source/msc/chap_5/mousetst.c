
// MOUSETST.C - A demo of the mouse driver with some added fun

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

#define ANT_DEAD       0  // the ant is dead (smashed)
#define ANT_EAST       1  // the ant is moving east
#define ANT_WEST       2  // the ant is moving west

#define HAMMER_UP      0  // the hammer is in it's resting position
#define HAMMER_MOVING  1  // the hammer is hammering!!!

// G L O B A L S  ////////////////////////////////////////////////////////////

pcx_picture image_pcx;  // general PCX image used to load background and imagery

sprite ant,             // the ant
       hammer;          // the players hammer

// M A I N //////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{
int index,     // loop variable
    mouse_x,   // mouse status
    mouse_y,
    buttons;

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// create the double buffer

Create_Double_Buffer(200);

// load the imagery for ant

PCX_Init((pcx_picture_ptr)&image_pcx);

PCX_Load("moreants.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the ant sprite

Sprite_Init((sprite_ptr)&ant,0,0,12,6,0,0,0,0,0,0);

// extract the bitmaps for the ant, there are 3 animation cells for each
// direction thus 6 cells

for (index=0; index<6; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&ant,index,index,0);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// load the imagery for players hammer

PCX_Init((pcx_picture_ptr)&image_pcx);

PCX_Load("hammer.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the hammer sprite that will take the place of the mouse
// pointer

Sprite_Init((sprite_ptr)&hammer,0,0,22,20,0,0,0,0,0,0);

// extract the bitmaps for the hammer there are 5

for (index=0; index<5; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&hammer,index,index,0);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// now load the picnik area background

PCX_Init((pcx_picture_ptr)&image_pcx);

PCX_Load("grass.pcx",(pcx_picture_ptr)&image_pcx,1);

// copy PCX image to double buffer

PCX_Copy_To_Buffer((pcx_picture_ptr)&image_pcx,double_buffer);

// delete the pcx image

PCX_Delete((pcx_picture_ptr)&image_pcx);

// scan under ant and hammer before entering the event loop, this must be
// done or else on the first cycle the "erase" function will draw garbage

Sprite_Under_Clip((sprite_ptr)&ant,double_buffer);
Sprite_Under_Clip((sprite_ptr)&hammer,double_buffer);

// reset the mouse and hide the pointer

Mouse_Control(MOUSE_RESET,NULL,NULL,&buttons);
Mouse_Control(MOUSE_HIDE,NULL,NULL,NULL);

// main event loop, process until keyboard hit

while(!kbhit())
     {

     // do animation cycle: 1. erase, 2.game logic, 3. scan, 4. draw

     // erase all objects by replacing what was under them

     if (ant.state!=ANT_DEAD)
         Sprite_Erase_Clip((sprite_ptr)&ant,double_buffer);

     Sprite_Erase_Clip((sprite_ptr)&hammer,double_buffer);

// PLAYERS HAMMER LOGIC

     // obtain the new position of mouse and state of buttons

     Mouse_Control(MOUSE_POSITION_BUTTONS,&mouse_x,&mouse_y,&buttons);

     // map the mouse position to the screen and assign it to hammer

     hammer.x = (mouse_x >> 1)-16;
     hammer.y = mouse_y;

     // test if player is trying to use hammer

     if (buttons==MOUSE_LEFT_BUTTON && hammer.state==HAMMER_UP)
        {
        // set state of hammer to moving

        hammer.state = HAMMER_MOVING;

        } // end if player trying to strike

     // test if hammer is animating

     if (hammer.state==HAMMER_MOVING)
        {
        // test if sequence complete

        if (++hammer.curr_frame==4)
           {
           hammer.state      = HAMMER_UP;
           hammer.curr_frame = 0;
           } // end if done

        // test if hammer is hitting ant

        if (hammer.curr_frame==3 && ant.state!=ANT_DEAD)
           {
           // do a collision test between the hammer and the ant

           if (ant.x > hammer.x && ant.x+12 < hammer.x+22 &&
               ant.y > hammer.y && ant.y+6 < hammer.y+20)
              {
              // kill ant

              ant.state = ANT_DEAD;

              // draw a smashed ant, use frame 5 of hammer

              // set current frame to blood splat

              hammer.curr_frame = 4;

              // draw the splat

              Sprite_Draw_Clip((sprite_ptr)&hammer,double_buffer,1);

              // restore the hammer frame

              hammer.curr_frame = 3;

              } // end if hammer hit ant

           } // end if smash test

        } // end if hammer moving

// ANT LOGIC

    // test if it's time to start an ant

    if (ant.state == ANT_DEAD && ((rand()%10)==0))
       {

       // which direction will ant move in

       if ((rand()%2)==0)
          {
          // move ant east

          ant.y         = rand()%200;    // starting y position
          ant.x         = 0;             // starting x position
          ant.counter_1 = 2 + rand()%10; // ant speed
          ant.state     = ANT_EAST;      // ant direction
          ant.curr_frame= 0;             // starting animation frame

          }
       else
          {
          // move ant west

          ant.y         = rand()%200;     // starting y position
          ant.x         = 320;            // starting x position
          ant.counter_1 = -2 - rand()%10; // ant speed
          ant.state     = ANT_WEST;       // ant direction
          ant.curr_frame= 0;              // starting animation frame

          } // end else west

       } // end if an ant is started

    // test if ant is alive

    if (ant.state!=ANT_DEAD)
       {
       // process ant

       // move the ant

       ant.x+=ant.counter_1;

       // is ant off screen?

       if (ant.x <0 || ant.x > 320)
          ant.state = ANT_DEAD;

       // animate the antm use proper animation cells based on direction
       // cells 0-2 are for eastward motion, cells 3-5 are for westward motion

       if (ant.state==ANT_EAST)
          if (++ant.curr_frame>2)
             ant.curr_frame=0;

       if (ant.state==ANT_WEST)
          if (++ant.curr_frame>5)
             ant.curr_frame=3;

       } // end ant is alive

     // ready to draw objects, but first scan background under them

     if (ant.state!=ANT_DEAD)
         Sprite_Under_Clip((sprite_ptr)&ant,double_buffer);

     Sprite_Under_Clip((sprite_ptr)&hammer,double_buffer);

     if (ant.state!=ANT_DEAD)
         Sprite_Draw_Clip((sprite_ptr)&ant,double_buffer,1);

     Sprite_Draw_Clip((sprite_ptr)&hammer,double_buffer,1);

     // display double buffer

     Display_Double_Buffer(double_buffer,0);

     // lock onto 18 frames per second max

     Time_Delay(1);

     } // end while

// exit in a very cool way

Screen_Transition(SCREEN_SWIPE_X);

// free up all resources

Sprite_Delete((sprite_ptr)&ant);
Sprite_Delete((sprite_ptr)&hammer);
Delete_Double_Buffer();

Set_Graphics_Mode(TEXT_MODE);

} // end main

