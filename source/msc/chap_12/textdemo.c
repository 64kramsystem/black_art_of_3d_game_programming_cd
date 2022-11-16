
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

// include all of our stuff

#include "black3.h"
#include "black4.h"
#include "black5.h"
#include "black6.h"
#include "black8.h"
#include "black9.h"
#include "black11.h"

// G L O B A L S /////////////////////////////////////////////////////////////

pcx_picture image_pcx;  // general PCX image used to load background and imagery

////////////////////////////////////////////////////////////////////////////////

void main(void)
{

int done=0,          // exit flag
    index,           // looping variable
    x=140,y=100,     // position of triangle
    curr_texture=0;  // current texture

// set graphics mode to 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// load in the text textures 64x64

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("textures.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the texture sprite

Sprite_Init((sprite_ptr)&textures,0,0,64,64,0,0,0,0,0,0);

// extract the bitmaps for the textures(four of them:stone, wood, slime, lava)

for (index=0; index<4; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&textures,index,index,0);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// draw textures on screen so user can see what is going on

Sprite_Draw((sprite_ptr)&textures,video_buffer,1);
textures.x+=64;
textures.curr_frame++;

Sprite_Draw((sprite_ptr)&textures,video_buffer,1);
textures.x+=64;
textures.curr_frame++;

Sprite_Draw((sprite_ptr)&textures,video_buffer,1);
textures.x+=64;
textures.curr_frame++;

Sprite_Draw((sprite_ptr)&textures,video_buffer,1);
textures.x+=64;
textures.curr_frame++;

// draw textured triangle

Draw_Triangle_2D_Text(x,y,x-50,y+60,x+30,y+80,video_buffer,curr_texture);

// main loop

while(!done)
     {
     // test for key

     if (kbhit())
        {

        // get the key

        switch(getch())
              {
              case ' ': // space bar to select next texture
                 {

                 if (++curr_texture>3)
                    curr_texture=0;

                 } break;

              case 27: // escape key
                 {
                 // exit system
                 done=1;

                 } break;

              default:break;

              } // end switch

        // draw textured triangle

        Draw_Triangle_2D_Text(x,y,x-50,y+60,x+30,y+80,video_buffer,curr_texture);

        } // end kbhit

     } // end while

// restore text mode

Set_Graphics_Mode(TEXT_MODE);

} // end main

