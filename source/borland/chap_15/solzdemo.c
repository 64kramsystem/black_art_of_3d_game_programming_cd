
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
#include "black15.h"

// G L O B A L S /////////////////////////////////////////////////////////////

object test_objects[4];                // objects in universe

// M A I N ////////////////////////////////////////////////////////////////////

void main(int argc,char **argv)
{

int done=0,
    z1=0,
    index;

char buffer[80];

float x,y,z;

// build all look up tables

Build_Look_Up_Tables();

// load in the test object

for (index=0; index<4; index++)
    {

    if (PLG_Load_Object(&test_objects[index],argv[1],1))
       printf("\nplg loaded.");
    else
       printf("\nCouldn't load file");

    } // end for index

// set the position of the object

for (index=0; index<4; index++)
    {
    test_objects[index].world_pos.x=-200 + (index%4)*100;
    test_objects[index].world_pos.y=0;
    test_objects[index].world_pos.z=200 + 300*(index>>2);
    } // end index


// set graphics to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// read the 3d color palette off disk

Load_Palette_Disk("standard.pal",(RGB_palette_ptr)&color_palette_3d);
Write_Palette(0,255,(RGB_palette_ptr)&color_palette_3d);

// allocate double buffer

Create_Double_Buffer(200);

// create a 200 line z buffer (128k)

Create_Z_Buffer(200);

// initialize the z buffer with a distant value

Fill_Z_Buffer(16000);

// install the isr keyboard driver

Keyboard_Install_Driver();

// main event loop

while(!done)
     {

     // compute starting time of this frame

     starting_time = Timer_Query();

     // erase all objects

     Fill_Double_Buffer(0);

     // initialize z buffer

     Fill_Z_Buffer(16000);

     // test what user is doing

     if (keyboard_state[MAKE_T])
        {

        x = light_source.x;

        y = cos(.2)*light_source.y - sin(.2) * light_source.z;
        z = sin(.2)*light_source.y + cos(.2) * light_source.z;

        light_source.y = y;
        light_source.z = z;

        }

     if (keyboard_state[MAKE_G])
        {

        y = light_source.y;

        x = cos(.2)*light_source.x + sin(.2) * light_source.z;
        z = -sin(.2)*light_source.x + cos(.2) * light_source.z;

        light_source.x = x;
        light_source.z = z;

        }

     if (keyboard_state[MAKE_B])
        {

        z = light_source.z;

        x = cos(.2)*light_source.x - sin(.2) * light_source.y;
        y = sin(.2)*light_source.x + cos(.2) * light_source.y;

        light_source.x = x;
        light_source.y = y;

        }

     if (keyboard_state[MAKE_Y])
        {

        x = light_source.x;

        y = cos(-.2)*light_source.y - sin(-.2) * light_source.z;
        z = sin(-.2)*light_source.y + cos(-.2) * light_source.z;

        light_source.y = y;
        light_source.z = z;

        }

     if (keyboard_state[MAKE_H])
        {

        y = light_source.y;

        x = cos(-.2)*light_source.x + sin(-.2) * light_source.z;
        z = -sin(-.2)*light_source.x + cos(-.2) * light_source.z;

        light_source.x = x;
        light_source.z = z;

        }

     if (keyboard_state[MAKE_N])
        {

        z = light_source.z;

        x = cos(-.2)*light_source.x - sin(-.2) * light_source.y;
        y = sin(-.2)*light_source.x + cos(-.2) * light_source.y;

        light_source.x = x;
        light_source.y = y;

        }

     if (keyboard_state[MAKE_UP])
        view_point.y+=20;

     if (keyboard_state[MAKE_DOWN])
        view_point.y-=20;

     if (keyboard_state[MAKE_RIGHT])
        view_point.x+=20;

     if (keyboard_state[MAKE_LEFT])
        view_point.x-=20;

     if (keyboard_state[MAKE_KEYPAD_PLUS])
        view_point.z+=20;


     if (keyboard_state[MAKE_KEYPAD_MINUS])
        view_point.z-=20;

     if (keyboard_state[MAKE_LFT_BRACKET])
        Scale_Object(&test_objects[0],1.1);

    if (keyboard_state[MAKE_RGT_BRACKET])
       Scale_Object(&test_objects[0],.9);

     if (keyboard_state[MAKE_Z])
        if ((view_angle.ang_x+=10)>360)
           view_angle.ang_x = 0;

     if (keyboard_state[MAKE_A])
        if ((view_angle.ang_x-=10)<0)
           view_angle.ang_x = 360;

     if (keyboard_state[MAKE_X])
        if ((view_angle.ang_y+=10)>360)
           view_angle.ang_y = 0;

     if (keyboard_state[MAKE_S])
        if ((view_angle.ang_y-=10)<0)
           view_angle.ang_y = 360;

     if (keyboard_state[MAKE_C])
        if ((view_angle.ang_z+=10)>360)
           view_angle.ang_z = 0;

     if (keyboard_state[MAKE_D])
        if ((view_angle.ang_z-=10)<0)
           view_angle.ang_z = 360;

     if (keyboard_state[MAKE_ESC])
        done=1;

     if (keyboard_state[MAKE_Q])
        test_objects[0].world_pos.x-=2;

     if (keyboard_state[MAKE_W])
        test_objects[0].world_pos.x+=2;

     Rotate_Object(&test_objects[0],3,6,9);


     // now that user has possible moved viewpoint, create the global
     // world to camera transformation matrix

     Create_World_To_Camera();

     // reset polygon list

     sprintf(buffer,"                           ");
     Print_String_DB(0,0,10,buffer,0);

     Generate_Poly_List(NULL,RESET_POLY_LIST);

     for (index=0; index<4; index++)
         {

         // test if object is visible

         if (!Remove_Object(&test_objects[index],OBJECT_CULL_XYZ_MODE))
            {
            // convert object local coordinates to world coordinate

            Local_To_World_Object(&test_objects[index]);

            // remove the backfaces and shade object

            Remove_Backfaces_And_Shade(&test_objects[index]);

            // convert world coordinates to camera coordinate

            World_To_Camera_Object(&test_objects[index]);

            // clip the objects polygons against viewing volume

            Clip_Object_3D(&test_objects[index],CLIP_Z_MODE);

            // generate the final polygon list

            Generate_Poly_List(&test_objects[index],ADD_TO_POLY_LIST);
            }
         else
            {
            sprintf(buffer,"%d, ",index);
            Print_String_DB(0+index*26,0,10,buffer,0);
            }
         } // end for object

     // draw the polygon list using z buffer

     Draw_Poly_List_Z();

     // display double buffer

     Display_Double_Buffer(double_buffer,0);

     // lock onto 18 frames per second max

     while((Timer_Query()-starting_time)<1);

     } // end while

// restore graphics mode back to text

Set_Graphics_Mode(TEXT_MODE);

// release the z buffer memory

Delete_Z_Buffer();

// restore the old keyboard driver

Keyboard_Remove_Driver();

// print out some stats

printf("\nSettings...\n");

printf("\nview point x=%f, y=%f, z=%f",view_point.x,view_point.y,view_point.z);
printf("\nview angle x=%d, y=%d, z=%d",view_angle.ang_x,view_angle.ang_y,view_angle.ang_z);
printf("\nlight source x=%f, y=%f, z=%f",light_source.x,light_source.y,light_source.z);

} // end main



