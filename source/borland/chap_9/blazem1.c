
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

#include "blazem0.h"

////////////////////////////////////////////////////////////////////////////////

int Get_Line(char *buffer)
{
// this function implements a crude line editor, it's used to input strings
// from keyboard

int index=0,
    ch;

// get the input string

while(1)
     {

     // has user hiy a key?

     if (kbhit())
        {
        // make a sound

        Digital_FX_Play(BLZKEY_VOC,2);

        // get the key

        ch = getch();

        // test for a numeric character

        if (ch>='0' && ch<='9')
           {

           buffer[index]   = ch;
           buffer[index+1] = 0;

           Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2+8,0,0,
                         buffer,
                         video_buffer);

           // test if end of line reached

           if (++index==12)
              index=11;

           } // end if numeric
        else
        if (ch==13)  // test for enter
           {
           // user is done, so exit

           // null terminate

           buffer[index] = 0;

           return(1);

           } // end if carriage return
        else
        if (ch==8 || ch==127) // back space or delete
           {
           if (--index <0)
              index=0;

           buffer[index]  =' ';
           buffer[index+1]=0;

           Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2+8,0,0,
                         buffer,
                         video_buffer);

           buffer[index]=0;

           // erase the character

           } // end if backspace
        else
        if (ch==27)
           return(0);

        } // end if kbhit

     } // end while

} // end Get_Line

//////////////////////////////////////////////////////////////////////////////

void Init_Novas(void)
{
// this function initializes all the super novas

int index_n, // looping variables
    index_c;

// process each nova

for (index_n=0; index_n<NUM_NOVAS; index_n++)
    {

    // set all novas to inactive

    novas[index_n].state    = NOVA_INACTIVE;
    novas[index_n].lifetime = 0;

    // intialize each cinder

    for (index_c=0; index_c<NUM_CINDERS; index_c++)
        {
        // clear out all fields

        novas[index_n].cinders[index_c].x = 0;
        novas[index_n].cinders[index_c].y = 0;

        novas[index_n].cinders[index_c].xv = 0;
        novas[index_n].cinders[index_c].yv = 0;

        novas[index_n].cinders[index_c].sx = 0;
        novas[index_n].cinders[index_c].sy = 0;

        novas[index_n].cinders[index_c].color = 0;
        novas[index_n].cinders[index_c].back_color = 0;

        // set timing fields

        novas[index_n].cinders[index_c].lifetime = 0;

        novas[index_n].cinders[index_c].counter   = 0;
        novas[index_n].cinders[index_c].threshold = 0;

        } // end for index

    } // end for index_n

} // end Init_Novas

/////////////////////////////////////////////////////////////////////////////

void Erase_Novas(void)
{
// this function replaces the what was under the novas

int index_n, // looping variables
    index_c;

particle_ptr bits;  // used as a high speed alias into cinders array

// process each nova

for (index_n=0; index_n<NUM_NOVAS; index_n++)
    {

    // is this nova active ?

    if (novas[index_n].state==NOVA_ACTIVE)
       {

       // alias a pointer to cinder array for speed

       bits = (particle_ptr)novas[index_n].cinders;

       // process each cinder

       for (index_c=0; index_c<NUM_CINDERS; index_c++)
           {

           // is this cinder visible?

           if (bits[index_c].visible)
              Write_Pixel_DB(bits[index_c].sx,bits[index_c].sy,
                             bits[index_c].back_color);

           } // end for index_c

       } // end if nova active

    } // end for index_n

} // end Erase_Novas

/////////////////////////////////////////////////////////////////////////////

void Under_Novas(void)
{

// this function scans whats under a nova

int index_n, // looping variables
    index_c,
    px_window,   // the starting postion of the players window
    py_window,
    cx,cy;       // local aliases for speed

particle_ptr bits;  // used as a high speed alias into cinders array

// compute starting position of players window so screen mapping can be done

px_window = players_x - 160+11;
py_window = players_y - 100+9;

// process each nova

for (index_n=0; index_n<NUM_NOVAS; index_n++)
    {

    // is this nova active ?

    if (novas[index_n].state==NOVA_ACTIVE)
       {

       // alias a pointer to cinder array for speed

       bits = (particle_ptr)novas[index_n].cinders;

       // process each cinder

       for (index_c=0; index_c<NUM_CINDERS; index_c++)
           {

           cx=bits[index_c].sx = (bits[index_c].x - px_window);
           cy=bits[index_c].sy = (bits[index_c].y - py_window);

           // test if cinder is visible on screen?

           if (cx>=320 || cx <0 || cy>=200  || cy<0)
              {

              // this cindere is invisible and has been clipped

              bits[index_c].visible = 0;

              // process next cinder

              continue;

              } // end visibility test

           // scan under cinder

           bits[index_c].back_color = Read_Pixel_DB(cx,cy);

           // set visibility flag

           bits[index_c].visible = 1;

           } // end for index_c

       } // end if nova active

    } // end for index_n

} // end Under_Novas

/////////////////////////////////////////////////////////////////////////////

void Draw_Novas(void)
{

// this function draws the novas

int index_n, // looping variables
    index_c;

particle_ptr bits;  // used as a high speed alias into cinders array

// process each nova

for (index_n=0; index_n<NUM_NOVAS; index_n++)
    {

    // is this nova active ?

    if (novas[index_n].state==NOVA_ACTIVE)
       {

       // alias a pointer to cinder array for speed

       bits = (particle_ptr)novas[index_n].cinders;

       // process each cinder

       for (index_c=0; index_c<NUM_CINDERS; index_c++)
           {

           // is this cinder visible?

           if (bits[index_c].visible)
              Write_Pixel_DB(bits[index_c].sx,bits[index_c].sy,
                             bits[index_c].color);

           } // end for index_c

       } // end if nova active

    } // end for index_n

} // end Draw_Novas

/////////////////////////////////////////////////////////////////////////////

