
// VOXTILE.C - CELL based, ray casted voxel engine

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

#define NUM_X_CELLS    32    // number of cells in x direction
#define NUM_Y_CELLS    32    // number of cells in y direction

#define CELL_WIDTH     64    // width of a cell in pixels
#define CELL_HEIGHT    64    // height of a cell in pixels

#define WORLD_X_SIZE   (NUM_X_CELLS*CELL_WIDTH)    // width of universe in pixels
#define WORLD_Y_SIZE   (NUM_Y_CELLS*CELL_HEIGHT)   // height of universe in pixels

// constants used to represent angles for the ray casting a 60 degree field of
// view

#define ANGLE_0        0
#define ANGLE_1        5
#define ANGLE_2        10
#define ANGLE_4        20
#define ANGLE_5        25
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

#define NUM_TEXTURES   4   // holds the number of textures in the system

// G L O B A L S  ////////////////////////////////////////////////////////////

pcx_picture image_pcx;        // general pcx image

sprite text_spr;              // this sprite holds the bitmap textures
                              // to drape over each cell

int play_x=1000,              // the current world x of player
    play_y=1000,              // the current world y of player
    play_z=150,               // the current world z of player, same as height
    play_ang=ANGLE_90,        // the current viewing angle of player
    play_dist = 70,
    mountain_scale=10;        // scaling factor for mountains


float play_dir_x,             // the direction the player is pointing in
      play_dir_y,
      play_dir_z,
      cos_look[ANGLE_360],     // cosine look up table
      sin_look[ANGLE_360],     // sin look up table
      sphere_cancel[ANGLE_60]; // cancels fish eye distortion


// this map holds the terrain texture cells
// each cell is really a bitmap texture, so by using cells and tiling much
// larger worlds can be constructed

char terrain_texture[] =

     { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
       1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
       1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
       1,0,0,3,3,0,0,0,0,0,0,1,1,1,0,0,0,1,0,0,3,0,0,0,0,0,0,0,3,0,0,1,
       1,0,0,3,3,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,3,0,0,0,0,0,0,0,3,3,0,1,
       1,0,0,3,3,0,0,0,0,0,0,2,2,1,0,0,0,0,0,0,3,0,0,3,3,0,0,0,3,3,0,1,
       1,0,0,0,3,0,0,2,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,3,3,0,0,0,0,3,0,1,
       1,0,0,0,0,0,0,2,0,2,2,2,0,0,3,0,3,3,3,0,0,0,0,3,3,0,0,0,0,0,0,1,
       1,0,0,0,0,0,0,2,0,0,0,0,0,0,3,0,3,3,3,0,0,0,0,3,0,0,0,0,0,0,0,1,
       1,0,0,2,0,0,0,2,0,0,0,0,0,0,3,0,3,3,0,0,0,0,0,0,0,2,0,0,2,0,0,1,
       1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,1,
       1,0,0,2,0,0,1,1,1,0,0,2,0,2,0,0,1,0,0,0,2,2,2,0,0,0,0,0,0,0,0,1,
       1,0,0,0,0,2,1,1,1,0,0,0,0,0,0,0,1,0,0,0,2,0,2,0,0,0,0,0,0,0,0,1,
       1,0,0,0,0,0,0,1,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
       1,0,0,0,0,2,0,0,0,3,0,0,0,2,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
       1,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
       1,0,2,0,0,0,0,0,3,0,0,0,0,0,3,0,0,0,0,0,1,0,3,0,0,0,0,0,0,0,0,1,
       1,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,2,0,0,1,0,3,0,0,0,2,0,0,0,0,1,
       1,0,0,0,0,2,0,2,0,0,2,0,3,0,0,0,0,2,2,0,1,0,0,0,0,2,0,0,0,0,0,1,
       1,0,2,0,0,2,0,0,0,0,2,0,0,0,3,0,0,2,0,0,0,0,3,3,0,2,2,2,0,0,0,1,
       1,0,0,0,0,2,0,0,0,0,2,0,0,0,3,0,0,0,0,2,0,0,0,0,0,2,0,2,0,0,0,1,
       1,0,0,0,0,2,0,0,0,0,0,3,0,0,0,0,0,0,2,2,0,0,0,3,0,0,2,2,0,0,0,1,
       1,0,0,0,0,0,2,2,2,2,0,0,0,0,3,0,0,0,2,0,0,0,0,0,0,0,2,0,0,0,0,1,
       1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,
       1,0,2,2,0,0,0,0,0,1,1,1,1,1,0,0,0,0,2,2,0,0,0,0,0,0,2,0,0,0,0,1,
       1,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,
       1,0,2,0,0,2,2,0,0,0,3,0,0,3,3,3,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,1,
       1,0,0,2,0,2,2,0,0,0,3,0,0,3,3,3,0,0,0,0,0,0,3,3,3,0,0,0,0,0,0,1,
       1,0,0,0,0,2,2,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,3,3,3,0,0,0,0,0,0,1,
       1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0,0,0,1,
       1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
       1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,

     };

// F U N C T I O N S ////////////////////////////////////////////////////////

