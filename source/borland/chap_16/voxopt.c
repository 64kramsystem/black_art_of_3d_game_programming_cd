
// VOXOPT.C - Single texture based, ray casted voxel engine

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

#define WORLD_X_SIZE   320  // width of universe
#define WORLD_Y_SIZE   200  // height of universe

// constants used to represent angles for the ray casting a 60 degree field of
// view

#define ANGLE_0        0
#define ANGLE_1        5
#define ANGLE_2        10
#define ANGLE_4        20
#define ANGLE_5        25
#define ANGLE_10       50
#define ANGLE_6        30
#define ANGLE_15       80
#define ANGLE_30       160
#define ANGLE_45       240
#define ANGLE_60       320
#define ANGLE_90       480
#define ANGLE_135      720
#define ANGLE_180      960
#define ANGLE_225      1200
#define ANGLE_270      1440
#define ANGLE_315      1680
#define ANGLE_360      1920

// there are 1920 degrees in our circle or 360/1920 is the conversion factor
// from real degrees to our degrees

#define ANGULAR_INCREMENT  ((float)0.1875)

// conversion constants from radians to degrees and vicversa

#define DEG_TO_RAD         ((float)3.1415926/(float)180)
#define RAD_TO_DEG         ((float)180/(float)3.1415926)

// G L O B A L S  ////////////////////////////////////////////////////////////

pcx_picture image_pcx;        // general pcx image

int play_x=1000,              // the current world x of player
    play_y=1000,              // the current world y of player
    play_z=150,
    play_ang=ANGLE_90,        // the current viewing angle of player
    play_dist = 70,
    mountain_scale=10;        // scaling factor for mountains

float play_dir_x,              // the direction the player is pointing in
      play_dir_y,
      play_dir_z,
      cos_look[ANGLE_360],     // cosine look up table
      sin_look[ANGLE_360],     // sin look up table
      sphere_cancel[ANGLE_60], // cancels fish eye distortion
      ray_length[100];         // holds ray length look up

// F U N C T I O N S ////////////////////////////////////////////////////////

int Initialize(char *filename)
{
// this function builds all the look up tables for the terrain generator and
// loads in the terrain texture map

int ang,           // looping variable
    row,
    row_inv;

float rad_angle;   // current angle in radians

// create sin and cos look up first

for (ang=0; ang<ANGLE_360; ang++)
    {
    // compute current angle in radians

    rad_angle = (float)ang*ANGULAR_INCREMENT*DEG_TO_RAD;

    // now compute the sin and cos

    sin_look[ang] = sin(rad_angle);
    cos_look[ang] = cos(rad_angle);

    } // end for ang

// create inverse cosine viewing distortion filter

for (ang=0; ang<ANGLE_30; ang++)
    {
    // compute current angle in radians

    rad_angle = (float)ang*ANGULAR_INCREMENT*DEG_TO_RAD;

    // now compute the sin and cos

    sphere_cancel[ang+ANGLE_30] = 1/cos(rad_angle);
    sphere_cancel[ANGLE_30-ang] = 1/cos(rad_angle);

    } // end for ang

// create the pre-computed ray length array

for (row = 100; row<150; row++)
    {

    // compute length of ray

    row_inv = 200-row;

    // use the current height and distance to compute length of ray.

    ray_length[row-100] = ((float)(play_dist*play_z)/(float)(play_z-row_inv));

   } // end for row

// intialize the pcx structure

PCX_Init((pcx_picture_ptr)&image_pcx);


// load in the textures

return(PCX_Load(filename, (pcx_picture_ptr)&image_pcx,1));

} // end Initialize

/////////////////////////////////////////////////////////////////////////////

