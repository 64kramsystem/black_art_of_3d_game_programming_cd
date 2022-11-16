
// VOLCANO.C - A demo of multiple data single logic programming

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
#include "black8.h"

// D E F I N E S ////////////////////////////////////////////////////////////

#define MAX_CINDERS        100  // maximum number of cinders in the simulation

#define VOLCANO_MOUTH_X    145  // position where cinders will be emitted
#define VOLCANO_MOUTH_Y    102

#define CINDER_START_COLOR 48     // starting cinder color
#define CINDER_END_COLOR   48+15  // ending cinder color

// S T R U C T U R E S //////////////////////////////////////////////////////

typedef struct cinder_typ
        {
        int x,y;         // position of cinder
        int xv,yv;       // velocity of cinder
        int color;       // color of cinder
        int lifetime;    // total number of frames cinder will live for
        int counter_1;   // tracks when cinder color will change
        int threshold_1; //
        int counter_2;   // tracks when gravity will be applied
        int threshold_2;
        int col;         // the current color of the cinder
        int under;       // this holds the pixel under the cinder

        } cinder, *cinder_ptr;

// G L O B A L S  ////////////////////////////////////////////////////////////

pcx_picture image_pcx;  // general PCX image used to load background and imagery

cinder cinders[MAX_CINDERS];  // this holds all the cinders
int active_cinders;           // nunmbr of active cinders in the world

// F U N C T I O N S /////////////////////////////////////////////////////////

void Init_Cinders(int starting_cinder, int ending_cinder)
{
// this function initializes cinders, it can initialize a sequence of cinders,
// or a single cinder

int index; // looping variable

// intialize each cinder or restart a single cinder

for (index=starting_cinder; index<=ending_cinder; index++)
    {
    // fill in position, velocity and color of cinder

    cinders[index].x = VOLCANO_MOUTH_X - 5 + rand() % 11;
    cinders[index].y = VOLCANO_MOUTH_Y - rand() % 3;

    cinders[index].xv = - 2 + rand() % 5;
    cinders[index].yv = - 5 - rand() % 5;

    cinders[index].col = CINDER_START_COLOR;

    // scan under cinder

    cinders[index].under = Read_Pixel_DB(cinders[index].x,cinders[index].y);

    // set timing fields

    cinders[index].lifetime = 20 + rand()%100;

    cinders[index].counter_1 = 0;
    cinders[index].counter_2 = 0;

    // set how long it will take for cinder to cool

    cinders[index].threshold_1 = 2 + rand()%6;

    // set how long it will take for gravity to take effect

    cinders[index].threshold_2 = 1+rand()%3;

    } // end for index

} // end Init_Cinders

/////////////////////////////////////////////////////////////////////////////

void Erase_Cinders(void)
{
// this function replaces the pixel that was under a cinder

int index; // looping variable

for (index=0; index<active_cinders; index++)
    if (cinders[index].y >=0)
        Write_Pixel_DB(cinders[index].x,cinders[index].y,cinders[index].under);

} // end Erase_Cinders

/////////////////////////////////////////////////////////////////////////////

void Scan_Cinders(void)
{
// this function scan under the cinders

int index; // looping variable

for (index=0; index<active_cinders; index++)
    if (cinders[index].y >=0)
        cinders[index].under = Read_Pixel_DB(cinders[index].x,cinders[index].y);

} // end Scan_Cinders

/////////////////////////////////////////////////////////////////////////////

void Draw_Cinders(void)
{
// this function draws the cinders

int index; // looping variable

for (index=0; index<active_cinders; index++)
    if (cinders[index].y >=0)
        Write_Pixel_DB(cinders[index].x,cinders[index].y,cinders[index].col);

} // end Draw_Cinders

/////////////////////////////////////////////////////////////////////////////

void Move_Cinders(void)
{
// this function moves and updates all the timing fields of the cinder
// it also applies gravity to the cinders

int index, // looping variable
    pixel; // used to read the pixels under the cinder


// process each cinder

for (index=0; index<active_cinders; index++)
    {
    // move the cinder

    cinders[index].x+=cinders[index].xv;
    cinders[index].y+=cinders[index].yv;

    // apply gravity

    if (++cinders[index].counter_2 >= cinders[index].threshold_2)
       {
       // apply a downward velocity of 1
       cinders[index].yv++;

       // reset gravity counter
       cinders[index].counter_2 = 0;

       } // end if time to update gravity

    // test if it's time to update cinder color

    if (++cinders[index].counter_1 >=cinders[index].threshold_1)
       {
       // reset counter
       cinders[index].counter_1 = 0;

       // test if cinder is already out

       if (cinders[index].col < CINDER_END_COLOR)
          cinders[index].col++;

       } // end if time for color change

    // test if cinder is dead, off screen, lifetime over, or hit moutain

    pixel = Read_Pixel_DB(cinders[index].x,cinders[index].y);

    // test if the pixel is part of the mountain

    if (pixel!=0 && (pixel < CINDER_START_COLOR || pixel > CINDER_END_COLOR))
       {
       // restart this cinder

       Init_Cinders(index,index);

       } // end if cinder hit mountain
    else
    if (--cinders[index].lifetime <= 0)
       {
       // restart this cinder

       Init_Cinders(index,index);

       } // end if cinders lifetime expired
    else
    if (cinders[index].x > 320 || cinders[index].x < 0)
       {
       // restart this cinder

       Init_Cinders(index,index);

       } // end if cinder is off screen

    } // end for index

} // end Move_Cinders

// M A I N //////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// create the double buffer

Create_Double_Buffer(200);

// now load the background image

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("volcano.pcx",(pcx_picture_ptr)&image_pcx,1);

// copy PCX image to double buffer

PCX_Copy_To_Buffer((pcx_picture_ptr)&image_pcx,double_buffer);
PCX_Delete((pcx_picture_ptr)&image_pcx);

// draw instructions

Print_String_DB(80,2,9,"Hit any key to exit",1);

// intialize all the cinders

Init_Cinders(0,19);

active_cinders = 20;

// main event loop, process until keyboard hit

while(!kbhit())
     {
     // erase all the cinders coming out of the volcano

     Erase_Cinders();

     // move all the cinders

     Move_Cinders();

     // scan under and draw the cinders

     Scan_Cinders();
     Draw_Cinders();

     // display double buffer

     Display_Double_Buffer(double_buffer,0);

     // lock onto 18 frames per second max

     Time_Delay(1);

     } // end while

// exit in a very cool way

Screen_Transition(SCREEN_DARKNESS);

// free up all resources

Delete_Double_Buffer();

Set_Graphics_Mode(TEXT_MODE);

} // end main