void Move_Novas(void)
{

// this function draws the novas

int index_n, // looping variables
    index_c;

particle_ptr bits;  // used as a high speed alias into cinders array

// process each nova

for (index_n=0; index_n<NUM_NOVAS; index_n++)
    {

    // is this nova active ?

    if (novas[index_n].state==NOVA_ACTIVE)
       {

       // alias a pointer to cinder array for speed

       bits = (particle_ptr)novas[index_n].cinders;

       // process each cinder

       for (index_c=0; index_c<NUM_CINDERS; index_c++)
           {
           // move the cinder

           bits[index_c].x+=bits[index_c].xv;
           bits[index_c].y+=bits[index_c].yv;

           // animate the cinder color

           if (++bits[index_c].counter > bits[index_c].threshold)
              {
              // decrement color

              if (++bits[index_c].color >= CINDER_END_COLOR)
                 bits[index_c].color = CINDER_END_COLOR;

              // reset counter

              bits[index_c].counter = 0;

              } // end if color change

           } // end for index_c

       // age the nova

       if (--novas[index_n].lifetime<=0)
          novas[index_n].state=NOVA_INACTIVE;

       } // end if nova active

    } // end for index_n

} // end Move_Novas

///////////////////////////////////////////////////////////////////////////////

void Start_Nova(int x,int y)
{
// this function starts a super nova explosion

int index_c, // loopnig variables
    index_n;


// hunt for an inactive nove

for (index_n=0; index_n<NUM_NOVAS; index_n++)
    {

    // test for inactive nova

    if (novas[index_n].state==NOVA_INACTIVE)
       {

       // activate nova

       novas[index_n].state = NOVA_ACTIVE;

       // set lifetime in frames

       novas[index_n].lifetime = 50 + rand()%20;

       // intialize each cinder or restart a single cinder

       for (index_c=0; index_c<NUM_CINDERS; index_c++)
           {
           // fill in position, velocity and color of cinder

           novas[index_n].cinders[index_c].x   = x;
           novas[index_n].cinders[index_c].y   = y;
           novas[index_n].cinders[index_c].sx  = 0;
           novas[index_n].cinders[index_c].sy  = 0;
           novas[index_n].cinders[index_c].xv  = -8 + rand()%16;
           novas[index_n].cinders[index_c].yv  = -8 + rand()%16;

           novas[index_n].cinders[index_c].color      = CINDER_START_COLOR;
           novas[index_n].cinders[index_c].back_color = 0;

           // set timing fields

           novas[index_n].cinders[index_c].counter = 0;
           novas[index_n].cinders[index_c].threshold = 2 + rand()%6;

           } // end for index_c

           // make sound

           Digital_FX_Play(BLZEXP2_VOC,0);

       // break out of out for

       break;

       } // end if nova found

    } // end for index_n

} // end Start_Nova

//////////////////////////////////////////////////////////////////////////////

void Line_H_2(int x1,int x2,int y,int color,unsigned char far *dest)
{
// draw a horizontal line to the destination buffer

_fmemset((char far *)(dest + ((y<<8) + (y<<6)) + x1),
         (unsigned char)color,x2-x1+1);

} // end Line_H_2

//////////////////////////////////////////////////////////////////////////////

void Line_V_2(int y1,int y2,int x,int color, unsigned char far *dest)
{
// draw a vertical line to destination buffer

unsigned char far *start_offset; // starting memory offset of line

int index; // loop index

// compute starting position

start_offset = dest + ((y1<<8) + (y1<<6)) + x;

for (index=0; index<=y2-y1; index++)
    {
    // set the pixel

    *start_offset = (unsigned char)color;

    // move downward to next line

    start_offset+=320;

    } // end for index

} // end Line_V_2

//////////////////////////////////////////////////////////////////////////////

void Init_Stars(void)
{
// this function initializes all the stars in the star field

int index; // looping variable

for (index=0; index<NUM_STARS; index++)
    {

    // select plane that star will be in

    switch(rand()%3)
          {
          case STAR_PLANE_0:
               {
               stars[index].color = STAR_COLOR_0;
               stars[index].plane = STAR_PLANE_0;
               } break;

          case STAR_PLANE_1:
               {
               stars[index].color = STAR_COLOR_1;
               stars[index].plane = STAR_PLANE_1;
               } break;

          case STAR_PLANE_2:
               {
               stars[index].color = STAR_COLOR_2;
               stars[index].plane = STAR_PLANE_2;
               } break;

          default:break;

          } // end switch

     // set fields that aren't plane specific

     stars[index].x = rand()%320;  // change this latter to reflect clipping
     stars[index].y = rand()%200;  // region
     stars[index].back_color = 0;

    } // end for index

} // end Init_Stars

/////////////////////////////////////////////////////////////////////////////

void Move_Stars(void)
{
// this function moves the star field, note that the star field is always
// ins screen coordinates, otherwise, we would need thousands of stars to
// fill up the universe instead of 50!

int index,   // looping variable
    star_x,  // used as fast aliases to star position
    star_y,
    plane_0_dx,
    plane_0_dy,
    plane_1_dx,
    plane_1_dy,
    plane_2_dx,
    plane_2_dy;

// pre-compute plane translations

plane_0_dx = players_dx >> 2;
plane_0_dy = players_dy >> 2;

plane_1_dx = players_dx >> 1;
plane_1_dy = players_dy >> 1;

plane_2_dx = players_dx;
plane_2_dy = players_dy;

// move all the stars based on the motion of the player

for (index=0; index<NUM_STARS; index++)
    {
    // locally cache star position to speed up calculations

    star_x = stars[index].x;
    star_y = stars[index].y;

    // test which star field star is in so it is translated with correct
    // perspective

    switch(stars[index].plane)
          {
          case STAR_PLANE_0:
               {
               // move the star based on differntial motion of player
               // far plane is divided by 4

               star_x+=plane_0_dx;
               star_y+=plane_0_dy;

               } break;

          case STAR_PLANE_1:
               {
               // move the star based on differntial motion of player
               // middle plane is divided by 2

               star_x+=plane_1_dx;
               star_y+=plane_1_dy;

               } break;

          case STAR_PLANE_2:
               {
               // move the star based on differntial motion of player
               // near plane is divided by 1

               star_x+=plane_2_dx;
               star_y+=plane_2_dy;

               } break;

          } // end switch plane

    // test if a star has flown off an edge

    if (star_x >= 320)
        star_x = star_x-320;
    else
    if (star_x < 0)
        star_x = 320 + star_x;

    if (star_y >= 200)
        star_y = star_y-200;
    else
    if (star_y < 0)
        star_y = 200+star_y;

    // reset stars position in structure

    stars[index].x = star_x;
    stars[index].y = star_y;

    } // end for index

} // end Move_Stars

