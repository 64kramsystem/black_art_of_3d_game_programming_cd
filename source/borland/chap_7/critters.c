
// CRITTERS.C - A demo of converge or flocking

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

#define NUM_CRITTERS  200

// S T R U C T U R E S ///////////////////////////////////////////////////////

typedef struct critter_typ
        {

        float x,y,    // position of critter
              xv,yv;  // velocity of critter

        int back;     // the background color under the critter

        } critter, *critter_ptr;

// G L O B A L S  ////////////////////////////////////////////////////////////

pcx_picture image_pcx;  // general PCX image used to load background and imagery

critter critters[NUM_CRITTERS];  // the array that hold all the critters

// M A I N //////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{

int index;   // loop variable


float speed,        // used to scale velocity vector of trajectory
      dx,dy,        // used to compute trajectory vectors
      length,       // length of trajectory vector, used to normalize
      centroid_x,   // center of mass of all critters
      centroid_y;

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// create the double buffer

Create_Double_Buffer(200);

// now load the background image

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("critback.pcx",(pcx_picture_ptr)&image_pcx,1);

// copy PCX image to double buffer

PCX_Copy_To_Buffer((pcx_picture_ptr)&image_pcx,double_buffer);
PCX_Delete((pcx_picture_ptr)&image_pcx);

// put up exit instructions

Print_String_DB(80,2,9,"Hit any key to exit",1);

// create the arrays of little critters

for (index=0; index<NUM_CRITTERS; index++)
    {

    // select a random position for critter

    critters[index].x = rand()%320;
    critters[index].y = rand()%200;

    } // end for index

// now compute the meeting place for the critters. This could be any point
// you wish, but in this case we'll use their centroid, but in many cases you
// may want them to converge upon a player or specific spot

centroid_x = 0;
centroid_y = 0;

for (index=0; index<NUM_CRITTERS; index++)
    {

    // average in next critter position

    centroid_x+=critters[index].x;
    centroid_y+=critters[index].y;

    } // end for index

// compute final average

centroid_x/=(float)NUM_CRITTERS;
centroid_y/=(float)NUM_CRITTERS;

// mark centroid

Print_String_DB(centroid_x,centroid_y,15,"C",1);

// now vector each critter toward centroid on a straight line trajectory

for (index=0; index<NUM_CRITTERS; index++)
    {
    // compute deltas

    dx = centroid_x-critters[index].x;
    dy = centroid_y-critters[index].y;

    // compute a unit vector pointing toward centroid for this critter

    length = sqrt(dx*dx+dy*dy);

    dx = dx/length;
    dy = dy/length;

    // now scale the vector by some factor to synthesize velocity

    speed = 2 + rand()%3;

    // compute trajectory vector

    critters[index].xv = dx*speed;
    critters[index].yv = dy*speed;

    } // end for index

// scan under all critters

for (index=0; index<NUM_CRITTERS; index++)
    critters[index].back = Read_Pixel_DB((int)critters[index].x,
                                         (int)critters[index].y);

// main event loop, process until keyboard hit

while(!kbhit())
     {

     // do animation cycle, erase, move draw...

     // erase all critters by replacing what was under them

     for (index=0; index<NUM_CRITTERS; index++)
         Write_Pixel_DB((int)critters[index].x,
                        (int)critters[index].y,
                        critters[index].back);

// BEGIN CONVERGENCE CODE ////////////////////////////////////////////////////

     // move critters toward centroid if they are far enough away

     for (index=0; index<NUM_CRITTERS; index++)
         {

         // test if critter is far enough away from centroid use
         // manhattan distance

         if ((abs(critters[index].x-centroid_x)+
              abs(critters[index].y-centroid_y)) > 20)
          {
          critters[index].x+=critters[index].xv;
          critters[index].y+=critters[index].yv;

          } // end if critter is far enough

         } // end for index

// END CONVERGENCE CODE //////////////////////////////////////////////////////


     // scan under critters

     for (index=0; index<NUM_CRITTERS; index++)
        critters[index].back=Read_Pixel_DB((int)critters[index].x,
                                           (int)critters[index].y);

     // draw critters

     for (index=0; index<NUM_CRITTERS; index++)
         Write_Pixel_DB((int)critters[index].x,
                        (int)critters[index].y,10);

     // display double buffer

     Display_Double_Buffer(double_buffer,0);

     // lock onto 9 frames per second max

     Time_Delay(2);

     } // end while

// exit in a very cool way

Screen_Transition(SCREEN_SWIPE_X);

// free up all resources

Delete_Double_Buffer();

Set_Graphics_Mode(TEXT_MODE);

} // end main

