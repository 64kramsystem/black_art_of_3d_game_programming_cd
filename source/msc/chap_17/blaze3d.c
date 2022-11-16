
// Starblazer 3-D

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
#include "black17.h"

// D E F I N E S /////////////////////////////////////////////////////////////

#define NUM_ASTEROIDS  18       // total number of asteroids in demo

#define SCANNER_X      121      // position of scanner
#define SCANNER_Y      151

// T Y P E D E F S ///////////////////////////////////////////////////////////

typedef struct asteroid_typ
        {
        int state;           // state of asteroid
        int xv,yv,zv;        // velocity of asteroid
        int rx,ry,rz;        // rotation rate of asteroid

        } asteroid, *asteroid_ptr;

// G L O B A L S /////////////////////////////////////////////////////////////

object test_objects[NUM_ASTEROIDS];  // objects in universe, need to be in global
                                     // data segement to make things easier

// F U N C T I O N S /////////////////////////////////////////////////////////

void Draw_Speedo(int speed)
{
// this function draws the digital speedometer

int mark_x=258, // starting tick mark
    index,      // looping variable
    color;      // color of tick mark

// compute number of iterations

speed/=5;

if (speed>=0)
   {
   // select foward color

   color=10;
   }
else
   {
   // set color to backward

   color=12;

   // take abs of speed

   speed=-speed;

   } // end else

// draw the ticks

for (index=0; index<speed; index++,mark_x+=6)
    {
    // draw a vertical line

    Line_V(4,14,mark_x,color);

    } // end for index

// erase remaining tick marks

for (; index<10; index++,mark_x+=6)
    {
    // draw a vertical line

    Line_V(4,14,mark_x,0);

    } // end for index

} // end Draw_Speedo

///////////////////////////////////////////////////////////////////////////////

void Draw_Scanner(asteroid_ptr asteroids,int draw)
{
// this function erases or draws the blips on the scanner

int index, // looping variable
    xb,yb, // position of blip in screen coordinates on scanner
    cb;    // color of blip

static int blip_x[NUM_ASTEROIDS+1], // position 0 holds the ship
           blip_y[NUM_ASTEROIDS+1], // position 0 holds the ship
           active_blips;            // number of active blips this cycle

// should the scanner image be erased or drawn

if (!draw)
   {
   // erase all the blips

   for (index=0; index<=active_blips; index++)
       Write_Pixel(SCANNER_X+blip_x[index], SCANNER_Y+blip_y[index],0);

   } // end if erase
else
   {
   // draw the scanner blips

   // draw asteroids above player as red, below as blue and player as green
   // the asteroids exits in positions 1..n and the player at 0

   active_blips=0;

   for (index=0; index<NUM_ASTEROIDS; index++)
       {
       // test if asteroids is alive, if so, draw it, and save it in record

       if (asteroids[index].state)
          {
          // increase active number of blips

          active_blips++;

          // compute screen coordinates of blip

          xb = (test_objects[index].world_pos.x+2500)/63;
          yb = (2500-test_objects[index].world_pos.z)/139;

          // compute color

          if (test_objects[index].world_pos.y<view_point.y)
             cb = 12;
          else
             cb = 9;

          // save the blip

          blip_x[active_blips]=xb;
          blip_y[active_blips]=yb;

          // draw the blip

          Write_Pixel(SCANNER_X+xb,SCANNER_Y+yb,cb);

          } // end if asteroid alive

       } // end for index

   // now process the ship blip

   blip_x[0]=(view_point.x+2500)/63;
   blip_y[0]=(2500-view_point.z)/139;

   Write_Pixel(SCANNER_X+blip_x[0],SCANNER_Y+blip_y[0],10);

   } // end else

} // end Draw_Scanner

// M A I N ////////////////////////////////////////////////////////////////////