/////////////////////////////////////////////////////////////////////////////

void Draw_Stars(void)
{
// this function draws all the stars

int index; // looping variable

for (index=0; index<NUM_STARS; index++)
    Write_Pixel_DB(stars[index].x,stars[index].y,stars[index].color);

} // end Draw_Stars

/////////////////////////////////////////////////////////////////////////////

void Erase_Stars(void)
{
// this function erases all the stars

int index; // looping variable

for (index=0; index<NUM_STARS; index++)
    Write_Pixel_DB(stars[index].x,stars[index].y,stars[index].back_color);

} // end Erase_Stars

/////////////////////////////////////////////////////////////////////////////

void Under_Stars(void)
{
// this function scans under each star

int index; // looping variable

for (index=0; index<NUM_STARS; index++)
    stars[index].back_color = Read_Pixel_DB(stars[index].x,stars[index].y);

} // end Under_Stars

/////////////////////////////////////////////////////////////////////////////

void Init_Asteroids(int small, int medium, int large)
{
// this function loads in the imagery for the asteroids, allocates all the
// memory for them, sets up all the fields and starts them at random positions

int index,  // looping variables
    frame;

static int first_time=1;  // used to track first time this function is called

// intialize all the large asteroid sprites

for (index=START_LARGE_ASTEROIDS; index<=END_LARGE_ASTEROIDS; index++)
    {

    if (first_time)
    Sprite_Init((sprite_ptr)&asteroids[index].rock,
                0,0,
                ASTEROID_LARGE_WIDTH,ASTEROID_LARGE_HEIGHT,
                0,0,0,0,0,0);

    // set position,velocity and type fields

    asteroids[index].xv         = -8 + rand()%16;
    asteroids[index].yv         = -8 + rand()%16;
    asteroids[index].x          = rand()%UNIVERSE_WIDTH;
    asteroids[index].y          = rand()%UNIVERSE_HEIGHT,

    asteroids[index].type       = ASTEROID_LARGE;
    asteroids[index].rock.state = ASTEROID_INACTIVE;

    // use this to control rotation rate

    asteroids[index].rock.threshold_1 = 1 + rand()%3;

    } // end for index

// intialize all the medium asteroid sprites

for (index=START_MEDIUM_ASTEROIDS; index<=END_MEDIUM_ASTEROIDS; index++)
    {

    if (first_time)
    Sprite_Init((sprite_ptr)&asteroids[index].rock,
                0,0,
                ASTEROID_MEDIUM_WIDTH,ASTEROID_MEDIUM_HEIGHT,
                0,0,0,0,0,0);

    // set velocity and type fields

    asteroids[index].xv         = -6 + rand()%12;
    asteroids[index].yv         = -6 + rand()%12;
    asteroids[index].x          = rand()%UNIVERSE_WIDTH;
    asteroids[index].y          = rand()%UNIVERSE_HEIGHT,
    asteroids[index].type       = ASTEROID_MEDIUM;
    asteroids[index].rock.state = ASTEROID_INACTIVE;

    // use this to control rotation rate

    asteroids[index].rock.threshold_1 = 1 + rand()%3;

    } // end for index

// intialize all the small asteroid sprites

for (index=START_SMALL_ASTEROIDS; index<=END_SMALL_ASTEROIDS; index++)
    {

    if (first_time)
    Sprite_Init((sprite_ptr)&asteroids[index].rock,
                0,0,
                ASTEROID_SMALL_WIDTH,ASTEROID_SMALL_HEIGHT,
                0,0,0,0,0,0);

    // set velocity and type fields

    asteroids[index].xv         = -4 + rand()%8;
    asteroids[index].yv         = -4 + rand()%8;
    asteroids[index].x          = rand()%UNIVERSE_WIDTH;
    asteroids[index].y          = rand()%UNIVERSE_HEIGHT,
    asteroids[index].type       = ASTEROID_SMALL;
    asteroids[index].rock.state = ASTEROID_INACTIVE;

    // use this to control rotation rate

    asteroids[index].rock.threshold_1 = 1 + rand()%3;

    } // end for index

// now load the imagery for the large asteroids

if (first_time)
{

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazelas.pcx", (pcx_picture_ptr)&image_pcx,1);

// extract the bitmaps for the asteroid, there are 8 animation cells

for (index=0; index<NUM_ASTEROID_FRAMES; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,
                   (sprite_ptr)&asteroids[START_LARGE_ASTEROIDS].rock,
                   index,index,0);

// now alias pointers of remaining asteroids sprites to same data that
// was allocated for the first large asteroids, this saves a lot of memory!

for (index=START_LARGE_ASTEROIDS+1; index<=END_LARGE_ASTEROIDS; index++)
    {
    // alias all the frame image pointer of this asteroid to the frames
    // of the first asteroid, no need to replicate this data in memory!

    for (frame=0; frame<NUM_ASTEROID_FRAMES; frame++)
        {
        // the image frames are within the sprite which is called rock which
        // is within the asteroid structure

        asteroids[index].rock.frames[frame] =
                asteroids[START_LARGE_ASTEROIDS].rock.frames[frame];

        } // end for frame

    // set number of frames field

    asteroids[index].rock.num_frames =
            asteroids[START_LARGE_ASTEROIDS].rock.num_frames;

    } // end for index

// delete the pcx file

PCX_Delete((pcx_picture_ptr)&image_pcx);


// now load the imagery for the medium asteroids

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazemas.pcx", (pcx_picture_ptr)&image_pcx,1);

// extract the bitmaps for the asteroid, there are 8 animation cells

for (index=0; index<NUM_ASTEROID_FRAMES; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,
                   (sprite_ptr)&asteroids[START_MEDIUM_ASTEROIDS].rock,
                   index,index,0);

// now alias pointers of remaining asteroids sprites to same data that
// was allocated for the first large asteroids, this saves a lot of memory!

for (index=START_MEDIUM_ASTEROIDS+1; index<=END_MEDIUM_ASTEROIDS; index++)
    {
    // alias all the frame image pointer of this asteroid to the frames
    // of the first asteroid, no need to replicate this data in memory!

    for (frame=0; frame<NUM_ASTEROID_FRAMES; frame++)
        {
        // the image frames are within the sprite which is called rock which
        // is within the asteroid structure

        asteroids[index].rock.frames[frame] =
                       asteroids[START_MEDIUM_ASTEROIDS].rock.frames[frame];

        } // end for frame

    // set number of frames field

    asteroids[index].rock.num_frames =
            asteroids[START_MEDIUM_ASTEROIDS].rock.num_frames;

    } // end for index

// delete the pcx file

PCX_Delete((pcx_picture_ptr)&image_pcx);

// finally load the imagery for the small asteroids

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazesas.pcx", (pcx_picture_ptr)&image_pcx,1);

// extract the bitmaps for the asteroid, there are 8 animation cells

for (index=0; index<NUM_ASTEROID_FRAMES; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,
                   (sprite_ptr)&asteroids[START_SMALL_ASTEROIDS].rock,
                   index,index,0);

// now alias pointers of remaining asteroids sprites to same data that
// was allocated for the first large asteroids, this saves a lot of memory!

for (index=START_SMALL_ASTEROIDS+1; index<=END_SMALL_ASTEROIDS; index++)
    {
    // alias all the frame image pointer of this asteroid to the frames
    // of the first asteroid, no need to replicate this data in memory!

    for (frame=0; frame<NUM_ASTEROID_FRAMES; frame++)
        {
        // the image frames are within the sprite which is called rock which
        // is within the asteroid structure

        asteroids[index].rock.frames[frame] =
                       asteroids[START_SMALL_ASTEROIDS].rock.frames[frame];

        } // end for frame

    // set number of frames field

    asteroids[index].rock.num_frames =
            asteroids[START_SMALL_ASTEROIDS].rock.num_frames;

    } // end for index

// delete the pcx file

PCX_Delete((pcx_picture_ptr)&image_pcx);

} // end if first time



// now start up the requested number of asteroids

// first the large

for (index=0; index<large; index++)
    {
    // look for inactive asteroids to start up

    if (asteroids[index+START_LARGE_ASTEROIDS].rock.state == ASTEROID_INACTIVE)
        asteroids[index+START_LARGE_ASTEROIDS].rock.state = ASTEROID_ACTIVE;

    } // end for index

// now the medium

for (index=0; index<medium; index++)
    {
    // look for inactive asteroids to start up

    if (asteroids[index+START_MEDIUM_ASTEROIDS].rock.state == ASTEROID_INACTIVE)
        asteroids[index+START_MEDIUM_ASTEROIDS].rock.state = ASTEROID_ACTIVE;

    } // end for index

// finally the small

for (index=0; index<small; index++)
    {
    // look for inactive asteroids to start up

    if (asteroids[index+START_SMALL_ASTEROIDS].rock.state == ASTEROID_INACTIVE)
        asteroids[index+START_SMALL_ASTEROIDS].rock.state = ASTEROID_ACTIVE;

    } // end for index

// test if this was the first tmie the function was called

if (first_time)
    first_time=0;

// what a pain!

} // end Init_Asteroids

