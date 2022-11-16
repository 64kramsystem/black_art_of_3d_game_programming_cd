
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
#include <search.h>             // this one is needed for qsort()

// include all of our stuff

#include "black3.h"
#include "black4.h"
#include "black5.h"
#include "black6.h"
#include "black8.h"
#include "black9.h"
#include "black11.h"

// G L O B A L S /////////////////////////////////////////////////////////////

object test_object;   // the test object

// M A I N ////////////////////////////////////////////////////////////////////

void main(int argc,char **argv)
{

int index,   // looping variable
    done=0;  // exit flag

char buffer[80]; // used to print strings

// load in the object from the command line

if (!PLG_Load_Object(&test_object,argv[1],1))
   {
   printf("\nCouldn't find file %s",argv[1]);
   return;

   } // end if

// position the object

Position_Object((object_ptr)&test_object,0,0,300);

// set the viewpoint

view_point.x = 0;
view_point.y = 0;
view_point.z = 0;

// create the sin/cos lookup tables used for the rotation function

Build_Look_Up_Tables();

// set graphics to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// allocate double buffer

Create_Double_Buffer(200);

// read the 3d color palette off disk

Load_Palette_Disk("standard.pal",(RGB_palette_ptr)&color_palette_3d);
Write_Palette(0,255,(RGB_palette_ptr)&color_palette_3d);

// install the isr keyboard driver

Keyboard_Install_Driver();

// set viewing distance

viewing_distance = 250;

// main event loop

while(!done)
     {

     // compute starting time of this frame

     starting_time = Timer_Query();

     // erase the screen

     Fill_Double_Buffer(0);

     // test what key(s) user is pressing

     // test if user is moving object to right

     if (keyboard_state[MAKE_RIGHT])
        test_object.world_pos.x+=5;

     // test if user is moving object to left

     if (keyboard_state[MAKE_LEFT])
        test_object.world_pos.x-=5;

     // test if user is moving object up

     if (keyboard_state[MAKE_UP])
        test_object.world_pos.y-=5;

     // test if user is moving object down

     if (keyboard_state[MAKE_DOWN])
        test_object.world_pos.y+=5;

     // test if user is moving object farther

     if (keyboard_state[MAKE_PGUP])
        test_object.world_pos.z+=15;

     // test if user is moving object nearer

     if (keyboard_state[MAKE_PGDWN])
        test_object.world_pos.z-=15;

     // test for exit key

     if (keyboard_state[MAKE_ESC])
        done=1;


     // rotate the object on all three axes

     Rotate_Object((object_ptr)&test_object,2,4,6);

//////////////////////////////////////////////////////////////////////////////

     // convert the local coordinates into world and camera coordinates for shading
     // and projection. note the viewer is at (0,0,0) with angles 0,0,0 so the transformaton
     // is simply to add the world position to each local vertex

     for (index=0; index<test_object.num_vertices; index++)
         {

         test_object.vertices_camera[index].x =
         test_object.vertices_world[index].x  =

                   test_object.vertices_local[index].x+test_object.world_pos.x;

         test_object.vertices_camera[index].y =
         test_object.vertices_world[index].y  =

                   test_object.vertices_local[index].y+test_object.world_pos.y;

         test_object.vertices_camera[index].z =
         test_object.vertices_world[index].z  =

                   test_object.vertices_local[index].z+test_object.world_pos.z;

         } // end for index

//////////////////////////////////////////////////////////////////////////////

     // shade and remove backfaces, ignore the backface part for now

     Remove_Backfaces_And_Shade((object_ptr)&test_object);

     // draw the object

     Draw_Object_Solid((object_ptr)&test_object);

     // print out position of object

     sprintf(buffer,"Object is at (%d,%d,%d)     ",(int)test_object.world_pos.x,
                                                   (int)test_object.world_pos.y,
                                                   (int)test_object.world_pos.z);

     Print_String_DB(0,0,9,buffer,0);

     // display double buffer

     Display_Double_Buffer(double_buffer,0);

     // lock onto 18 frames per second max

     while((Timer_Query()-starting_time)<1);

     } // end while

// restore graphics mode back to text

Set_Graphics_Mode(TEXT_MODE);

// restore the old keyboard driver

Keyboard_Remove_Driver();

} // end main