void Line_VDB(int y1,int y2,int x,int color)
{
// draw a vertical line, note that a memset function can no longer be
// used since the pixel addresses are no longer contiguous in memory
// note that the end points of the line must be on the screen

unsigned char far *start_offset; // starting memory offset of line

int index, // loop index
    temp;  // used for temporary storage during swap

// make sure y2 > y1

if (y1>y2)
   {
   temp = y1;
   y1   = y2;
   y2   = temp;
   } // end swap

// compute starting position

start_offset = double_buffer + ((y1<<8) + (y1<<6)) + x;

for (index=0; index<=y2-y1; index++)
    {
    // set the pixel

    *start_offset = (unsigned char)color;

    // move downward to next line

    start_offset+=320;

    } // end for index

} // end Line_VDB

//////////////////////////////////////////////////////////////////////////////

void Initialize(void)
{
// this function builds all the look up tables for the terrain generator

int ang,           // looping variable
    index;         // looping variable

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


// load the texture tiles into memory

// intialize the pcx structure

PCX_Init((pcx_picture_ptr)&image_pcx);

// load in the textures

PCX_Load("voxtext.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the texture sprite

Sprite_Init((sprite_ptr)&text_spr,0,0,64,64,0,0,0,0,0,0);

// extract the bitmaps for the textures

for (index=0; index<NUM_TEXTURES; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&text_spr,index,index,0);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);


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

int curr_ang,               // current angle being processed
    xr,yr,                  // location of ray in world coords
    x_cell,y_cell,          // the cell the ray hit
    x_fine,y_fine,          // the texture coordinates the ray hit
    texture_index,          // which texture
    pixel_color,            // the color of textel
    ray,                    // looping variable
    row,                    // the current video row begin processed
    row_inv,                // the inverted row to make upward positive
    scale,                  // the scale of the current strip
    top,            // top of strip
    bottom;         // bottom of strip


float ray_length;           // the length of the ray after distortion compensation

// start the current angle off -30 degrees to the left of the player's
// current viewing direction

curr_ang = play_ang - ANGLE_30;

// test for underflow

if (curr_ang < 0)
   curr_ang+=ANGLE_360;

// cast a series of rays for every column of the screen

for (ray=1; ray<320; ray++)
    {

    // for each column compute the pixels that should be displayed
    // for each screen pixel, process from top to bottom

    for (row = 100; row<150; row++)
        {
        // compute length of ray

        row_inv = 200-row;

        // use the current height and distance to compute length of ray.

        ray_length = sphere_cancel[ray] * ((float)(play_dist*play_z)/
                                           (float)(play_z-row_inv));

        // rotate ray into position of sample

        xr = (int)((float)play_x + ray_length * cos_look[curr_ang]);
        yr = (int)((float)play_y - ray_length * sin_look[curr_ang]);

        // compute cell of sampling tip

        x_cell = xr >> 5; //  / NUM_X_CELLS;
        y_cell = yr >> 5; //  / NUM_Y_CELLS;

        // compute texture coords

        x_fine = xr & 0x003F; // % CELL_WIDTH;
        y_fine = yr & 0x003F; // % CELL_HEIGHT;

        // locate texture index

        texture_index = terrain_texture[x_cell + (y_cell<<5)]; // *NUM_X_CELLS];

        // using texture index locate texture pixel in textures

        pixel_color = text_spr.frames[texture_index][x_fine + (y_fine<<6)]; // *CELL_WIDTH];

        // draw the strip

        scale = (int)mountain_scale*pixel_color/(int)(ray_length+1);

        top        = 50+row-scale;

        bottom     = top + scale;

        Line_VDB(top,bottom,ray,pixel_color);

        } // end for row

    // move to next angle

    if (++curr_ang >= ANGLE_360)
       curr_ang=ANGLE_0;

    } // end for ray

} // end Draw_Terrain

// M A I N //////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{

char buffer[80]; // text output buffer

int done=0;      // exit flag

float speed=0;   // speed of player

// check to see if command line parms are correct

if (argc<=1)
   {
   // not enough parms

   printf("\nUsage: voxtile.exe height");
   printf("\nExample: voxtile 15\n");

   // return to DOS

   exit(1);

   } // end if

// set scale of mountains

mountain_scale = atoi(argv[1]);

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// install keyboard driver

Keyboard_Install_Driver();

// build the look up tables and load textures

Initialize();

// create a double buffer

Create_Double_Buffer(200);

// display terrain manually first time

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
            if ((play_ang+=ANGLE_5) >= ANGLE_360)
               play_ang-=ANGLE_360;

        if (keyboard_state[MAKE_LEFT])
            if ((play_ang-=ANGLE_5) < 0)
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


        // print out tactical

        sprintf(buffer,"Height = %d Distance = %d     ",play_z,play_dist);
        Print_String_DB(0,0,10,buffer,0);

        sprintf(buffer,"Pos: X=%d, Y=%d, Z=%d    ",play_x,play_y,play_z);
        Print_String_DB(0,10,10,buffer,0);

        // show buffer

        Display_Double_Buffer(double_buffer,0);

        } // end if

     } // end while

// reset back to text mode

Set_Graphics_Mode(TEXT_MODE);

// remove the keyboard handler

Keyboard_Remove_Driver();

} // end main



