
// LIGHT.C - An example of real-time event loops

// I N C L U D E S ////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <graphics.h>

// D E F I N E S /////////////////////////////////////////////////////////////

// directions

#define NORTH   0
#define EAST    1
#define SOUTH   2
#define WEST    3

// F U N C T I O N S /////////////////////////////////////////////////////////

void Delay(int clicks)
{
// this function uses the internal timer to delay a number of clock ticks

unsigned long far *clock = (unsigned long far *)0x0000046CL;

unsigned long now;

// get current time

now = *clock;

// wait for number of click to pass

while(abs(*clock - now) < clicks){}

} // end Delay

//////////////////////////////////////////////////////////////////////////////

void Draw_Game_Grid(void)
{
// this function draws a game grid out of horizontal and vertical lines

short x,y;

// the the line color to white

setcolor(15);

// draw the verical lines

for (x=0; x<320; x+=20)
	 {

	 // position the pen and draw the line

	 moveto(x,0);
	 lineto(x,199);

	 } // end for x

// draw the horizontal lines

for (y=0; y<200; y+=20)
	 {

	 // position the pen and draw the line

	 moveto(0,y);
	 lineto(319,y);

	 } // end for y

} // end Draw_Game_Grid

// M A I N ////////////////////////////////////////////////////////////////////

void main(void)
{

int done=0,                    // main event loop exit flag
	 player_x         = 160,    // starting position and direction of player
	 player_y         = 150,
	 player_direction = NORTH;

	 int driver=2, mode=2;

// SECTION 1 //////////////////////////////////////////////////////////////////

// set the graphics mode to mode 13h 320x200x256

initgraph(&driver,&mode,"");

// draw the game grid

Draw_Game_Grid();

// SECTION 2 //////////////////////////////////////////////////////////////////

// begin real time event loop

while(!done)
     {

// SECTION 3 //////////////////////////////////////////////////////////////////

	  // is the player steering his light cycle or trying to exit?

	  if (kbhit())
		  {

        // test for <A>,<S> or <Q>

        switch(getch())
              {

              case 'a': // turn left
                   {

						 // turn 90 to the left

						 if (--player_direction<NORTH)
							 player_direction=WEST;


                   } break;

				  case 's': // turn right
                   {

                   // turn 90 to the right

                   if (++player_direction>WEST)
							 player_direction=NORTH;

                   } break;

				  case 'q': // quit game
						 {
						 // set exit flag to true

                   done=1;

                   } break;

              default:break; // do nothing

              } // end switch

		  } // end if kbhit

// SECTION 4 //////////////////////////////////////////////////////////////////

	  // at this point we need to move the light cycle in the direction its
	  // currently pointing

     switch(player_direction)
           {

           case NORTH:
                {

                if (--player_y<0)
						 player_y = 199;

                } break;

			  case SOUTH:
					 {
					 if (++player_y>199)
                   player_y = 0;

                } break;

           case EAST:
                {
                if (++player_x>319)
                   player_x = 0;

					 } break;

			  case WEST:
					 {
					 if (--player_x<0)
						 player_y = 319;

                } break;

           default:break;


           } // end switch direction

// SECTION 5 //////////////////////////////////////////////////////////////////

     // render the lightcyle

	  putpixel((short)player_x,(short)player_y,(short)9);

	  // wait a moment and lock the game to 18 fps

	  Delay(1);

	  } // end while

// SECTION 6 //////////////////////////////////////////////////////////////////

// restore the video mode back to text

closegraph();

} // end main