/////////////////////////////////////////////////////////////////////////////

void Start_Asteroid(int x,int y,int type)
{
// this function is used to start up an asteroid at the sent position
// later possible implement velocity?

int index;  // looping variable

// which kind of asteroid is being requested?

switch(type)
      {
      case ASTEROID_LARGE:
           {
           // scan for inactive asteroid

           for (index=START_LARGE_ASTEROIDS; index<=END_LARGE_ASTEROIDS; index++)
               {
               // is this asteroid being used?

               if (asteroids[index].rock.state==ASTEROID_INACTIVE)
                  {
                  // set fields of asteroid and return
                  asteroids[index].xv         = -8 + rand()%16;
                  asteroids[index].yv         = -8 + rand()%16;
                  asteroids[index].x          = x;
                  asteroids[index].y          = y,
                  asteroids[index].rock.state = ASTEROID_ACTIVE;
                  return;

                  } // end if found an asteroid

               } // end for index

           } break;

      case ASTEROID_MEDIUM:
           {
           // scan for inactive asteroid

           for (index=START_MEDIUM_ASTEROIDS; index<=END_MEDIUM_ASTEROIDS; index++)
               {
               // is this asteroid being used?

               if (asteroids[index].rock.state==ASTEROID_INACTIVE)
                  {
                  // set fields of asteroid and return
                  asteroids[index].xv         = -6 + rand()%12;
                  asteroids[index].yv         = -6 + rand()%12;
                  asteroids[index].x          = x;
                  asteroids[index].y          = y,
                  asteroids[index].rock.state = ASTEROID_ACTIVE;
                  return;

                  } // end if found an asteroid

               } // end for index

           } break;

      case ASTEROID_SMALL:
           {
           // scan for inactive asteroid

           for (index=START_SMALL_ASTEROIDS; index<=END_SMALL_ASTEROIDS; index++)
               {
               // is this asteroid being used?

               if (asteroids[index].rock.state==ASTEROID_INACTIVE)
                  {
                  // set fields of asteroid and return

                  asteroids[index].xv         = -4 + rand()%8;
                  asteroids[index].yv         = -4 + rand()%8;
                  asteroids[index].x          = x;
                  asteroids[index].y          = y,
                  asteroids[index].rock.state = ASTEROID_ACTIVE;
                  return;

                  } // end if found an asteroid

               } // end for index

           } break;

      } // end swtich

} // end Start_Asteroid

//////////////////////////////////////////////////////////////////////////////

void Erase_Asteroids(void)
{
// this function traverses the asteroid list and erases all asteroids that
// are active

int index;

// erase all asteroids that are within screen window

for (index=0; index<NUM_ASTEROIDS; index++)
    {
    // test if this asteroids is active

    if (asteroids[index].rock.state==ASTEROID_ACTIVE)
       Sprite_Erase_Clip((sprite_ptr)&asteroids[index].rock,double_buffer);

    } // end for index

} // Erase_Asteroids

//////////////////////////////////////////////////////////////////////////////

void Draw_Asteroids(void)
{
// this function traverses the asteroid list and draws all asteroids that
// are active

int index;

for (index=0; index<NUM_ASTEROIDS; index++)
    {
    // test if this asteroids is active

    if (asteroids[index].rock.state==ASTEROID_ACTIVE)
        Sprite_Draw_Clip((sprite_ptr)&asteroids[index].rock,double_buffer,1);

    } // end for index

} // Draw_Asteroids

//////////////////////////////////////////////////////////////////////////////

