
// FLOATER.C - A demo of 2-D terrain following

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

sprite speeder;         // the floating speeder


// M A I N //////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{

int index,         // loop variable
    terr_y=160,    // these are used to draw random terrain
    terr_draw=1,
    rough,        // roughness of terrain ,input by user
    x,y,
    hover_height = 2; // minimum terrain following height

// query user about terrain roughness and terrain following height

printf("\nEnter the roughness of terrain from 1-10?");
scanf("%d",&rough);

printf("\nEnter the the terrain following height from 1-50?");
scanf("%d",&hover_height);

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// create the double buffer

Create_Double_Buffer(200);

// load the imagery for the speeder

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("floatspd.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the speeder

Sprite_Init((sprite_ptr)&speeder,320,100,40,10,0,0,0,0,0,0);

// extract the bitmaps for the speeder, there are 4 animation cells

for (index=0; index<4; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&speeder,index,index,0);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// draw the terrain one vetical strip at a time

// seed the random number generator with current time

srand(*(int far *)0x0000046CL);

for (x=0; x<320; x++)
    {
    // test if its time to change directions

    if (--terr_draw<=0)
       {

       terr_draw=rand()%(20/rough);
       terr_y = terr_y - 1 + rand()%3;

       } // end if time to select new direction

    // draw a vertical strip at current x location

    Write_Pixel_DB(x,terr_y,15);

    for (y=terr_y+1; y<200 ;y++)
        Write_Pixel_DB(x,y,200+rand()%16);

    } // end for index

// scan background before entering event loop

Sprite_Under_Clip((sprite_ptr)&speeder,double_buffer);

// put up exit instructions

Print_String_DB(80,2,9,"Hit any key to exit",1);

// main event loop, process until keyboard hit

while(!kbhit())
     {

     // do animation cycle, erase, move draw...

     // erase all objects by replacing what was under them

     Sprite_Erase_Clip((sprite_ptr)&speeder,double_buffer);

     // move speeder

     // if there is no terrain under speeder then apply downward thrust
     // at constant velocity

     if (!Read_Pixel_DB(speeder.x+4,speeder.y+12+hover_height))
         speeder.y+=2;

     // now horizontal thrust

     speeder.x-=6;

     // now probe under speeder for terrain and apply upward thrust
     // if crust is found

     if (Read_Pixel_DB(speeder.x+4,speeder.y+12+hover_height))
        speeder.y-=2;

     // test if speeder has moved off screen

     if (speeder.x <-40)
        speeder.x = 320;

     // this should never happen, but just in case

     if (speeder.y > 200)
         speeder.y=200;

     // animate speeder

     if (++speeder.curr_frame == 4)
        speeder.curr_frame = 0;

     // ready to draw speeder, but first scan background under it

     Sprite_Under_Clip((sprite_ptr)&speeder,double_buffer);

     Sprite_Draw_Clip((sprite_ptr)&speeder,double_buffer,1);

     // display double buffer

     Display_Double_Buffer(double_buffer,0);

     // lock onto 18 frames per second max

     Time_Delay(1);

     } // end while

// exit in a very cool way

Screen_Transition(SCREEN_DARKNESS);

// free up all resources

Sprite_Delete((sprite_ptr)&speeder);
Delete_Double_Buffer();

Set_Graphics_Mode(TEXT_MODE);

} // end main