void Draw_Terrain(int play_x,
                  int play_y,
                  int play_z,
                  int play_ang,
                  int play_dist)
{
// this function draws the entire terrain based on the location and orientation
// of the player's viewpoint

int curr_ang,       // current angle being processed
    xr,yr,          // location of ray in world coords
    x_fine,y_fine,  // the texture coordinates the ray hit
    pixel_color,    // the color of textel
    ray,            // looping variable
    row,            // the current video row begin processed
    scale,          // the scale of the current strip
    top,            // top of strip
    last_scale,
    last_top,
    diff,
    index;          // looping variable

float ray_length_final;   // the length of the ray after distortion compensation

unsigned char far *start_offset;  // used by inline line drawer

// start the current angle off -30 degrees to the left of the player's
// current viewing direction

curr_ang = play_ang - ANGLE_30;

// test for underflow

if (curr_ang < 0)
   curr_ang+=ANGLE_360;

// cast a series of rays for every column of the screen

for (ray=1; ray<320; ray++)
    {

    // reset last top and scale

    last_scale = 0;

    last_top = 0;

    // for each column compute the pixels that should be displayed
    // for each screen pixel, process from top to bottom

    for (row = 149; row>=100; row--)
        {

        // use the current height and distance to compute length of ray.

        ray_length_final = sphere_cancel[ray] * ray_length[row-100];

        // rotate ray into position of sample

        xr = (int)((float)play_x + ray_length_final * cos_look[curr_ang]);
        yr = (int)((float)play_y - ray_length_final * sin_look[curr_ang]);

        // compute texture coords

        x_fine = xr % WORLD_X_SIZE;
        y_fine = yr % WORLD_Y_SIZE;

        // using texture index locate texture pixel in textures

        pixel_color = image_pcx.buffer[x_fine + (y_fine<<8) + (y_fine<<6)];

        // draw the strip

        scale = (int)mountain_scale*pixel_color/(int)(ray_length_final+1);

        // test if we need to draw this segment

        if (scale>=last_scale)
           {

           diff = scale-last_scale;

           top  = 50+row-scale;

           // compute starting position

           start_offset = double_buffer + ((top<<8) + (top<<6)) + ray;

           for (index=0; index<=diff; index++)
               {
               // set the pixel

               *start_offset = (unsigned char)pixel_color;

               // move downward to next line

               start_offset+=320;

               } // end for index

            } // end if scale

        last_scale = scale;

        } // end for row

    // move to next angle

    if (++curr_ang >= ANGLE_360)
       curr_ang=ANGLE_0;

    } // end for ray

} // end Draw_Terrain

// M A I N //////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{
char buffer[80];

int done=0; // exit flag

float speed=0;  // speed of player

// check to see if command line parms are correct

if (argc<=2)
   {
   // not enough parms

   printf("\nUsage: voxopt.exe filename.pcx height");
   printf("\nExample: voxopt voxterr4.pcx 10\n");

   // return to DOS

   exit(1);

   } // end if

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// create a double buffer

Create_Double_Buffer(200);

// create look up tables and load textures

if (!Initialize(argv[1]))
   {

   printf("\nError loading file %s",argv[1]);
   exit(1);

   } // end if

// install keyboard driver

Keyboard_Install_Driver();

// set scale of mountains

mountain_scale = atoi(argv[2]);

// draw the first frame

Draw_Terrain(play_x,
             play_y,
             play_z,
             play_ang,
             play_dist);

Display_Double_Buffer(double_buffer,0);

// main event loop

while(!done)
     {

     // reset velocity

     speed = 0;

     // test if user is hitting keyboard

     if (keys_active)
        {
        // what is user trying to do

        // change viewing distance

        if (keyboard_state[MAKE_F])
           play_dist+=10;

        if (keyboard_state[MAKE_C])
           play_dist-=10;

        // change viewing height

        if (keyboard_state[MAKE_U])
           play_z+=10;

        if (keyboard_state[MAKE_D])
           play_z-=10;

        // change viewing position

        if (keyboard_state[MAKE_RIGHT])
            if ((play_ang+=ANGLE_10) >= ANGLE_360)
               play_ang-=ANGLE_360;

        if (keyboard_state[MAKE_LEFT])
            if ((play_ang-=ANGLE_10) < 0)
               play_ang+=ANGLE_360;

        // move foward

        if (keyboard_state[MAKE_UP])
           speed=20;

        // move backward

        if (keyboard_state[MAKE_DOWN])
           speed=-20;

        // exit demo

        if (keyboard_state[MAKE_ESC])
           done=1;


        // compute trajectory vector for this view angle

        play_dir_x = cos_look[play_ang];
        play_dir_y = -sin_look[play_ang];
        play_dir_z = 0;

        // translate viewpoint

        play_x+=speed*play_dir_x;
        play_y+=speed*play_dir_y;
        play_z+=speed*play_dir_z;

        // draw the terrain

        Fill_Double_Buffer(0);

        Draw_Terrain(play_x,
                     play_y,
                     play_z,
                     play_ang,
                     play_dist);

        // draw tactical

        sprintf(buffer,"Height = %d Distance = %d     ",play_z,play_dist);
        Print_String_DB(0,0,10,buffer,0);

        sprintf(buffer,"Pos: X=%d, Y=%d, Z=%d    ",play_x,play_y,play_z);
        Print_String_DB(0,10,10,buffer,0);

        Display_Double_Buffer(double_buffer,0);

        } // end if

     } // end while

// reset back to text mode

Set_Graphics_Mode(TEXT_MODE);

// remove the keyboard handler

Keyboard_Remove_Driver();

} // end main