void main(int argc,char **argv)
{

int done=0,         // exit flag
    index,          // looping variable
    ship_pitch=0,   // current direction of ship
    ship_yaw=0,
    ship_roll=0,
    ship_speed = 0;

vector_3d unit_z          = {0,0,1,1},  // a unit vector in the Z direction
          ship_direction  = {0,0,1,1};  // the ships direction

asteroid asteroids[NUM_ASTEROIDS];   // the asteroid field
char buffer[80];                     // output string buffer
pcx_picture image_pcx;               // used to load in the background imagery

matrix_4x4 rotate;                   // used to build up rotation matrix

// set 2-D clipping region to take into consideration the instrument panels

poly_clip_min_y  = 0;
poly_clip_max_y  = 121;

// set up viewing and 3D clipping parameters

clip_near_z      = 100,
clip_far_z       = 4000,
viewing_distance = 250;

// turn the damn light up a bit!

ambient_light    = 8;

light_source.x   = 0.918926;
light_source.y   = 0.248436;
light_source.z   = -0.306359;

// build all look up tables

Build_Look_Up_Tables();

// load in small asteroids

for (index=0; index<6; index++)
    {
    if (PLG_Load_Object(&test_objects[index],"pyramid.plg",2))
       printf("\nplg loaded.");
    else
       printf("\nCouldn't load file");

    } // end for index

// load in medium asteroids

for (index=6; index<12; index++)
    {
    if (PLG_Load_Object(&test_objects[index],"diamond.plg",3))
       printf("\nplg loaded.");
    else
       printf("\nCouldn't load file");

    } // end for index

// load in large asteroids

for (index=12; index<18; index++)
    {
    if (PLG_Load_Object(&test_objects[index],"cube.plg",4))
       printf("\nplg loaded.");
    else
       printf("\nCouldn't load file");

    } // end for index

// position and set velocity of all asteroids

for (index=0; index<NUM_ASTEROIDS; index++)
    {
    // set position

    test_objects[index].world_pos.x = -1000 + rand()%2000;
    test_objects[index].world_pos.y = -1000 + rand()%2000;
    test_objects[index].world_pos.z = -1000 + rand()%2000;

    // set velocity

    asteroids[index].xv = -20+rand()%40;
    asteroids[index].yv = -20+rand()%40;
    asteroids[index].zv = -20+rand()%40;

    // set angular rotation rate

    asteroids[index].rx = rand()%10;
    asteroids[index].ry = rand()%10;
    asteroids[index].rz = rand()%10;

    // set state to alive

    asteroids[index].state = 1;

    } // end for

// set graphics to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// read the 3D color palette off disk

Load_Palette_Disk("standard.pal",(RGB_palette_ptr)&color_palette_3d);
Write_Palette(0,255,(RGB_palette_ptr)&color_palette_3d);

// allocate double buffer

Create_Double_Buffer(122);

// load in background image

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blz3dcoc.pcx", (pcx_picture_ptr)&image_pcx,1);
PCX_Show_Buffer((pcx_picture_ptr)&image_pcx);

// install the isr keyboard driver

Keyboard_Install_Driver();

// scan the asteroid field

Draw_Scanner((asteroid_ptr)&asteroids[0],1);

// main event loop

while(!done)
     {

     // compute starting time of this frame

     starting_time = Timer_Query();

     // erase all objects

     // Draw_Poly_List(1);

     Fill_Double_Buffer_32(0);

     // erase the scanner

     Draw_Scanner((asteroid_ptr)&asteroids[0],0);

     // test what user is doing


     // change ship velocity

     if (keyboard_state[MAKE_RGT_BRACKET])
        {
        // speed up

        if ((ship_speed+=5)>50)
            ship_speed=50;

        } // end if speed increase

     if (keyboard_state[MAKE_LFT_BRACKET])
        {
        // slow down

        if ((ship_speed-=5)<-50)
            ship_speed=-50;

        } // end if speed decrease

     // test for turns

     if (keyboard_state[MAKE_RIGHT])
        {
        // rotate ship to right

        if ((ship_yaw+=4)>=360)
           ship_yaw-=360;

        } // end if

     if (keyboard_state[MAKE_LEFT])
        {

        // rotate ship to left

        if ((ship_yaw-=4)<0)
           ship_yaw+=360;

        } // end if

     // test for up and down

     if (keyboard_state[MAKE_UP])
        {

        view_point.y-=25;

        } // end if

     if (keyboard_state[MAKE_DOWN])
        {

        view_point.y+=25;

        } // end if

     // test for exit

     if (keyboard_state[MAKE_ESC])
        done=1;

     // rotate trajectory vector to align with view direction

     Mat_Identity_4x4(rotate);

     rotate[0][0] = ( cos_look[ship_yaw]);
     rotate[0][2] = (-sin_look[ship_yaw]);
     rotate[2][0] = ( sin_look[ship_yaw]);
     rotate[2][2] = ( cos_look[ship_yaw]);

     // x component

     ship_direction.x =

              unit_z.x * rotate[0][0] +
              unit_z.z * rotate[2][0];

     // y component

     ship_direction.y =

              unit_z.y * rotate[1][1];

     // z component

     ship_direction.z =

              unit_z.x * rotate[0][2] +
              unit_z.z * rotate[2][2];

     // move viewpoint based on ship trajectory

     view_point.x+=ship_direction.x*ship_speed;
     view_point.y+=ship_direction.y*ship_speed;
     view_point.z+=ship_direction.z*ship_speed;

     // test ship against universe boundaries

     if (view_point.x>2500)
         view_point.x=-2500;
     else
     if (view_point.x<-2500)
         view_point.x=2500;

     if (view_point.y>2500)
         view_point.y=-2500;
     else
     if (view_point.y<-2500)
         view_point.y=2500;

     if (view_point.z>2500)
         view_point.z=-2500;
     else
     if (view_point.z<-2500)
         view_point.z=2500;

     // set view angles based on trajectory of ship

     view_angle.ang_x = ship_pitch;
     view_angle.ang_y = ship_yaw;
     view_angle.ang_z = ship_roll;

     // rotate asteroids

     for (index=0; index<NUM_ASTEROIDS; index++)
          {
          // rotate object based on rotation rates

          Rotate_Object(&test_objects[index],asteroids[index].rx,
                                             asteroids[index].ry,
                                             asteroids[index].rz);

          } // end for index

     // translate asteroids

     for (index=0; index<NUM_ASTEROIDS; index++)
         {

         // translate each asteroids world position

         test_objects[index].world_pos.x+=asteroids[index].xv;
         test_objects[index].world_pos.y+=asteroids[index].yv;
         test_objects[index].world_pos.z+=asteroids[index].zv;

         // test if the asteroid has gone off the screen anywhere

         if (test_objects[index].world_pos.x > 2500 ||
             test_objects[index].world_pos.x < -2500)
            asteroids[index].xv=-asteroids[index].xv;

         if (test_objects[index].world_pos.y > 2500 ||
             test_objects[index].world_pos.y < -2500)
            asteroids[index].yv=-asteroids[index].yv;

         if (test_objects[index].world_pos.z > 2500 ||
             test_objects[index].world_pos.z < -2500)
            asteroids[index].zv=-asteroids[index].zv;

         }  // end motion

     // now that user has possible moved viewpoint, create the global
     // world to camera transformation matrix

     Create_World_To_Camera();

     // reset the polygon list

     Generate_Poly_List(NULL,RESET_POLY_LIST);

     // perform general 3-D pipeline

     for (index=0; index<NUM_ASTEROIDS; index++)
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

         }  // end for

     // sort the polygons

     Sort_Poly_List();

     // draw the polygon list

     Draw_Poly_List();

     // display the pitch yaw and row of ship

     sprintf(buffer,"%4d",ship_pitch);
     Print_String(80,6,10,buffer,0);

     sprintf(buffer,"%4d",ship_yaw);
     Print_String(144,6,10,buffer,0);

     sprintf(buffer,"%4d",ship_roll);
     Print_String(208,6,10,buffer,0);

     // draw speedo

     Draw_Speedo(ship_speed);

     // draw the scanner

     Draw_Scanner((asteroid_ptr)&asteroids[0],1);

     // display double buffer

     Display_Double_Buffer_32(double_buffer,18);

     // lock onto 18 frames per second max

     while((Timer_Query()-starting_time)<1);

     } // end while

// restore graphics mode back to text

Set_Graphics_Mode(TEXT_MODE);

// restore the old keyboard driver

Keyboard_Remove_Driver();

} // end main