void Under_Asteroids(void)
{

// this function traverses the asteroid list and scans under all asteroids that
// are active, note that this function is the only one that computes the screen
// coordinates of the asteroid sprites, placing the computation in the other
// functions would be redundant, hence the sprite coordinates at this function
// are undefined, and after this function they have been remapped to the video
// screen relative to the players position


int index,
    px_window,   // the starting postion of the players window
    py_window;

// compute starting position of players window so screen mapping can be done

px_window = players_x - 160+11;
py_window = players_y - 100+9;

// now scan under all asteroids

for (index=0; index<NUM_ASTEROIDS; index++)
    {
    // test if this asteroids is active

    if (asteroids[index].rock.state==ASTEROID_ACTIVE)
       {

       // postion asteroid correctly on view screen, note this is very similar
       // to what we will do in 3-D when we translate all the objects in the
       // universe to the viewer position

       asteroids[index].rock.x = (asteroids[index].x-px_window);
       asteroids[index].rock.y = (asteroids[index].y-py_window);

       Sprite_Under_Clip((sprite_ptr)&asteroids[index].rock,double_buffer);

       } // end if active

    } // end for index

} // Under_Asteroids

//////////////////////////////////////////////////////////////////////////////

void Move_Asteroids(void)
{

// this function traverses the asteroid list and moves and test for collsions
// note that the sprite positions of the asteroids are not touched only the
// universe or "world" coordinates are

int index,         // looping variable
    ast_x,ast_y;   // used for aliasing

// process each asteroid

for (index=0; index<NUM_ASTEROIDS; index++)
    {
    // test if this asteroids is active

    if (asteroids[index].rock.state==ASTEROID_ACTIVE)
       {
       // move the asteroid

       ast_x=asteroids[index].x;
       ast_y=asteroids[index].y;

       ast_x+=asteroids[index].xv;
       ast_y+=asteroids[index].yv;

       // test if asteroid is off screen bounadary

       if (ast_x>UNIVERSE_WIDTH+UNIVERSE_BORDER)
           ast_x = -UNIVERSE_BORDER;
       else
       if (ast_x<-UNIVERSE_BORDER)
           ast_x = UNIVERSE_WIDTH+UNIVERSE_BORDER;

       if (ast_y>UNIVERSE_HEIGHT+UNIVERSE_BORDER)
           ast_y = -UNIVERSE_BORDER;
       else
       if (ast_y<-UNIVERSE_BORDER)
           ast_y = UNIVERSE_HEIGHT+UNIVERSE_BORDER;

       // restore variables in structure

       asteroids[index].x = ast_x;
       asteroids[index].y = ast_y;

       // animate asteroid

       if (++asteroids[index].rock.counter_1>=asteroids[index].rock.threshold_1)
          {
          // reset counter

          asteroids[index].rock.counter_1 = 0;

          if (++asteroids[index].rock.curr_frame >= NUM_ASTEROID_FRAMES)
              asteroids[index].rock.curr_frame = 0;

          } // end if time to animate

       // test if asteroid has hit player

       // test for collision

       if (players_x+SHIP_WIDTH/2  >= ast_x                             &&
           players_y+SHIP_HEIGHT/2 >= ast_y                             &&
           players_x+SHIP_WIDTH/2  <= ast_x+asteroids[index].rock.width &&
           players_y+SHIP_HEIGHT/2 <= ast_y+asteroids[index].rock.height)
          {
          // kill the asteroid and the missile

          asteroids[index].rock.state = ASTEROID_INACTIVE;

          // what kind of asteroid did we have?

          switch(asteroids[index].type)
                {

                case ASTEROID_LARGE:
                     {

                     // start one medium and one small asteroid
                     //  (if possible)

                     Start_Asteroid(asteroids[index].x,
                                    asteroids[index].y,
                                    ASTEROID_MEDIUM);

                     Start_Asteroid(asteroids[index].x+ASTEROID_LARGE_WIDTH/2,
                                    asteroids[index].y+ASTEROID_LARGE_HEIGHT/2,
                                    ASTEROID_SMALL);

                     } break;

                case ASTEROID_MEDIUM:
                     {

                     // start two small asteroids (if possible)

                     Start_Asteroid(asteroids[index].x,
                                    asteroids[index].y,
                                    ASTEROID_SMALL);

                     Start_Asteroid(asteroids[index].x+ASTEROID_MEDIUM_WIDTH/2,
                                    asteroids[index].y+ASTEROID_MEDIUM_HEIGHT/2,
                                    ASTEROID_SMALL);

                     } break;

                case ASTEROID_SMALL:
                     {

                     // start a randomly positioned asteroid of any size
                     // at the worm hole

                     Start_Asteroid(WORMHOLE_X,WORMHOLE_Y,rand()%3);

                     } break;

                default:break;

                } // end switch

          // were shields up?

          if (players_shields)
             {
             // start explosion
             Start_Explosion(players_x,players_y,2);
             } // end if shields up
          else
             { // say bye bye!

             Start_Explosion(players_x,players_y,2);
             Start_Nova(players_x+SHIP_WIDTH/2,players_y+SHIP_HEIGHT/2);

             // start players death sequence

             Start_Players_Death();

             } // end if else

          } // end if player has been hit

       // test if asteroid has hit remote

       // test for collision

       if (linked)
       {
       if (remotes_x+SHIP_WIDTH/2  >= ast_x                             &&
           remotes_y+SHIP_HEIGHT/2 >= ast_y                             &&
           remotes_x+SHIP_WIDTH/2  <= ast_x+asteroids[index].rock.width &&
           remotes_y+SHIP_HEIGHT/2 <= ast_y+asteroids[index].rock.height)
          {
          // kill the asteroid and the missile

          asteroids[index].rock.state = ASTEROID_INACTIVE;

          // what kind of asteroid did we have?

          switch(asteroids[index].type)
                {

                case ASTEROID_LARGE:
                     {
                     // start one medium and one small asteroid
                     //  (if possible)

                     Start_Asteroid(asteroids[index].x,
                                    asteroids[index].y,
                                    ASTEROID_MEDIUM);

                     Start_Asteroid(asteroids[index].x+ASTEROID_LARGE_WIDTH/2,
                                    asteroids[index].y+ASTEROID_LARGE_HEIGHT/2,
                                    ASTEROID_SMALL);

                     } break;

                case ASTEROID_MEDIUM:
                     {
                     // start two small asteroids (if possible)

                     Start_Asteroid(asteroids[index].x,
                                    asteroids[index].y,
                                    ASTEROID_SMALL);

                     Start_Asteroid(asteroids[index].x+ASTEROID_MEDIUM_WIDTH/2,
                                    asteroids[index].y+ASTEROID_MEDIUM_HEIGHT/2,
                                    ASTEROID_SMALL);

                     } break;

                case ASTEROID_SMALL:
                     {

                     // start a randomly positioned asteroid of any size
                     // at the worm hole

                     Start_Asteroid(WORMHOLE_X,WORMHOLE_Y,rand()%3);

                     } break;

                default:break;

                } // end switch

          // were shields up?

          if (remotes_shields)
             {
             // start explosion
             Start_Explosion(remotes_x,remotes_y,2);
             } // end if shields up
          else
             { // say bye bye!

             Start_Explosion(remotes_x,remotes_y,2);
             Start_Nova(remotes_x+SHIP_WIDTH/2,remotes_y+SHIP_HEIGHT/2);

             // start remotes death sequence

             Start_Remotes_Death();

             } // end if else

          } // end if remote has been hit

       } // end if linked

       } // end if asteroids is active

    } // end for index

} // Move_Asteroids

