
// ALIEN.C - A demo of parallax scrolling

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

// D E F I N E S /////////////////////////////////////////////////////////////

#define MOUNTAIN_Y      106   // the starting vertical position of the mountains

#define GRASS_1_Y       150   // the starting vertical positions of the grass
#define GRASS_2_Y       155   // layers
#define GRASS_3_Y       163
#define GRASS_4_Y       172
#define GRASS_5_Y       188

#define MOUNTAIN_HEIGHT (1+149-106)  // the height of each layer

#define GRASS_1_HEIGHT  (1+154-150)
#define GRASS_2_HEIGHT  (1+162-155)
#define GRASS_3_HEIGHT  (1+171-163)
#define GRASS_4_HEIGHT  (1+187-172)
#define GRASS_5_HEIGHT  (1+199-188)

// G L O B A L S  ////////////////////////////////////////////////////////////

pcx_picture image_pcx;  // general PCX image used to load background and imagery

sprite alien;      // our rocket sleding alien

layer mountains,   // the layers for the mountains and grass
      grass_1,
      grass_2,
      grass_3,
      grass_4,
      grass_5;

int mountain_x=0,  // positions of scan window in each layer
    grass_1_x=0,
    grass_2_x=0,
    grass_3_x=0,
    grass_4_x=0,
    grass_5_x=0;

RGB_color fire_color = {63,0,0};   // used for engines

// M A I N //////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{
int done=0;  // main event loop exit flag

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// create the double buffer

Create_Double_Buffer(200);

// load the imagery

PCX_Init((pcx_picture_ptr)&image_pcx);

PCX_Load("alienimg.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the alien sprite

Sprite_Init((sprite_ptr)&alien,160,160,32,18,0,0,0,0,0,0);

// extract the bitmap for the alien

PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&alien,0,0,0);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// now load the background that will be scrolled

PCX_Init((pcx_picture_ptr)&image_pcx);

PCX_Load("alienwld.pcx",(pcx_picture_ptr)&image_pcx,1);

PCX_Copy_To_Buffer((pcx_picture_ptr)&image_pcx,double_buffer);

PCX_Delete((pcx_picture_ptr)&image_pcx);

// create the layers

Layer_Create((layer_ptr)&mountains,SCREEN_WIDTH,MOUNTAIN_HEIGHT);

Layer_Create((layer_ptr)&grass_1,  SCREEN_WIDTH,GRASS_1_HEIGHT);
Layer_Create((layer_ptr)&grass_2,  SCREEN_WIDTH,GRASS_2_HEIGHT);
Layer_Create((layer_ptr)&grass_3,  SCREEN_WIDTH,GRASS_3_HEIGHT);
Layer_Create((layer_ptr)&grass_4,  SCREEN_WIDTH,GRASS_4_HEIGHT);
Layer_Create((layer_ptr)&grass_5,  SCREEN_WIDTH,GRASS_5_HEIGHT);

// scan layers out of double buffer, could have easily scanned from PCX
// file...just personal taste

Layer_Build((layer_ptr)&mountains,0,0,
            double_buffer,0,MOUNTAIN_Y,SCREEN_WIDTH,MOUNTAIN_HEIGHT);

Layer_Build((layer_ptr)&grass_1,0,0,
            double_buffer,0,GRASS_1_Y,SCREEN_WIDTH,GRASS_1_HEIGHT);

Layer_Build((layer_ptr)&grass_2,0,0,
            double_buffer,0,GRASS_2_Y,SCREEN_WIDTH,GRASS_2_HEIGHT);

Layer_Build((layer_ptr)&grass_3,0,0,
            double_buffer,0,GRASS_3_Y,SCREEN_WIDTH,GRASS_3_HEIGHT);

Layer_Build((layer_ptr)&grass_4,0,0,
            double_buffer,0,GRASS_4_Y,SCREEN_WIDTH,GRASS_4_HEIGHT);

Layer_Build((layer_ptr)&grass_5,0,0,
            double_buffer,0,GRASS_5_Y,SCREEN_WIDTH,GRASS_5_HEIGHT);

// main event loop, process until keyboard hit

while(!kbhit())
     {

     // move the alien

     if ( (alien.x+=2) > 320)
       alien.x = -32;

     // move each layer

     if ((mountain_x+=1) >= 319)
         mountain_x -= 320;

     if ((grass_1_x+=2) > 319)
         grass_1_x -= 320;

     if ((grass_2_x+=4) > 319)
         grass_2_x -= 320;

     if ((grass_3_x+=8) > 319)
         grass_3_x -= 320;

     if ((grass_4_x+=14) > 319)
         grass_4_x -= 320;

     if ((grass_5_x+=24) > 319)
         grass_5_x -= 320;

     // draw layers

     Layer_Draw((layer_ptr)&mountains,mountain_x,0,
                double_buffer,MOUNTAIN_Y,MOUNTAIN_HEIGHT,0);

     // update background layer positions

     Layer_Draw((layer_ptr)&grass_1,grass_1_x,0,
                double_buffer,GRASS_1_Y,GRASS_1_HEIGHT,0);

     Layer_Draw((layer_ptr)&grass_2,grass_2_x,0,
                double_buffer,GRASS_2_Y,GRASS_2_HEIGHT,0);

     Layer_Draw((layer_ptr)&grass_3,grass_3_x,0,
                double_buffer,GRASS_3_Y,GRASS_3_HEIGHT,0);

     Layer_Draw((layer_ptr)&grass_4,grass_4_x,0,
                double_buffer,GRASS_4_Y,GRASS_4_HEIGHT,0);

     Layer_Draw((layer_ptr)&grass_5,grass_5_x,0,
                double_buffer,GRASS_5_Y,GRASS_5_HEIGHT,0);

     // draw the sprite on top of layers

     alien.visible = 1;
     Sprite_Draw_Clip((sprite_ptr)&alien,double_buffer,1);

     // change color of fire

     fire_color.red = 20 + rand() % 44;

     Write_Color_Reg(32,(RGB_color_ptr)&fire_color);

     // display double buffer

     Display_Double_Buffer(double_buffer,0);

     // lock onto 18 frames per second max

     Time_Delay(1);

     } // end while

// exit in a very cool way

Screen_Transition(SCREEN_SWIPE_Y);

// free up all resources

Sprite_Delete((sprite_ptr)&alien);

Delete_Double_Buffer();

Set_Graphics_Mode(TEXT_MODE);

} // end main

