
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

object test_object[4];   // the test objects

// M A I N ////////////////////////////////////////////////////////////////////

void main(int argc,char **argv)
{

int index,   // looping variable
    done=0;  // exit flag

char buffer[80]; // used to print strings

// load in the objects from the command line

for (index=0; index<4; index++)
    {
    if (!PLG_Load_Object((object_ptr)&test_object[index],argv[1],1))
       {
       printf("\nCouldn't find file %s",argv[1]);
       return;

       } // end if

    } // end for index

// position the objects 300 units in front of user

for (index=0; index<4; index++)
    Position_Object((object_ptr)&test_object[index],-150+index*100,0,300);

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

     // test if user is moving viewpoint in positive X

     if (keyboard_state[MAKE_RIGHT])
        view_point.x+=5;

     // test if user is moving viewpoint in negative X

     if (keyboard_state[MAKE_LEFT])
        view_point.x-=5;

     // test if user is moving viewpoint in positive Y

     if (keyboard_state[MAKE_UP])
        view_point.y+=5;

     // test if user is moving viewpoint in negative Y

     if (keyboard_state[MAKE_DOWN])
        view_point.y-=5;

     // test if user is moving viewpoint in positive Z

     if (keyboard_state[MAKE_PGUP])
        view_point.z+=5;

     // test if user is moving viewpoint in negative Z

     if (keyboard_state[MAKE_PGDWN])
        view_point.z-=5;

     // this section takes care of view angle rotation


     if (keyboard_state[MAKE_Z])
        {

        if ((view_angle.ang_x+=10)>360)
           view_angle.ang_x = 0;


        } // end if

     if (keyboard_state[MAKE_A])
        {

        if ((view_angle.ang_x-=10)<0)
           view_angle.ang_x = 360;

        } // end if

     if (keyboard_state[MAKE_X])
        {

        if ((view_angle.ang_y+=10)>360)
           view_angle.ang_y = 0;

        } // end if

     if (keyboard_state[MAKE_S])
        {

        if ((view_angle.ang_y-=5)<0)
           view_angle.ang_y = 360;

        } // end if

     if (keyboard_state[MAKE_C])
        {

        if ((view_angle.ang_z+=5)>360)
           view_angle.ang_z = 0;

        } // end if


     if (keyboard_state[MAKE_D])
        {

        if ((view_angle.ang_z-=5)<0)
           view_angle.ang_z = 360;

        } // end if

     // test for exit key

     if (keyboard_state[MAKE_ESC])
        done=1;


     // create the global world to camera transformation matrix

     Create_World_To_Camera();

     // blank object removal message area

     sprintf(buffer,"Objects Removed                       ");
     Print_String_DB(0,180,10,buffer,0);

     // process each object

     for (index=0; index<4; index++)
     {
     // test if this object should be processed

     if (!Remove_Object(&test_object[index],OBJECT_CULL_XYZ_MODE))
        {

        // convert to world coordinates

        Local_To_World_Object((object_ptr)&test_object[index]);

        // shade and remove backfaces, ignore the backface part for now
        // notice that backface shadin and backface removal is done in world coordinates

        Remove_Backfaces_And_Shade((object_ptr)&test_object[index]);

        // convert to camera coordinates

        World_To_Camera_Object((object_ptr)&test_object[index]);

        // draw the object

        Draw_Object_Solid((object_ptr)&test_object[index]);

        } // end if object is visible
     else
        {

        sprintf(buffer,"%d, ",index);
        Print_String_DB(128+index*16,180,10,buffer,0);
        }

     } // end for index

     // print out viewpoint

     sprintf(buffer,"Viewpoint is at (%d,%d,%d)     ",(int)view_point.x,
                                                      (int)view_point.y,
                                                      (int)view_point.z);

     Print_String_DB(0,0,10,buffer,0);

     sprintf(buffer,"Viewangle is at (%d,%d,%d)     ",(int)view_angle.ang_x,
                                                      (int)view_angle.ang_y,
                                                      (int)view_angle.ang_z);
     Print_String_DB(0,10,10,buffer,0);

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