//////////////////////////////////////////////////////////////////////////////

void Tech_Print(int x,int y,char *string,unsigned char far *destination)
{
// this function is used to print text out like a teletypwriter,it looks
// cool, trust me!

int length,  // length of input string
    index,   // looping variable
    counter; // used to time process

char buffer[3];  // a little string used to call font engine with

// compute length of input string

length = strlen(string);

// print the string out a character at a time

for (index=0; index<length; index++)
    {
    // the first character is the actual printable character

    buffer[0] = string[index];

    // this is a little cursor kind of thing

    buffer[1] = '<';

    // null terminate

    buffer[2] = 0;

    // print the string

    Font_Engine_1(x,y,0,0,buffer,destination);

    // move to next position

    x+=(TECH_FONT_WIDTH+1);

    // wait a bit  1/70th of a second

    Wait_For_Vertical_Retrace();

    // clear the cursor

    } // end for

    // clear the cursor

    buffer[0] = ' ';
    buffer[1] = ' ';
    buffer[2] = 0;

    Font_Engine_1(x,y,0,0,buffer,destination);

// done!

} // end Tech_Print

//////////////////////////////////////////////////////////////////////////////

void Font_Engine_1(int x,int y,
                   int font,int color,
                   char *string,unsigned char far *destination)
{
// this function prints a string out using one of the graphics fonts that
// we have drawn, note this first version doesn't use the font field, but
// we'll throw it in to keep the interface open for a future version

static int font_loaded=0;   // this is used to track the first time the
                            // function is loaded

int index,    // loop index
    c_index,  // character index
    length;   // used to compute lengths of strings

// test if this is the first time this function is called, if so load the
// font

if (!font_loaded)
   {

   // load the 4x7 tech font

   PCX_Init((pcx_picture_ptr)&image_pcx);
   PCX_Load("blazefnt.pcx", (pcx_picture_ptr)&image_pcx,1);

   // allocate memory for each bitmap and load character

   for (index=0; index<NUM_TECH_FONT; index++)
       {
       // allocate memory for charcter

       Bitmap_Allocate((bitmap_ptr)&tech_font[index],
                       TECH_FONT_WIDTH,TECH_FONT_HEIGHT);

       // set size of character

       tech_font[index].width = TECH_FONT_WIDTH;
       tech_font[index].height= TECH_FONT_HEIGHT;

       // extract bitmap from PCX buffer

       tech_font[index].x = 1 + (index % 16) * (TECH_FONT_WIDTH+1);
       tech_font[index].y = 1 + (index / 16) * (TECH_FONT_HEIGHT+1);

       Bitmap_Get((bitmap_ptr)&tech_font[index],
                  (unsigned char far *)image_pcx.buffer);

       } // end for index
   // font is loaded, delete pcx file and set flag

   PCX_Delete((pcx_picture_ptr)&image_pcx);

   font_loaded=1;

   } // end if first time
else
   {
   // print the sent string

   // pre-compute length of string

   length=strlen(string);

   // print the string character by character

   for (index=0; index<length; index++)
       {
       // extract the character index from the space character

       c_index = string[index] - ' ';

       // set bitmap position

       tech_font[c_index].y = y;
       tech_font[c_index].x = x;

       // display bitmap

       Bitmap_Put((bitmap_ptr)&tech_font[c_index],
                  (unsigned char far*)destination,0);

       // move to next character position

       x+=(TECH_FONT_WIDTH+1);

       } // end for index

   } // end else print string

} // end Font_Engine_1

/////////////////////////////////////////////////////////////////////////////

void Clear_Display(int color)
{
// this function fills the setup display screen with a color

int y;

// clear display with horizontal lines

for (y=DISPLAY_Y; y<DISPLAY_Y + DISPLAY_HEIGHT; y++)
     Line_H(DISPLAY_X,DISPLAY_X+DISPLAY_WIDTH-1,y,color);

} // end Clear_Display

/////////////////////////////////////////////////////////////////////////////

void Intro_Title(void)
{

// load in the starblazer title screen

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazeint.pcx",(pcx_picture_ptr)&image_pcx,1);

// done with data so delete it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// show the PCX buffer

PCX_Show_Buffer((pcx_picture_ptr)&image_pcx);

// do special effects

// wait for a sec

Time_Delay(10);

Do_Starburst();

Time_Delay(50);

} // end Intro_Title

//////////////////////////////////////////////////////////////////////////////

void Closing_Screen(void)
{
// this function prints the credits

// blank the screen

Fill_Screen(0);

// restore pallete

Write_Palette(0,255,(RGB_palette_ptr)&game_palette);

if (music_enabled)
   {
   Music_Stop();
   Music_Play((music_ptr)&song,10);
   }

// draw the credits

Tech_Print(10,50,"MUSICAL MASTERY BY",video_buffer);
Time_Delay(20);
Tech_Print(20,60,"DEAN HUDSON OF",video_buffer);
Time_Delay(20);
Tech_Print(30,70,"ECLIPSE PRODUCTIONS",video_buffer);
Time_Delay(20);

Tech_Print(10,100,"MIDPAK INSTRUMENTATION CONSULTING BY",video_buffer);
Time_Delay(20);
Tech_Print(20,110,"ROB WALLACE OF",video_buffer);
Time_Delay(20);
Tech_Print(30,120,"WALLACE MUSIC & SOUND",video_buffer);

// wait a sec

Time_Delay(125);

// fade away

Screen_Transition(SCREEN_DARKNESS);

} // end Closing_Screen

