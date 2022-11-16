
// JUMPER.C - A demo of spider jumping around using patterns

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

#define NUM_PATTERNS 6

// G L O B A L S  ////////////////////////////////////////////////////////////

pcx_picture image_pcx;  // general PCX image used to load background and imagery

sprite spider;          // the jumping spider

// these are the patterns encoded as strings, with a max of 64 commands
// kinda like DNA patterns

char *patterns[NUM_PATTERNS] = {

"rrrrrrrrrrrrruuuuuuuuuuuulllllllllllllllldddddddddddddlllllllll.",
"urururururururururrrrrrrrdrdrdrdrdrdrdrdrlllllllllllllddddllddd.",
"rrrrurrrrurrrruururururururuulululululululdldldldldrdrdrdrdrdrd.",
"xxxxxxxxxxxuuuuuuuuuudddduuuudddduuuddduuuddduuuddduuuddduuulll.",
"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrruuuuuuuuuuuuuuuuuuuuuuuuuuuuuu.",
"lllllllllllllllrrrrrrrrrrrrrrrrrddddddddddddddddxxxxxxxrrrrrruu.", };


// M A I N //////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{

int index;         // loop variable

char buffer[64];   // used to print strings

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// create the double buffer

Create_Double_Buffer(200);

// load the imagery for the spider

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("jumpspd.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the spider

Sprite_Init((sprite_ptr)&spider,160,100,30,24,0,0,0,0,0,0);

spider.state     = 0;   // select pattern one
spider.counter_1 = 0;

// extract the bitmaps for the spider, there are 4 animation cells

for (index=0; index<4; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&spider,index,index,0);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// now load the background image

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("jumpbak.pcx",(pcx_picture_ptr)&image_pcx,1);

// copy PCX image to double buffer

PCX_Copy_To_Buffer((pcx_picture_ptr)&image_pcx,double_buffer);
PCX_Delete((pcx_picture_ptr)&image_pcx);

// scan background before entering event loop

Sprite_Under_Clip((sprite_ptr)&spider,double_buffer);

// put up exit instructions

Print_String_DB(80,2,9,"Hit any key to exit",1);

// main event loop, process until keyboard hit

while(!kbhit())
     {

     // do animation cycle, erase, move draw...

     // erase all objects by replacing what was under them

     Sprite_Erase_Clip((sprite_ptr)&spider,double_buffer);


// BEGIN PATTERN MOTION LOGIC ///////////////////////////////////////////////

     // move spider, test if there is still data commands in current pattern

     if (patterns[spider.state][spider.counter_1]!='.')
        {
        // what is the command

        switch(patterns[spider.state][spider.counter_1])
              {

              case 'r': // right
                   {
                   // move spider

                   spider.x+=4;

                   // test if off edge

                   if (spider.x > 320)
                      spider.x=-30;

                   } break;

              case 'l': // left
                   {

                   // move spider

                   spider.x-=4;

                   // test if off edge

                   if (spider.x < -30)
                      spider.x=320;

                   } break;

              case 'u': // up
                   {

                   // move spider

                   spider.y-=4;

                   // test if off edge

                   if (spider.y <-24)
                      spider.y=200;

                   } break;

              case 'd': // down
                   {

                   // move spider

                   spider.y+=4;

                   // test if off edge

                   if (spider.y > 200)
                      spider.y=-24;

                   } break;

              case 'x': // do nothing
                   {

                   } break;

              default:break;

              } // end switch
        // increment pattern index

        spider.counter_1++;

        } // end if pattern data hasn't been consumed
     else
        {
        // select a new pattern and reset pattern index

        spider.state     = rand()%NUM_PATTERNS;
        spider.counter_1 = 0;

        } // end else pattern done

// END PATTERN MOTION LOGIC //////////////////////////////////////////////////

     // animate spider

     if (++spider.curr_frame == 4)
        spider.curr_frame = 0;

     // display current pattern and data

     sprintf(buffer,"Pattern #%d, data=%c",spider.state,
                                patterns[spider.state][spider.counter_1]);

     Print_String_DB(88,190,15,buffer,0);

     // ready to draw spider, but first scan background under it

     Sprite_Under_Clip((sprite_ptr)&spider,double_buffer);

     Sprite_Draw_Clip((sprite_ptr)&spider,double_buffer,1);

     // display double buffer

     Display_Double_Buffer(double_buffer,0);

     // lock onto 18 frames per second max

     Time_Delay(1);

     } // end while

// exit in a very cool way

Screen_Transition(SCREEN_DARKNESS);

// free up all resources

Sprite_Delete((sprite_ptr)&spider);
Delete_Double_Buffer();

Set_Graphics_Mode(TEXT_MODE);

} // end main

