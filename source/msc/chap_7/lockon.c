
// LOCKON.C - A demo of tracking and evasion algorithms

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

// these are the animation cell indices for the alien walking in all the
// directions

#define ALIEN_START_RIGHT   0
#define ALIEN_START_LEFT    4
#define ALIEN_START_UP      8
#define ALIEN_START_DOWN    12

#define ALIEN_END_RIGHT     3
#define ALIEN_END_LEFT      7
#define ALIEN_END_UP        11
#define ALIEN_END_DOWN      15

// these are the directions the alien can move in

#define ALIEN_RIGHT         0
#define ALIEN_LEFT          1
#define ALIEN_UP            2
#define ALIEN_DOWN          3

// G L O B A L S  ////////////////////////////////////////////////////////////

pcx_picture image_pcx;  // general PCX image used to load background and imagery

sprite alien, creature; // the players alien and the creature

// M A I N //////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{

int index,    // loop variable
    chase=1,  // used to select creatures mode of operation, 1=chase, 0=evade
    done=0;   // event loop exit flag

char buffer[64]; // used to print srtrings

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// create the double buffer

Create_Double_Buffer(200);

// load the imagery for the players alien

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("lockaln.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the alien sprite

Sprite_Init((sprite_ptr)&alien,32,164,8,12,0,0,0,0,0,0);

// start the alien walking down

alien.state      = ALIEN_DOWN;
alien.curr_frame = ALIEN_START_DOWN;

// extract the bitmaps for the alien, there are 16 animation cells

for (index=0; index<16; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&alien,index,index,0);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// load the imagery for creature

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("lockcrt.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the creature sprite

Sprite_Init((sprite_ptr)&creature,160,100,24,12,0,0,0,0,0,0);

// extract the bitmaps for the creature, there are 4 animation cells

for (index=0; index<4; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&creature,index,index,0);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// now load the background image

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("lockbak.pcx",(pcx_picture_ptr)&image_pcx,1);

// copy PCX image to double buffer

PCX_Copy_To_Buffer((pcx_picture_ptr)&image_pcx,double_buffer);
PCX_Delete((pcx_picture_ptr)&image_pcx);

// scan under alien and creature before entering the event loop, this must be
// done or else on the first cycle the "erase" function will draw garbage

Sprite_Under_Clip((sprite_ptr)&alien,double_buffer);
Sprite_Under_Clip((sprite_ptr)&creature,double_buffer);

// install new keyboard driver

Keyboard_Install_Driver();

// put up exit instructions

Print_String_DB(96,2,9,"Press Q to exit",0);

// main event loop, process until keyboard hit

while(!done)
     {

     // do animation cycle, erase, move draw...

     // erase all objects by replacing what was under them

     Sprite_Erase_Clip((sprite_ptr)&alien,double_buffer);
     Sprite_Erase_Clip((sprite_ptr)&creature,double_buffer);

     // move player

     // test for right motion

     if (keyboard_state[MAKE_RIGHT])
        {
        // move alien

        if ((alien.x+=2)>320)
            alien.x=-8;

        // first test if alien was already moving right

        if (alien.state==ALIEN_RIGHT)
           {
           // animate and test for end of sequence

           if (++alien.curr_frame==ALIEN_END_RIGHT)
              alien.curr_frame = ALIEN_START_RIGHT;
           }
        else
           {
           // set state and current frame to right

           alien.state      = ALIEN_RIGHT;
           alien.curr_frame = ALIEN_START_RIGHT;

           } // end else

        } // end if right

     // test for left motion
     else
     if (keyboard_state[MAKE_LEFT])
        {
        // move alien

        if ((alien.x-=2)<-8)
             alien.x=320;

        // first test if alien was already moving left

        if (alien.state==ALIEN_LEFT)
           {
           // animate and test for end of sequence

           if (++alien.curr_frame==ALIEN_END_LEFT)
              alien.curr_frame = ALIEN_START_LEFT;
           }
        else
           {
           // set state and current frame to right

           alien.state      = ALIEN_LEFT;
           alien.curr_frame = ALIEN_START_LEFT;

           } // end else

        } // end if left

     // test for upward motion
     else
     if (keyboard_state[MAKE_UP])
        {
        // move alien

        if ((alien.y-=2) < -12)
           alien.y=200;

        // first test if alien was already moving up

        if (alien.state==ALIEN_UP)
           {
           // animate and test for end of sequence

           if (++alien.curr_frame==ALIEN_END_UP)
              alien.curr_frame = ALIEN_START_UP;
           }
        else
           {
           // set state and current frame to right

           alien.state      = ALIEN_UP;
           alien.curr_frame = ALIEN_START_UP;

           } // end else

        } // end if up

     // test for downward motion
     else
     if (keyboard_state[MAKE_DOWN])
        {
        // move alien

        if ((alien.y+=2)>200)
            alien.y=-12;

        // first test if alien was already moving down

        if (alien.state==ALIEN_DOWN)
           {
           // animate and test for end of sequence

           if (++alien.curr_frame==ALIEN_END_DOWN)
              alien.curr_frame = ALIEN_START_DOWN;

           }
        else
           {
           // set state and current frame to right

           alien.state      = ALIEN_DOWN;
           alien.curr_frame = ALIEN_START_DOWN;

           } // end else

        } // end if down

     // test for tracking toggle

     if (keyboard_state[MAKE_SPACE])
        {
        chase=-chase;
        while(keyboard_state[MAKE_SPACE]);
        }

     // test for exit key

     if (keyboard_state[MAKE_Q])
        done=1;

// BEGIN TRACKING LOGIC ///////////////////////////////////////////////////////

     // move creature, test if creature is chasing or evading player

     if (chase==1)
        {
        // track on x coordinate

        if (alien.x > creature.x)
           creature.x++;
        else
        if (alien.x < creature.x)
           creature.x--;

        // now track on y coordinate

        if (alien.y > creature.y)
           creature.y++;
        else
        if (alien.y < creature.y)
           creature.y--;

        } // end if chase
     else
        {
        // must be evading

        // evade on x coordinate

        if (alien.x < creature.x)
           creature.x++;
        else
        if (alien.x > creature.x)
           creature.x--;

        // now evade on y coordinate

        if (alien.y < creature.y)
           creature.y++;
        else
        if (alien.y > creature.y)
           creature.y--;

        } // end else

     // test of creature has moved off screen

     if (creature.x>310)
        creature.x = 310;
     else
     if (creature.x<-14)
        creature.x = -14;

      if (creature.y>190)
         creature.y = 190;
      else
      if (creature.y<-2)
         creature.y = -2;

// END TRACKING LOGIC /////////////////////////////////////////////////////////

     // do animation for creature

     if (++creature.curr_frame==4)
        creature.curr_frame=0;

     // ready to draw objects, but first scan background under them

     Sprite_Under_Clip((sprite_ptr)&alien,double_buffer);
     Sprite_Under_Clip((sprite_ptr)&creature,double_buffer);

     Sprite_Draw_Clip((sprite_ptr)&alien,double_buffer,1);
     Sprite_Draw_Clip((sprite_ptr)&creature,double_buffer,1);

     // display text message of current tracking mode

     if (chase==1)
        sprintf(buffer,"Creature is Chasing!!!");
     else
        sprintf(buffer,"Creature is Evading!!!");

     Print_String_DB(64,190,12,buffer,0);

     // display double buffer

     Display_Double_Buffer(double_buffer,0);

     // lock onto 18 frames per second max

     Time_Delay(1);

     } // end while

// exit in a very cool way

Screen_Transition(SCREEN_WHITENESS);

// free up all resources

Sprite_Delete((sprite_ptr)&alien);
Sprite_Delete((sprite_ptr)&creature);
Delete_Double_Buffer();

Set_Graphics_Mode(TEXT_MODE);

Keyboard_Remove_Driver();

} // end main