//////////////////////////////////////////////////////////////////////////////

void Intro_Waite(void)
{

// load in the waite group title screen

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("waite.pcx",(pcx_picture_ptr)&image_pcx,1);

// done with data so delete it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// show the PCX buffer

PCX_Show_Buffer((pcx_picture_ptr)&image_pcx);

// do special effects

// wait for a sec

Time_Delay(40);

Screen_Transition(SCREEN_WHITENESS);

// blank the screen

Fill_Screen(0);

} // end Intro_Waite

//////////////////////////////////////////////////////////////////////////////

void Intro_Controls(void)
{
// this function displays the controls screen

// load in the starblazer controls screen

PCX_Init((pcx_picture_ptr)&image_controls);
PCX_Load("blazecon.pcx",(pcx_picture_ptr)&image_controls,1);

// copy controls data to video buffer

PCX_Show_Buffer((pcx_picture_ptr)&image_controls);

// scan under button sprite and draw

Sprite_Under((sprite_ptr)&button_1,video_buffer);
Sprite_Draw((sprite_ptr)&button_1,video_buffer,1);

// delete pcx file

PCX_Delete((pcx_picture_ptr)&image_controls);

} // end Intro_Controls

////////////////////////////////////////////////////////////////////////////

void Intro_Briefing(void)
{
// this function displays the controls screen

int done=0, // exit flag
    page=0, // current page user is reading
    index;  // looping variable

// load in the starblazer controls screen

PCX_Init((pcx_picture_ptr)&image_controls);
PCX_Load("blazeins.pcx",(pcx_picture_ptr)&image_controls,1);

// copy controls data to video buffer

PCX_Show_Buffer((pcx_picture_ptr)&image_controls);

// delete pcx file

PCX_Delete((pcx_picture_ptr)&image_controls);

// display the first page

for (index=0; index<NUM_LINES_PAGE; index++)
    Font_Engine_1(78,24+index*8,0,0,instructions[index+page*17],video_buffer);

// enter main event loop

while(!done)
     {
     // has the user pressed a key

     if (keys_active>0)
        {

        if (keyboard_state[MAKE_UP])
           {
           // page up

           if (--page<0)
              page = 0;

           // press button

           button_3.x = 185;
           button_3.y = 170;
           button_3.curr_frame = 3;

           Sprite_Draw((sprite_ptr)&button_3,video_buffer,1);

           Digital_FX_Play(BLZKEY_VOC,2);

           Time_Delay(2);

           button_3.curr_frame = 2;

           Sprite_Draw((sprite_ptr)&button_3,video_buffer,1);

           } // end if up

        if (keyboard_state[MAKE_DOWN])
           {
           // page down

           if (++page>=NUM_PAGES)
              page = NUM_PAGES-1;


           // press button

           button_3.x = 106;
           button_3.y = 170;
           button_3.curr_frame = 1;

           Sprite_Draw((sprite_ptr)&button_3,video_buffer,1);

           Digital_FX_Play(BLZKEY_VOC,2);

           Time_Delay(2);

           button_3.curr_frame = 0;

           Sprite_Draw((sprite_ptr)&button_3,video_buffer,1);

           } // end if down

        if (keyboard_state[MAKE_ESC])
           {
           done=1;
           } // end if esc

        // refresh display

        for (index=0; index<NUM_LINES_PAGE; index++)
            Font_Engine_1(78,24+index*8,0,0,instructions[index+page*17],video_buffer);

        } // end if a key has been pressed

     // do the scrolling lite thing

     Panel_FX();

     // wait a sec

     Time_Delay(1);

     } // end main while

} // end Intro_Briefing

////////////////////////////////////////////////////////////////////////////

void Load_Explosions(void)
{
// this function loads the bitmap explosions

int index,  // looping variable
    frames; // looping variable

// load the imagery for the explosions

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazeexp.pcx", (pcx_picture_ptr)&image_pcx,1);

// load each explosion in

for (index=0; index<NUM_EXPLOSIONS; index++)
    {
    // initialize each sprite

    Sprite_Init((sprite_ptr)&explosions[index],0,0,28,22,0,0,0,0,0,0);

    // extract the animation frames

    for (frames=0; frames<NUM_EXPLOSION_FRAMES; frames++)
        PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&explosions[index],frames,frames,0);

    } // end index

// delete the pcx file

PCX_Delete((pcx_picture_ptr)&image_pcx);

} // end Load_Explosions

////////////////////////////////////////////////////////////////////////////

void Load_Icons(void)
{
// this function loads various icons for the game

int index; // looping variable

// load the imagery for the control buttons on the setup screen

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazebt1.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the button sprite

Sprite_Init((sprite_ptr)&button_1,118-10,63,10,8,0,0,0,0,0,0);

button_1.counter_1 = 0;  // button is on the 0th element in the list

// extract the bitmaps for the button, there are 4 animation cells

for (index=0; index<4; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&button_1,index,index,0);

// load in display selection buttons

// intialize the button sprite

Sprite_Init((sprite_ptr)&button_2,0,DISPLAY_Y+DISPLAY_HEIGHT-6,10,8,0,0,0,0,0,0);

// extract the bitmaps for the button, there are 2 animation cells

for (index=0; index<2; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&button_2,index,index,1);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);


// load the imagery for the briefing control buttons

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazebt3.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the button sprite

Sprite_Init((sprite_ptr)&button_3,0,0,42,12,0,0,0,0,0,0);

// extract the bitmaps for the button, there are 4 animation cells

for (index=0; index<4; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&button_3,index,index,0);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// load the imagery for the display bitmaps

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazedis.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the display sprite

Sprite_Init((sprite_ptr)&displays,DISPLAY_X,DISPLAY_Y+6,72,20,0,0,0,0,0,0);

// extract the bitmaps for the display bitmaps, there are 2 images

for (index=0; index<2; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&displays,index,index,0);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);

} // end Load_Icons

////////////////////////////////////////////////////////////////////////////

void Load_Ships(void)
{
int index; // looping variable

// load the imagery for the local ships

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazeshl.pcx", (pcx_picture_ptr)&image_pcx,1);

// load in the imagery for the local gryfon and raptor

Sprite_Init((sprite_ptr)&gryfon_l,0,0,22,18,0,0,0,0,0,0);
Sprite_Init((sprite_ptr)&raptor_l,0,0,22,18,0,0,0,0,0,0);

// there are 32 animation cells per ship

for (index=0; index<16; index++)
    {
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&gryfon_l,index,index%12,index/12);
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&raptor_l,index,index%12,2+index/12);

    // these frames are with engines on

    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&gryfon_l,index+16,index%12,4+index/12);
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&raptor_l,index+16,index%12,4+2+index/12);

    } // end for index

PCX_Delete((pcx_picture_ptr)&image_pcx);

// now the remote gryfon and raptor

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazeshr.pcx", (pcx_picture_ptr)&image_pcx,1);

Sprite_Init((sprite_ptr)&gryfon_r,0,0,22,18,0,0,0,0,0,0);
Sprite_Init((sprite_ptr)&raptor_r,0,0,22,18,0,0,0,0,0,0);


// there are 32 animation cells per ship

for (index=0; index<16; index++)
    {
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&gryfon_r,index,index%12,index/12);
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&raptor_r,index,index%12,2+index/12);

    // these frames are with engines on

    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&gryfon_r,index+16,index%12,4+index/12);
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&raptor_r,index+16,index%12,4+2+index/12);

    } // end for index

// initialize the player and remote sprites

Sprite_Init((sprite_ptr)&players_ship,160-11,100-9,22,18,0,0,0,0,0,0);
Sprite_Init((sprite_ptr)&remotes_ship,0,0,22,18,0,0,0,0,0,0);

// intialize the starburst

Sprite_Init((sprite_ptr)&starburst,0,0,22,18,0,0,0,0,0,0);

// extract the bitmaps for the startburst, there are 6 animation cells

for (index=0; index<6; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&starburst,index,index,8);

PCX_Delete((pcx_picture_ptr)&image_pcx);

} // end Load_Ships

////////////////////////////////////////////////////////////////////////////

void Do_Starburst(void)
{
int x,y,number,index;

// select a random number of startb bursts

number = 2+rand()%3;

for (index=0; index<number; index++)
    {

    // select position for starburst

    starburst.x=160+(rand()%140);
    starburst.y=80+rand()%20;

    Sprite_Under((sprite_ptr)&starburst,video_buffer);

    // do starburst

    for (starburst.curr_frame=0; starburst.curr_frame<6; starburst.curr_frame++)
        {
        Sprite_Erase((sprite_ptr)&starburst,video_buffer);
        Sprite_Under((sprite_ptr)&starburst,video_buffer);
        Sprite_Draw((sprite_ptr)&starburst,video_buffer,1);

        Time_Delay(1+rand()%2);

        } // end for starburst frame

        // erase the starburst

        Sprite_Erase((sprite_ptr)&starburst,video_buffer);

    } // end index

} // end Do_Starburst

////////////////////////////////////////////////////////////////////////////

int Display_Select(int current)
{
// this function is used to select between two choices in the display window

// compute starting position of selection icon based on default selection

button_2.x = DISPLAY_X + 14 + current*40;

// scan under selection icon

Sprite_Under((sprite_ptr)&button_2,video_buffer);
Sprite_Draw((sprite_ptr)&button_2,video_buffer,1);

// until user exits process event loop

while(1)
     {
     // get input

     if (keys_active>0)
        {

        // what is user trying to do?

        if (keyboard_state[MAKE_RIGHT])
           {
           if (++current>1)
              current = 0;

           Digital_FX_Play(BLZKEY_VOC,2);

           Time_Delay(1);

           } // if if right


        else
        if (keyboard_state[MAKE_LEFT])
           {
           if (--current<0)
              current = 1;

           Digital_FX_Play(BLZKEY_VOC,2);

           Time_Delay(1);

           } // end if left

        else
        if (keyboard_state[MAKE_ESC])
           {
           // print that selection was aborted

           Clear_Display(0);

           Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2,0,0,
                         "ABORTED...",
                         video_buffer);

           Digital_FX_Play(BLZABRT_VOC,1);

           Time_Delay(5);

           Clear_Display(0);

           // return selection aborted

           return(-1);

           } // end if esc

        else
        if (keyboard_state[MAKE_ENTER])
           {
           // carriage return, making selection
           // illuminate button for a second

           // draw button down

           button_2.curr_frame = 1;
           Sprite_Draw((sprite_ptr)&button_2,video_buffer,1);

           Digital_FX_Play(BLZKEY_VOC,2);

           Time_Delay(5);

           // now draw button up

           button_2.curr_frame = 0;
           Sprite_Draw((sprite_ptr)&button_2,video_buffer,1);

           Clear_Display(0);

           Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2,0,0,
                         "SELECTION",
                         video_buffer);

           Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2+8,0,0,
                         "RECORDED",
                         video_buffer);

           Digital_FX_Play(BLZSEL_VOC,1);

           Time_Delay(5);

           Clear_Display(0);

           // return the selection

           return(current);

           } // end if enter

        // erase selection icon

        Sprite_Erase((sprite_ptr)&button_2,video_buffer);

        // compute x position

        button_2.x = DISPLAY_X + 14 + current*40;

        // scan under and draw selection icon

        Sprite_Under((sprite_ptr)&button_2,video_buffer);
        Sprite_Draw((sprite_ptr)&button_2,video_buffer,1);

        } // end if kbhit

     // perform special effects

     Panel_FX();

     // wait a bit

     Time_Delay(1);

     } // end while

} // Display_Select

///////////////////////////////////////////////////////////////////////////////

void Copy_Frames(sprite_ptr dest, sprite_ptr source)
{
// this function is used to copy the image frames from one sprite to another

int index; // looping variable

for (index=0; index<source->num_frames; index++)
    {
    // assign next frame

    dest->frames[index] = source->frames[index];

    } // end for index

// set up dest fields

dest->num_frames = source->num_frames;
dest->curr_frame = 0;

} // end Copy_Frames

/////////////////////////////////////////////////////////////////////////////
